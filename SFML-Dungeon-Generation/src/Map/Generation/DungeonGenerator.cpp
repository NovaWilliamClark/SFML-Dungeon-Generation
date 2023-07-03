#include "DungeonGenerator.hpp"
#include "Utility/Random.hpp"
#include "Utility/Constants.hpp"
#include "Map/Map.hpp"

#include <delaunator.hpp>

DungeonGenerator::DungeonGenerator(Map& t_map) : m_map(&t_map)
{
	m_dungeonTileWidth = m_map->getMapWidth();
	m_dungeonTileHeight = m_map->getMapHeight();
}

DungeonGenerator::~DungeonGenerator()
= default;

void DungeonGenerator::generate()
{
	// Reset Dungeon

	// Reset to a new seed
	Random::getInstance().reset();

	generateRooms();
	generateDelaunay();
	generateMST();
	addLoopsToMST();
	generateCorridors();
}

void DungeonGenerator::generateViaSetSeed()
{
	// Does not reset seed

	// Reset Dungeon
	generateRooms();
	generateDelaunay();
	generateMST();
	addLoopsToMST();
	generateCorridors();
}

bool DungeonGenerator::isDungeonGenerating() const
{
	return m_dungeonIsGeneration;
}

bool DungeonGenerator::overlapping(const Room& t_a, const Room& t_b) const
{
	return t_a.x < t_b.x + t_b.width + m_roomSeparation &&
		t_a.x + t_a.width + m_roomSeparation > t_b.x &&
		t_a.y < t_b.y + t_b.height + m_roomSeparation &&
		t_a.y + t_a.height + m_roomSeparation > t_b.y;
}

bool DungeonGenerator::tooCloseToEdge(const Room& t_room) const
{
	return t_room.x <= m_roomSeparation || t_room.y <= m_roomSeparation ||
		t_room.x + t_room.width >= m_dungeonTileWidth - m_roomSeparation ||
		t_room.y + t_room.height >= m_dungeonTileHeight - m_roomSeparation;
}

void DungeonGenerator::generateRooms()
{
	// generate a number of rooms
	const size_t roomCount = Random::getInstance().generateInt(3, 20);
	size_t failedAttempts = 0;
	constexpr size_t MAX_FAILED_ATTEMPTS = 50;


	auto isValidRoom = [&](const Room& t_newRoom)
	{
		// Not a valid room if its too close to the dungeons edges
		if (tooCloseToEdge(t_newRoom))
		{
			return false;
		}

		// Returns true if the room does not overlap with any other rooms
		return std::ranges::none_of(m_rooms, [&](const Room& t_room)
			{
				return overlapping(t_newRoom, t_room);
			});
	};

	while (m_rooms.size() < roomCount && failedAttempts < MAX_FAILED_ATTEMPTS)
	{
		// Generate a new room
		const int width = Random::getInstance().generateInt(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
		const int height = Random::getInstance().generateInt(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
		const int x = Random::getInstance().generateInt(0, m_dungeonTileWidth - MIN_ROOM_SIZE);
		const int y = Random::getInstance().generateInt(0, m_dungeonTileHeight - MIN_ROOM_SIZE);
		Room newRoom(x, y, width, height);
		// Check if the room is valid
		if (isValidRoom(newRoom))
		{
			// Add the room to the list of rooms
			m_rooms.push_back(newRoom);
			failedAttempts = 0;

			// TODO: DEBUG MODE

			convertRoomToTiles(newRoom);
			failedAttempts = 0;
		}
		else
		{
			++failedAttempts;
			// TODO: DEBUG MODE
		}
	}
}

void DungeonGenerator::convertRoomToTiles(const Room& t_room) const
{
	for (int y = t_room.y; y < t_room.y + t_room.height; ++y)
	{
		for (int x = t_room.x; x < t_room.x + t_room.width; ++x)
		{
			m_map->setTile(y, x, Tile(x * TILE_SIZE, y * TILE_SIZE));
		}
	}
}

void DungeonGenerator::generateDelaunay()
{
	std::vector<double> roomCenters;
	// Extract room centers and populate input vector
	for (const auto& room : m_rooms)
	{
		roomCenters.push_back(room.center.x);
		roomCenters.push_back(room.center.y);
	}

	const delaunator::Delaunator delaunay(roomCenters);
	for (std::size_t i = 0; i < delaunay.triangles.size(); i += 3) {
		for (int j = 0; j < 3; ++j) {
			const std::size_t p1Idx = delaunay.triangles[i + j];
			const std::size_t p2Idx = delaunay.triangles[i + ((j + 1) % 3)];

			const float dx = roomCenters[2 * p1Idx] - roomCenters[2 * p2Idx];
			const float dy = roomCenters[2 * p1Idx + 1] - roomCenters[2 * p2Idx + 1];
			const float weight = std::sqrt(dx * dx + dy * dy);

			Edge edge(static_cast<int>(p1Idx), static_cast<int>(p2Idx), weight);
			m_delaunayEdges.push_back(edge);

			//TODO:: DEBUG MODE
		}
	}
}

void DungeonGenerator::generateMST()
{
	m_mstEdges = MinimumSpanningTree::generateMST(m_delaunayEdges, m_rooms);
}

void DungeonGenerator::addLoopsToMST()
{
	// Loop through the remaining edges from the triangulation
	for (const auto& edge : m_delaunayEdges) {

		const float num = Random::getInstance().generateFloat(0.f, 1.f);
		// If the edge is not in the MST and has a chance, add it to the MST to create a loop
		const float chance = m_rooms.size() > 6 ? 0.15f : 0.1f;
	
		// Check if the edge is already in the MST
		const bool alreadyInMST = std::ranges::find(m_mstEdges, edge) != m_mstEdges.end();
		if (!alreadyInMST && num < chance) {
			m_mstEdges.push_back(edge);
		}
	}
}

void DungeonGenerator::generateCorridors()
{
	
	for (const Edge& edge : m_mstEdges)
	{
		// Get the rooms to connect
		Room& roomA = m_rooms[edge.pointA];
		Room& roomB = m_rooms[edge.pointB];
	
		// Decide on a corridor type
		EPathType pathType;
		const float roll = Random::getInstance().generateFloat(0.f, 1.f);
		if (roll < 0.33f)
		{
			pathType = EPathType::STRAIGHT;
		}
		else if (roll < 0.66f)
		{
			pathType = EPathType::CORRIDOR;
		}
		else
		{
			pathType = EPathType::WINDING_ROAD;
		}

		// Generate the corridor
		switch (pathType)
		{
		case EPathType::STRAIGHT:
			generateStraightCorridor(roomA, roomB);
			break;
		case EPathType::CORRIDOR:
			generateWallHuggingCorridor(roomA, roomB);
			break;
		case EPathType::WINDING_ROAD:
			generateWindingRoad(roomA, roomB);
			break;
		}
	}
}

void DungeonGenerator::generateStraightCorridor(const Room& t_roomA, const Room& t_roomB) const
{
	const int xA = t_roomA.center.x / TILE_SIZE;
	const int yA = t_roomA.center.y / TILE_SIZE;

	const int xB = t_roomB.center.x / TILE_SIZE;
	const int yB = t_roomB.center.y / TILE_SIZE;

	const int xSign = xA < xB ? 1 : -1;
	const int ySign = yA < yB ? 1 : -1;
	
	for (int x = xA; x != xB; x += xSign) {

		Tile tile(x * TILE_SIZE, yA * TILE_SIZE);
		m_map->setTile(yA, x, tile);

	}
	for (int y = yA; y != yB; y += ySign) {

		Tile tile(xB * TILE_SIZE, y * TILE_SIZE);
		m_map->setTile(y, xB, tile);
	}
}

void DungeonGenerator::generateWallHuggingCorridor(const Room& t_roomA, const Room& t_roomB) const
{
	const int xA = t_roomA.center.x / TILE_SIZE;
	const int yA = t_roomA.center.y / TILE_SIZE;

	const int xB = t_roomB.center.x / TILE_SIZE;
	const int yB = t_roomB.center.y / TILE_SIZE;

	const int xSign = xA < xB ? 1 : -1;
	const int ySign = yA < yB ? 1 : -1;

	// First, connect horizontally along the x-axis
	for (int x = xA; x != xB; x += xSign) {
		Tile tile(x * TILE_SIZE, yA * TILE_SIZE);
		m_map->setTile(yA, x, tile);
	}

	// Then connect vertically along the y-axis
	for (int y = yA; y != yB; y += ySign) {
		Tile tile(xB * TILE_SIZE, y * TILE_SIZE);
		m_map->setTile(y, xB, tile);
	}
}

void DungeonGenerator::generateWindingRoad(const Room& t_roomA, const Room& t_roomB) const
{
	int xA = t_roomA.center.x / TILE_SIZE;
	int yA = t_roomA.center.y / TILE_SIZE;

	const int xB = t_roomB.center.x / TILE_SIZE;
	const int yB = t_roomB.center.y / TILE_SIZE;

	while (xA != xB || yA != yB)
	{
		const int dx = xB - xA;
		const int dy = yB - yA;

		// Decide whether to move in X or Y direction
		if (Random::getInstance().generateInt(0, 1) == 0)
		{
			xA += dx > 0 ? 1 : -1;
		}
		else
		{
			yA += dy > 0 ? 1 : -1;
		}

		// Create the tile
		Tile tile(xA * TILE_SIZE, yA * TILE_SIZE);
		m_map->setTile(yA, xA, tile);
	}
}


/************************************************************************/
/*                             DEBUG FUNCTION                           */
/************************************************************************/

void DungeonGenerator::drawRoomPlacement(const Room& t_room, bool t_valid) const
{
	// TODO
}

void DungeonGenerator::drawTriangulation(const Edge& t_edge)
{
    // TODO
}

void DungeonGenerator::drawMST(const Edge& t_edge)
{
    // TODO
}

void DungeonGenerator::drawLoopEdges(const Edge& t_edge)
{
    // TODO
}

void DungeonGenerator::drawCorridorStep(const Tile& t_corridorTile) const
{
    // TODO
}

void DungeonGenerator::drawRoomStep(const Tile& t_roomTile) const
{
    // TODO
}