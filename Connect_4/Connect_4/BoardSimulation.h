#pragma once
#include "stdafx.h"
#include "GameMode.h"

namespace connect4
{
	typedef std::vector<std::vector<char>> placed_discs;
	namespace ai
	{
		typedef std::map<unsigned char, unsigned char> potential_win_discs_summary;

		class BoardSimulation
		{
		public:
			//START SIMULATION WITH (ACTIVEPLAYER-1) % 2 -> gets right player in first move iteration for negamax
			BoardSimulation(const placed_discs& placedDiscs, const char& currentPlayer, const Move& lastMove);
			~BoardSimulation();

			std::vector<Move> GetNextPlayersPossibleMoves(); //return possible input columns
			BoardSimulation MakeNextPlayersMove(const Move& move);
			void Undo(const Move& move);
			int EvaluatePlayerSituation(); //evaluate currents player situation
			bool IsGameOver(); //is a further move possible?

		private:
			placed_discs m_PlacedDiscs;
			std::vector<unsigned char> m_NextUsableRowInColumn;
			char m_ActivePlayer;
			Move m_LastMove;

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

			std::vector<potential_win_discs_summary> searchPotentialWinDiscs();
			static void initPotentialWinDiscsWithZero(std::vector<potential_win_discs_summary>& p);
			void searchPotentialWinDiscsHorizontal(std::vector<potential_win_discs_summary>& disc, const char& playerNumber);
			void searchPotentialWinDiscsVertical(std::vector<potential_win_discs_summary>& disc, const char& playerNumber);
			void searchPotentialWinDiscsDiagonalUpRight(std::vector<potential_win_discs_summary>& disc, const char& playerNumber);
			void searchPotentialWinDiscsDiagonalDownRight(std::vector<potential_win_discs_summary>& disc, const char& playerNumber);
			void searchCounterHelper(std::vector<potential_win_discs_summary>& potentialDisc, const Move& move, const char& player, unsigned char& discCount, unsigned char& unusedCount);

			static int sumUpPotentialDiscs(potential_win_discs_summary& discs);
			void initializeNextUsableRows();
		};
	}
}
