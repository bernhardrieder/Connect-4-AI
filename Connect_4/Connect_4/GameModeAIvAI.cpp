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


	while (!chipSet)
	{
		auto simulation = ai::BoardSimulation(m_PlacedPlayerChips, (m_ActivePlayer + 1) % 2, m_LastMove);
		//Move bestMove = ai::Negamax::GetBestMove(simulation, 7);
		Move bestMove = ai::Negamax::GetBestMoveWithAB(simulation, 10);
		//std::cout << "\nAI BESTMOVE: " << to_string(bestMove) << std::endl;
		chipSet = chipInput(bestMove.column, chipHoles);
	}
}
