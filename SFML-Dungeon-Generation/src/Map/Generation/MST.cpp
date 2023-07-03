#include "MST.hpp"
#include "../../Map/Generation/Room.hpp"

int MinimumSpanningTree::find(std::vector<int>& t_parent, const int t_i)
{
    if (t_parent[t_i] == -1) {
        return t_i;
    }
    return find(t_parent, t_parent[t_i]);
}

void MinimumSpanningTree::unionSets(std::vector<int>& t_parent, const int t_x, const int t_y)
{
    const int xSet = find(t_parent, t_x);
    const int ySet = find(t_parent, t_y);
    t_parent[xSet] = ySet;
}

std::vector<Edge> MinimumSpanningTree::generateMST(std::vector<Edge> t_delaunayEdges, const std::vector<Room>& t_rooms)
{
    //Sort edges by weight
    std::ranges::sort(t_delaunayEdges, [](const Edge& t_a, const Edge& t_b) {return t_a.weight < t_b.weight;});

    // Create subsets for each vertex
    std::vector parent(t_rooms.size(), -1);

    std::vector<Edge> mstEdges;

    for (const auto& edge : t_delaunayEdges) {
        const int u = find(parent, edge.pointA);
        const int v = find(parent, edge.pointB);

        if (u != v) {
            mstEdges.push_back(edge);
            unionSets(parent, u, v);

            //if (m_debugMode)
            //{
            //	drawMST(edge);
            //}
        }
    }

    return mstEdges;
}

void MinimumSpanningTree::drawDebug()
{}
