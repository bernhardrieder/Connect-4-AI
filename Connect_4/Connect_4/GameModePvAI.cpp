#include "stdafx.h"

using namespace connect4;

GameModePvAI::GameModePvAI() : GameMode(), m_Player(0), m_Ai(1)
{
}


GameModePvAI::~GameModePvAI()
{
}

void GameModePvAI::CheckInputEvent(const sf::Event& event, ChipHoles& chipHoles)
{
	if (m_HasSomebodyWon) return;
	if (m_ActivePlayer == m_Player)
		chipInputPlayer(event, chipHoles);
	if (m_ActivePlayer == m_Ai)
		chipInputAi(chipHoles);
}

void GameModePvAI::chipInputPlayer(const sf::Event& event, ChipHoles& chipHoles)
{
	GameMode::chipInputPlayer(event, chipHoles);
}

void GameModePvAI::chipInputAi(ChipHoles& chipHoles)
{
	bool chipSet = false;

	while(!chipSet)
	{
		int targetColumn = RandomInt(0, GlobalVariables::GetColumnCount() - 1); //search best number! implement AI!
		chipSet = chipInput(targetColumn, chipHoles);
	}
}
