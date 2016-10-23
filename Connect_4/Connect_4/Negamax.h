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
			enum class Modes
			{
				SimpleNegamax,
				AlphaBetaPruning
			};
			static Move GetBestMove(BoardSimulation& boardSimulation);
			static Move GetBestMoveSimple(BoardSimulation& boardSimulation, const int& depth);
			static Move GetBestMoveWithAB(BoardSimulation& boardSimulation, const int& depth);
			static void SetDepth(const int& depth);
			static int GetDepth();
			static void SetMode(Modes mode);
			static Modes GetMode();
		private:
			Negamax() {}
			~Negamax() {}
			static NegamaxResult negamax(BoardSimulation& boardSimulation, const int& depth);
			static NegamaxResult abNegamax(BoardSimulation& boardSimulation, const int& depth, int alpha, int beta);
			static bool checkIfMoveIsUsable(const Move& move);

			static Modes m_Mode;
			static int m_Depth;
		};


	}
}
