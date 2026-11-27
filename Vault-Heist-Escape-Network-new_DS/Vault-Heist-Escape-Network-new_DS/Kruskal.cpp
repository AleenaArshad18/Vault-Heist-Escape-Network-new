
#include "Kruskal.h"
#include "UnionFind.h"
#include <iostream>

using namespace std;

bool Kruskal::findMST(const Graph& graph, Edge mstEdges[], int& mstCount, int& totalWeight) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    mstCount = 0;
    totalWeight = 0;

    // An empty graph cannot have an MST.

    if (vertexCount == 0)
    {
        cout << "Graph is empty!" << endl;
        return false;
    }

    // Kruskal requires an undirected graph.

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (edge->directed)
        {
            cout << "Kruskal requires an undirected graph!" << endl;
            return false;
        }
    }

    // A graph with one vertex already has an MST.

    if (vertexCount == 1)
    {
        return true;
    }

    // Create a temporary copy of all edges.

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

    // Create UnionFind to track connected groups.

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

            // An MST contains exactly V-1 edges.

            if (mstCount == vertexCount - 1)
            {
                break;
            }
        }
    }

    delete[] sortedEdges;

    // If we could not select V-1 edges, the graph is disconnected.

    if (mstCount != vertexCount - 1)
    {
        cout << "Minimum Spanning Tree does not exist. Graph is disconnected!" << endl;
        return false;
    }

    return true;
}
