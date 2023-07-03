#include "Map.hpp"
#include <cmath>

Map::Map(const size_t t_mapWidthTiles, const size_t t_mapHeightTile)
{
	m_widthTiles = t_mapWidthTiles;
	m_heightTiles = t_mapHeightTile;
	m_tiles.resize(m_widthTiles, std::vector<Tile>(t_mapHeightTile));
}

size_t Map::getMapWidth() const
{
	return m_widthTiles;
}

size_t Map::getMapHeight() const
{
	return m_heightTiles;
}

void Map::setTile(const int t_x, const int t_y, const Tile& t_tile)
{
	m_tiles[t_y][t_x] = t_tile;
}

const Tile& Map::getTile(const int t_x, const int t_y) const
{
	return m_tiles[t_y][t_x];
}

bool Map::checkCollision(const int t_x, const int t_y) const
{
	return m_tiles[t_y][t_x].isSolid();
}

void Map::computeVisibility(const int t_x, const int t_y, const int t_radius)
{

	// Compute visibility
	for (int i = std::max(0, t_x - t_radius); i <= std::min(static_cast<int>(m_widthTiles) - 1, t_x + t_radius); ++i)
	{
		for (int j = std::max(0, t_y - t_radius); j <= std::min(static_cast<int>(m_widthTiles) - 1, t_y + t_radius); ++j)
		{
			// Simple Euclidean distance for visibility
			if (std::hypot(i - t_x, j - t_y) <= t_radius)
			{
				m_tiles[j][i].setSeen();
			}
		}
	}
}

std::vector<std::pair<int, int>> Map::getPath(int t_startX, int t_startY, int t_endX, int t_endY)
{
	return {};
}

void Map::draw(sf::RenderTarget& t_target) const
{
	for (auto& row : m_tiles)
{
		for (auto& tile : row)
		{
			tile.draw(t_target);
		}
	}
}