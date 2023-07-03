#pragma once

#include <SFML/Graphics.hpp>

#include "Room.hpp"
#include "MST.hpp"
#include "Map/Tile.hpp"

class Map;

class DungeonGenerator
{
public:
	enum class EPathType
	{
		STRAIGHT,
		CORRIDOR,
		WINDING_ROAD,
	};

	static DungeonGenerator& getInstance(Map& t_map)
	{
		static DungeonGenerator instance(t_map);
		return instance;
	}

	void generate();
	void generateViaSetSeed();
	[[nodiscard]] bool isDungeonGenerating() const;

private:
	explicit DungeonGenerator(Map& t_map);
	~DungeonGenerator();

	bool m_dungeonIsGeneration{};

	Map* m_map = nullptr;

	size_t m_dungeonTileWidth{};
	size_t m_dungeonTileHeight{};
	size_t m_roomSeparation = 3;

	Tile::EType m_empty = Tile::EType::EMPTY;
	Tile::EType m_floor = Tile::EType::FLOOR;
	Tile::EType m_corridor = Tile::EType::CORRIDOR;
	Tile::EType m_wall = Tile::EType::WALL;

	std::vector<Room> m_rooms;
	std::vector<Edge> m_delaunayEdges;
	std::vector<Edge> m_mstEdges;

	[[nodiscard]] bool overlapping(const Room& t_a, const Room& t_b) const; // Checks if two rooms overlap
	[[nodiscard]] bool tooCloseToEdge(const Room& t_room) const; // Checks if a room is too close to the edge of the dungeon

	void generateRooms();
	void convertRoomToTiles(const Room& t_room) const;
	void generateDelaunay();
	void generateMST();
	void addLoopsToMST();
	void generateCorridors();
	void generateStraightCorridor(const Room& t_roomA, const Room& t_roomB) const;
	void generateWallHuggingCorridor(const Room& t_roomA, const Room& t_roomB) const;
	void generateWindingRoad(const Room& t_roomA, const Room& t_roomB) const;

	// DEBUG
	bool m_debugMode{};
	std::vector<sf::VertexArray> m_delaunayDebugEdges;
	std::vector<sf::VertexArray> m_mstDebugEdges;
	std::vector<sf::VertexArray> m_loopDebugEdges;
	std::vector<std::vector<Tile>> m_tiles;

	void drawRoomPlacement(const Room& t_room, bool t_valid) const;
	void drawTriangulation(const Edge& t_edge);
	void drawMST(const Edge& t_edge);
	void drawLoopEdges(const Edge& t_edge);
	void drawCorridorStep(const Tile& t_corridorTile) const;
	void drawRoomStep(const Tile& t_roomTile) const;
	std::vector<std::vector<Tile>>& getTiles();
};
