#include "stdafx.h"

using namespace connect4;

Board::Board() : m_GameMode(nullptr), m_ChipHoles(nullptr)
{
	m_ChipHoles = new ChipHoles(35.f);
	createBoard();
}

Board::~Board()
{
	if(m_ChipHoles != nullptr)
		delete m_ChipHoles;
	if(m_GameMode != nullptr)
		delete m_GameMode;
	m_Texts.clear();
}

void Board::Draw(sf::RenderWindow& window)
{
	m_ChipHoles->Draw(window);
	for (auto i : m_Texts)
		window.draw(i);
}

sf::VideoMode Board::GetVideoMode() const
{
	return m_VideoMode;
}

bool Board::HasSomebodyWon(sf::Color& outWinColor) const
{
	return m_GameMode->HasSomebodyWon(outWinColor);
}

void Board::Reset() const
{
	m_ChipHoles->Reset();
}

void Board::ProcessEvent(const sf::Event& event) const
{
	if (m_GameMode == nullptr) return;
	m_GameMode->CheckInputEvent(event, *m_ChipHoles);
}

void Board::SetGameMode(GameMode::Modes mode)
{
	useGameMode(mode);
}

void Board::createBoard()
{
	m_ChipHoles->CreateHoles(BorderOffset, ElementOffset);

	float chipRadius = m_ChipHoles->GetChipRadius();
	std::vector<sf::Vector2f> holes = m_ChipHoles->GetBottomHolesPositions();
	int column = 0;
	for(sf::Vector2f hole : holes)
	{
		sf::Text text;
		text.setFont(GlobalVariables::GetTextFont());
		text.setString(std::to_string(++column));
		int characterSizeInPixel = 24;
		text.setCharacterSize(characterSizeInPixel); // in pixels, not points!
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Bold);

		hole.x += chipRadius / 2 + characterSizeInPixel / 2;
		hole.y += ElementOffset.y + chipRadius * 2;
		text.setPosition(hole);
		m_Texts.push_back(text);
	}

	m_VideoMode.width = static_cast<int>((holes.end()-1)->x + chipRadius * 2 + BorderOffset.x);
	m_VideoMode.height = static_cast<int>((holes.end()-1)->y + chipRadius * 2 + BorderOffset.y);
}

void Board::useGameMode(GameMode::Modes mode)
{
	if(m_GameMode != nullptr)
		delete m_GameMode;

	switch (mode)
	{
	case GameMode::Modes::PlayerVsPlayer: m_GameMode = new GameModePvP(); break;
		case GameMode::Modes::PlayerVsAi: m_GameMode = new GameModePvAI(); break;
		case GameMode::Modes::AiVsAi: m_GameMode = new GameModeAIvAI(); break;
	}

}
