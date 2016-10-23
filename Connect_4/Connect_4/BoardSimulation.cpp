#include "stdafx.h"
#include "BoardSimulation.h"

using namespace connect4;
using namespace connect4::ai;

BoardSimulation::BoardSimulation(const placed_discs& placedDiscs, const char& currentPlayer, const connect4::Move& lastMove) : m_PlacedDiscs(placedDiscs), m_ActivePlayer(currentPlayer), m_LastMove(lastMove)
{
	initializeNextUsableRows();
}

BoardSimulation::~BoardSimulation()
{
}

std::vector<Move> BoardSimulation::GetNextPlayersPossibleMoves()
{
	std::vector<Move> possibleMoves;
	for(char col = 0; col < GlobalVariables::GetColumnCount(); ++col)
	{
		if (m_NextUsableRowInColumn[col] != GlobalVariables::GetRowCount())
			possibleMoves.push_back(Move(m_NextUsableRowInColumn[col], col));
	}

	return std::move(possibleMoves);
}

BoardSimulation BoardSimulation::MakeNextPlayersMove(const Move& move)
{
	char nextPlayer = (m_ActivePlayer + 1) % 2;
	m_PlacedDiscs[move.row][move.column] = nextPlayer;
	return BoardSimulation(m_PlacedDiscs, nextPlayer, move);
}

void BoardSimulation::Undo(const Move& move)
{
	m_PlacedDiscs[move.row][move.column] = -1;
}

int BoardSimulation::EvaluatePlayerSituation()
{
	//evaluate game for point of view of current player

	if (m_IsDraw)
		return 0;
	if(m_PlayerWon[m_ActivePlayer])
		return GlobalVariables::Short_Mininum;
	if(m_PlayerWon[(m_ActivePlayer+1)%2])
		return GlobalVariables::Short_Maximum;

	//check active disccombinations with possible winmatch of each player and save in list
	auto bestDiscs = searchPotentialWinDiscs();

	//sum up all combinations with predefined points for each player
	int sumPlayer[2]{ sumUpPotentialDiscs(bestDiscs[0]), sumUpPotentialDiscs(bestDiscs[1]) };

	//subtract current player form opponent points points
	return sumPlayer[(m_ActivePlayer + 1) % 2] - sumPlayer[m_ActivePlayer];
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
	return GameMode::CheckForWin(m_LastMove, m_PlacedDiscs, m_ActivePlayer);
}

bool BoardSimulation::hasOpponentPlayerWon() const
{
	return GameMode::CheckForWin(m_LastMove, m_PlacedDiscs, (m_ActivePlayer + 1) % 2);;
}

bool BoardSimulation::isDraw() const
{
	bool isDraw = true;
	for (const unsigned char& i : m_NextUsableRowInColumn)
	{
		isDraw &= (i == GlobalVariables::GetRowCount());
		if (!isDraw)
			break;
	}

	return isDraw;
}


std::vector<potential_win_discs_summary> BoardSimulation::searchPotentialWinDiscs()
{
	std::vector<potential_win_discs_summary> disc { potential_win_discs_summary(), potential_win_discs_summary()};

	initPotentialWinDiscsWithZero(disc);

	for (char player = 0; player < 2; ++player)
	{
		searchPotentialWinDiscsHorizontal(disc, player,0);
		searchPotentialWinDiscsVertical(disc, player);
		searchPotentialWinDiscsDiagonalDownRight(disc, player);
		searchPotentialWinDiscsDiagonalUpRight(disc, player);
	}

	return std::move(disc);
}

void BoardSimulation::initPotentialWinDiscsWithZero(std::vector<potential_win_discs_summary>& p)
{
	for (auto& a : p)
	{
		a.emplace(1, 0); //key and value for 1 disc
		a.emplace(2, 0); //key and value for 2 discs
		a.emplace(3, 0); //key and value for 3 discs
		a.emplace(4, 0); //key and value for 4 discs
	}
}

void BoardSimulation::searchPotentialWinDiscsHorizontal(std::vector<potential_win_discs_summary>& potentialDisc, const char& player, char rows)
{
	for (char row = 0; row < GlobalVariables::GetRowCount(); ++row)
	{
		unsigned char discCount = 0;
		unsigned char unusedCount = 0;

		for (char column = 0; column < GlobalVariables::GetColumnCount(); ++column)
		{
			searchCounterHelper(potentialDisc, Move(row, column), player, discCount, unusedCount);
		}
	}
}

void BoardSimulation::searchPotentialWinDiscsVertical(std::vector<potential_win_discs_summary>& potentialDisc, const char& player)
{
	for (char column = 0; column < GlobalVariables::GetColumnCount(); ++column)
	{
		unsigned char discCount = 0;
		unsigned char unusedCount = 0;

		for (char row = 0; row < GlobalVariables::GetRowCount(); ++row)
		{
			searchCounterHelper(potentialDisc, Move(row, column), player, discCount, unusedCount);
		}
	}
}

void BoardSimulation::searchPotentialWinDiscsDiagonalUpRight(std::vector<potential_win_discs_summary>& potentialDisc, const char& player)
{
	unsigned char discCount = 0;
	unsigned char unusedCount = 0;

	//not all diagonal rows have place for 4 in a row!!
	for (char i = 2; i > -1; --i)
	{
		discCount = 0;
		unusedCount = 0;
		char column = 0;
		for (char row = i; row < GlobalVariables::GetRowCount(); ++row, ++column)
		{
			searchCounterHelper(potentialDisc, Move(row, column), player, discCount, unusedCount);
		}
	}
	for (char i = 1; i < 4; ++i)
	{
		discCount = 0;
		unusedCount = 0;
		char row = 0;
		for (char column = i; column < GlobalVariables::GetColumnCount(); ++column, ++row)
		{
			searchCounterHelper(potentialDisc, Move(row, column), player, discCount, unusedCount);
		}
	}
}

void BoardSimulation::searchPotentialWinDiscsDiagonalDownRight(std::vector<potential_win_discs_summary>& potentialDisc, const char& player)
{
	unsigned char discCount = 0;
	unsigned char unusedCount = 0;

	//not all diagonal rows have place for 4 in a row!!
	for (char i = 5; i > 2; --i)
	{
		discCount = 0;
		unusedCount = 0;
		char column = 0;
		for (char row = i; row > -1; --row, ++column)
		{
			searchCounterHelper(potentialDisc, Move(row, column), player, discCount, unusedCount);
		}
	}
	for (char i = 1; i < 4; ++i)
	{
		discCount = 0;
		unusedCount = 0;
		char row = GlobalVariables::GetRowCount() -1;
		for (char column = i; column < GlobalVariables::GetColumnCount(); ++column, --row)
		{
			searchCounterHelper(potentialDisc, Move(row, column), player, discCount, unusedCount);
		}
	}
	
}

void BoardSimulation::searchCounterHelper(std::vector<potential_win_discs_summary>& potentialDisc, const Move& move, const char& player, unsigned char& discCount, unsigned char& unusedCount)
{
	char& disc = m_PlacedDiscs[move.row][move.column];
	if (disc == player || disc == -1) //-1 is an unused discslot and a potential use
	{
		if (disc == player)
			++discCount;
		else
			++unusedCount;

		if ((discCount != 0 && discCount + unusedCount == GlobalVariables::GetWinAmount()) || discCount == GlobalVariables::GetWinAmount())
		{
			++potentialDisc[player][discCount];
			if (discCount == GlobalVariables::GetWinAmount())
				m_PlayerWon[player] = true;
		}
	}
	else
	{
		discCount = 0;
		unusedCount = 0;
	}
}

int BoardSimulation::sumUpPotentialDiscs(potential_win_discs_summary& discs)
{	
	if (discs[4] != 0)
	{
		return std::numeric_limits<short>::max();
	}
	
	int sum = 0;
	sum += discs[1] * 1;
	sum += discs[2] * 5;
	sum += discs[3] * 20;
	return sum;
}

void BoardSimulation::initializeNextUsableRows()
{
	m_NextUsableRowInColumn.resize(GlobalVariables::GetColumnCount());
	for (auto& i : m_NextUsableRowInColumn)
		i = GlobalVariables::GetRowCount();
	for (char column = 0; column < GlobalVariables::GetColumnCount(); ++column)
	{
		for (char row = 0; row < GlobalVariables::GetRowCount(); ++row)
		{
			if (m_PlacedDiscs[row][column] == -1)
			{
				m_NextUsableRowInColumn[column] = row;
				break;
			}
		}
	}
}
