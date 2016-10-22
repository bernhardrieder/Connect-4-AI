#include "stdafx.h"

using namespace connect4;

sf::Font connect4::GlobalVariables::m_TextFont;
unsigned char connect4::GlobalVariables::m_RowCount = 6;
unsigned char connect4::GlobalVariables::m_ColumnCount = 7;
sf::Color connect4::GlobalVariables::m_PlayerColors[2] = {sf::Color::Yellow, sf::Color::Red};
unsigned char GlobalVariables::m_WinAmount = 4;
std::string GlobalVariables::m_FontFile = "arial.ttf";
