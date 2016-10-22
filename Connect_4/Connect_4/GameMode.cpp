#include "stdafx.h"

using namespace connect4;

Move Move::None = Move(std::numeric_limits<unsigned char>::max(), std::numeric_limits<unsigned char>::max());

GameMode::GameMode()
{
	initPlayerChips();
	GlobalVariables::GetPlayerColors(m_PlayerColors[0], m_PlayerColors[1]);
}

GameMode::~GameMode()
{
}

bool GameMode::CheckForWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer)
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

sf::Color GameMode::GetActivePlayerColor()
{
	return m_PlayerColors[m_ActivePlayer];
}

bool GameMode::HasSomebodyWon(sf::Color& outWinColor) const
{
	outWinColor = m_WinColor;
	return m_HasSomebodyWon;
}

bool GameMode::chipInput(int column, ChipHoles& chipHoles)
{
	if (m_HasSomebodyWon) return false;
	int row;
	if(chipHoles.PutChipInColumn(column, m_PlayerColors[m_ActivePlayer], row))
	{
		saveChipInputForPlayer(row, column);
		m_LastMove = Move(row, column);
		if (CheckForWin(m_LastMove, m_PlacedPlayerChips, m_ActivePlayer))
			setActivePlayerToWinner();
		else
			m_ActivePlayer = (m_ActivePlayer + 1) % 2;
		
		return true;
	}
	return false;
}

void GameMode::chipInputPlayer(const sf::Event& event, ChipHoles& chipHoles)
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
			chipInput(m_ChoosenColumn, chipHoles);
		}
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

bool GameMode::winCheckHelperForHorizontalAndVertical(char clampValue, char clampMax, std::function<bool(const char&)> func)
{
	unsigned char min = Clamp<char>(clampValue - 3, 0, clampMax);
	unsigned char max = Clamp<char>(clampValue + 3, 0, clampMax);
	unsigned char count = 0;
	static const unsigned char winAmount = GlobalVariables::GetWinAmount();

	for (; min <= max; ++min)
	{
		if (func(min))
		{
			++count;
			if (count == winAmount)
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
	return winCheckHelperForDiagonal(Move(lastMove.row - 3, lastMove.column - 3), perIterationChange, ifCondInIteration);
}

//checks '\'
bool GameMode::checkForDiagonalDownRightWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer)
{
	std::function<void(Move&)> perIterationChange = [](Move& move) { ++move.row; --move.column; };
	std::function<bool(const Move&)> ifCondInIteration = [&placedPlayerChips, &activePlayer](const Move& move) { return placedPlayerChips[move.row][move.column] == activePlayer; };
	return winCheckHelperForDiagonal(Move(lastMove.row - 3, lastMove.column + 3), perIterationChange, ifCondInIteration);
}

bool GameMode::winCheckHelperForDiagonal(Move lastMove, std::function<void(Move&)> perIterationChange, std::function<bool(const Move&)> iterationIfCond)
{
	int count = 0;

	static const unsigned char rowCount = GlobalVariables::GetRowCount();
	static const unsigned char columnCount = GlobalVariables::GetColumnCount();
	static const unsigned char winAmount = GlobalVariables::GetWinAmount();

	for (int i = 0; i < 7; ++i, perIterationChange(lastMove))
	{
		if (lastMove.row < 0 || lastMove.column < 0 || lastMove.row >= rowCount || lastMove.column >= columnCount)
			continue;
		if (iterationIfCond(lastMove))
		{
			++count;
			if (count == winAmount)
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
	m_WinColor = m_PlayerColors[m_ActivePlayer];

	std::cout << "Player " << m_ActivePlayer + 1 << " with color " << to_string(m_WinColor) << " has won the game!\n";
}

void GameMode::initPlayerChips()
{
	m_PlacedPlayerChips.resize(GlobalVariables::GetRowCount());
	for (auto& a : m_PlacedPlayerChips)
	{
		a.resize(GlobalVariables::GetColumnCount());
		for (auto& i : a)
			i = -1;
	}
}

void GameMode::saveChipInputForPlayer(int row, int column)
{
	m_PlacedPlayerChips[row][column] = m_ActivePlayer;

	//std::cout << "Player " << m_ActivePlayer +1 << " has following chips placed:\n";
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