#pragma once
#include "stdafx.h"
#include "ChipHoles.h"

namespace connect4
{
	class GameMode
	{
	public:
		enum class Modes
		{
			PlayerVsPlayer,
			PlayerVsAi,
			AiVsAi
		};

		virtual void CheckInputEvent(const sf::Event& event, ChipHoles& chipHoles) = 0;
		bool HasSomebodyWon(sf::Color& outWinColor) const;
		virtual ~GameMode();

	protected:
		GameMode();

		void chipInput(int column, ChipHoles& board);
		void initPlayerChips();

	private:
		void saveChipInputForPlayer(int row, int column);
		bool checkForWin(int row, int column);
		bool checkForVerticalWin(int row, int column);
		bool checkForHorizontalWin(int row, int column);
		bool checkForDiagonalUpRightWin(int row, int column); //checks '/'
		bool checkForDiagonalDownRightWin(int row, int column); //checks '\'
		void setActivePlayerToWinner();
		//http://de.cppreference.com/w/cpp/language/lambda
		static bool winCheckHelperForHorizontalAndVertical(int clampValue, int clampMax, std::function<bool(int)> func);
		bool winCheckHelperForDiagonal(int row, int column, std::function<void(int&, int&)> funcForRowAndColumnPerIteration);

		sf::Color m_PlayerColors[2];
		int m_ActivePlayer = 0;
		bool m_HasSomebodyWon = false;
		sf::Color m_WinColor = sf::Color::White;

		std::vector<std::vector<bool>> m_PlayerChips[2];
	};
}

