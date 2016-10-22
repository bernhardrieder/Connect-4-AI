#include "stdafx.h"

using namespace connect4;

ChipHoles::ChipHoles(float chipRadius) : m_RowCount(GlobalVariables::GetRowCount()), m_ColumnCount(GlobalVariables::GetColumnCount()), m_ChipRadius(chipRadius)
{
	m_ChipHoles.resize(m_RowCount*m_ColumnCount);
	m_BottomHolesPositions.resize(m_ColumnCount);
	Reset();
}

ChipHoles::~ChipHoles()
{
	m_ChipHoles.clear();
	m_BottomHolesPositions.clear();
}

void ChipHoles::Draw(sf::RenderWindow& window)
{
	for(auto i : m_ChipHoles)
		window.draw(i);
}

void ChipHoles::CreateHoles(sf::Vector2f borderOffset, sf::Vector2f elementOffset)
{
	int invertedRow = 0;
	for (int row = m_RowCount - 1; row >= 0; --row, ++invertedRow)
	{
		for (int column = m_ColumnCount - 1; column >= 0; --column)
		{
			int currentElement = invertedRow*m_ColumnCount + column;
			m_ChipHoles[currentElement] = sf::CircleShape(m_ChipRadius);
			sf::CircleShape& shape = m_ChipHoles[currentElement];
			shape.setFillColor(sf::Color::White);

			sf::Vector2f offset;
			offset.x = borderOffset.x + elementOffset.x*static_cast<float>(column) + m_ChipRadius*static_cast<float>(column) * 2;
			offset.y = borderOffset.y + elementOffset.y*static_cast<float>(row) + m_ChipRadius*static_cast<float>(row) * 2 + m_ChipRadius*2;

			shape.setPosition(offset);

			if (row == m_RowCount - 1)
			{
				m_BottomHolesPositions[column] = offset;
			}
		}
	}
}

bool ChipHoles::PutChipInColumn(int column, sf::Color chipColor, int& outRow)
{
	int& row = m_UsedChipHolesRowPerColumn[column];
	if (row < m_RowCount)
	{
		//std::cout << "chip input in row  " << row +1<< " and column "<< column+1 << std::endl;
		m_ChipHoles[row*m_ColumnCount + column].setFillColor(chipColor);
		outRow = row++;
		return true;
	}
	return false;
}

bool ChipHoles::AreAllHolesFilled()
{
	for(auto i : m_UsedChipHolesRowPerColumn)
	{
		if (i != m_RowCount) return false;
	}
	return true;
}

float ChipHoles::GetChipRadius() const
{
	return m_ChipRadius;
}

std::vector<sf::Vector2f> ChipHoles::GetBottomHolesPositions() const
{
	return m_BottomHolesPositions;
}

void ChipHoles::Reset()
{
	for (auto& row : m_UsedChipHolesRowPerColumn)
		row = 0;
	for (auto& hole : m_ChipHoles)
		hole.setFillColor(sf::Color::Black);
}
