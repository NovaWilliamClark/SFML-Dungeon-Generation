#pragma once
#include <SFML/Graphics/RectangleShape.hpp>

class Tile {
public:
	enum class EType
	{
		/*Dungeon*/
		EMPTY,
		FLOOR,
		CORRIDOR,
		WALL,
		STAIRS,
	};


	Tile();
	Tile(float t_x, float t_y);
	Tile(float t_x, float t_y, const sf::Texture* t_texture, bool t_isSolid, bool t_isVisited = false, EType t_tileType = EType::EMPTY);
	void draw(sf::RenderTarget& t_target) const;
	bool isSolid() const;
	bool hasBeenSeen() const;
	void setSeen();

private:
	sf::RectangleShape m_sprite{};
	bool m_solid{};
	bool m_beenSeen{};
	EType m_type{};
};
