#include "stdafx.h"

using namespace connect4;

Game::Game() : m_WinnerColor(sf::Color::White), m_IsRunning(true), m_RestartGame(false), m_ActiveGamemode(GameMode::Modes::PlayerVsPlayer)
{
}

Game::~Game()
{
}

int Game::Execute(int argc, char* argv[])
{
	if (argc < 2)
	{
		showUsage(argv[0]);
		std::cerr << "Game started with default values for AI!\n\n";
	}
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help"))
		{
			showUsage(argv[0]);
			return 0;
		}
		//else if ((arg == "-d") || (arg == "--destination")) 
		//{
		//	if (i + 1 < argc) { // Make sure we aren't at the end of argv!
		//		destination = argv[i++]; // Increment 'i' so we don't get the argument as the next argv[i].
		//	}
		//	else { // Uh-oh, there was no argument to the destination option.
		//		std::cerr << "--destination option requires one argument." << std::endl;
		//		return 1;
		//	}
		//}
	}
	startGame();
	return 0;
}

void Game::showUsage(std::string name)
{
	std::cerr << "Usage: " << name << " <option(s)> AI parameters\n\n"
		<< "Options:\n"
		<< "\t-h,--help\t\tShow this help message\n"
		//<< "\t-d,--destination DESTINATION\tSpecify the destination path"
		<< std::endl;
}

void Game::selectGameMode()
{
	sf::Text startText;
	startText.setFont(GlobalVariables::GetTextFont());
	startText.setString("Select a Connect 4 gamemode:");
	startText.setFillColor(sf::Color::White);
	startText.setCharacterSize(50);
	startText.setPosition(20, 15);

	sf::Text playerVsPlayer = startText;
	playerVsPlayer.setString("1 - Player versus Player");
	playerVsPlayer.setPosition(70, 65);
	sf::Text playerVsAi = startText;
	playerVsAi.setString("2 - Player versus AI");
	playerVsAi.setPosition(70, 115);
	sf::Text aiVsAi = startText;
	aiVsAi.setString("3 - AI versus AI");
	aiVsAi.setPosition(70, 165);

	std::vector<sf::Drawable*> drawables;
	drawables.push_back(&startText);
	drawables.push_back(&playerVsPlayer);
	drawables.push_back(&playerVsAi);
	drawables.push_back(&aiVsAi);

	Board& board = m_Board;
	GameMode::Modes& mode = m_ActiveGamemode;

	std::function<void(sf::RenderWindow&, sf::Event&)> func =
		[&board, &mode](sf::RenderWindow& window, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num3)
		{
			int target = event.key.code - 27;

			switch (target)
			{
			case 0:
				mode = GameMode::Modes::PlayerVsPlayer; break;
			case 1:
				mode = GameMode::Modes::PlayerVsAi; break;
			case 2:
				mode = GameMode::Modes::AiVsAi; break;
			}
			window.close();
		}
	};
	renderLoop(sf::VideoMode(750, 250), drawables, func);
	board.SetGameMode(m_ActiveGamemode);
}

void Game::startGameMode()
{
	sf::RenderWindow window(m_Board.GetVideoMode(), m_WindowCaption);

	while (m_IsRunning && window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				m_IsRunning = false;
			}
			else if(m_ActiveGamemode == GameMode::Modes::PlayerVsPlayer || m_ActiveGamemode == GameMode::Modes::PlayerVsAi)
				m_Board.ProcessEvent(event);
		}
		//if ai vs ai - process without waiting for an event
		if (m_ActiveGamemode == GameMode::Modes::AiVsAi)
			m_Board.ProcessEvent(event);

		window.clear(sf::Color::Blue);
		m_Board.Draw(window);
		window.display();

		if (m_Board.HasSomebodyWon(m_WinnerColor))
		{
			showWinner();
			break;
		}
	}
	window.close();
	if (m_RestartGame) startGame();
}

void Game::startGame()
{
	m_RestartGame = false;
	m_IsRunning = true;
	m_Board.Reset();
	selectGameMode();
	startGameMode();
	showWinner();
}

void Game::showWinner()
{
	sf::CircleShape circle;
	circle.setFillColor(m_WinnerColor);
	circle.setRadius(100);
	circle.setPosition(170, 150);
	sf::Text winText;
	winText.setFont(GlobalVariables::GetTextFont());
	winText.setString("WINNER:");
	winText.setFillColor(sf::Color::White);
	winText.setCharacterSize(100);
	winText.setPosition(50, 10);
	sf::Text restartText = winText;
	restartText.setCharacterSize(50);
	restartText.setString("   Press R to restart, \nor E to end the game.");
	restartText.setPosition(25, 360);

	std::vector<sf::Drawable*> drawables;
	drawables.push_back(&circle);
	drawables.push_back(&winText);
	drawables.push_back(&restartText);

	bool& restartGame = m_RestartGame;
	bool& isRunning = m_IsRunning;

	std::function<void(sf::RenderWindow&, sf::Event&)> func = 
		[&restartGame, &isRunning](sf::RenderWindow& window, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case  sf::Keyboard::R:
				restartGame = true;
				window.close();
				break;
			case sf::Keyboard::E:
				isRunning = false;
				window.close();
				break;
			}
		}
	};
	renderLoop(sf::VideoMode(530, 500), drawables, func);
}

void Game::renderLoop(const sf::VideoMode videoMode, const std::vector<sf::Drawable*>& drawable, std::function<void(sf::RenderWindow&, sf::Event&)> funcEvents)
{
	sf::RenderWindow window(videoMode, m_WindowCaption);
	while (m_IsRunning && window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				m_IsRunning = false;
			}
			else 
			{
				funcEvents(window, event);
			}
		}

		window.clear(sf::Color::Blue);
		for(auto& i : drawable)
			window.draw(*i);

		window.display();
	}
	if (window.isOpen())
		window.close();
}
