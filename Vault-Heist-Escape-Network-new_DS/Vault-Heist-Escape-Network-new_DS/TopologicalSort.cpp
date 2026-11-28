
#include "TopologicalSort.h"
#include "EventRecorder.h"
#include <iostream>

using namespace std;

bool TopologicalSort::kahnSort(const Graph& graph, int result[], int& resultCount) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    resultCount = 0;

    // An empty graph cannot be sorted.

    if (vertexCount == 0)
    {
        return false;
    }

    // Topological Sort requires every edge to be directed.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (!edge->directed)
        {
            cout << "Topological Sort requires a directed graph!" << endl;
            return false;
        }
    }

    // Create arrays for indegree and queue.

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

    // If not all vertices were processed, the graph has a cycle.

    if (resultCount != vertexCount)
    {
        cout << "Graph contains a cycle. Topological sorting is not possible." << endl;
        return false;
    }

    return true;
}
