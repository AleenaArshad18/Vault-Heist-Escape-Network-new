#include "CriticalPoints.h"
#include "EventRecorder.h"
#include <iostream>

using namespace std;

void CriticalPoints::DFS(
    const Graph& graph,
    int currentIndex,
    int parentEdgeID,
    int& timer,
    bool visited[],
    int discovery[],
    int low[],
    Edge bridges[],
    int& bridgeCount,
    bool articulationFlags[],
    EventRecorder* recorder) const
{
    visited[currentIndex] = true;

    discovery[currentIndex] = timer;
    low[currentIndex] = timer;

    timer++;

    const Vertex* currentVertex = graph.getVertexAt(currentIndex);

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::VERTEX_DISCOVERED,
            currentVertex->id, -1, -1, -1,
            discovery[currentIndex], low[currentIndex],
            "Vertex discovered by DFS"
        ));

    int children = 0;

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
                low[currentIndex],
                "Examining edge"
            ));

        if (current->edgeID == parentEdgeID)
        {
            current = current->next;
            continue;
        }

        if (!visited[neighbour])
        {
            children++;

            DFS(
                graph,
                neighbour,
                current->edgeID,
                timer,
                visited,
                discovery,
                low,
                bridges,
                bridgeCount,
                articulationFlags,
                recorder
            );

            int oldLow = low[currentIndex];

            if (low[neighbour] < low[currentIndex])
                low[currentIndex] = low[neighbour];

            if (recorder != nullptr && oldLow != low[currentIndex])
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::LOW_LINK_UPDATED,
                    currentVertex->id,
                    current->edgeID,
                    currentVertex->id,
                    neighbourVertex->id,
                    oldLow,
                    low[currentIndex],
                    "Low-link value updated"
                ));

            if (low[neighbour] > discovery[currentIndex])
            {
                bridges[bridgeCount].id = current->edgeID;
                bridges[bridgeCount].fromID = currentVertex->id;
                bridges[bridgeCount].toID = neighbourVertex->id;
                bridges[bridgeCount].weight = current->weight;
                bridges[bridgeCount].capacity = current->capacity;
                bridges[bridgeCount].directed = false;

                bridgeCount++;

                if (recorder != nullptr)
                    recorder->record(AlgorithmEvent(
                        AlgorithmEvent::BRIDGE_FOUND,
                        currentVertex->id,
                        current->edgeID,
                        currentVertex->id,
                        neighbourVertex->id,
                        current->weight,
                        low[neighbour],
                        "Bridge found"
                    ));
            }

            if (parentEdgeID != -1 &&
                low[neighbour] >= discovery[currentIndex])
            {
                articulationFlags[currentIndex] = true;
            }
        }
        else
        {
            int oldLow = low[currentIndex];

            if (discovery[neighbour] < low[currentIndex])
                low[currentIndex] = discovery[neighbour];

            if (recorder != nullptr && oldLow != low[currentIndex])
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::LOW_LINK_UPDATED,
                    currentVertex->id,
                    current->edgeID,
                    currentVertex->id,
                    neighbourVertex->id,
                    oldLow,
                    low[currentIndex],
                    "Low-link updated using back edge"
                ));
        }

        current = current->next;
    }

    if (parentEdgeID == -1 && children > 1)
        articulationFlags[currentIndex] = true;
}


bool CriticalPoints::findCriticalPoints(
    const Graph& graph,
    Edge bridges[],
    int& bridgeCount,
    int articulationPoints[],
    int& articulationCount,
    EventRecorder* recorder) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    bridgeCount = 0;
    articulationCount = 0;

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_STARTED,
            -1, -1, -1, -1, 0, 0,
            "Critical Points started"
        ));

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (edge->directed)
        {
            cout << "Bridges and articulation points require an undirected graph!" << endl;

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ALGORITHM_FAILED,
                    -1, edge->id, edge->fromID, edge->toID,
                    0, 0, "Critical Points requires an undirected graph"
                ));

            return false;
        }
    }

    bool* visited = new bool[vertexCount];
    int* discovery = new int[vertexCount];
    int* low = new int[vertexCount];
    bool* articulationFlags = new bool[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
        discovery[i] = -1;
        low[i] = -1;
        articulationFlags[i] = false;
    }

    int timer = 0;

    for (int i = 0; i < vertexCount; i++)
    {
        if (!visited[i])
        {
            DFS(
                graph,
                i,
                -1,
                timer,
                visited,
                discovery,
                low,
                bridges,
                bridgeCount,
                articulationFlags,
                recorder
            );
        }
    }

    for (int i = 0; i < vertexCount; i++)
    {
        if (articulationFlags[i])
        {
            const Vertex* vertex = graph.getVertexAt(i);

            articulationPoints[articulationCount] = vertex->id;
            articulationCount++;

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ARTICULATION_FOUND,
                    vertex->id, -1, -1, -1,
                    discovery[i], low[i],
                    "Articulation point found"
                ));
        }
    }

    delete[] visited;
    delete[] discovery;
    delete[] low;
    delete[] articulationFlags;

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1,
            bridgeCount, articulationCount,
            "Critical Points completed"
        ));

    return true;
}