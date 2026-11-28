
#include "Prim.h"
#include "MinHeap.h"
#include "EventRecorder.h"

bool Prim::findMST(
    const Graph& graph,
    int startVertexID,
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
            startVertexID, -1, -1, -1,
            0, 0, "Prim started"
        ));

    if (vertexCount == 0)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1,
                0, 0, "Prim failed: empty graph"
            ));

        return false;
    }

    for (int i = 0; i < edgeCount; i++)
    {
        const Edge* edge = graph.getEdgeAt(i);

        if (edge->directed)
        {
            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ALGORITHM_FAILED,
                    -1, edge->id, edge->fromID, edge->toID,
                    0, 0, "Prim requires an undirected graph"
                ));

            return false;
        }
    }

    int startIndex = graph.getVertexIndexByID(startVertexID);

    if (startIndex == -1)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                startVertexID, -1, -1, -1,
                0, 0, "Prim failed: starting vertex not found"
            ));

        return false;
    }

    if (vertexCount == 1)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_COMPLETED,
                -1, -1, -1, -1,
                0, 0, "Prim completed"
            ));

        return true;
    }

    bool* visited = new bool[vertexCount];

    for (int i = 0; i < vertexCount; i++)
        visited[i] = false;

    MinHeap minHeap(edgeCount + 1);

    visited[startIndex] = true;

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::VERTEX_DISCOVERED,
            startVertexID, -1, -1, -1,
            0, 0, "Starting vertex discovered"
        ));

    // Add all edges leaving the starting vertex.
    const EdgeNode* current = graph.getAdjacencyHead(startIndex);

    while (current != nullptr)
    {
        int neighbour = current->destinationIndex;

        const Vertex* fromVertex = graph.getVertexAt(startIndex);
        const Vertex* toVertex = graph.getVertexAt(neighbour);

        minHeap.insert(
            current->weight,
            neighbour,
            startIndex,
            current->edgeID
        );

        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::EDGE_EXAMINED,
                toVertex->id,
                current->edgeID,
                fromVertex->id,
                toVertex->id,
                current->weight,
                0,
                "Examining edge"
            ));

        current = current->next;
    }

    HeapNode selectedNode;

    while (mstCount < vertexCount - 1 &&
        minHeap.extractMin(selectedNode))
    {
        int fromIndex = selectedNode.parentIndex;
        int toIndex = selectedNode.vertexIndex;

        // Ignore an edge if its destination is already in the MST.
        if (visited[toIndex])
            continue;

        // Edge IDs are assigned sequentially by Graph, so use direct lookup.
        const Edge* selectedEdge = graph.getEdgeAt(selectedNode.edgeID);

        if (selectedEdge == nullptr)
        {
            delete[] visited;
            return false;
        }

        mstEdges[mstCount] = *selectedEdge;

        mstCount++;
        totalWeight += selectedNode.priority;

        visited[toIndex] = true;

        const Vertex* fromVertex = graph.getVertexAt(fromIndex);
        const Vertex* toVertex = graph.getVertexAt(toIndex);

        if (recorder != nullptr)
        {
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::EDGE_SELECTED,
                toVertex->id,
                selectedNode.edgeID,
                fromVertex->id,
                toVertex->id,
                selectedNode.priority,
                totalWeight,
                "Minimum edge selected for MST"
            ));

            recorder->record(AlgorithmEvent(
                AlgorithmEvent::VERTEX_DISCOVERED,
                toVertex->id,
                selectedNode.edgeID,
                fromVertex->id,
                toVertex->id,
                selectedNode.priority,
                0,
                "New vertex added to MST"
            ));

            recorder->record(AlgorithmEvent(
                AlgorithmEvent::VERTEX_PROCESSED,
                toVertex->id,
                -1, -1, -1,
                totalWeight,
                mstCount,
                "Vertex processed"
            ));
        }

        // Add edges from the newly selected vertex.
        current = graph.getAdjacencyHead(toIndex);

        while (current != nullptr)
        {
            int neighbour = current->destinationIndex;

            if (!visited[neighbour])
            {
                const Vertex* neighbourVertex =
                    graph.getVertexAt(neighbour);

                minHeap.insert(
                    current->weight,
                    neighbour,
                    toIndex,
                    current->edgeID
                );

                if (recorder != nullptr)
                    recorder->record(AlgorithmEvent(
                        AlgorithmEvent::EDGE_EXAMINED,
                        neighbourVertex->id,
                        current->edgeID,
                        toVertex->id,
                        neighbourVertex->id,
                        current->weight,
                        0,
                        "Examining edge"
                    ));
            }

            current = current->next;
        }
    }

    delete[] visited;

    if (mstCount != vertexCount - 1)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1,
                mstCount, vertexCount - 1,
                "Prim failed: graph is disconnected"
            ));

        return false;
    }

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1,
            totalWeight, mstCount,
            "Prim completed"
        ));

    return true;
}
