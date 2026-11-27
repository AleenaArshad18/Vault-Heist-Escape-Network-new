
#include "BellmanFord.h"
#include <iostream>

using namespace std;

bool BellmanFord::findShortestPaths(const Graph& graph, int startVertexID, int distances[], int previous[]) const
{
    const int INF = 999999;

    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    // Initialize output arrays.

    for (int i = 0; i < vertexCount; i++)
    {
        distances[i] = INF;
        previous[i] = -1;
    }

    // Validate starting vertex.

    int startIndex = graph.getVertexIndexByID(startVertexID);

    if (startIndex == -1)
    {
        cout << "Starting vertex not found!" << endl;
        return false;
    }

    distances[startIndex] = 0;

    // Relax all edges V-1 times.

    for (int round = 1; round < vertexCount; round++)
    {
        bool updated = false;

        for (int i = 0; i < edgeCount; i++)
        {
            const Edge* edge = graph.getEdgeAt(i);

            int fromIndex = graph.getVertexIndexByID(edge->fromID);
            int toIndex = graph.getVertexIndexByID(edge->toID);

            // Relax forward direction.

            if (distances[fromIndex] != INF)
            {
                long long newDistance = (long long)distances[fromIndex] + edge->weight;

                if (newDistance < distances[toIndex] && newDistance > -INF)
                {
                    distances[toIndex] = (int)newDistance;
                    previous[toIndex] = fromIndex;
                    updated = true;
                }
            }

            // Relax reverse direction for undirected edges.

            if (!edge->directed && distances[toIndex] != INF)
            {
                long long newDistance = (long long)distances[toIndex] + edge->weight;

                if (newDistance < distances[fromIndex] && newDistance > -INF)
                {
                    distances[fromIndex] = (int)newDistance;
                    previous[fromIndex] = toIndex;
                    updated = true;
                }
            }
        }

        // Stop early if no distances changed.

        if (!updated)
        {
            break;
        }
    }

    // Check for reachable negative-weight cycles.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        int fromIndex = graph.getVertexIndexByID(edge->fromID);
        int toIndex = graph.getVertexIndexByID(edge->toID);

        if (distances[fromIndex] != INF)
        {
            long long newDistance = (long long)distances[fromIndex] + edge->weight;

            if (newDistance < distances[toIndex])
            {
                cout << "Negative-weight cycle detected!" << endl;
                return false;
            }
        }

        if (!edge->directed && distances[toIndex] != INF)
        {
            long long newDistance = (long long)distances[toIndex] + edge->weight;

            if (newDistance < distances[fromIndex])
            {
                cout << "Negative-weight cycle detected!" << endl;
                return false;
            }
        }
    }

    return true;
}
