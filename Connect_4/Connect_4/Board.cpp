#include "stdafx.h"

using namespace connect4;

Board::Board() : m_GameMode(nullptr), m_ChipHoles(nullptr)
{
	m_ChipHoles = std::make_unique<ChipHoles>(35.f);
	createBoard();
}

Board::~Board()
{
	m_ChipHoles.release();
	m_GameMode.release();
	m_Texts.clear();
}

void Board::Draw(sf::RenderWindow& window) const
{
	m_ChipHoles->Draw(window);
	drawActivePlayerChip(window);
}

sf::VideoMode Board::GetVideoMode() const
{
	return m_VideoMode;
}

bool Board::HasSomebodyWon(sf::Color& outWinColor) const
{
	return m_GameMode->HasSomebodyWon(outWinColor);
}

bool Board::AreAllHolesFilledWithChips() const
{
	return m_ChipHoles->AreAllHolesFilled();
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
	m_VideoMode.width = static_cast<int>((holes.end()-1)->x + chipRadius * 2 + BorderOffset.x);
	m_VideoMode.height = static_cast<int>((holes.end()-1)->y + chipRadius * 2 + BorderOffset.y);
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

void Board::drawActivePlayerChip(sf::RenderWindow& window) const
{
	std::vector<sf::Vector2f> holes = m_ChipHoles->GetBottomHolesPositions();
	float chipRadius = m_ChipHoles->GetChipRadius();
	int activeColumn = m_GameMode->GetChoosenColumn();
	float choosenX = holes[activeColumn].x;
	sf::Color chipColor = m_GameMode->GetActivePlayerColor();

	sf::CircleShape circle;
	circle.setRadius(chipRadius);
	circle.setFillColor(chipColor);
	circle.setPosition(choosenX, -chipRadius/2);

	window.draw(circle);
	
}
