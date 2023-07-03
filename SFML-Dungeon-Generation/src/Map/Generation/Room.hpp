#pragma once
#include <SFML/System/Vector2.hpp>
#include "../../Utility/Constants.hpp"

class Room
{
public:
	Room(const int t_x, const int t_y, const int t_width, const int t_height) : x(t_x), y(t_y), width(t_width), height(t_height)
	{
		center = sf::Vector2i((x + width / 2.0) * TILE_SIZE, (y + height / 2.0) * TILE_SIZE);
	}
	int x, y, width, height;
	sf::Vector2i center;
};

