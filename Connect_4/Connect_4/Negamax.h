#pragma once

namespace connect4
{
	namespace ai
	{
		class Negamax
		{
		public:
			static Move GetBestMove(BoardSimulation boardSimulation, char depth);
			static Move GetBestMoveWithAB(BoardSimulation boardSimulation, char depth);
		protected:
			Negamax() {}
			~Negamax() {}
			
			static std::tuple<int, Move> negamax(BoardSimulation boardSimulation, char depth);
			static std::tuple<int, Move> abNegamax(BoardSimulation boardSimulation, char depth, int alpha, int beta);
			static bool checkIfMoveUsable(const Move& move);

			static short MIN_Infinity;
			static short MAX_Infinity;
		};


	}
}
