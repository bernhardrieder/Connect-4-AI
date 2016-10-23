#include "stdafx.h"

using namespace connect4;

Game::Game() : m_WinnerColor(sf::Color::White), m_IsRunning(true), m_RestartGame(false), m_IsDraw(false), m_ActiveGamemode(GameMode::Modes::PlayerVsPlayer)
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
		ai::Negamax::SetDepth(4);
		ai::Negamax::SetMode(ai::Negamax::Modes::SimpleNegamax);
		std::cerr << "ERROR: No user input found!\nGame will start with default AI values! Simple negamax with a depth of 4!\n\n";
	}
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help"))
		{
			showUsage(argv[0]);
			return 0;
		}
		if ((arg == "-ai:m:1") || (arg == "--ai:mode:1"))
		{
			ai::Negamax::SetMode(ai::Negamax::Modes::SimpleNegamax);
			ai::Negamax::SetDepth(GlobalVariables::Integer_Maximum);
			break;
		}
		if ((arg == "-ai:m:2") || (arg == "--ai:mode:2"))
		{
			ai::Negamax::SetMode(ai::Negamax::Modes::SimpleNegamax);
			if (i + 2 < argc)
			{
				std::string argDepth = argv[i + 1];
				std::string argDepthNum = argv[i + 2];
				if ((argDepth == "-d") || (argDepth == "--depth"))
				{
					ai::Negamax::SetDepth(std::stoi(argDepthNum));
					break;
				}
			}
			else
			{
				ai::Negamax::SetDepth(4);
				std::cout << "No given depth for negamax - initialized with standard depth of 4!\n";
				break;
			}
		}
		if ((arg == "-ai:m:3") || (arg == "--ai:mode:3"))
		{
			if (i + 2 < argc)
			{
				ai::Negamax::SetMode(ai::Negamax::Modes::AlphaBetaPruning);
				std::string argDepth = argv[i + 1];
				std::string argDepthNum = argv[i + 2];
				if ((argDepth == "-d") || (argDepth == "--depth"))
				{
					int depth = std::stoi(argDepthNum);
					if(depth == 5 || depth == 8 || depth == 10)
					{
						ai::Negamax::SetDepth(depth);
						break;
					}
				}
			}
			std::cerr << "--ai:mode:3 option requires --depth argument with value 5, 8 or 10." << std::endl;
			return 1;
		}
	}
	startGame();
	return 0;
}

void Game::showUsage(std::string name)
{
	std::cerr << "Usage: " << name << " <option(s)> AI parameters\n\n"
		<< "Options:\n"
		<< "\t-h,--help\t\tShow this help message\n"
		<< "\t-ai:m:1, --ai:mode:1\tNegamax infinte depth and no static evaluation function.\n"
		<< "\t-ai:m:2, --ai:mode:2\tNegamax and static evaluation function. Choose a depth with '-d', '--depth'.\n"
		<< "\t-ai:m:3, --ai:mode:3\tNegamax with alpha-beta-pruning and static evaluation function. Choose a depth of 5, 8 or 10 with '-d', '--depth'.\n"
		<< "\t-d, --depth\t\tSets negamax depth.\n"
		<< std::endl;
}

void Game::selectGameMode()
{
	if (!m_IsRunning) return;
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
	sf::Text info = startText;
	info.setString("Use left/ right/ down arrow keys for ingame input");
	info.setCharacterSize(30);
	info.setPosition(40, 265);

	std::vector<sf::Drawable*> drawables;
	drawables.push_back(&startText);
	drawables.push_back(&playerVsPlayer);
	drawables.push_back(&playerVsAi);
	drawables.push_back(&aiVsAi);
	drawables.push_back(&info);

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
	renderLoop(sf::VideoMode(750, 350), drawables, func);
	board.SetGameMode(m_ActiveGamemode);
}

void Game::startGameMode()
{
	if (!m_IsRunning) return;
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

		window.clear(sf::Color::Black);
		m_Board.Draw(window);
		window.display();

		if (m_Board.HasSomebodyWon(m_WinnerColor))
		{
			showWinnerOrDraw();
			break;
		}
		//its possible to win the game with the last chip, so check and set this after has won check!
		if(m_Board.AreAllHolesFilledWithDiscs())
		{
			m_IsDraw = true;
			showWinnerOrDraw();
			break;
		}
	}
	window.close();
	if (m_RestartGame) startGame();
}

void Game::startGame()
{
	m_IsDraw = false;
	m_RestartGame = false;
	m_IsRunning = true;
	m_Board.Reset();
	selectGameMode();
	startGameMode();
	showWinnerOrDraw();
}

void Game::showWinnerOrDraw()
{
	if (!m_IsRunning) return;
	sf::CircleShape circle;
	circle.setFillColor(m_IsDraw ? sf::Color::White : m_WinnerColor);
	circle.setRadius(100);
	circle.setPosition(170, 150);
	sf::Text winText;
	winText.setFont(GlobalVariables::GetTextFont());
	winText.setString(m_IsDraw ? " DRAW!" : "WINNER:");
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
	bool& isDraw = m_IsDraw;

	std::function<void(sf::RenderWindow&, sf::Event&)> func = 
		[&restartGame, &isRunning, &isDraw](sf::RenderWindow& window, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case  sf::Keyboard::R:
				restartGame = true;
				isDraw = false;
				window.close();
				break;
			case sf::Keyboard::E:
				isRunning = false;
				isDraw = false;
				window.close();
				break;
			}
		}
	};
	renderLoop(sf::VideoMode(530, 500), drawables, func);
}

void Game::renderLoop(const sf::VideoMode videoMode, const std::vector<sf::Drawable*>& drawable, std::function<void(sf::RenderWindow&, sf::Event&)> funcEvents)
{
	if (!m_IsRunning) return;
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

		window.clear(sf::Color::Black);
		for(auto& i : drawable)
			window.draw(*i);

		window.display();
	}
	if (window.isOpen())
		window.close();
}
