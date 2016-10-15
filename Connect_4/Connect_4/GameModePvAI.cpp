#include "stdafx.h"

using namespace connect4;

GameModePvAI::GameModePvAI() : GameMode()
{
}


GameModePvAI::~GameModePvAI()
{
}

void GameModePvAI::CheckInputEvent(const sf::Event& event, ChipHoles& chipHoles)
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
