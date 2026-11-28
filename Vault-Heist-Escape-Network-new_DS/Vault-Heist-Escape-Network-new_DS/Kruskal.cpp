
#include "Kruskal.h"
#include "UnionFind.h"
#include "MinHeap.h"
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
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_STARTED,
            -1, -1, -1, -1,
            0, 0, "Kruskal started"
        ));

    if (vertexCount == 0)
    {
        cout << "Graph is empty!" << endl;

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1,
                0, 0, "Kruskal failed: empty graph"
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
                -1, -1, -1, -1,
                0, 0, "Kruskal completed"
            ));

        return true;
    }

    MinHeap minHeap(edgeCount + 1);

    // Insert every graph edge into the min-heap.
    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        minHeap.insert(
            edge->weight,
            edge->fromID,
            edge->toID,
            edge->id
        );
    }

    UnionFind groups(vertexCount);

    HeapNode currentNode;

    while (mstCount < vertexCount - 1 &&
        minHeap.extractMin(currentNode))
    {
        // Edge IDs are assigned sequentially by Graph, so use direct lookup.
        const Edge* currentEdge = graph.getEdgeAt(currentNode.edgeID);

        if (currentEdge == nullptr)
            continue;

        int fromIndex =
            graph.getVertexIndexByID(currentEdge->fromID);

        int toIndex =
            graph.getVertexIndexByID(currentEdge->toID);

        if (fromIndex == -1 || toIndex == -1)
            continue;

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::EDGE_EXAMINED,
                -1,
                currentEdge->id,
                currentEdge->fromID,
                currentEdge->toID,
                currentEdge->weight,
                0,
                "Examining edge"
            ));

        if (groups.unite(fromIndex, toIndex))
        {
            mstEdges[mstCount] = *currentEdge;

            mstCount++;
            totalWeight += currentEdge->weight;

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::EDGE_SELECTED,
                    -1,
                    currentEdge->id,
                    currentEdge->fromID,
                    currentEdge->toID,
                    currentEdge->weight,
                    totalWeight,
                    "Edge selected for MST"
                ));
        }
        else
        {
            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::EDGE_REJECTED,
                    -1,
                    currentEdge->id,
                    currentEdge->fromID,
                    currentEdge->toID,
                    currentEdge->weight,
                    0,
                    "Edge rejected because it creates a cycle"
                ));
        }
    }

    if (mstCount != vertexCount - 1)
    {
        cout << "Minimum Spanning Tree does not exist. Graph is disconnected!" << endl;

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1,
                mstCount, vertexCount - 1,
                "Kruskal failed: graph is disconnected"
            ));

        return false;
    }

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1,
            totalWeight, mstCount,
            "Kruskal completed"
        ));

    return true;
}
