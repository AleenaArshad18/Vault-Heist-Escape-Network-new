#include "TopologicalSort.h"

using namespace std;

bool TopologicalSort::kahnSort(const Graph& graph, int result[], int& resultCount) const
{
    int vertexCount = graph.getVertexCount();

    resultCount = 0;

    if (vertexCount == 0)
    {
        return false;
    }

    int* indegree = new int[vertexCount];
    int* queue = new int[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        indegree[i] = 0;
    }

    // Calculate indegree of every vertex.

    for (int i = 0; i < vertexCount; i++)
    {
        const EdgeNode* current = graph.getAdjacencyHead(i);

        while (current != nullptr)
        {
            indegree[current->destinationIndex]++;
            current = current->next;
        }
    }

    int front = 0;
    int rear = 0;

    // Add all vertices with indegree zero.

    for (int i = 0; i < vertexCount; i++)
    {
        if (indegree[i] == 0)
        {
            queue[rear] = i;
            rear++;
        }
    }

    // Process vertices.

    while (front < rear)
    {
        int currentIndex = queue[front];
        front++;

        const Vertex* vertex = graph.getVertexAt(currentIndex);

        result[resultCount] = vertex->id;
        resultCount++;

        const EdgeNode* current = graph.getAdjacencyHead(currentIndex);

        while (current != nullptr)
        {
            int neighbour = current->destinationIndex;

            indegree[neighbour]--;

            if (indegree[neighbour] == 0)
            {
                queue[rear] = neighbour;
                rear++;
            }

            current = current->next;
        }
    }

    delete[] indegree;
    delete[] queue;

    return resultCount == vertexCount;
}