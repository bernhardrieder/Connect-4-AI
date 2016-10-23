#include "stdafx.h"

using namespace connect4;

GameModePvP::GameModePvP() : GameMode()
{
}

GameModePvP::~GameModePvP()
{
}

void GameModePvP::CheckInputEvent(const sf::Event& event, DiscHoles& chipHoles)
{
	if (m_HasSomebodyWon) return;
	GameMode::discInputPlayer(event, chipHoles);
}
