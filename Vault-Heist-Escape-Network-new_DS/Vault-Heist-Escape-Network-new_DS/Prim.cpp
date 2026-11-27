#include "Prim.h"

using namespace std;

bool Prim::findMST(const Graph& graph, int startVertexID, Edge mstEdges[], int& mstCount, int& totalWeight) const
{
    int vertexCount = graph.getVertexCount();

    mstCount = 0;
    totalWeight = 0;

    if (vertexCount == 0)
    {
        return false;
    }

    if (vertexCount == 1)
    {
        return graph.getVertexIndexByID(startVertexID) != -1;
    }

    int startIndex = graph.getVertexIndexByID(startVertexID);

    if (startIndex == -1)
    {
        return false;
    }

    bool* visited = new bool[vertexCount];

    const int INF = 1000000000;

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    visited[startIndex] = true;

    while (mstCount < vertexCount - 1)
    {
        int minimumWeight = INF;
        int selectedFrom = -1;
        int selectedTo = -1;

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
                    }

                    current = current->next;
                }
            }
        }

        if (selectedTo == -1)
        {
            delete[] visited;
            return false;
        }

        visited[selectedTo] = true;

        const Vertex* fromVertex = graph.getVertexAt(selectedFrom);
        const Vertex* toVertex = graph.getVertexAt(selectedTo);

        mstEdges[mstCount].fromID = fromVertex->id;
        mstEdges[mstCount].toID = toVertex->id;
        mstEdges[mstCount].weight = minimumWeight;

        mstCount++;
        totalWeight += minimumWeight;
    }

    delete[] visited;

    return true;
}