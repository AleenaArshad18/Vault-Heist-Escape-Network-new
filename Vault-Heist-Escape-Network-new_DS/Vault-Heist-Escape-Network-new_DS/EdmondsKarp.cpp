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

bool EdmondsKarp::findMaxFlow(
    const Graph& graph,
    int sourceID,
    int sinkID,
    int& maxFlow,
    Edge minCutEdges[],
    int& minCutCount,
    int edgeFlows[],
    EventRecorder* recorder) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    maxFlow = 0;
    minCutCount = 0;

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_STARTED,
            sourceID, -1, -1, -1, 0, 0,
            "Edmonds-Karp started"
        ));

    int source = graph.getVertexIndexByID(sourceID);
    int sink = graph.getVertexIndexByID(sinkID);

    if (vertexCount == 0 || source == -1 || sink == -1 || source == sink)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                sourceID, -1, sourceID, sinkID,
                0, 0, "Invalid source or sink"
            ));

        return false;
    }

    if (edgeCount > 0 && minCutEdges == nullptr)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1,
                0, 0, "Minimum cut output array is missing"
            ));

        return false;
    }

    // Validate directed edges and nonnegative capacities.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (!edge->directed || edge->capacity < 0)
        {
            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ALGORITHM_FAILED,
                    -1, edge->id, edge->fromID, edge->toID,
                    edge->capacity, 0,
                    "Edmonds-Karp requires directed edges with nonnegative capacities"
                ));

            return false;
        }

        int from = graph.getVertexIndexByID(edge->fromID);
        int to = graph.getVertexIndexByID(edge->toID);

        if (from == -1 || to == -1)
        {
            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ALGORITHM_FAILED,
                    -1, edge->id, edge->fromID, edge->toID,
                    0, 0, "Invalid edge endpoint"
                ));

            return false;
        }
    }

    if (edgeFlows != nullptr)
    {
        for (int i = 0; i < edgeCount; i++)
            edgeFlows[i] = 0;
    }

    // Each original edge gets a forward and reverse residual edge.

    ResidualEdge* residualEdges = new ResidualEdge[edgeCount * 2];
    int* adjacencyHead = new int[vertexCount];

    for (int i = 0; i < vertexCount; i++)
        adjacencyHead[i] = -1;

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        int from = graph.getVertexIndexByID(edge->fromID);
        int to = graph.getVertexIndexByID(edge->toID);

        int forwardIndex = 2 * i;
        int reverseIndex = 2 * i + 1;

        residualEdges[forwardIndex].from = from;
        residualEdges[forwardIndex].to = to;
        residualEdges[forwardIndex].capacity = edge->capacity;
        residualEdges[forwardIndex].reverseIndex = reverseIndex;
        residualEdges[forwardIndex].next = adjacencyHead[from];
        residualEdges[forwardIndex].originalEdgeIndex = i;
        residualEdges[forwardIndex].isOriginal = true;

        adjacencyHead[from] = forwardIndex;

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

    // Repeatedly find augmenting paths using BFS.

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
                        break;
                }

                edgeIndex = residualEdges[edgeIndex].next;
            }

            if (visited[sink])
                break;
        }

        // No more augmenting paths.

        if (!visited[sink])
            break;

        // Find bottleneck capacity.

        int pathFlow = 2147483647;
        int current = sink;

        while (current != source)
        {
            int edgeIndex = parentEdge[current];

            if (residualEdges[edgeIndex].capacity < pathFlow)
                pathFlow = residualEdges[edgeIndex].capacity;

            current = residualEdges[edgeIndex].from;
        }

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::AUGMENTING_PATH_FOUND,
                sinkID, -1, sourceID, sinkID,
                pathFlow, static_cast<int>(totalFlow),
                "Augmenting path found"
            ));

        // Update residual capacities and original edge flows.

        current = sink;

        while (current != source)
        {
            int edgeIndex = parentEdge[current];
            int reverseIndex = residualEdges[edgeIndex].reverseIndex;

            residualEdges[edgeIndex].capacity -= pathFlow;
            residualEdges[reverseIndex].capacity += pathFlow;

            int originalIndex = residualEdges[edgeIndex].originalEdgeIndex;

            if (edgeFlows != nullptr)
            {
                if (residualEdges[edgeIndex].isOriginal)
                    edgeFlows[originalIndex] += pathFlow;
                else
                    edgeFlows[originalIndex] -= pathFlow;
            }

            if (recorder != nullptr)
            {
                const Edge* originalEdge = graph.getEdgeAt(originalIndex);

                int flowChange = residualEdges[edgeIndex].isOriginal
                    ? pathFlow : -pathFlow;

                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::FLOW_UPDATED,
                    originalEdge->toID,
                    originalEdge->id,
                    originalEdge->fromID,
                    originalEdge->toID,
                    flowChange,
                    edgeFlows != nullptr ? edgeFlows[originalIndex] : 0,
                    "Original edge flow updated"
                ));
            }

            current = residualEdges[edgeIndex].from;
        }

        totalFlow += pathFlow;
    }

    // Find vertices reachable from source in final residual graph.

    for (int i = 0; i < vertexCount; i++)
        visited[i] = false;

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

    // Find original edges crossing the minimum cut.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        int from = graph.getVertexIndexByID(edge->fromID);
        int to = graph.getVertexIndexByID(edge->toID);

        if (visited[from] && !visited[to])
        {
            minCutEdges[minCutCount] = *edge;
            minCutCount++;

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::MIN_CUT_EDGE,
                    edge->toID,
                    edge->id,
                    edge->fromID,
                    edge->toID,
                    edge->capacity,
                    0,
                    "Minimum cut edge found"
                ));
        }
    }

    // Release allocated memory.

    delete[] parentEdge;
    delete[] visited;
    delete[] queue;
    delete[] adjacencyHead;
    delete[] residualEdges;

    if (totalFlow > 2147483647)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1,
                0, 0, "Maximum flow exceeds integer range"
            ));

        return false;
    }

    maxFlow = static_cast<int>(totalFlow);

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1,
            maxFlow, minCutCount,
            "Edmonds-Karp completed"
        ));

    return true;
}