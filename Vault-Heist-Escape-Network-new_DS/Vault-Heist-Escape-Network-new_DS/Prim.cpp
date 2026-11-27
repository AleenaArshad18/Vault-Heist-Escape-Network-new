
#include "Prim.h"

using namespace std;

bool Prim::findMST(const Graph& graph, int startVertexID, Edge mstEdges[], int& mstCount, int& totalWeight) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    mstCount = 0;
    totalWeight = 0;

    // An empty graph cannot have an MST.

    if (vertexCount == 0)
    {
        return false;
    }

    // Prim requires an undirected graph.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (edge->directed)
        {
            return false;
        }
    }

    // Validate the starting vertex.

    int startIndex = graph.getVertexIndexByID(startVertexID);

    if (startIndex == -1)
    {
        return false;
    }

    // A graph with one vertex already has an MST.

    if (vertexCount == 1)
    {
        return true;
    }

    // Create and initialize visited array.

    bool* visited = new bool[vertexCount];

    const int INF = 1000000000;

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    // Start from the selected vertex.

    visited[startIndex] = true;

    // Continue until V-1 edges have been selected.

    while (mstCount < vertexCount - 1)
    {
        int minimumWeight = INF;

        int selectedFrom = -1;
        int selectedTo = -1;
        int selectedEdgeID = -1;

        // Find the minimum-weight edge connecting
        // a visited vertex to an unvisited vertex.

        for (int i = 0; i < vertexCount; i++)
        {
            if (visited[i])
            {
                const EdgeNode* current = graph.getAdjacencyHead(i);

                while (current != nullptr)
                {
                    int neighbour = current->destinationIndex;

                    if (!visited[neighbour] && current->weight < minimumWeight)
                    {
                        minimumWeight = current->weight;
                        selectedFrom = i;
                        selectedTo = neighbour;
                        selectedEdgeID = current->edgeID;
                    }

                    current = current->next;
                }
            }
        }

        // No connecting edge means the graph is disconnected.

        if (selectedTo == -1)
        {
            delete[] visited;
            return false;
        }

        // Find the original edge so all its fields are copied.

        const Edge* selectedEdge = nullptr;

        for (int i = 0; i < edgeCount; i++)
        {
            const Edge* edge = graph.getEdgeAt(i);

            if (edge->id == selectedEdgeID)
            {
                selectedEdge = edge;
                break;
            }
        }

        if (selectedEdge == nullptr)
        {
            delete[] visited;
            return false;
        }

        // Add the selected edge to the MST.

        mstEdges[mstCount] = *selectedEdge;

        mstCount++;
        totalWeight += minimumWeight;

        // Mark the newly connected vertex as visited.

        visited[selectedTo] = true;
    }

    delete[] visited;

    return true;
}
