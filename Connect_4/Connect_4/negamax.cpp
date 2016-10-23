#include "stdafx.h"

using namespace connect4;
using namespace connect4::ai;

int Negamax::m_Depth = 0;
Negamax::Modes Negamax::m_Mode = Modes::SimpleNegamax;

Move Negamax::GetBestMove(BoardSimulation& boardSimulation)
{
	Move move;
	//std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	switch (m_Mode)
	{
		case Modes::SimpleNegamax: move = GetBestMoveSimple(boardSimulation, GetDepth());
		case Modes::AlphaBetaPruning: move = GetBestMoveWithAB(boardSimulation, GetDepth());
	}

	//std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	//std::cout << "Ply: Negamax with depth " << static_cast<short>(GetDepth()) << ": " << time_span.count() << " seconds\n";

	return move;
}

// start getBestMove from previous player point of view -> create boardsimulation with (activeplayer-1)%2 for player
Move Negamax::GetBestMoveSimple(BoardSimulation& boardSimulation, const int& depth)
{
	auto result = negamax(boardSimulation, depth);
	Move& move = result.move;	//move;

	if (checkIfMoveIsUsable(move))
		std::cerr << "ERROR: negamax results in an error move!!\n";
	return move;
}

Move Negamax::GetBestMoveWithAB(BoardSimulation& boardSimulation, const int& depth)
{
	auto result = abNegamax(boardSimulation, depth, GlobalVariables::Short_Mininum, GlobalVariables::Short_Maximum);
	Move& move = result.move;	//move;

	if (checkIfMoveIsUsable(move))
		std::cerr << "ERROR: negamax with ab pruning results in an error move!!\n";
	return move;
}

void Negamax::SetDepth(const int& depth)
{
	m_Depth = depth;
}

int Negamax::GetDepth()
{
	return m_Depth;
}

void Negamax::SetMode(Modes mode)
{
	m_Mode = mode;
}

Negamax::Modes Negamax::GetMode()
{
	return m_Mode;
}

NegamaxResult Negamax::negamax(BoardSimulation& boardSimulation, const int& depth)
{
	// start with previous player
	// Check if we’re done recursing		
	if (depth == 0 || boardSimulation.IsGameOver())
		return NegamaxResult(boardSimulation.EvaluatePlayerSituation(), Move::None); // create evaluate function which returns evaluation score based on point of view of current player

	// Otherwise bubble up values from below
	int bestScore = GlobalVariables::Integer_Minimum; //root node
	Move bestMove = Move::None; //root node

	// Go through each move
	for (const Move& move : boardSimulation.GetNextPlayersPossibleMoves())
	{
		BoardSimulation newBoard = boardSimulation.MakeNextPlayersMove(move);
		boardSimulation.Undo(move);

		// Recurse
		int recursedScore = -negamax(newBoard, depth - 1).score;

		// Update the best score
		if (recursedScore > bestScore)
		{
			bestScore = recursedScore;
			bestMove.column = move.column;
			bestMove.row = move.row;
		}
	}

	// Return the score and the best move
	return NegamaxResult(bestScore, bestMove);
}

NegamaxResult Negamax::abNegamax(BoardSimulation& boardSimulation, const int& depth, int alpha, int beta)
{
	// Check if we’re done recursing
	if (depth == 0 || boardSimulation.IsGameOver())
		return NegamaxResult(boardSimulation.EvaluatePlayerSituation(), Move::None);

	// Otherwise bubble up values from below
	int bestScore = GlobalVariables::Integer_Minimum;
	Move bestMove = Move::None;

	// Go through each move
	for (const Move& move : boardSimulation.GetNextPlayersPossibleMoves())
	{
		BoardSimulation newBoard = boardSimulation.MakeNextPlayersMove(move);
		boardSimulation.Undo(move);

		// Recurse
		int recursedScore = -abNegamax(newBoard, depth - 1, -beta, -std::max(alpha, bestScore)).score;

		// Update the best score
		if (recursedScore > bestScore)
		{
			bestScore = recursedScore;
			bestMove.column = move.column;
			bestMove.row = move.row;

			//If we’re outside the bounds, then prune: exit immediately
			if (bestScore >= beta)
				return NegamaxResult(bestScore, bestMove);
		}
	}

	// Return the score and the best move
	return NegamaxResult(bestScore, bestMove);
}

bool Negamax::checkIfMoveIsUsable(const Move& move)
{
	return move.row == Move::None.row && move.column == Move::None.column;
}
