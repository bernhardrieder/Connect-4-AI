#include "stdafx.h"
#include "BoardSimulation.h"

using namespace connect4;
using namespace connect4::ai;

BoardSimulation::BoardSimulation(const placed_chips placedChips, const char currentPlayer, connect4::Move lastMove) : m_PlacedPlayerChips(placedChips), m_ActivePlayer(currentPlayer), m_LastMove(lastMove)
{
	//CHECKED -> WORKS AS INTENDED
	//init consumed rows
	m_NextUsableRowInColumn.resize(GlobalVariables::GetColumnCount());
	for(auto& i : m_NextUsableRowInColumn)
		i = GlobalVariables::GetRowCount();
	for(char column = 0; column < GlobalVariables::GetColumnCount(); ++column )
	{
		for (char row = 0; row < GlobalVariables::GetRowCount(); ++row)
		{
			if (placedChips[row][column] == -1)
			{
				m_NextUsableRowInColumn[column] = row; 
				//std::cout << "next usable row: " << static_cast<short>(m_NextUsableRowInColumn[column] + 1) << " in Column: " << static_cast<short>(column + 1) << std::endl;
				break;
			}
		}
	}
}

BoardSimulation::~BoardSimulation()
{
}

std::vector<Move> BoardSimulation::GetNextPlayersPossibleMoves()
{
	std::vector<Move> possibleMoves;
	static const unsigned char& maxRow = connect4::GlobalVariables::GetRowCount();
	static const unsigned char& maxColumn = connect4::GlobalVariables::GetColumnCount();
	for(char col = 0; col < maxColumn; ++col)
	{
		//std::cout << "next usable row: " << static_cast<short>(m_NextUsableRowInColumn[col]+1) << " in Column: " << static_cast<short>(col+1) << std::endl;
		if (m_NextUsableRowInColumn[col] != maxRow)
			possibleMoves.push_back(Move(m_NextUsableRowInColumn[col], col));
	}

	//std::cout << "Possible Moves:\n";
	//for(auto i : possibleMoves)
	//{
	//	std::cout << to_string(i) << std::endl;
	//}
	return std::move(possibleMoves);
}

//MANIPULATE ACTIVE BOARD?????
BoardSimulation BoardSimulation::MakeNextPlayersMove(/*const unsigned char& targetColumn*/ const Move& move) const
{
	//check if column is available? shouldnt be possible to due getpossiblemoves was called before
	//Move move (++m_NextUsableRowInColumn[targetColumn], targetColumn);
	char nextPlayer = (m_ActivePlayer + 1) % 2;
	//m_PlacedPlayerChips[move.row][move.column] = nextPlayer;
	placed_chips discs = m_PlacedPlayerChips;
	discs[move.row][move.column] = nextPlayer;

	return BoardSimulation(discs, nextPlayer, move);
}

int BoardSimulation::EvaluatePlayerSituation()
{
	//evaluate game for point of view of current player
	//NEGAMAX CALLER EVALUATION_SIGN = -1
	//NEGAMAX OPPONEN EVALUATION_SIGN = 1
	//char multiplicator = 1;
	//if (m_ActivePlayer == 0)
	//	multiplicator = -1; //opponen -> -1
	//if (m_ActivePlayer == 1)
	//	multiplicator = 1;

	////////////if player wins the game then return + INFINITY -> use standard GameMode logic
	//////////if(m_ActivePlayerWon)
	//////////	return m_EvaluationSign*std::numeric_limits<short>::max(); //use as +infinity

	//else if nor player or opponent (isgameover) wins the game then return 0 -> use standard GameMode logic
	
	
	//if (m_PlayerWon[m_ActivePlayer])
	//{
	//	//std::cout << "active player won" << std::endl;
	//	return std::numeric_limits<short>::max(); //use as +infinity
	//}
	if (m_IsDraw)
		return 0;
	//if(m_PlayerWon[m_ActivePlayer])
	//{
	//	std::cout << "player "<< to_string(GlobalVariables::GetPlayerColor(m_ActivePlayer)) <<" won" << std::endl;
	//	return std::numeric_limits<short>::max(); //use as +infinity
	//}
	//if(m_PlayerWon[(m_ActivePlayer+1)%2])
	//{
	//	std::cout << "player " << to_string(GlobalVariables::GetPlayerColor((m_ActivePlayer + 1) % 2)) << " won" << std::endl;
	//	return std::numeric_limits<short>::min(); //use as +infinity
	//}
	//if (m_PlayerWon[0])
	//{
	//	//std::cout << "player " << to_string(GlobalVariables::GetPlayerColor(m_ActivePlayer)) << " won" << std::endl;
	//	return std::numeric_limits<short>::max(); //use as +infinity
	//}
	//if (m_PlayerWon[1])
	//{
	//	//std::cout << "player " << to_string(GlobalVariables::GetPlayerColor((m_ActivePlayer + 1) % 2)) << " won" << std::endl;
	//	return std::numeric_limits<short>::min(); //use as +infinity
	//}

	//////////////else if player loses the game then return - INFINITY -> use standard GameMode logic
	//////////if (m_OpponentPlayerWon)
	//////////	return m_EvaluationSign*std::numeric_limits<short>::min(); //use as -infinity



	//ELSE 

	//delete useless chips from board and fill with "not usable" char -> -10 or something like this?
	//HOW CHECK THIS USELESS CHIPS???? What are useless chips -> chips without gap between or cant be used anymore because isolation
	//use derivation of GameMode winChecker function. if unused chip is available in a row, then use this as possible use
	//so -> check a row and if there isn't a 4+ possible, then mark them as useless and mark the others with +2 -> so e.g. if you check horizontal and detect a 4+ and afterwards vertical is checked and no +4 is detected, dont eliminate active players +2's
	//eliminateUselessChips(m_PlacedPlayerChips);

	//FORGET ELMINIATION -> INSTEAD SEARCH BEST DISCS


	//then check active chipcombinations with possible winmatch of each player and save in list
	//weigth all combinations with predefined points
	/*
	PREDEFINED WEIGHTING
		1 chip = 0 
		2 chips in a row = 50
		3 chips in a row = ALARM!! 100 ?
		4 chips in a row = +infinity
	*/
	//sum up all combinations for each player
	auto bestDiscs = searchBestChips();
	int sumPlayer[2]{ sumUpPotentialDiscs(bestDiscs[0]), sumUpPotentialDiscs(bestDiscs[1]) };
	//subtract opponent points from current player points
	//return this points
	//int discSum = m_EvaluationSign*(sumPlayer[0] - sumPlayer[1]); // OPTION 1
	int discSum = (sumPlayer[(m_ActivePlayer + 1) % 2] - sumPlayer[m_ActivePlayer]); //OPTION 2


	//int discSum = (sumPlayer[m_ActivePlayer] - sumPlayer[(m_ActivePlayer + 1) % 2]); //OPTION 1
	//int discSum = (sumPlayer[0] - sumPlayer[1]); // OPTION 1
	//int discSum = m_EvaluationSign*(sumPlayer[1] - sumPlayer[0]); // OPTION 2
	//int discSum = m_EvaluationSign*(sumPlayer[m_ActivePlayer] - sumPlayer[(m_ActivePlayer + 1) % 2]); //OPTION 1
	//int discSum = m_EvaluationSign*(sumPlayer[m_ActivePlayer] - sumPlayer[(m_ActivePlayer + 1) % 2]); //OPTION 1
	//int discSum = (sumPlayer[m_ActivePlayer] - sumPlayer[(m_ActivePlayer + 1) % 2]); //OPTION 1
	//int discSum = m_EvaluationSign*(sumPlayer[(m_ActivePlayer + 1) % 2] - sumPlayer[m_ActivePlayer]); //OPTION 2
	//int discSum = m_EvaluationSign*(std::max(sumPlayer[(m_ActivePlayer + 1) % 2], sumPlayer[m_ActivePlayer])); //OPTION 3 at least a try
	//std::cout << "evaluate player: " << to_string(GlobalVariables::GetPlayerColor(m_ActivePlayer)) << std::endl;
	//std::cout << "ev_sign = " << static_cast<short>(m_EvaluationSign) << "; player sum = " << sumPlayer[m_ActivePlayer] << ", opponent sum = " << sumPlayer[(m_ActivePlayer + 1) % 2] << std::endl;
	//std::cout << "sum option 1: " << static_cast<int>(m_EvaluationSign*(sumPlayer[0] - sumPlayer[1])) << ", sum option 2: " << static_cast<int>(m_EvaluationSign*(sumPlayer[1] - sumPlayer[0])) << std::endl;
	//std::cout << "discSum = " << discSum << std::endl;
	return discSum;
}

bool BoardSimulation::IsGameOver()
{
	m_PlayerWon[m_ActivePlayer] = hasActivePlayerWon();
	if (m_PlayerWon[m_ActivePlayer])
		return true;

	m_PlayerWon[(m_ActivePlayer+1)%2] = hasOpponentPlayerWon();
	if (m_PlayerWon[(m_ActivePlayer + 1) % 2])
		return true;

	m_IsDraw = isDraw();
	return m_IsDraw;
}

bool BoardSimulation::hasActivePlayerWon() const
{
	bool test = GameMode::CheckForWin(m_LastMove, m_PlacedPlayerChips, m_ActivePlayer);

	if(test)
	{
		sf::Color pColors[2];
		GlobalVariables::GetPlayerColors(pColors[0], pColors[1]);
		//std::cout << "Player with color " << to_string(pColors[m_ActivePlayer]) << " would win the game!-------------------------------------------------------------\n";
	}

	return test;
}

bool BoardSimulation::hasOpponentPlayerWon() const
{
	bool test = GameMode::CheckForWin(m_LastMove, m_PlacedPlayerChips, (m_ActivePlayer + 1) % 2);

	if (test)
	{
		sf::Color pColors[2];
		GlobalVariables::GetPlayerColors(pColors[0], pColors[1]);
		//std::cout << "Player with color " << to_string(pColors[(m_ActivePlayer + 1) % 2]) << " would win the game!-------------------------------------------------------------\n";
	}

	return test;
}

bool BoardSimulation::isDraw() const
{
	bool isDraw = true;
	static const unsigned char& maxRow = connect4::GlobalVariables::GetRowCount();
	for (const unsigned char& i : m_NextUsableRowInColumn)
	{
		isDraw &= (i == maxRow);
		if (!isDraw)
			break;
	}

	return isDraw;
}

//void BoardSimulation::eliminateUselessChips(placed_chips& placedChips)
//{	
//	//delete useless chips from board and fill with "not usable" char -> -10 or something like this?
//	//HOW CHECK THIS USELESS CHIPS???? What are useless chips -> chips without gap between or cant be used anymore because isolation
//	//use derivation of GameMode winChecker function. if unused chip is available in a row, then use this as possible use
//	//so -> check a row and if there isn't a 4+ possible, then mark them as useless and mark the others with +2 -> so e.g. if you check horizontal and detect a 4+ and afterwards vertical is checked and no +4 is detected, dont eliminate active players +2's
//
//	static const unsigned char maxRow = GlobalVariables::GetRowCount();
//	static const unsigned char maxColumn = GlobalVariables::GetColumnCount();	
//	static const unsigned char winAmount = GlobalVariables::GetWinAmount();
//	
//	/*
//	mark useful positions with +2 for each player
//	mark useless with initial number
//	use unused (marked with -1) as possible uses
//	*/
//
//	//unsigned char playerCount[2]{ 0 };
//
//	//for(bool player = 0; player == 1; ++player)
//	//{
//	//	//horizontal
//	//	for(char row = 0; row < maxRow; ++row)
//	//	{
//	//		for (char column = 0; column < maxColumn; ++column)
//	//		{
//	//			if (placedChips[row][column] == player /*player1*/ || placedChips[row][column] == -1)
//	//			{
//	//				++playerCount[player];
//	//				if(playerCount[player] == winAmount)
//	//				{
//	//					/* mark the four last positions as useful */
//	//					for (int i = column; i == column - 4; --i)
//	//						if(placedChips[row][i] == player) 
//	//							placedChips[row][i] += 2;
//	//				}
//	//			}
//	//			else
//	//				playerCount[player] = 0;
//	//		
//	//		}
//	//	}
//	//}
//
//	std::function<char(placed_chips&, const Move&, bool player)> test = [](placed_chips& placedChips, const Move& move, bool player)
//	{
//		char count = 0;
//		for (int i = move.column; i > move.column - 4; --i)
//			if (placedChips[move.row][i] == player)
//				++count;
//		
//		return count;
//	};
//	std::map<unsigned char, unsigned char> usefulChipsSum[2];
//	
//	//if useful chip found, increment with +1. [0] -> 1 chip, [1] -> 2 chip, [2] -> 3 chip
//	//unsigned char usefulChipsSum[2][3]; 
//	unsigned char playerCount[2]{ 0 };
//
//	for (bool player = 0; player == 1; ++player)
//	{
//		//horizontal
//		for (char row = 0; row < maxRow; ++row)
//		{
//			for (char column = 0; column < maxColumn; ++column)
//			{
//				if (placedChips[row][column] == player || placedChips[row][column] == -1)
//				{
//					++playerCount[player];
//					if (playerCount[player] == winAmount)
//					{
//						char chipCount = test(placedChips, Move(row, column), player) - 1;
//						if (chipCount > 0) 
//							++usefulChipsSum[player][chipCount];
//					}
//				}
//				else
//					playerCount[player] = 0;
//
//			}
//		}
//	}
//}

void BoardSimulation::initBestChipsWithZero(std::vector<potential_win_discs_summary>& p)
{
	for(auto& a : p)
	{
		a.emplace(1, 0); //key and value for 1 chip
		a.emplace(2, 0); //key and value for 2 chips
		a.emplace(3, 0); //key and value for 3 chips
		a.emplace(4, 0); //key and value for 3 chips
	}
}

std::vector<potential_win_discs_summary> BoardSimulation::searchBestChips()
{

	//std::cout << "------------------------------ START SEARCHING DISCS ------------------------------" << std::endl;
	std::vector<potential_win_discs_summary> disc { potential_win_discs_summary(), potential_win_discs_summary()};
	initBestChipsWithZero(disc);

	searchBestChipsHorizontal(disc);
	searchBestChipsVertical(disc);
	searchBestChipsDiagonalDownRight(disc);
	searchBestChipsDiagonalUpRight(disc);

	//std::cout << "------------------------------ END SEARCHING DISCS ------------------------------" << std::endl;
	return disc;
}

void BoardSimulation::searchBestChipsHorizontal(std::vector<potential_win_discs_summary>& potentialDisc)
{
	static const unsigned char maxRow = GlobalVariables::GetRowCount();
	static const unsigned char maxColumn = GlobalVariables::GetColumnCount();
	static const unsigned char winAmount = GlobalVariables::GetWinAmount();

	for (char player = 0; player < 2; ++player)
	{
		for (char row = 0; row < maxRow; ++row)
		{
			unsigned char discCount[2]{ 0,0 };
			unsigned char unusedCount[2]{ 0,0 };

			for (char column = 0; column < maxColumn; ++column)
			{
				char& disc = m_PlacedPlayerChips[row][column];
				if (disc == player || disc == -1) //-1 is an unused diskslot and a potential use
				{
					if (disc == player)
						++discCount[player];
					else
						++unusedCount[player];

					if ((discCount[player] != 0 && discCount[player] + unusedCount[player] == winAmount) || discCount[player]==winAmount)
					{
						++potentialDisc[player][discCount[player]];
						if(discCount[player] == winAmount)
							m_PlayerWon[player] = true;
						//std::cout << "Found potential disc in horizontal axis, discs in a row: " << static_cast<short>(discCount[player]) << std::endl;
					}
				}
				else
				{
					discCount[player] = 0;
					unusedCount[player] = 0;
				}

			}
		}
	}
}

void BoardSimulation::searchBestChipsVertical(std::vector<potential_win_discs_summary>& potentialDisc)
{
	static const unsigned char maxRow = GlobalVariables::GetRowCount();
	static const unsigned char maxColumn = GlobalVariables::GetColumnCount();
	static const unsigned char winAmount = GlobalVariables::GetWinAmount();

	for (char player = 0; player < 2; ++player)
	{
		for (char column = 0; column < maxColumn; ++column)
		{
			unsigned char discCount[2]{ 0,0 };
			unsigned char unusedCount[2]{ 0,0 };

			for (char row = 0; row < maxRow; ++row)
			{
				char& disc = m_PlacedPlayerChips[row][column];
				if (disc == player || disc == -1) //-1 is an unused diskslot and a potential use
				{
					if (disc == player)
						++discCount[player];
					else
						++unusedCount[player];

					if ((discCount[player] != 0 && discCount[player] + unusedCount[player] == winAmount) || discCount[player] == winAmount)
					{
						++potentialDisc[player][discCount[player]]; 
						if (discCount[player] == winAmount)
							m_PlayerWon[player] = true;
						//std::cout << "Found potential disc in vertical axis, discs in a row: " << static_cast<short>(discCount[player]) << std::endl;
					}
				}
				else
				{
					discCount[player] = 0;
					unusedCount[player] = 0;
				}

			}
		}
	}
}

void BoardSimulation::searchBestChipsDiagonalUpRight(std::vector<potential_win_discs_summary>& potentialDisc)
{
	static const unsigned char maxRow = GlobalVariables::GetRowCount();
	static const unsigned char maxColumn = GlobalVariables::GetColumnCount();
	static const unsigned char winAmount = GlobalVariables::GetWinAmount();

	auto& playerWon = m_PlayerWon;
	placed_chips& discs = m_PlacedPlayerChips;
	unsigned char discCount[2]{ 0,0 };
	unsigned char unusedCount[2]{ 0,0 };
	std::function<void(const char&, const char&, const char&)> func = [&discs, &discCount, &unusedCount, &potentialDisc, &playerWon](const char& row, const char& column, const char& player)
	{
		char& disc = discs[row][column];
		if (disc == player || disc == -1) //-1 is an unused discslot and a potential use
		{
			if (disc == player)
				++discCount[player];
			else
				++unusedCount[player];

			if ((discCount[player] != 0 && discCount[player] + unusedCount[player] == winAmount) || discCount[player] == winAmount)
			{
				++potentialDisc[player][discCount[player]];
				if (discCount[player] == winAmount)
					playerWon[player] = true;
				//std::cout << "Found potential disc in searchBestChipsDiagonalUpRight, discs in a row: " << static_cast<short>(discCount[player]) << std::endl;
			}
		}
		else
		{
			discCount[player] = 0;
			unusedCount[player] = 0;
		}
	};
	for (char player = 0; player < 2; ++player)
	{
		for(char i = 2; i > -1; --i)
		{
			discCount[player] = 0;
			unusedCount[player] = 0;
			char column = 0;
			for (char row = i; row < maxRow; ++row, ++column)
			{
				func(row, column, player);
			}
		}
		for(char i = 1; i < 4; ++i)
		{
			discCount[player] = 0;
			unusedCount[player] = 0;
			char row = 0;
			for(char column = i; column < maxColumn; ++column, ++row)
			{
				func(row, column, player);
			}
		}
	}
}

void BoardSimulation::searchBestChipsDiagonalDownRight(std::vector<potential_win_discs_summary>& potentialDisc)
{
	static const unsigned char maxRow = GlobalVariables::GetRowCount();
	static const unsigned char maxColumn = GlobalVariables::GetColumnCount();
	static const unsigned char winAmount = GlobalVariables::GetWinAmount();

	auto& playerWon = m_PlayerWon;
	placed_chips& discs = m_PlacedPlayerChips;
	unsigned char discCount[2]{ 0,0 };
	unsigned char unusedCount[2]{ 0,0 };
	std::function<void(const char&, const char&, const char&)> func = [&discs, &discCount, &unusedCount, &potentialDisc, &playerWon](const char& row, const char& column, const char& player)
	{
		char& disc = discs[row][column];
		if (disc == player || disc == -1) //-1 is an unused discslot and a potential use
		{
			if (disc == player)
				++discCount[player];
			else
				++unusedCount[player];

			if ((discCount[player] != 0 && discCount[player] + unusedCount[player] == winAmount) || discCount[player] == winAmount)
			{
				++potentialDisc[player][discCount[player]];
				if (discCount[player] == winAmount)
					playerWon[player] = true;
				//std::cout << "Found potential disc in searchBestChipsDiagonalDownRight, discs in a row: " << static_cast<short>(discCount[player]) << std::endl;
			}
		}
		else
		{
			discCount[player] = 0;
			unusedCount[player] = 0;
		}
	};
	for (char player = 0; player < 2; ++player)
	{
		//not all diagonal rows have place for 4 in a row!!
		for (char i = 5; i > 2; --i)
		{
			discCount[player] = 0;
			unusedCount[player] = 0;
			char column = 0;
			for (char row = i; row > -1; --row, ++column)
			{
				func(row, column, player);
			}
		}
		for (char i = 1; i < 4; ++i)
		{
			discCount[player] = 0;
			unusedCount[player] = 0;
			char row = maxRow-1;
			for (char column = i; column < maxColumn; ++column, --row)
			{
				func(row, column, player);
			}
		}
	}
}

int BoardSimulation::sumUpPotentialDiscs(potential_win_discs_summary& discs)
{	
	/*
	PREDEFINED WEIGHTING
	1 chip = 0
	2 chips in a row = 50
	3 chips in a row = ALARM!! 100 ?
	4 chips in a row = +infinity // NOT POSSIBLE -> due to IsGameOver check before evaluate!!!
	*/
	
	if (discs[4] != 0)
	{
		//std::cout << "----------------- FOR IN A ROW!! ----------------- \n";
		return std::numeric_limits<short>::max();
	}
	
	int sum = 0;
	sum += discs[1] * 1;
	sum += discs[2] * 5;
	sum += discs[3] * 20;
	return sum;
}
