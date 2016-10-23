#include "stdafx.h"

using namespace connect4;

Board::Board() : m_GameMode(nullptr), m_DiscHoles(nullptr)
{
	m_DiscHoles = std::make_unique<DiscHoles>(35.f);
	createBoard();
}

Board::~Board()
{
	m_DiscHoles.release();
	m_GameMode.release();
}

void Board::Draw(sf::RenderWindow& window) const
{
	window.draw(m_BackgroundShape);
	m_DiscHoles->Draw(window);
	drawActivePlayerDisc(window);
}

sf::VideoMode Board::GetVideoMode() const
{
	return m_VideoMode;
}

bool Board::HasSomebodyWon(sf::Color& outWinColor) const
{
	return m_GameMode->HasSomebodyWon(outWinColor);
}

bool Board::AreAllHolesFilledWithDiscs() const
{
	return m_DiscHoles->AreAllHolesFilled();
}

void Board::Reset() const
{
	m_DiscHoles->Reset();
}

void Board::ProcessEvent(const sf::Event& event) const
{
	if (m_GameMode == nullptr) return;
	m_GameMode->CheckInputEvent(event, *m_DiscHoles);
}

void Board::SetGameMode(GameMode::Modes mode)
{
	useGameMode(mode);
}

void Board::createBoard()
{
	m_DiscHoles->CreateHoles(BorderOffset, ElementOffset);

	float chipRadius = m_DiscHoles->GetDiscRadius();
	std::vector<sf::Vector2f> holes = m_DiscHoles->GetBottomHolesPositions();
	m_VideoMode.width = static_cast<int>((holes.end()-1)->x + chipRadius * 2 + BorderOffset.x);
	m_VideoMode.height = static_cast<int>((holes.end()-1)->y + chipRadius * 2 + BorderOffset.y);
	m_BackgroundShape.setSize(sf::Vector2f(static_cast<float>(m_VideoMode.width), static_cast<float>(m_VideoMode.height)));
	m_BackgroundShape.setPosition(0, chipRadius * 2);
	m_BackgroundShape.setFillColor(sf::Color::Blue);
}

void Board::useGameMode(GameMode::Modes mode)
{
	if (m_GameMode != nullptr)
		m_GameMode.release();

	switch (mode)
	{
		case GameMode::Modes::PlayerVsPlayer: m_GameMode = std::make_unique<GameModePvP>(); break;
		case GameMode::Modes::PlayerVsAi: m_GameMode = std::make_unique<GameModePvAI>(); break;
		case GameMode::Modes::AiVsAi: m_GameMode = std::make_unique<GameModeAIvAI>(); break;
	}

}

void Board::drawActivePlayerDisc(sf::RenderWindow& window) const
{
	float xPosOfChoosenColumn = m_DiscHoles->GetBottomHolesPositions()[m_GameMode->GetChoosenColumn()].x;

	sf::CircleShape circle;
	circle.setRadius(m_DiscHoles->GetDiscRadius());
	circle.setFillColor(m_GameMode->GetActivePlayerColor());
	circle.setPosition(xPosOfChoosenColumn, 0);

	window.draw(circle);
}
