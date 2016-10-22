#pragma once

namespace connect4
{
	namespace ai
	{
		struct NegamaxResult
		{
			NegamaxResult(const int& score, const Move& move)
			{
				this->score = score;
				this->move = move;
			}
			int score;
			Move move;
		};
		class Negamax
		{
		public:
			static Move GetBestMove(BoardSimulation& boardSimulation, const char& depth);
			static Move GetBestMoveWithAB(BoardSimulation& boardSimulation, const char& depth);
		protected:
			Negamax() {}
			~Negamax() {}
			static NegamaxResult negamax(BoardSimulation& boardSimulation, const char& depth);
			static NegamaxResult abNegamax(BoardSimulation& boardSimulation, const char& depth, int alpha, int beta);
			static bool checkIfMoveUsable(const Move& move);
		};


	}
}
