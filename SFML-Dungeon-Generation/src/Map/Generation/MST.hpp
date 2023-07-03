#pragma once
#include <algorithm>
#include <vector>

class Room;

struct Edge {
	int pointA;
	int pointB;
	float weight;

	Edge(const int t_pointA, const int t_pointB, const float t_weight)
	{
		pointA = t_pointA;
		pointB = t_pointB;
		weight = t_weight;
	}

	// Add the == binary operator to compare two Edge instances
	bool operator==(const Edge& t_other) const {
		// Check if both pointA and pointB are the same in both instances (considering both possible orders)
		return (pointA == t_other.pointA && pointB == t_other.pointB) || (pointA == t_other.pointB && pointB == t_other.pointA);
	}
};

class MinimumSpanningTree
{
public:
	//MST helper functions
	static int find(std::vector<int>& t_parent, int t_i);
	static void unionSets(std::vector<int>& t_parent, int t_x, int t_y);

	static std::vector<Edge> generateMST(std::vector<Edge> t_delaunayEdges, const std::vector<Room>& t_rooms);

private:
	void drawDebug();
};

