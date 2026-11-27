
#include "Dijkstra.h"
#include <iostream>

using namespace std;

bool Dijkstra::findShortestPaths(const Graph& graph, int startVertexID, int distances[], int previous[]) const
{
    const int INF = 1000000000;

    int vertexCount = graph.getVertexCount();

    // Initialize output arrays first.

    for (int i = 0; i < vertexCount; i++)
    {
        distances[i] = INF;
        previous[i] = -1;
    }

    // Find the starting vertex.

    int startIndex = graph.getVertexIndexByID(startVertexID);

    if (startIndex == -1)
    {
        cout << "Starting vertex not found!" << endl;
        return false;
    }

    // Dijkstra does not support negative edge weights.

    if (graph.hasNegativeWeight())
    {
        cout << "Dijkstra cannot work with negative weights!" << endl;
        return false;
    }

    bool* visited = new bool[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    distances[startIndex] = 0;

    // Process vertices one by one.

    for (int count = 0; count < vertexCount; count++)
    {
        int minimumDistance = INF;
        int currentIndex = -1;

        // Find the unvisited vertex with the smallest distance.

        for (int i = 0; i < vertexCount; i++)
        {
            if (!visited[i] && distances[i] < minimumDistance)
            {
                minimumDistance = distances[i];
                currentIndex = i;
            }
        }

        // Stop if remaining vertices are unreachable.

        if (currentIndex == -1)
        {
            break;
        }

        visited[currentIndex] = true;

        // Explore adjacent vertices.

        const EdgeNode* current = graph.getAdjacencyHead(currentIndex);

        while (current != nullptr)
        {
            int neighbourIndex = current->destinationIndex;

            if (!visited[neighbourIndex])
            {
                // Prevent integer overflow.

                if (current->weight <= INF - distances[currentIndex])
                {
                    int newDistance = distances[currentIndex] + current->weight;

                    if (newDistance < distances[neighbourIndex])
                    {
                        distances[neighbourIndex] = newDistance;
                        previous[neighbourIndex] = currentIndex;
                    }
                }
            }

            current = current->next;
        }
    }

    delete[] visited;

    return true;
}
