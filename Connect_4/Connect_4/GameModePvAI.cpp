#include "stdafx.h"

using namespace connect4;

GameModePvAI::GameModePvAI() : GameMode(), m_Player(0), m_Ai(1)
{
}

GameModePvAI::~GameModePvAI()
{
}

void GameModePvAI::CheckInputEvent(const sf::Event& event, DiscHoles& chipHoles)
{
	if (m_HasSomebodyWon) return;
	if (m_ActivePlayer == m_Player)
		GameMode::discInputPlayer(event, chipHoles);
	if (m_ActivePlayer == m_Ai)
		GameMode::discInputAi(chipHoles);
}