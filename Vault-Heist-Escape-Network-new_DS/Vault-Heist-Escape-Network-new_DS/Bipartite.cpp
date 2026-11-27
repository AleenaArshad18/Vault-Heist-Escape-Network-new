
#include "Bipartite.h"
#include <iostream>

using namespace std;

bool Bipartite::checkBipartite(const Graph& graph, int colours[]) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    // Initialize all vertices as uncoloured.

    for (int i = 0; i < vertexCount; i++)
    {
        colours[i] = -1;
    }

    // Bipartite checking requires an undirected graph.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (edge->directed)
        {
            cout << "Bipartite checking requires an undirected graph!" << endl;
            return false;
        }
    }

    // Create a queue for BFS.

    int* queue = new int[vertexCount];

    // Check every connected component.

    for (int start = 0; start < vertexCount; start++)
    {
        if (colours[start] != -1)
        {
            continue;
        }

        int front = 0;
        int rear = 0;

        // Assign the starting vertex colour 0.

        colours[start] = 0;

        queue[rear] = start;
        rear++;

        while (front < rear)
        {
            int currentIndex = queue[front];
            front++;

            const EdgeNode* current = graph.getAdjacencyHead(currentIndex);

            while (current != nullptr)
            {
                int neighbour = current->destinationIndex;

                // If neighbour is uncoloured, assign opposite colour.

                if (colours[neighbour] == -1)
                {
                    colours[neighbour] = 1 - colours[currentIndex];

                    queue[rear] = neighbour;
                    rear++;
                }

                // Adjacent vertices cannot have the same colour.

                else if (colours[neighbour] == colours[currentIndex])
                {
                    delete[] queue;
                    return false;
                }

                current = current->next;
            }
        }
    }

    delete[] queue;

    return true;
}
