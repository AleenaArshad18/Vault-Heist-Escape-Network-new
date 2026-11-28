#include "Kruskal.h"
#include "UnionFind.h"
#include "EventRecorder.h"
#include <iostream>

using namespace std;

bool Kruskal::findMST(
    const Graph& graph,
    Edge mstEdges[],
    int& mstCount,
    int& totalWeight,
    EventRecorder* recorder) const
{
    int vertexCount = graph.getVertexCount();
    int edgeCount = graph.getEdgeCount();

    mstCount = 0;
    totalWeight = 0;

    if (recorder != nullptr)
    {
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_STARTED,
            -1, -1, -1, -1, 0, 0,
            "Kruskal started"
        ));
    }

    if (vertexCount == 0)
    {
        cout << "Graph is empty!" << endl;

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1, 0, 0,
                "Kruskal failed: empty graph"
            ));

        return false;
    }

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (edge->directed)
        {
            cout << "Kruskal requires an undirected graph!" << endl;

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ALGORITHM_FAILED,
                    -1, edge->id, edge->fromID, edge->toID,
                    0, 0, "Kruskal requires an undirected graph"
                ));

            return false;
        }
    }

    if (vertexCount == 1)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_COMPLETED,
                -1, -1, -1, -1, 0, 0,
                "Kruskal completed"
            ));

        return true;
    }

    Edge* sortedEdges = new Edge[edgeCount];

    for (int i = 0; i < edgeCount; i++)
        sortedEdges[i] = *graph.getEdgeAt(i);

    // Selection sort by ascending edge weight.

    for (int i = 0; i < edgeCount - 1; i++)
    {
        int minimumIndex = i;

        for (int j = i + 1; j < edgeCount; j++)
        {
            if (sortedEdges[j].weight < sortedEdges[minimumIndex].weight)
                minimumIndex = j;
        }

        Edge temp = sortedEdges[i];
        sortedEdges[i] = sortedEdges[minimumIndex];
        sortedEdges[minimumIndex] = temp;
    }

    UnionFind groups(vertexCount);

    for (int i = 0; i < edgeCount; i++)
    {
        int fromIndex = graph.getVertexIndexByID(sortedEdges[i].fromID);
        int toIndex = graph.getVertexIndexByID(sortedEdges[i].toID);

        if (fromIndex == -1 || toIndex == -1)
            continue;

        if (recorder != nullptr)
        {
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::EDGE_EXAMINED,
                -1,
                sortedEdges[i].id,
                sortedEdges[i].fromID,
                sortedEdges[i].toID,
                sortedEdges[i].weight,
                0,
                "Examining edge"
            ));
        }

        if (groups.unite(fromIndex, toIndex))
        {
            mstEdges[mstCount] = sortedEdges[i];

            mstCount++;
            totalWeight += sortedEdges[i].weight;

            if (recorder != nullptr)
            {
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::EDGE_SELECTED,
                    -1,
                    sortedEdges[i].id,
                    sortedEdges[i].fromID,
                    sortedEdges[i].toID,
                    sortedEdges[i].weight,
                    totalWeight,
                    "Edge selected for MST"
                ));
            }

            if (mstCount == vertexCount - 1)
                break;
        }
        else
        {
            if (recorder != nullptr)
            {
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::EDGE_REJECTED,
                    -1,
                    sortedEdges[i].id,
                    sortedEdges[i].fromID,
                    sortedEdges[i].toID,
                    sortedEdges[i].weight,
                    0,
                    "Edge rejected because it creates a cycle"
                ));
            }
        }
    }

    delete[] sortedEdges;

    if (mstCount != vertexCount - 1)
    {
        cout << "Minimum Spanning Tree does not exist. Graph is disconnected!" << endl;

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1, mstCount, vertexCount - 1,
                "Kruskal failed: graph is disconnected"
            ));

        return false;
    }

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1, totalWeight, mstCount,
            "Kruskal completed"
        ));

    return true;
}