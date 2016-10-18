#include "stdafx.h"

using namespace connect4;

GameModeAIvAI::GameModeAIvAI() : GameMode()
{
}


GameModeAIvAI::~GameModeAIvAI()
{
}

void GameModeAIvAI::CheckInputEvent(const sf::Event& event, ChipHoles& chipHoles)
{
	if (m_HasSomebodyWon) return;
	chipInputAi(chipHoles);
}

void GameModeAIvAI::chipInputAi(ChipHoles& chipHoles)
{
	bool chipSet = false;

	while (!chipSet)
	{
		int targetColumn = RandomInt(0, GlobalVariables::GetColumnCount() - 1); //search best number! implement AI!
		chipSet = chipInput(targetColumn, chipHoles);
	}
}
