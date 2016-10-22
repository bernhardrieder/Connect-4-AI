#pragma once
#include "stdafx.h"

namespace connect4
{
	class GlobalVariables
	{
	public:
		static sf::Font& GetTextFont();
		static unsigned char GetRowCount();
		static unsigned char GetColumnCount();
		static void GetPlayerColors(sf::Color& outColorPlayer1, sf::Color& outColorPlayer2);
		static sf::Color GetPlayerColor(char playerNumber);
		static unsigned char GetWinAmount();
	private:
		static sf::Font m_TextFont;
		static unsigned char m_RowCount;
		static unsigned char m_ColumnCount;
		static sf::Color m_PlayerColors[2];
		static unsigned char m_WinAmount;
		static std::string m_FontFile;
	};

	inline unsigned char GlobalVariables::GetWinAmount()
	{
		return m_WinAmount;
	}

	inline sf::Font& GlobalVariables::GetTextFont()
	{
		static bool needInit = true;
		if(needInit)
		{
			if (!m_TextFont.loadFromFile(m_FontFile))
				std::cerr << "could not load font: " << m_FontFile << std::endl;
			needInit = false;
		}
		return m_TextFont;
	}

	inline unsigned char GlobalVariables::GetRowCount()
	{
		return m_RowCount;
	}

	inline unsigned char GlobalVariables::GetColumnCount()
	{
		return m_ColumnCount;
	}

	inline void GlobalVariables::GetPlayerColors(sf::Color& outColorPlayer1, sf::Color& outColorPlayer2)
	{
		outColorPlayer1 = m_PlayerColors[0];
		outColorPlayer2 = m_PlayerColors[1];
	}

	inline sf::Color GlobalVariables::GetPlayerColor(char playerNumber)
	{
		return m_PlayerColors[playerNumber];
	}
}
