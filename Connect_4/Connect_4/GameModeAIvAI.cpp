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

int Random(int min, int max)
{
	return min + (rand() % static_cast<int>(max - min + 1));
}

void GameModeAIvAI::chipInputAi(ChipHoles& chipHoles)
{
	bool chipSet = false;

	while (!chipSet)
	{
		int targetColumn = Random(0, GlobalVariables::GetColumnCount() - 1); //search best number! implement AI!
		chipSet = chipInput(targetColumn, chipHoles);
	}
}
