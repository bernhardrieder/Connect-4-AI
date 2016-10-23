#include "stdafx.h"

using namespace connect4;

GameModeAIvAI::GameModeAIvAI() : GameMode()
{	
}

GameModeAIvAI::~GameModeAIvAI()
{
}

void GameModeAIvAI::CheckInputEvent(const sf::Event& event, DiscHoles& chipHoles)
{
	if (m_HasSomebodyWon) return;
	GameMode::discInputAi(chipHoles);
}
