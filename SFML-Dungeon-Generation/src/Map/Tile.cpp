#include "Tile.hpp"
#include "../Utility/Constants.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Tile::Tile() = default;

Tile::Tile(const float t_x, const float t_y)
{
	m_sprite.setPosition(t_x, t_y);
	m_sprite.setFillColor(sf::Color(0, 128, 128, 255));
	m_sprite.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	m_solid = false;
	m_beenSeen = false;
	m_type = EType::EMPTY;
}

Tile::Tile(const float t_x, const float t_y, const sf::Texture* t_texture, const bool t_isSolid, const bool t_isVisited, const EType t_tileType)
{
	m_sprite.setPosition(t_x, t_y);
	m_sprite.setTexture(t_texture);
	m_solid = t_isSolid;
	m_beenSeen = t_isVisited;
	m_type = t_tileType;
}

void Tile::draw(sf::RenderTarget& t_target) const
{
	t_target.draw(m_sprite);
}

bool Tile::isSolid() const
{
	return m_solid;
}

bool Tile::hasBeenSeen() const
{
	return m_beenSeen;
}

void Tile::setSeen()
{
	m_beenSeen = true;
}
