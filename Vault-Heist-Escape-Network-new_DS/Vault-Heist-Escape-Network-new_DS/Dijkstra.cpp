
#include "Dijkstra.h"
#include "MinHeap.h"
#include "EventRecorder.h"
#include <iostream>

using namespace std;

bool Dijkstra::findShortestPaths(
    const Graph& graph,
    int startVertexID,
    int distances[],
    int previous[],
    EventRecorder* recorder) const
{
    const int INF = 1000000000;

    int vertexCount = graph.getVertexCount();

    for (int i = 0; i < vertexCount; i++)
    {
        distances[i] = INF;
        previous[i] = -1;
    }

    int startIndex = graph.getVertexIndexByID(startVertexID);

    if (startIndex == -1)
    {
        cout << "Starting vertex not found!" << endl;

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                startVertexID, -1, -1, -1,
                0, 0, "Starting vertex not found"
            ));

        return false;
    }

    if (graph.hasNegativeWeight())
    {
        cout << "Dijkstra cannot work with negative weights!" << endl;

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1,
                0, 0, "Negative edge weight"
            ));

        return false;
    }

    bool* visited = new bool[vertexCount];

    for (int i = 0; i < vertexCount; i++)
        visited[i] = false;

    MinHeap minHeap(vertexCount + graph.getEdgeCount() + 1);

    distances[startIndex] = 0;
    minHeap.insert(0, startIndex);

    if (recorder != nullptr)
    {
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_STARTED,
            startVertexID, -1, -1, -1,
            0, 0, "Dijkstra started"
        ));

        recorder->record(AlgorithmEvent(
            AlgorithmEvent::VERTEX_DISCOVERED,
            startVertexID, -1, -1, -1,
            0, 0, "Starting vertex discovered"
        ));
    }

    HeapNode currentNode;

    while (minHeap.extractMin(currentNode))
    {
        int currentIndex = currentNode.vertexIndex;

        // Ignore outdated heap entries.
        if (visited[currentIndex] ||
            currentNode.priority != distances[currentIndex])
        {
            continue;
        }

        visited[currentIndex] = true;

        const Vertex* currentVertex =
            graph.getVertexAt(currentIndex);

        if (recorder != nullptr)
        {
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::DISTANCE_FINALIZED,
                currentVertex->id, -1, -1, -1,
                distances[currentIndex], 0,
                "Shortest distance finalized"
            ));

            recorder->record(AlgorithmEvent(
                AlgorithmEvent::VERTEX_PROCESSED,
                currentVertex->id, -1, -1, -1,
                distances[currentIndex], 0,
                "Processing vertex"
            ));
        }

        const EdgeNode* current =
            graph.getAdjacencyHead(currentIndex);

        while (current != nullptr)
        {
            int neighbourIndex = current->destinationIndex;

            const Vertex* neighbourVertex =
                graph.getVertexAt(neighbourIndex);

            if (recorder != nullptr)
            {
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::EDGE_EXAMINED,
                    neighbourVertex->id,
                    current->edgeID,
                    currentVertex->id,
                    neighbourVertex->id,
                    current->weight,
                    distances[neighbourIndex],
                    "Examining edge"
                ));
            }

            if (!visited[neighbourIndex] &&
                current->weight <= INF - distances[currentIndex])
            {
                int newDistance =
                    distances[currentIndex] + current->weight;

                if (newDistance < distances[neighbourIndex])
                {
                    int oldDistance = distances[neighbourIndex];

                    distances[neighbourIndex] = newDistance;
                    previous[neighbourIndex] = currentIndex;

                    minHeap.insert(
                        newDistance,
                        neighbourIndex
                    );

                    if (recorder != nullptr)
                    {
                        recorder->record(AlgorithmEvent(
                            AlgorithmEvent::VERTEX_DISCOVERED,
                            neighbourVertex->id,
                            current->edgeID,
                            currentVertex->id,
                            neighbourVertex->id,
                            newDistance,
                            oldDistance,
                            "Vertex discovered"
                        ));

                        recorder->record(AlgorithmEvent(
                            AlgorithmEvent::EDGE_RELAXED,
                            neighbourVertex->id,
                            current->edgeID,
                            currentVertex->id,
                            neighbourVertex->id,
                            oldDistance,
                            newDistance,
                            "Distance updated"
                        ));
                    }
                }
            }

            current = current->next;
        }
    }

    delete[] visited;

    if (recorder != nullptr)
    {
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1,
            0, 0, "Dijkstra completed"
        ));
    }

    return true;
}
