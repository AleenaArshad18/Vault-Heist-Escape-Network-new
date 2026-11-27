#include "CriticalPoints.h"

using namespace std;

void CriticalPoints::DFS(const Graph& graph, int currentIndex, int parentEdgeID, int& timer,
    bool visited[], int discovery[], int low[], Edge bridges[], int& bridgeCount,
    bool articulationFlags[]) const
{
    visited[currentIndex] = true;

    discovery[currentIndex] = timer;
    low[currentIndex] = timer;

    timer++;

    int children = 0;

    const EdgeNode* current = graph.getAdjacencyHead(currentIndex);

    while (current != nullptr)
    {
        int neighbour = current->destinationIndex;

        // Ignore the edge used to reach the current vertex.

        if (current->edgeID == parentEdgeID)
        {
            current = current->next;
            continue;
        }

        if (!visited[neighbour])
        {
            children++;

            DFS(graph, neighbour, current->edgeID, timer, visited,
                discovery, low, bridges, bridgeCount, articulationFlags);

            // Update the low value.

            if (low[neighbour] < low[currentIndex])
            {
                low[currentIndex] = low[neighbour];
            }

            // Check bridge condition.

            if (low[neighbour] > discovery[currentIndex])
            {
                const Vertex* fromVertex = graph.getVertexAt(currentIndex);
                const Vertex* toVertex = graph.getVertexAt(neighbour);

                bridges[bridgeCount].id = current->edgeID;
                bridges[bridgeCount].fromID = fromVertex->id;
                bridges[bridgeCount].toID = toVertex->id;
                bridges[bridgeCount].weight = current->weight;
                bridges[bridgeCount].capacity = current->capacity;
                bridges[bridgeCount].directed = false;

                bridgeCount++;
            }

            // Check articulation point condition.

            if (parentEdgeID != -1 && low[neighbour] >= discovery[currentIndex])
            {
                articulationFlags[currentIndex] = true;
            }
        }
        else
        {
            // Back edge: update low using the neighbour's discovery time.

            if (discovery[neighbour] < low[currentIndex])
            {
                low[currentIndex] = discovery[neighbour];
            }
        }

        current = current->next;
    }

    // Special condition for DFS root.

    if (parentEdgeID == -1 && children > 1)
    {
        articulationFlags[currentIndex] = true;
    }
}


bool CriticalPoints::findCriticalPoints(const Graph& graph, Edge bridges[], int& bridgeCount,
    int articulationPoints[], int& articulationCount) const
{
    int vertexCount = graph.getVertexCount();

    bridgeCount = 0;
    articulationCount = 0;

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

    // Process every connected component.

    for (int i = 0; i < vertexCount; i++)
    {
        if (!visited[i])
        {
            DFS(graph, i, -1, timer, visited, discovery, low,
                bridges, bridgeCount, articulationFlags);
        }
    }

    // Collect articulation point vertex IDs.

    for (int i = 0; i < vertexCount; i++)
    {
        if (articulationFlags[i])
        {
            const Vertex* vertex = graph.getVertexAt(i);

            articulationPoints[articulationCount] = vertex->id;
            articulationCount++;
        }
    }

    delete[] visited;
    delete[] discovery;
    delete[] low;
    delete[] articulationFlags;

    return true;
}