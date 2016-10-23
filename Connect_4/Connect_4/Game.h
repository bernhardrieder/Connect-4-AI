#pragma once
#include "stdafx.h"
#include "Board.h"

namespace connect4
{
	class Game
	{
	public:
		Game();
		~Game();

		int Execute(int argc, char* argv[]);
	private:
		static void showUsage(std::string name);
		void selectGameMode();
		void startGameMode();
		void startGame();
		void showWinnerOrDraw();

		void renderLoop(const sf::VideoMode videoMode, const std::vector<sf::Drawable*>& drawable, std::function<void(sf::RenderWindow&, sf::Event&)> funcEvents);

		Board m_Board;
		sf::Color m_WinnerColor;
		bool m_IsRunning = true;
		bool m_RestartGame = false;
		bool m_IsDraw = false;
		std::string m_WindowCaption = "Connect 4";
		GameMode::Modes m_ActiveGamemode;
	};
}