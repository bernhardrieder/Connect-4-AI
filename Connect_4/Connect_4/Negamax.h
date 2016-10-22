#pragma once

namespace connect4
{
	namespace ai
	{
		class Negamax
		{
		public:
			static Move GetBestMove(BoardSimulation boardSimulation, char depth, char ev_sign);
			static Move GetBestMoveWithAB(BoardSimulation boardSimulation, int maxDepth);
		protected:
			Negamax() {}
			~Negamax() {}

			static std::tuple<int, Move> negamax(BoardSimulation boardSimulation, char depth, char ev_sign);
			static std::tuple<int, Move> abNegamax(BoardSimulation boardSimulation, int maxDepth, int currentDepth, int alpha, int beta);
			static bool checkIfMoveUsable(const Move& move);

			static short MIN_Infinity;
			static short MAX_Infinity;
		};


	}
}