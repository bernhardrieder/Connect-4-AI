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

		void CheckInputEvent(const sf::Event& event, ChipHoles& chipHoles) override;

	private:
		void chipInputAi(ChipHoles& chipHoles);
	};
}