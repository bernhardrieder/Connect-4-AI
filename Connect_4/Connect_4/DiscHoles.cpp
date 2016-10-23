#include "stdafx.h"

using namespace connect4;

DiscHoles::DiscHoles(float discRadius) : m_RowCount(GlobalVariables::GetRowCount()), m_ColumnCount(GlobalVariables::GetColumnCount()), m_DiscRadius(discRadius)
{
	m_DiscHoles.resize(m_RowCount*m_ColumnCount);
	m_BottomHolesPositions.resize(m_ColumnCount);
	Reset();
}

DiscHoles::~DiscHoles()
{
	m_DiscHoles.clear();
	m_BottomHolesPositions.clear();
}

void DiscHoles::Draw(sf::RenderWindow& window)
{
	for(auto i : m_DiscHoles)
		window.draw(i);
}

void DiscHoles::CreateHoles(const sf::Vector2f& borderOffset, const sf::Vector2f& elementOffset)
{
	int invertedRow = 0;
	for (int row = m_RowCount - 1; row >= 0; --row, ++invertedRow)
	{
		for (int column = m_ColumnCount - 1; column >= 0; --column)
		{
			int currentElement = invertedRow*m_ColumnCount + column;
			m_DiscHoles[currentElement] = sf::CircleShape(m_DiscRadius);
			sf::CircleShape& shape = m_DiscHoles[currentElement];
			shape.setFillColor(sf::Color::White);

			sf::Vector2f offset;
			offset.x = borderOffset.x + elementOffset.x*static_cast<float>(column) + m_DiscRadius*static_cast<float>(column) * 2;
			offset.y = borderOffset.y + elementOffset.y*static_cast<float>(row) + m_DiscRadius*static_cast<float>(row) * 2 + m_DiscRadius*2;

			shape.setPosition(offset);

			if (row == m_RowCount - 1)
			{
				m_BottomHolesPositions[column] = offset;
			}
		}
	}
}

bool DiscHoles::PutChipInColumn(const int& column, const sf::Color& discColor, int& outRow)
{
	char& row = m_UsedDiscHolesRowPerColumn[column];
	if (row < m_RowCount)
	{
		//std::cout << "chip input in row  " << row +1<< " and column "<< column+1 << std::endl;
		m_DiscHoles[row*m_ColumnCount + column].setFillColor(discColor);
		outRow = row++;
		return true;
	}
	return false;
}

bool DiscHoles::AreAllHolesFilled()
{
	for(auto i : m_UsedDiscHolesRowPerColumn)
	{
		if (i != m_RowCount) return false;
	}
	return true;
}

float DiscHoles::GetDiscRadius() const
{
	return m_DiscRadius;
}

std::vector<sf::Vector2f> DiscHoles::GetBottomHolesPositions() const
{
	return m_BottomHolesPositions;
}

void DiscHoles::Reset()
{
	for (auto& row : m_UsedDiscHolesRowPerColumn)
		row = 0;
	for (auto& hole : m_DiscHoles)
		hole.setFillColor(sf::Color::Black);
}
