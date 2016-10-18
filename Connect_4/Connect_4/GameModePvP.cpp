#include "stdafx.h"

using namespace connect4;


GameModePvP::GameModePvP() : GameMode()
{
}

GameModePvP::~GameModePvP()
{
}

void GameModePvP::CheckInputEvent(const sf::Event& event, ChipHoles& chipHoles)
{
	GameMode::chipInputPlayer(event, chipHoles);
}
