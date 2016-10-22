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
	if (m_LastMove.column == Move::None.column)
	{
		m_LastMove = Move(0, 0);
	}
	while(!chipSet)
	{	
		//Move bestMove = ai::Negamax::GetBestMoveWithAB(ai::BoardSimulation(m_PlacedPlayerChips, m_Player, m_LastMove), 10);
		Move bestMove = ai::Negamax::GetBestMove(ai::BoardSimulation(m_PlacedPlayerChips, m_Player, m_LastMove), 4);
		//std::cout << "\nAI BESTMOVE: " << to_string(bestMove) << std::endl;
		chipSet = chipInput(bestMove.column, chipHoles);
	}
}
