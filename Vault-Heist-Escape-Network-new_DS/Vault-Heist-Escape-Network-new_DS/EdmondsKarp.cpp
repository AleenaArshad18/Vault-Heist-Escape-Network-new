
#include "EdmondsKarp.h"
#include "EventRecorder.h"

using namespace std;

struct ResidualEdge
{
    int from;
    int to;
    int capacity;
    int reverseIndex;
    int next;
    int originalEdgeIndex;
    bool isOriginal;
};

bool EdmondsKarp::findMaxFlow(const Graph& graph, int sourceID, int sinkID,
    int& maxFlow, Edge minCutEdges[], int& minCutCount,
    int edgeFlows[]) const
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

    if (edgeCount > 0 && minCutEdges == nullptr)
    {
        return false;
    }

    // Edmonds-Karp requires directed edges with nonnegative capacities.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (!edge->directed || edge->capacity < 0)
        {
            return false;
        }

        int from = graph.getVertexIndexByID(edge->fromID);
        int to = graph.getVertexIndexByID(edge->toID);

        if (from == -1 || to == -1)
        {
            return false;
        }
    }

    // Initialize the optional flow output array.

    if (edgeFlows != nullptr)
    {
        for (int i = 0; i < edgeCount; i++)
        {
            edgeFlows[i] = 0;
        }
    }

    // Each original edge gets two residual records:
    // one forward edge and one reverse edge.

    ResidualEdge* residualEdges = new ResidualEdge[edgeCount * 2];
    int* adjacencyHead = new int[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        adjacencyHead[i] = -1;
    }

    // Build the residual adjacency lists.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        int from = graph.getVertexIndexByID(edge->fromID);
        int to = graph.getVertexIndexByID(edge->toID);

        int forwardIndex = 2 * i;
        int reverseIndex = 2 * i + 1;

        // Forward residual edge.

        residualEdges[forwardIndex].from = from;
        residualEdges[forwardIndex].to = to;
        residualEdges[forwardIndex].capacity = edge->capacity;
        residualEdges[forwardIndex].reverseIndex = reverseIndex;
        residualEdges[forwardIndex].next = adjacencyHead[from];
        residualEdges[forwardIndex].originalEdgeIndex = i;
        residualEdges[forwardIndex].isOriginal = true;

        adjacencyHead[from] = forwardIndex;

        // Reverse residual edge.

        residualEdges[reverseIndex].from = to;
        residualEdges[reverseIndex].to = from;
        residualEdges[reverseIndex].capacity = 0;
        residualEdges[reverseIndex].reverseIndex = forwardIndex;
        residualEdges[reverseIndex].next = adjacencyHead[to];
        residualEdges[reverseIndex].originalEdgeIndex = i;
        residualEdges[reverseIndex].isOriginal = false;

        adjacencyHead[to] = reverseIndex;
    }

    int* parentEdge = new int[vertexCount];
    bool* visited = new bool[vertexCount];
    int* queue = new int[vertexCount];

    long long totalFlow = 0;

    // Find augmenting paths using BFS.

    while (true)
    {
        for (int i = 0; i < vertexCount; i++)
        {
            visited[i] = false;
            parentEdge[i] = -1;
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

            int edgeIndex = adjacencyHead[current];

            while (edgeIndex != -1)
            {
                int neighbour = residualEdges[edgeIndex].to;

                if (!visited[neighbour] &&
                    residualEdges[edgeIndex].capacity > 0)
                {
                    visited[neighbour] = true;
                    parentEdge[neighbour] = edgeIndex;

                    queue[rear] = neighbour;
                    rear++;

                    if (neighbour == sink)
                    {
                        break;
                    }
                }

                edgeIndex = residualEdges[edgeIndex].next;
            }

            if (visited[sink])
            {
                break;
            }
        }

        // No more augmenting paths exist.

        if (!visited[sink])
        {
            break;
        }

        // Find the bottleneck capacity.

        int pathFlow = 2147483647;
        int current = sink;

        while (current != source)
        {
            int edgeIndex = parentEdge[current];

            if (residualEdges[edgeIndex].capacity < pathFlow)
            {
                pathFlow = residualEdges[edgeIndex].capacity;
            }

            current = residualEdges[edgeIndex].from;
        }

        // Update the residual capacities and original edge flows.

        current = sink;

        while (current != source)
        {
            int edgeIndex = parentEdge[current];
            int reverseIndex = residualEdges[edgeIndex].reverseIndex;

            residualEdges[edgeIndex].capacity -= pathFlow;
            residualEdges[reverseIndex].capacity += pathFlow;

            // Track flow on the corresponding original edge.

            if (edgeFlows != nullptr)
            {
                int originalIndex =
                    residualEdges[edgeIndex].originalEdgeIndex;

                if (residualEdges[edgeIndex].isOriginal)
                {
                    edgeFlows[originalIndex] += pathFlow;
                }
                else
                {
                    edgeFlows[originalIndex] -= pathFlow;
                }
            }

            current = residualEdges[edgeIndex].from;
        }

        totalFlow += pathFlow;
    }

    // Find vertices reachable from the source in the final residual graph.

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

        int edgeIndex = adjacencyHead[current];

        while (edgeIndex != -1)
        {
            int neighbour = residualEdges[edgeIndex].to;

            if (!visited[neighbour] &&
                residualEdges[edgeIndex].capacity > 0)
            {
                visited[neighbour] = true;

                queue[rear] = neighbour;
                rear++;
            }

            edgeIndex = residualEdges[edgeIndex].next;
        }
    }

    // Original edges crossing the final cut form the minimum cut.

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

    delete[] parentEdge;
    delete[] visited;
    delete[] queue;
    delete[] adjacencyHead;
    delete[] residualEdges;

    // The function returns an int, so reject an unrepresentable result.

    if (totalFlow > INT_MAX)
    {
        return false;
    }

    maxFlow = static_cast<int>(totalFlow);

    return true;
}
