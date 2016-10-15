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
		void Draw(sf::RenderWindow& window);
		void ProcessEvent(const sf::Event& event) const;
		void SetGameMode(GameMode::Modes mode);
		sf::VideoMode GetVideoMode() const;
		bool HasSomebodyWon(sf::Color& outWinColor) const;
		//GameMode* GetGameMode() const;
		//ChipHoles* GetChipHoles();
		void Reset() const;

		const sf::Vector2f BorderOffset = sf::Vector2f(40, 60);
		const sf::Vector2f ElementOffset = sf::Vector2f(15, 15);

	private:
		void createBoard();
		void useGameMode(GameMode::Modes mode);

		sf::VideoMode m_VideoMode;
		GameMode* m_GameMode = nullptr;
		ChipHoles* m_ChipHoles = nullptr;

		std::vector<sf::Text> m_Texts;
	};


}
