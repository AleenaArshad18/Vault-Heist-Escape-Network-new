
#include "BellmanFord.h"
#include "EventRecorder.h"
#include <iostream>

using namespace std;

bool BellmanFord::findShortestPaths(
    const Graph& graph,
    int startVertexID,
    int distances[],
    int previous[],
    EventRecorder* recorder) const
{
    const int INF = 1000000000;

    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

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
        {
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                startVertexID, -1, -1, -1,
                0, 0, "Starting vertex not found"
            ));
        }

        return false;
    }

    distances[startIndex] = 0;

    if (recorder != nullptr)
    {
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_STARTED,
            startVertexID, -1, -1, -1,
            0, 0, "Bellman-Ford started"
        ));

        recorder->record(AlgorithmEvent(
            AlgorithmEvent::VERTEX_DISCOVERED,
            startVertexID, -1, -1, -1,
            0, 0, "Starting vertex discovered"
        ));
    }

    // Relax all edges V-1 times.

    for (int round = 1; round < vertexCount; round++)
    {
        bool updated = false;

        for (int i = 0; i < edgeCount; i++)
        {
            const Edge* edge = graph.getEdgeAt(i);

            int fromIndex = graph.getVertexIndexByID(edge->fromID);
            int toIndex = graph.getVertexIndexByID(edge->toID);

            // Examine the forward direction.

            if (recorder != nullptr)
            {
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::EDGE_EXAMINED,
                    edge->toID,
                    edge->id,
                    edge->fromID,
                    edge->toID,
                    edge->weight,
                    distances[fromIndex],
                    "Examining forward edge"
                ));
            }

            // Relax the forward direction.

            if (distances[fromIndex] != INF)
            {
                long long newDistance =
                    (long long)distances[fromIndex] + edge->weight;

                if (newDistance < distances[toIndex] &&
                    newDistance > -INF)
                {
                    int oldDistance = distances[toIndex];

                    distances[toIndex] = (int)newDistance;
                    previous[toIndex] = fromIndex;
                    updated = true;

                    if (recorder != nullptr)
                    {
                        if (oldDistance == INF)
                        {
                            recorder->record(AlgorithmEvent(
                                AlgorithmEvent::VERTEX_DISCOVERED,
                                edge->toID,
                                edge->id,
                                edge->fromID,
                                edge->toID,
                                (int)newDistance,
                                oldDistance,
                                "Vertex discovered"
                            ));
                        }

                        recorder->record(AlgorithmEvent(
                            AlgorithmEvent::EDGE_RELAXED,
                            edge->toID,
                            edge->id,
                            edge->fromID,
                            edge->toID,
                            oldDistance,
                            (int)newDistance,
                            "Distance updated"
                        ));
                    }
                }
            }

            // Undirected edges can also be relaxed in reverse.

            if (!edge->directed)
            {
                if (recorder != nullptr)
                {
                    recorder->record(AlgorithmEvent(
                        AlgorithmEvent::EDGE_EXAMINED,
                        edge->fromID,
                        edge->id,
                        edge->toID,
                        edge->fromID,
                        edge->weight,
                        distances[toIndex],
                        "Examining reverse edge"
                    ));
                }

                if (distances[toIndex] != INF)
                {
                    long long newDistance =
                        (long long)distances[toIndex] + edge->weight;

                    if (newDistance < distances[fromIndex] &&
                        newDistance > -INF)
                    {
                        int oldDistance = distances[fromIndex];

                        distances[fromIndex] = (int)newDistance;
                        previous[fromIndex] = toIndex;
                        updated = true;

                        if (recorder != nullptr)
                        {
                            if (oldDistance == INF)
                            {
                                recorder->record(AlgorithmEvent(
                                    AlgorithmEvent::VERTEX_DISCOVERED,
                                    edge->fromID,
                                    edge->id,
                                    edge->toID,
                                    edge->fromID,
                                    (int)newDistance,
                                    oldDistance,
                                    "Vertex discovered"
                                ));
                            }

                            recorder->record(AlgorithmEvent(
                                AlgorithmEvent::EDGE_RELAXED,
                                edge->fromID,
                                edge->id,
                                edge->toID,
                                edge->fromID,
                                oldDistance,
                                (int)newDistance,
                                "Distance updated"
                            ));
                        }
                    }
                }
            }
        }

        // Stop early if no distances changed.

        if (!updated)
        {
            break;
        }
    }

    // Check for reachable negative-weight cycles.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        int fromIndex = graph.getVertexIndexByID(edge->fromID);
        int toIndex = graph.getVertexIndexByID(edge->toID);

        if (distances[fromIndex] != INF)
        {
            long long newDistance =
                (long long)distances[fromIndex] + edge->weight;

            if (newDistance < distances[toIndex])
            {
                cout << "Negative-weight cycle detected!" << endl;

                if (recorder != nullptr)
                {
                    recorder->record(AlgorithmEvent(
                        AlgorithmEvent::NEGATIVE_CYCLE_DETECTED,
                        edge->toID,
                        edge->id,
                        edge->fromID,
                        edge->toID,
                        (int)newDistance,
                        distances[toIndex],
                        "Reachable negative-weight cycle detected"
                    ));

                    recorder->record(AlgorithmEvent(
                        AlgorithmEvent::ALGORITHM_FAILED,
                        -1, -1, -1, -1,
                        0, 0, "Bellman-Ford failed: negative cycle"
                    ));
                }

                return false;
            }
        }

        if (!edge->directed && distances[toIndex] != INF)
        {
            long long newDistance =
                (long long)distances[toIndex] + edge->weight;

            if (newDistance < distances[fromIndex])
            {
                cout << "Negative-weight cycle detected!" << endl;

                if (recorder != nullptr)
                {
                    recorder->record(AlgorithmEvent(
                        AlgorithmEvent::NEGATIVE_CYCLE_DETECTED,
                        edge->fromID,
                        edge->id,
                        edge->toID,
                        edge->fromID,
                        (int)newDistance,
                        distances[fromIndex],
                        "Reachable negative-weight cycle detected"
                    ));

                    recorder->record(AlgorithmEvent(
                        AlgorithmEvent::ALGORITHM_FAILED,
                        -1, -1, -1, -1,
                        0, 0, "Bellman-Ford failed: negative cycle"
                    ));
                }

                return false;
            }
        }
    }

    if (recorder != nullptr)
    {
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1,
            0, 0, "Bellman-Ford completed"
        ));
    }

    return true;
}
