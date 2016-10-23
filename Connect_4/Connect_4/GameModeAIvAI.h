#pragma once
#include "stdafx.h"

namespace connect4
{
	class GameModeAIvAI :
		public GameMode
	{
	public:
		GameModeAIvAI();
		~GameModeAIvAI() override;

		void CheckInputEvent(const sf::Event& event, DiscHoles& chipHoles) override;
	};
}