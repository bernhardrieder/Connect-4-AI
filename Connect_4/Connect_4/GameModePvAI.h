#pragma once
#include "stdafx.h"

namespace connect4
{
	class GameModePvAI :
		public GameMode
	{
	public:
		GameModePvAI();
		~GameModePvAI() override;

		void CheckInputEvent(const sf::Event& event, ChipHoles& chipHoles) override;
	};
}