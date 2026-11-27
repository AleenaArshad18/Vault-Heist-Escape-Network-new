#include "BellmanFord.h"
#include <iostream>

using namespace std;

bool BellmanFord::findShortestPaths(const Graph& graph, int startVertexID, int distances[], int previous[]) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    int startIndex = graph.getVertexIndexByID(startVertexID);

    if (startIndex == -1)
    {
        cout << "Starting vertex not found!" << endl;
        return false;
    }

    const int INF = 1000000000;

    for (int i = 0; i < vertexCount; i++)
    {
        distances[i] = INF;
        previous[i] = -1;
    }

    distances[startIndex] = 0;

    // Relax all edges V-1 times

    for (int round = 1; round < vertexCount; round++)
    {
        bool updated = false;

        for (int i = 0; i < edgeCount; i++)
        {
            const Edge* edge = graph.getEdgeAt(i);

            int fromIndex = graph.getVertexIndexByID(edge->fromID);
            int toIndex = graph.getVertexIndexByID(edge->toID);

            // Relax the forward direction

            if (distances[fromIndex] != INF)
            {
                int newDistance = distances[fromIndex] + edge->weight;

                if (newDistance < distances[toIndex])
                {
                    distances[toIndex] = newDistance;
                    previous[toIndex] = fromIndex;
                    updated = true;
                }
            }

            // Relax reverse direction for undirected edges

            if (!edge->directed && distances[toIndex] != INF)
            {
                int newDistance = distances[toIndex] + edge->weight;

                if (newDistance < distances[fromIndex])
                {
                    distances[fromIndex] = newDistance;
                    previous[fromIndex] = toIndex;
                    updated = true;
                }
            }
        }

        if (!updated)
        {
            break;
        }
    }

    // Check for a reachable negative-weight cycle

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        int fromIndex = graph.getVertexIndexByID(edge->fromID);
        int toIndex = graph.getVertexIndexByID(edge->toID);

        if (distances[fromIndex] != INF)
        {
            if (distances[fromIndex] + edge->weight < distances[toIndex])
            {
                cout << "Negative-weight cycle detected!" << endl;
                return false;
            }
        }

        if (!edge->directed && distances[toIndex] != INF)
        {
            if (distances[toIndex] + edge->weight < distances[fromIndex])
            {
                cout << "Negative-weight cycle detected!" << endl;
                return false;
            }
        }
    }

    return true;
}