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

	private:
		void chipInputPlayer(const sf::Event& event, ChipHoles& chipHoles);
		void chipInputAi(ChipHoles& chipHoles);

		int m_Player;
		int m_Ai;
	};
}