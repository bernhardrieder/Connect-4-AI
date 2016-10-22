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

void GameModeAIvAI::chipInputAi(ChipHoles& chipHoles)
{
	bool chipSet = false;

	if (m_LastMove.column == Move::None.column)
	{
		m_LastMove = Move(0, 0);
	}
	while (!chipSet)
	{
		//Move bestMove = ai::Negamax::GetBestMove(ai::BoardSimulation(m_PlacedPlayerChips, (m_ActivePlayer+1)%2, m_LastMove), 7);
		Move bestMove = ai::Negamax::GetBestMoveWithAB(ai::BoardSimulation(m_PlacedPlayerChips, (m_ActivePlayer + 1) % 2, m_LastMove), 10);
		//std::cout << "\nAI BESTMOVE: " << to_string(bestMove) << std::endl;
		chipSet = chipInput(bestMove.column, chipHoles);
	}
}
