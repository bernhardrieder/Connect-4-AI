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
	{	//NEGAMAX CALLER EVALUATION_SIGN = -1
		//NEGAMAX OPPONENT EVALUATION_SIGN = 1
		Move bestMove = ai::Negamax::GetBestMove(ai::BoardSimulation(m_PlacedPlayerChips, m_Player, m_LastMove, 1), 4, -1);
		std::cout << "\nAI BESTMOVE: " << to_string(bestMove) << std::endl;
		int targetColumn = RandomInt(0, GlobalVariables::GetColumnCount() - 1); //search best number! implement AI!
		chipSet = chipInput(bestMove.column, chipHoles);
	}
}
