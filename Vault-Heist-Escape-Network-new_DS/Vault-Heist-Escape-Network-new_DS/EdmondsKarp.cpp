#include "EdmondsKarp.h"

using namespace std;

bool EdmondsKarp::findMaxFlow(const Graph& graph, int sourceID, int sinkID,
    int& maxFlow, Edge minCutEdges[], int& minCutCount) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    maxFlow = 0;
    minCutCount = 0;

    int source = graph.getVertexIndexByID(sourceID);
    int sink = graph.getVertexIndexByID(sinkID);

    if (vertexCount == 0 || source == -1 || sink == -1 || source == sink)
    {
        return false;
    }

    // Edmonds-Karp requires directed edges.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (!edge->directed || edge->capacity < 0)
        {
            return false;
        }
    }

    // Create the residual capacity matrix.

    int** residual = new int* [vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        residual[i] = new int[vertexCount];

        for (int j = 0; j < vertexCount; j++)
        {
            residual[i][j] = 0;
        }
    }

    // Add original edge capacities.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        int from = graph.getVertexIndexByID(edge->fromID);
        int to = graph.getVertexIndexByID(edge->toID);

        if (from != -1 && to != -1)
        {
            residual[from][to] += edge->capacity;
        }
    }

    int* parent = new int[vertexCount];
    bool* visited = new bool[vertexCount];
    int* queue = new int[vertexCount];

    // Find augmenting paths using BFS.

    while (true)
    {
        for (int i = 0; i < vertexCount; i++)
        {
            visited[i] = false;
            parent[i] = -1;
        }

        int front = 0;
        int rear = 0;

        queue[rear] = source;
        rear++;

        visited[source] = true;

        while (front < rear)
        {
            int current = queue[front];
            front++;

            for (int neighbour = 0; neighbour < vertexCount; neighbour++)
            {
                if (!visited[neighbour] && residual[current][neighbour] > 0)
                {
                    visited[neighbour] = true;
                    parent[neighbour] = current;

                    queue[rear] = neighbour;
                    rear++;

                    if (neighbour == sink)
                    {
                        break;
                    }
                }
            }

            if (visited[sink])
            {
                break;
            }
        }

        // No more augmenting paths.

        if (!visited[sink])
        {
            break;
        }

        // Find the bottleneck capacity of the path.

        int pathFlow = 1000000000;

        int current = sink;

        while (current != source)
        {
            int previous = parent[current];

            if (residual[previous][current] < pathFlow)
            {
                pathFlow = residual[previous][current];
            }

            current = previous;
        }

        // Update residual capacities.

        current = sink;

        while (current != source)
        {
            int previous = parent[current];

            residual[previous][current] -= pathFlow;
            residual[current][previous] += pathFlow;

            current = previous;
        }

        maxFlow += pathFlow;
    }

    // Find vertices reachable from source in the final residual graph.

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    int front = 0;
    int rear = 0;

    queue[rear] = source;
    rear++;

    visited[source] = true;

    while (front < rear)
    {
        int current = queue[front];
        front++;

        for (int neighbour = 0; neighbour < vertexCount; neighbour++)
        {
            if (!visited[neighbour] && residual[current][neighbour] > 0)
            {
                visited[neighbour] = true;

                queue[rear] = neighbour;
                rear++;
            }
        }
    }

    // Original edges crossing from reachable to unreachable form the minimum cut.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        int from = graph.getVertexIndexByID(edge->fromID);
        int to = graph.getVertexIndexByID(edge->toID);

        if (visited[from] && !visited[to])
        {
            minCutEdges[minCutCount] = *edge;
            minCutCount++;
        }
    }

    // Release dynamically allocated memory.

    delete[] parent;
    delete[] visited;
    delete[] queue;

    for (int i = 0; i < vertexCount; i++)
    {
        delete[] residual[i];
    }

    delete[] residual;

    return true;
}