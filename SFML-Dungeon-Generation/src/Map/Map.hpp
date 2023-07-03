#pragma once
#include <vector>
#include "Tile.hpp"

class Map {
public:
	Map() = default;
	Map(size_t t_mapWidthTiles, size_t t_mapHeightTile);

	[[nodiscard]] size_t getMapWidth() const;
	[[nodiscard]] size_t getMapHeight() const;

	void setTile(int t_x, int t_y, const Tile& t_tile);

	[[nodiscard]] const Tile& getTile(int t_x, int t_y) const;
	[[nodiscard]] bool checkCollision(int t_x, int t_y) const;

	// Render
	void draw(sf::RenderTarget& t_target) const;

	// Fog of war & path finding
	void computeVisibility(int t_x, int t_y, int t_radius);
	std::vector<std::pair<int, int>> getPath(int t_startX, int t_startY, int t_endX, int t_endY);

private:
	std::vector<std::vector<Tile>> m_tiles;
	size_t m_widthTiles{};
	size_t m_heightTiles{};
};