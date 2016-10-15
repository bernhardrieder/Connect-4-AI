#pragma once
#include "stdafx.h"

namespace connect4
{
	class GlobalVariables
	{
	public:
		static sf::Font& GetTextFont();
		static int GetRowCount();
		static int GetColumnCount();
		static void GetPlayerColors(sf::Color& outColorPlayer1, sf::Color& outColorPlayer2);
		static int GetWinAmount();
	private:
		static sf::Font m_TextFont;
		static int m_RowCount;
		static int m_ColumnCount;
		static sf::Color m_PlayerColors[2];
		static int m_WinAmount;
		static std::string m_FontFile;
	};

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

	inline int GlobalVariables::GetRowCount()
	{
		return m_RowCount;
	}

	inline int GlobalVariables::GetColumnCount()
	{
		return m_ColumnCount;
	}

	inline void GlobalVariables::GetPlayerColors(sf::Color& outColorPlayer1, sf::Color& outColorPlayer2)
	{
		outColorPlayer1 = m_PlayerColors[0];
		outColorPlayer2 = m_PlayerColors[1];
	}

	inline int GlobalVariables::GetWinAmount()
	{
		return m_WinAmount;
	}
}
