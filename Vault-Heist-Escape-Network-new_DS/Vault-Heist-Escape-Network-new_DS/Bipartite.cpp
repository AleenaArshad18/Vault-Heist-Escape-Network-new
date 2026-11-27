#include "Bipartite.h"

using namespace std;

bool Bipartite::checkBipartite(const Graph& graph, int colours[]) const
{
    int vertexCount = graph.getVertexCount();

    int* queue = new int[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        colours[i] = -1;
    }

    // Check every connected component.

    for (int start = 0; start < vertexCount; start++)
    {
        if (colours[start] != -1)
        {
            continue;
        }

        int front = 0;
        int rear = 0;

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

                if (colours[neighbour] == -1)
                {
                    colours[neighbour] = 1 - colours[currentIndex];

                    queue[rear] = neighbour;
                    rear++;
                }
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