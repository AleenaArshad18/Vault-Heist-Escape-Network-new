#include "Kruskal.h"
#include "UnionFind.h"

using namespace std;

bool Kruskal::findMST(const Graph& graph, Edge mstEdges[], int& mstCount, int& totalWeight) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    mstCount = 0;
    totalWeight = 0;

    if (vertexCount == 0)
    {
        return false;
    }

    if (vertexCount == 1)
    {
        return true;
    }

    Edge* sortedEdges = new Edge[edgeCount];

    for (int i = 0; i < edgeCount; i++)
    {
        sortedEdges[i] = *graph.getEdgeAt(i);
    }

    // Sort edges by ascending weight using selection sort.

    for (int i = 0; i < edgeCount - 1; i++)
    {
        int minimumIndex = i;

        for (int j = i + 1; j < edgeCount; j++)
        {
            if (sortedEdges[j].weight < sortedEdges[minimumIndex].weight)
            {
                minimumIndex = j;
            }
        }

        Edge temp = sortedEdges[i];
        sortedEdges[i] = sortedEdges[minimumIndex];
        sortedEdges[minimumIndex] = temp;
    }

    UnionFind groups(vertexCount);

    // Process edges from smallest weight to largest.

    for (int i = 0; i < edgeCount; i++)
    {
        int fromIndex = graph.getVertexIndexByID(sortedEdges[i].fromID);
        int toIndex = graph.getVertexIndexByID(sortedEdges[i].toID);

        if (fromIndex == -1 || toIndex == -1)
        {
            continue;
        }

        // Select the edge only if it connects different groups.

        if (groups.unite(fromIndex, toIndex))
        {
            mstEdges[mstCount] = sortedEdges[i];

            mstCount++;
            totalWeight += sortedEdges[i].weight;

            if (mstCount == vertexCount - 1)
            {
                break;
            }
        }
    }

    delete[] sortedEdges;

    return mstCount == vertexCount - 1;
}