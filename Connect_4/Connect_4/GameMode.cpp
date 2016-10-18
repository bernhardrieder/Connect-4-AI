#include "stdafx.h"

using namespace connect4;

GameMode::GameMode()
{
	initPlayerChips();
	GlobalVariables::GetPlayerColors(m_PlayerColors[0], m_PlayerColors[1]);
}

GameMode::~GameMode()
{
}

int GameMode::GetChoosenColumn() const
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
		if (checkForWin(row, column))
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
			m_ChoosenColumn = Clamp(--m_ChoosenColumn, 0, GlobalVariables::GetColumnCount() - 1);
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			m_ChoosenColumn = Clamp(++m_ChoosenColumn, 0, GlobalVariables::GetColumnCount() - 1);
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			chipInput(m_ChoosenColumn, chipHoles);
		}
	}
}

bool GameMode::checkForWin(int row, int column)
{
	if (checkForVerticalWin(row, column))
		return true;
	if (checkForHorizontalWin(row, column))
		return true;
	if (checkForDiagonalDownRightWin(row, column))
		return true;
	if (checkForDiagonalUpRightWin(row, column))
		return true;
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
	for (auto& i : m_PlayerChips)
	{
		i.resize(GlobalVariables::GetRowCount());
		for (auto& a : i)
			a.resize(GlobalVariables::GetColumnCount());
	}
}

void GameMode::saveChipInputForPlayer(int row, int column)
{
	m_PlayerChips[m_ActivePlayer][row][column] = true;

	//std::cout << "Player " << m_ActivePlayer +1 << " has following chips placed:\n";
	//for(int r = GlobalVariables::GetRowCount()-1 ; r >= 0 ; --r)
	//{
	//	std::cout << "Row " << r+1 << ":\t";
	//	for(int c = 0; c < GlobalVariables::GetColumnCount(); ++c)
	//	{
	//		char x = m_PlayerChips[m_ActivePlayer][r][c] ? 'X': 'O';
	//		std::cout << x << " ";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << std::endl;
}

bool GameMode::checkForVerticalWin(int row, int column)
{
	auto& playerChips = m_PlayerChips[m_ActivePlayer];
	std::function<bool(int)> func = [&playerChips, &column](int it) { return playerChips[it][column]; };
	return winCheckHelperForHorizontalAndVertical(row, GlobalVariables::GetRowCount()-1, func);
}

bool GameMode::checkForHorizontalWin(int row, int column)
{
	auto& playerChips = m_PlayerChips[m_ActivePlayer];
	std::function<bool(int)> func = [&playerChips, &row](int it) { return playerChips[row][it]; };
	return winCheckHelperForHorizontalAndVertical(column, GlobalVariables::GetColumnCount()-1, func);
}

bool GameMode::winCheckHelperForHorizontalAndVertical(int clampValue, int clampMax, std::function<bool(int)> func)
{
	int min = Clamp(clampValue - 3, 0, clampMax);
	int max = Clamp(clampValue + 3, 0, clampMax);
	int count = 0;
	int winAmount = GlobalVariables::GetWinAmount();

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
bool GameMode::checkForDiagonalUpRightWin(int row, int column)
{
	std::function<void(int&, int&)> func = [](int& r, int& c) { ++r; ++c; };
	return winCheckHelperForDiagonal(row - 3, column - 3, func);
}

//checks '\'
bool GameMode::checkForDiagonalDownRightWin(int row, int column)
{
	std::function<void(int&, int&)> func = [](int& r, int& c) { ++r; --c; };
	return winCheckHelperForDiagonal(row-3, column+3, func);
}

bool GameMode::winCheckHelperForDiagonal(int row, int column, std::function<void(int&, int&)> funcForRowAndColumnPerIteration)
{
	auto& playerChips = m_PlayerChips[m_ActivePlayer];
	int count = 0;

	int rowCount = GlobalVariables::GetRowCount();
	int columnCount = GlobalVariables::GetColumnCount();
	int winAmount = GlobalVariables::GetWinAmount();

	for (int i = 0; i < 7; ++i, funcForRowAndColumnPerIteration(row, column))
	{
		if (row < 0 || column < 0 || row >= rowCount || column >= columnCount)
			continue;
		if (playerChips[row][column])
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