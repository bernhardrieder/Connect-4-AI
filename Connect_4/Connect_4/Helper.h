#pragma once

template<typename T>
static T Clamp(const T& x, const T& low, const T& high)
{
	return x < low ? low : (x > high ? high : x);
}

inline std::string to_string(sf::Color color)
{
	if (color == sf::Color::White)
		return "White";
	if (color == sf::Color::Black)
		return "Black";
	if (color == sf::Color::Blue)
		return "Blue";
	if (color == sf::Color::Cyan)
		return "Cyan";
	if (color == sf::Color::Green)
		return "Green";
	if (color == sf::Color::Magenta)
		return "Magenta";
	if (color == sf::Color::Red)
		return "Red";
	if (color == sf::Color::Yellow)
		return "Yellow";
	if (color == sf::Color::Transparent)
		return "Transparent";
}

inline int RandomInt(int min, int max)
{
	return min + (rand() % static_cast<int>(max - min + 1));
}