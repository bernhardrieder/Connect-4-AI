#pragma once
#include "stdafx.h"

namespace connect4
{
	class ChipHoles
	{
	public:
		ChipHoles(float chipRadius);
		~ChipHoles();

		void Draw(sf::RenderWindow& window);
		void CreateHoles(sf::Vector2f borderOffset, sf::Vector2f elementOffset);
		bool PutChipInColumn(int column, sf::Color chipColor, int& outRow);

		float GetChipRadius() const;
		std::vector<sf::Vector2f> GetBottomHolesPositions() const;
		void Reset();

	private:

		int m_RowCount;
		int m_ColumnCount;
		float m_ChipRadius = 35.f;
		std::vector<sf::CircleShape> m_ChipHoles;
		int m_UsedChipHolesRowPerColumn[7];
		std::vector<sf::Vector2f> m_BottomHolesPositions;
	};
}

