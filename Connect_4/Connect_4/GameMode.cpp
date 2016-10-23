#include "stdafx.h"

using namespace connect4;

Move Move::None = Move(std::numeric_limits<unsigned char>::max(), std::numeric_limits<unsigned char>::max());

GameMode::GameMode()
{
	initPlayerDiscs();
}

GameMode::~GameMode()
{
}

bool GameMode::CheckForWin(const Move& lastMove, const placed_discs& placedPlayerChips, const char& activePlayer)
{
	if (checkForVerticalWin(lastMove, placedPlayerChips, activePlayer))
		return true;
	if (checkForHorizontalWin(lastMove, placedPlayerChips, activePlayer))
		return true;
	if (checkForDiagonalDownRightWin(lastMove, placedPlayerChips, activePlayer))
		return true;
	if (checkForDiagonalUpRightWin(lastMove, placedPlayerChips, activePlayer))
		return true;
	return false;
}

unsigned char GameMode::GetChoosenColumn() const
{
	return m_ChoosenColumn;
}

sf::Color GameMode::GetActivePlayerColor() const
{
	return GlobalVariables::GetPlayerColor(m_ActivePlayer);
}

bool GameMode::HasSomebodyWon(sf::Color& outWinColor) const
{
	outWinColor = m_WinColor;
	return m_HasSomebodyWon;
}

bool GameMode::discInput(const char& column, DiscHoles& chipHoles)
{
	if (m_HasSomebodyWon) return false;
	int row;
	if(chipHoles.PutChipInColumn(column, GetActivePlayerColor(), row))
	{
		m_LastMove = Move(row, column);
		saveDiscInputForPlayer(m_LastMove);
		if (CheckForWin(m_LastMove, m_PlacedDiscs, m_ActivePlayer))
			setActivePlayerToWinner();
		else
			m_ActivePlayer = (m_ActivePlayer + 1) % 2;
		
		return true;
	}
	return false;
}

void GameMode::discInputPlayer(const sf::Event& event, DiscHoles& chipHoles)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Left)
		{
			m_ChoosenColumn = Clamp<char>(--m_ChoosenColumn, 0, GlobalVariables::GetColumnCount() - 1);
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			m_ChoosenColumn = Clamp<char>(++m_ChoosenColumn, 0, GlobalVariables::GetColumnCount() - 1);
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			discInput(m_ChoosenColumn, chipHoles);
		}
	}
}

void GameMode::discInputAi(DiscHoles& chipHoles)
{
	bool chipSet = false;

	while (!chipSet)
	{
		//start simulation with previous player!
		auto simulation = ai::BoardSimulation(m_PlacedDiscs, (m_ActivePlayer + 1) % 2, m_LastMove);
		Move bestMove = ai::Negamax::GetBestMove(simulation);
		chipSet = discInput(bestMove.column, chipHoles);
	}
}

bool GameMode::checkForVerticalWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer)
{
	char col = lastMove.column;
	std::function<bool(const char&)> func = [&placedPlayerChips, &col, &activePlayer](const char& it) { return placedPlayerChips[it][col] == activePlayer; };
	return winCheckHelperForHorizontalAndVertical(lastMove.row, GlobalVariables::GetRowCount() - 1, func);
}

bool GameMode::checkForHorizontalWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer)
{
	char row = lastMove.row;
	std::function<bool(const char&)> func = [&placedPlayerChips, &row, &activePlayer](const char& it) { return placedPlayerChips[row][it] == activePlayer; };
	return winCheckHelperForHorizontalAndVertical(lastMove.column, GlobalVariables::GetColumnCount() - 1, func);
}

bool GameMode::winCheckHelperForHorizontalAndVertical(char clampValue, const char& clampMax, std::function<bool(const char&)>& func)
{
	unsigned char min = Clamp<char>(clampValue - 3, 0, clampMax);
	unsigned char max = Clamp<char>(clampValue + 3, 0, clampMax);
	unsigned char count = 0;

	for (; min <= max; ++min)
	{
		if (func(min))
		{
			++count;
			if (count == GlobalVariables::GetWinAmount())
				return true;
		}
		else
			count = 0;
	}
	return false;
}


// checks '/'
bool GameMode::checkForDiagonalUpRightWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer)
{
	std::function<void(Move&)> perIterationChange = [](Move& move) { ++move.row; ++move.column; };
	std::function<bool(const Move&)> ifCondInIteration = [&placedPlayerChips, &activePlayer](const Move& move) { return placedPlayerChips[move.row][move.column] == activePlayer; };
	Move move = Move(lastMove.row - 3, lastMove.column - 3);
	return winCheckHelperForDiagonal(move, perIterationChange, ifCondInIteration);
}

//checks '\'
bool GameMode::checkForDiagonalDownRightWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer)
{
	std::function<void(Move&)> perIterationChange = [](Move& move) { ++move.row; --move.column; };
	std::function<bool(const Move&)> ifCondInIteration = [&placedPlayerChips, &activePlayer](const Move& move) { return placedPlayerChips[move.row][move.column] == activePlayer; };
	Move move = Move(lastMove.row - 3, lastMove.column + 3);
	return winCheckHelperForDiagonal(move, perIterationChange, ifCondInIteration);
}

bool GameMode::winCheckHelperForDiagonal(Move& lastMove, std::function<void(Move&)>& perIterationChange, std::function<bool(const Move&)>& iterationIfCond)
{
	int count = 0;

	for (int i = 0; i < 7; ++i, perIterationChange(lastMove))
	{
		if (lastMove.row < 0 || lastMove.column < 0 || lastMove.row >= GlobalVariables::GetRowCount() || lastMove.column >= GlobalVariables::GetColumnCount())
			continue;
		if (iterationIfCond(lastMove))
		{
			++count;
			if (count == GlobalVariables::GetWinAmount())
				return true;
		}
		else
			count = 0;
	}
	return false;
}

void GameMode::setActivePlayerToWinner()
{
	m_HasSomebodyWon = true;
	m_WinColor = GetActivePlayerColor();
	//std::cout << "Player " << m_ActivePlayer + 1 << " with color " << to_string(m_WinColor) << " has won the game!\n";
}

void GameMode::initPlayerDiscs()
{
	m_PlacedDiscs.resize(GlobalVariables::GetRowCount());
	for (auto& a : m_PlacedDiscs)
	{
		a.resize(GlobalVariables::GetColumnCount());
		for (auto& i : a)
			i = -1;
	}
}

void GameMode::saveDiscInputForPlayer(const Move& move)
{
	m_PlacedDiscs[move.row][move.column] = m_ActivePlayer;

	//std::cout << "Player " << m_ActivePlayer +1 << " has following discs placed:\n";
	//for(int r = GlobalVariables::GetRowCount()-1 ; r >= 0 ; --r)
	//{
	//	std::cout << "Row " << r+1 << ":\t";
	//	for(int c = 0; c < GlobalVariables::GetColumnCount(); ++c)
	//	{
	//		char x = m_PlacedPlayerChips[r][c] == m_ActivePlayer? 'X': 'O';
	//		std::cout << x << " ";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << std::endl;
}