#pragma once

namespace connect4
{
	namespace ai
	{
		typedef std::vector<std::vector<char>> placed_chips;
		typedef std::map<unsigned char, unsigned char> potential_win_discs_summary;

		class BoardSimulation
		{
		public:
			//START SIMULATION WITH (ACTIVEPLAYER-1) % 2 -> gets right player in first move iteration for negamax
			BoardSimulation(const placed_chips placedChips, const char currentPlayer, connect4::Move lastMove);
			~BoardSimulation();

			std::vector<Move> GetNextPlayersPossibleMoves(); //return possible input columns
			BoardSimulation MakeNextPlayersMove(/*const unsigned char& targetColumn*/ const Move& move) const;
			int EvaluatePlayerSituation(); //evaluate currents player situation
			bool IsGameOver(); //is a further move possible?

		private:
			placed_chips m_PlacedPlayerChips;
			std::vector<unsigned char> m_NextUsableRowInColumn;
			char m_ActivePlayer;
			connect4::Move m_LastMove;

			/*
				From Wikpedia:
				ROOT EVALUATION_SIGN PLAYER 1 = 1
				ROOT EVALUATION_SIGN PLAYER 2 = -1
			*/

			//char m_EvaluationSign;

			bool m_PlayerWon[2] {false, false};
			bool m_IsDraw = false;

			bool hasActivePlayerWon() const;
			bool hasOpponentPlayerWon() const;
			bool isDraw() const;

			//void eliminateUselessChips(placed_chips& placesChips);

			static void initBestChipsWithZero(std::vector<potential_win_discs_summary>& p);
			std::vector<potential_win_discs_summary> searchBestChips();
			void searchBestChipsHorizontal(std::vector<potential_win_discs_summary>& disc);
			void searchBestChipsVertical(std::vector<potential_win_discs_summary>& disc);
			void searchBestChipsDiagonalUpRight(std::vector<potential_win_discs_summary>& disc);
			void searchBestChipsDiagonalDownRight(std::vector<potential_win_discs_summary>& disc);

			int sumUpPotentialDiscs(potential_win_discs_summary& discs);
		};
	}
}
