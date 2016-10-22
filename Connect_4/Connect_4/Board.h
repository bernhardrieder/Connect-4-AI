#pragma once
#include "stdafx.h"
#include "GameMode.h"
#include "ChipHoles.h"

namespace connect4
{
	class Board
	{
	public:
		Board();
		~Board();

		// draw everything here...
		void Draw(sf::RenderWindow& window) const; 
		void ProcessEvent(const sf::Event& event) const;
		void SetGameMode(GameMode::Modes mode);
		sf::VideoMode GetVideoMode() const;
		bool HasSomebodyWon(sf::Color& outWinColor) const;
		bool AreAllHolesFilledWithChips() const;
		void Reset() const;

		const sf::Vector2f BorderOffset = sf::Vector2f(15, 15);
		const sf::Vector2f ElementOffset = sf::Vector2f(15, 15);

	private:
		void createBoard();
		void useGameMode(GameMode::Modes mode);
		void drawActivePlayerChip(sf::RenderWindow& window) const;

		sf::VideoMode m_VideoMode;
		std::unique_ptr<GameMode> m_GameMode = nullptr;
		std::unique_ptr<ChipHoles> m_ChipHoles = nullptr;

		sf::RectangleShape m_BackgroundShape;

	};


}
