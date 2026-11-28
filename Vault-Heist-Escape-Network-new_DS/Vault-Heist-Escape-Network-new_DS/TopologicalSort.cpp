#include "TopologicalSort.h"
#include "EventRecorder.h"
#include <iostream>

using namespace std;

bool TopologicalSort::kahnSort(
    const Graph& graph,
    int result[],
    int& resultCount,
    EventRecorder* recorder) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    resultCount = 0;

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_STARTED,
            -1, -1, -1, -1, 0, 0,
            "Topological Sort started"
        ));

    if (vertexCount == 0)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1, 0, 0,
                "Topological Sort failed: empty graph"
            ));

        return false;
    }

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (!edge->directed)
        {
            cout << "Topological Sort requires a directed graph!" << endl;

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ALGORITHM_FAILED,
                    -1, edge->id, edge->fromID, edge->toID,
                    0, 0, "Topological Sort requires directed edges"
                ));

            return false;
        }
    }

    int* indegree = new int[vertexCount];
    int* queue = new int[vertexCount];

    for (int i = 0; i < vertexCount; i++)
        indegree[i] = 0;

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

    for (int i = 0; i < vertexCount; i++)
    {
        if (indegree[i] == 0)
        {
            queue[rear] = i;
            rear++;

            const Vertex* vertex = graph.getVertexAt(i);

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::VERTEX_DISCOVERED,
                    vertex->id, -1, -1, -1,
                    0, 0, "Vertex has zero indegree"
                ));
        }
    }

    while (front < rear)
    {
        int currentIndex = queue[front];
        front++;

        const Vertex* vertex = graph.getVertexAt(currentIndex);

        result[resultCount] = vertex->id;
        resultCount++;

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::VERTEX_PROCESSED,
                vertex->id, -1, -1, -1,
                resultCount, 0, "Vertex added to topological order"
            ));

        const EdgeNode* current = graph.getAdjacencyHead(currentIndex);

        while (current != nullptr)
        {
            int neighbour = current->destinationIndex;
            const Vertex* neighbourVertex = graph.getVertexAt(neighbour);

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::EDGE_EXAMINED,
                    neighbourVertex->id,
                    current->edgeID,
                    vertex->id,
                    neighbourVertex->id,
                    current->weight,
                    indegree[neighbour],
                    "Examining outgoing edge"
                ));

            indegree[neighbour]--;

            if (indegree[neighbour] == 0)
            {
                queue[rear] = neighbour;
                rear++;

                if (recorder != nullptr)
                    recorder->record(AlgorithmEvent(
                        AlgorithmEvent::VERTEX_DISCOVERED,
                        neighbourVertex->id,
                        current->edgeID,
                        vertex->id,
                        neighbourVertex->id,
                        0, 0, "Vertex now has zero indegree"
                    ));
            }

            current = current->next;
        }
    }

    delete[] indegree;
    delete[] queue;

    if (resultCount != vertexCount)
    {
        cout << "Graph contains a cycle. Topological sorting is not possible." << endl;

        if (recorder != nullptr)
        {
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::CYCLE_DETECTED,
                -1, -1, -1, -1,
                resultCount, vertexCount,
                "Directed cycle detected"
            ));

            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1,
                resultCount, vertexCount,
                "Topological Sort failed: graph contains a cycle"
            ));
        }

        return false;
    }

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1,
            resultCount, 0, "Topological Sort completed"
        ));

    return true;
}