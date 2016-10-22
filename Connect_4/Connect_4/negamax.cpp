#include "stdafx.h"

using namespace connect4;
using namespace connect4::ai;

short Negamax::MIN_Infinity = std::numeric_limits<short>::min();
short Negamax::MAX_Infinity = std::numeric_limits<short>::max();

 // start getBestMove from previous player point of view -> create boardsimulation with (activeplayer-1)%2 for player
 // this means: we start from the current state of the game, and this is/was the last players move
Move Negamax::GetBestMove(BoardSimulation boardSimulation, char depth, char ev_sign)
{
	// Get the result of a negamax run and return the move
	std::tuple<int, Move> result = negamax(boardSimulation, depth, ev_sign);
	Move move = std::get<1>(result);	//move;
	//std::cout << "best move score = " << std::get <0>(result) << std::endl;
	if (checkIfMoveUsable(move))
		std::cerr << "ERROR: simple negmax results in an error move!!\n";
	return move;
}

Move Negamax::GetBestMoveWithAB(BoardSimulation boardSimulation, int maxDepth)
{
	// Get the result of a negamax run and return the move
	std::tuple<int, Move> result = abNegamax(boardSimulation, maxDepth, 0, MIN_Infinity, MAX_Infinity);
	Move move = std::get<1>(result);	//move;
	//std::cout << "best move score = " << std::get <0>(result) << std::endl;
	if (checkIfMoveUsable(move))
		std::cerr << "ERROR: negmax with ab pruning results in an error move!!\n";
	return move;
}

std::tuple<int, Move> Negamax::negamax(BoardSimulation boardSimulation, char depth, char ev_sign)
{
	//start with previous player
	// Check if we’re done recursing		
	
	//NEGAMAX CALLER EVALUATION_SIGN = -1
	//NEGAMAX OPPONEN EVALUATION_SIGN = 1
	if (depth == 0 || boardSimulation.IsGameOver())
		return std::make_tuple(ev_sign*boardSimulation.EvaluatePlayerSituation(), Move::None ); // create evaluate function which returns evaluation score based on point of view of current player with the current move -> means its alternating!!

	// Otherwise bubble up values from below
	int bestScore = std::numeric_limits<int>::min(); //root node OR current state of the game
	Move bestMove = Move::None; //root node OR current state of the game

	// Go through each move
	for (Move move : boardSimulation.GetNextPlayersPossibleMoves())
	{
		//std::cout << "NEGAMAX: Make " << to_string(move) << std::endl;
		BoardSimulation newBoard = boardSimulation.MakeNextPlayersMove(move); //make board simulates move with current player -> current player will be choosen in makeMove() (THIS) method

		// Recurse
		auto res = negamax(newBoard, depth - 1, -ev_sign);
		int recursedScore = -std::get<0>(res);
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
	return std::make_tuple(bestScore, bestMove);
}

std::tuple<int, Move> Negamax::abNegamax(BoardSimulation boardSimulation, int maxDepth, int currentDepth, int alpha, int beta)
{
	// Check if we’re done recursing
	if (boardSimulation.IsGameOver() || currentDepth == maxDepth)
		return std::make_tuple(boardSimulation.EvaluatePlayerSituation(), Move::None); // like negamax or not? page 683 describes board.evaluate(player)

	// Otherwise bubble up values from below
	int bestScore = MIN_Infinity;
	Move bestMove = Move::None;

	// Go through each move
	for (Move move : boardSimulation.GetNextPlayersPossibleMoves())
	{
		BoardSimulation newBoard = boardSimulation.MakeNextPlayersMove(move);

		// Recurse
		auto res = abNegamax(newBoard, maxDepth, currentDepth + 1, -beta, -std::max(alpha, bestScore));
		int currentScore = -std::get<0>(res);

		// Update the best score
		if (currentScore > bestScore)
		{
			bestScore = currentScore;
			bestMove = move;

			//If we’re outside the bounds, then prune: exit immediately
			if (bestScore >= beta)
				return std::make_tuple(bestScore, bestMove);
		}
	}

	// Return the score and the best move
	return std::make_tuple(bestScore, bestMove);
}

bool Negamax::checkIfMoveUsable(const Move& move)
{
	return move.row == Move::None.row && move.column == Move::None.column;
}
