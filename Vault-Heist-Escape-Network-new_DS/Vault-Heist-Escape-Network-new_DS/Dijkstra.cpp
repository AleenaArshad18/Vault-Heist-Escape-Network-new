#include "Dijkstra.h"
#include <iostream>

using namespace std;

void Dijkstra::findShortestPaths(const Graph& graph, int startVertexID, int distances[], int previous[]) const
{
    int vertexCount = graph.getVertexCount();

    int startIndex = graph.getVertexIndexByID(startVertexID);

    if (startIndex == -1)
    {
        cout << "Starting vertex not found!" << endl;
        return;
    }

    if (graph.hasNegativeWeight())
    {
        cout << "Dijkstra cannot work with negative weights!" << endl;
        return;
    }

    bool* visited = new bool[vertexCount];

    const int INF = 1000000000;

    for (int i = 0; i < vertexCount; i++)
    {
        distances[i] = INF;
        previous[i] = -1;
        visited[i] = false;
    }

    distances[startIndex] = 0;

    for (int count = 0; count < vertexCount; count++)
    {
        int minimumDistance = INF;
        int currentIndex = -1;

        for (int i = 0; i < vertexCount; i++)
        {
            if (!visited[i] && distances[i] < minimumDistance)
            {
                minimumDistance = distances[i];
                currentIndex = i;
            }
        }

        if (currentIndex == -1)
        {
            break;
        }

        visited[currentIndex] = true;

        const EdgeNode* current = graph.getAdjacencyHead(currentIndex);

        while (current != nullptr)
        {
            int neighbourIndex = current->destinationIndex;

            if (!visited[neighbourIndex])
            {
                int newDistance = distances[currentIndex] + current->weight;

                if (newDistance < distances[neighbourIndex])
                {
                    distances[neighbourIndex] = newDistance;
                    previous[neighbourIndex] = currentIndex;
                }
            }

            current = current->next;
        }
    }

    delete[] visited;
}