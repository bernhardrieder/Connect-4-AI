#include "stdafx.h"

using namespace connect4;
using namespace connect4::ai;

 // start getBestMove from previous player point of view -> create boardsimulation with (activeplayer-1)%2 for player
 // this means: we start from the current state of the game, and this is/was the last players move
Move Negamax::GetBestMove(BoardSimulation& boardSimulation, const char& depth)
{
	// Get the result of a negamax run and return the move
	auto result = negamax(boardSimulation, depth);
	Move& move = result.second;	//move;
	//std::cout << "best move score = " << std::get <0>(result) << std::endl;
	if (checkIfMoveUsable(move))
		std::cerr << "ERROR: simple negmax results in an error move!!\n";
	return move;
}

Move Negamax::GetBestMoveWithAB(BoardSimulation& boardSimulation, const char& depth)
{
	// Get the result of a negamax run and return the move
	auto result = abNegamax(boardSimulation, depth, GlobalVariables::Short_Mininum, GlobalVariables::Short_Maximum);
	Move& move = result.second;	//move;
	//std::cout << "best move score = " << std::get <0>(result) << std::endl;
	if (checkIfMoveUsable(move))
		std::cerr << "ERROR: negmax with ab pruning results in an error move!!\n";
	return move;
}

std::pair<int, Move> Negamax::negamax(BoardSimulation& boardSimulation, const char& depth)
{
	//start with previous player
	// Check if we’re done recursing		
	
	//NEGAMAX CALLER EVALUATION_SIGN = -1
	//NEGAMAX OPPONEN EVALUATION_SIGN = 1
	if (depth == 0 || boardSimulation.IsGameOver())
		return std::make_pair(boardSimulation.EvaluatePlayerSituation(), Move::None ); // create evaluate function which returns evaluation score based on point of view of current player with the current move -> means its alternating!!

	// Otherwise bubble up values from below
	int bestScore = GlobalVariables::Integer_Minimum; //root node OR current state of the game
	Move bestMove = Move::None; //root node OR current state of the game

	// Go through each move
	for (const Move& move : boardSimulation.GetNextPlayersPossibleMoves())
	{
		//std::cout << "NEGAMAX: Make " << to_string(move) << std::endl;
		BoardSimulation newBoard = boardSimulation.MakeNextPlayersMove(move); //make board simulates move with current player -> current player will be choosen in makeMove() (THIS) method
		boardSimulation.Undo(move);
		// Recurse
		int recursedScore = -negamax(newBoard, depth - 1).first;
		//std::cout << "current score = " << currentScore << ", best score = "<< bestScore << std::endl;

		// Update the best score
		if (recursedScore > bestScore)
		{
			bestScore = recursedScore;
			bestMove.column = move.column;
			bestMove.row = move.row;
			//std::cout << "new best score = " << bestScore << std::endl;
		}
	}

	// Return the score and the best move
	return std::make_pair(bestScore, bestMove);
}

std::pair<int, Move> Negamax::abNegamax(BoardSimulation& boardSimulation, const char& depth, int alpha, int beta)
{
	// Check if we’re done recursing
	if (depth == 0 || boardSimulation.IsGameOver())
		return std::make_pair(boardSimulation.EvaluatePlayerSituation(), Move::None); // like negamax or not? page 683 describes board.evaluate(player)

	// Otherwise bubble up values from below
	int bestScore = GlobalVariables::Integer_Minimum;
	Move bestMove = Move::None;

	// Go through each move
	for (const Move& move : boardSimulation.GetNextPlayersPossibleMoves())
	{
		BoardSimulation newBoard = boardSimulation.MakeNextPlayersMove(move);
		boardSimulation.Undo(move);
		// Recurse
		int recursedScore = -abNegamax(newBoard, depth - 1, -beta, -std::max(alpha, bestScore)).first;

		// Update the best score
		if (recursedScore > bestScore)
		{
			bestScore = recursedScore;
			bestMove.column = move.column;
			bestMove.row = move.row;

			//If we’re outside the bounds, then prune: exit immediately
			if (bestScore >= beta)
				return std::make_pair(bestScore, bestMove);
		}
	}

	// Return the score and the best move
	return std::make_pair(bestScore, bestMove);
}

bool Negamax::checkIfMoveUsable(const Move& move)
{
	return move.row == Move::None.row && move.column == Move::None.column;
}
