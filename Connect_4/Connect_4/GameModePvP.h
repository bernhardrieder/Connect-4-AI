#pragma once
#include "stdafx.h"

namespace connect4
{
	class GameModePvP :
		public GameMode
	{
	public:
		GameModePvP();
		~GameModePvP() override;

		void CheckInputEvent(const sf::Event& event, ChipHoles& chipHoles) override;
	};
}