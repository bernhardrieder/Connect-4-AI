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
		void showWinner();

		void renderLoop(const sf::VideoMode videoMode, const std::vector<sf::Drawable*>& drawable, std::function<void(sf::RenderWindow&, sf::Event&)> funcEvents);

		//std::shared_ptr<sf::RenderWindow> m_RenderWindow = nullptr;
		Board m_Board;
		sf::Color m_WinnerColor;
		bool m_IsRunning = true;
		bool m_RestartGame = false;
		std::string m_WindowCaption = "Connect 4";
	};
}