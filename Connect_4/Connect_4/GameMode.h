#pragma once
#include "stdafx.h"
#include "DiscHoles.h"

namespace connect4
{
	typedef std::vector<std::vector<char>> placed_discs;
	struct Move
	{
		Move()
		{
			this->row = None.row;
			this->column = None.column;
		}
		Move(const unsigned char& row, const unsigned char& column)
		{
			this->row = row;
			this->column = column;
		}
		unsigned char row;
		unsigned char column;

		static Move None;
	};

	class GameMode
	{
	public:
		enum class Modes
		{
			PlayerVsPlayer,
			PlayerVsAi,
			AiVsAi
		};

		virtual void CheckInputEvent(const sf::Event& event, DiscHoles& chipHoles) = 0;
		unsigned char GetChoosenColumn() const;
		sf::Color GetActivePlayerColor() const;
		bool HasSomebodyWon(sf::Color& outWinColor) const;
		virtual ~GameMode();

		static bool CheckForWin(const Move& lastMove, const placed_discs& placedPlayerChips, const char& activePlayer);
	protected:
		GameMode();

		bool discInput(const char& column, DiscHoles& board);
		virtual void discInputPlayer(const sf::Event& event, DiscHoles& chipHoles);
		virtual void discInputAi(DiscHoles& chipHoles);

		void initPlayerDiscs();

		char m_ActivePlayer = 0;
		bool m_HasSomebodyWon = false;
		char m_ChoosenColumn = 3;
		placed_discs m_PlacedDiscs;
		Move m_LastMove = Move(0,0);
	private:
		void saveDiscInputForPlayer(const Move& move);
		static bool checkForVerticalWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer);
		static bool checkForHorizontalWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer);
		static bool checkForDiagonalUpRightWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer); //checks '/'
		static bool checkForDiagonalDownRightWin(const Move& lastMove, const std::vector<std::vector<char>>& placedPlayerChips, const char& activePlayer); //checks '\'
		void setActivePlayerToWinner();
		static bool winCheckHelperForHorizontalAndVertical(char clampValue, const char& clampMax, std::function<bool(const char&)>& func);
		static bool winCheckHelperForDiagonal(Move& lastMove, std::function<void(Move&)>& perIterationChange, std::function<bool(const Move&)>& iterationIfCond);

		sf::Color m_WinColor = sf::Color::White;
	};
}

