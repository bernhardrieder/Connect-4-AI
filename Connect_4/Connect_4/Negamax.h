#pragma once

namespace connect4
{
	namespace ai
	{
		class Negamax
		{
		public:
			static Move GetBestMove(BoardSimulation& boardSimulation, const char& depth);
			static Move GetBestMoveWithAB(BoardSimulation& boardSimulation, const char& depth);
		protected:
			Negamax() {}
			~Negamax() {}
			static std::pair<int, Move> negamax(BoardSimulation& boardSimulation, const char& depth);
			static std::pair<int, Move> abNegamax(BoardSimulation& boardSimulation, const char& depth, int alpha, int beta);
			static bool checkIfMoveUsable(const Move& move);
		};


	}
}
