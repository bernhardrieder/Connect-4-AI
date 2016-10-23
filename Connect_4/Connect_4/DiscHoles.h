#pragma once
#include "stdafx.h"

namespace connect4
{
	class DiscHoles
	{
	public:
		DiscHoles(float discRadius);
		~DiscHoles();

		void Draw(sf::RenderWindow& window);
		void CreateHoles(const sf::Vector2f& borderOffset, const sf::Vector2f& elementOffset);
		bool PutChipInColumn(const int& column, const sf::Color& discColor, int& outRow);
		bool AreAllHolesFilled();
		float GetDiscRadius() const;
		std::vector<sf::Vector2f> GetBottomHolesPositions() const;
		void Reset();

	private:

		char m_RowCount;
		char m_ColumnCount;
		float m_DiscRadius = 35.f;
		std::vector<sf::CircleShape> m_DiscHoles;
		char m_UsedDiscHolesRowPerColumn[7];
		std::vector<sf::Vector2f> m_BottomHolesPositions;
	};
}

