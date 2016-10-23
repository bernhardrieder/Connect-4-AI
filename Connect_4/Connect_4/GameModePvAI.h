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

		void CheckInputEvent(const sf::Event& event, DiscHoles& chipHoles) override;

	private:

		int m_Player;
		int m_Ai;
	};
}