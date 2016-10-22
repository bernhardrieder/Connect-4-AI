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

	while(!chipSet)
	{	
		auto simulation = ai::BoardSimulation(m_PlacedPlayerChips, m_Player, m_LastMove);
		//Move bestMove = ai::Negamax::GetBestMoveWithAB(simulation, 10);
		Move bestMove = ai::Negamax::GetBestMove(simulation, 4);
		//std::cout << "\nAI BESTMOVE: " << to_string(bestMove) << std::endl;
		chipSet = chipInput(bestMove.column, chipHoles);
	}
}
