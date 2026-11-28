#include "Bipartite.h"
#include "EventRecorder.h"
#include <iostream>

using namespace std;

bool Bipartite::checkBipartite(
    const Graph& graph,
    int colours[],
    EventRecorder* recorder) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_STARTED,
            -1, -1, -1, -1, 0, 0,
            "Bipartite checking started"
        ));

    for (int i = 0; i < vertexCount; i++)
        colours[i] = -1;

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (edge->directed)
        {
            cout << "Bipartite checking requires an undirected graph!" << endl;

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ALGORITHM_FAILED,
                    -1, edge->id, edge->fromID, edge->toID,
                    0, 0, "Bipartite checking requires an undirected graph"
                ));

            return false;
        }
    }

    int* queue = new int[vertexCount];

    for (int start = 0; start < vertexCount; start++)
    {
        if (colours[start] != -1)
            continue;

        int front = 0;
        int rear = 0;

        colours[start] = 0;

        queue[rear] = start;
        rear++;

        const Vertex* startVertex = graph.getVertexAt(start);

        if (recorder != nullptr)
        {
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::VERTEX_DISCOVERED,
                startVertex->id, -1, -1, -1,
                0, 0, "Starting vertex discovered"
            ));

            recorder->record(AlgorithmEvent(
                AlgorithmEvent::VERTEX_COLORED,
                startVertex->id, -1, -1, -1,
                0, -1, "Vertex colored with group 0"
            ));
        }

        while (front < rear)
        {
            int currentIndex = queue[front];
            front++;

            const Vertex* currentVertex = graph.getVertexAt(currentIndex);

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::VERTEX_PROCESSED,
                    currentVertex->id, -1, -1, -1,
                    colours[currentIndex], 0,
                    "Processing vertex"
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
                        currentVertex->id,
                        neighbourVertex->id,
                        current->weight,
                        colours[neighbour],
                        "Examining edge"
                    ));

                if (colours[neighbour] == -1)
                {
                    colours[neighbour] = 1 - colours[currentIndex];

                    queue[rear] = neighbour;
                    rear++;

                    if (recorder != nullptr)
                    {
                        recorder->record(AlgorithmEvent(
                            AlgorithmEvent::VERTEX_DISCOVERED,
                            neighbourVertex->id,
                            current->edgeID,
                            currentVertex->id,
                            neighbourVertex->id,
                            0, 0, "New vertex discovered"
                        ));

                        recorder->record(AlgorithmEvent(
                            AlgorithmEvent::VERTEX_COLORED,
                            neighbourVertex->id,
                            current->edgeID,
                            currentVertex->id,
                            neighbourVertex->id,
                            colours[neighbour],
                            colours[currentIndex],
                            "Vertex assigned opposite colour"
                        ));
                    }
                }
                else if (colours[neighbour] == colours[currentIndex])
                {
                    delete[] queue;

                    if (recorder != nullptr)
                    {
                        recorder->record(AlgorithmEvent(
                            AlgorithmEvent::CYCLE_DETECTED,
                            neighbourVertex->id,
                            current->edgeID,
                            currentVertex->id,
                            neighbourVertex->id,
                            colours[neighbour],
                            colours[currentIndex],
                            "Adjacent vertices have the same colour"
                        ));

                        recorder->record(AlgorithmEvent(
                            AlgorithmEvent::ALGORITHM_FAILED,
                            -1, -1, -1, -1,
                            0, 0, "Graph is not bipartite"
                        ));
                    }

                    return false;
                }

                current = current->next;
            }
        }
    }

    delete[] queue;

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1, vertexCount, 0,
            "Graph is bipartite"
        ));

    return true;
}