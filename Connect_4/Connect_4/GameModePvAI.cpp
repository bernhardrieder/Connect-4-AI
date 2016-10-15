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
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num7)
		{
			int targetColumn = event.key.code - 27;
			chipInput(targetColumn, chipHoles);
		}
	}
}

int RandomInt(int min, int max)
{
	return min + (rand() % static_cast<int>(max - min + 1));
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
