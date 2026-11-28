#include "Prim.h"
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
            startVertexID, -1, -1, -1, 0, 0,
            "Prim started"
        ));

    if (vertexCount == 0)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_FAILED,
                -1, -1, -1, -1, 0, 0,
                "Prim failed: empty graph"
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
                startVertexID, -1, -1, -1, 0, 0,
                "Prim failed: starting vertex not found"
            ));

        return false;
    }

    if (vertexCount == 1)
    {
        if (recorder != nullptr)
            recorder->record(AlgorithmEvent(
                AlgorithmEvent::ALGORITHM_COMPLETED,
                -1, -1, -1, -1, 0, 0,
                "Prim completed"
            ));

        return true;
    }

    bool* visited = new bool[vertexCount];

    const int INF = 1000000000;

    for (int i = 0; i < vertexCount; i++)
        visited[i] = false;

    visited[startIndex] = true;

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::VERTEX_DISCOVERED,
            startVertexID, -1, -1, -1, 0, 0,
            "Starting vertex discovered"
        ));

    while (mstCount < vertexCount - 1)
    {
        int minimumWeight = INF;

        int selectedFrom = -1;
        int selectedTo = -1;
        int selectedEdgeID = -1;

        for (int i = 0; i < vertexCount; i++)
        {
            if (visited[i])
            {
                const EdgeNode* current = graph.getAdjacencyHead(i);

                while (current != nullptr)
                {
                    int neighbour = current->destinationIndex;

                    if (!visited[neighbour])
                    {
                        if (recorder != nullptr)
                        {
                            const Vertex* fromVertex = graph.getVertexAt(i);
                            const Vertex* toVertex = graph.getVertexAt(neighbour);

                            recorder->record(AlgorithmEvent(
                                AlgorithmEvent::EDGE_EXAMINED,
                                toVertex->id,
                                current->edgeID,
                                fromVertex->id,
                                toVertex->id,
                                current->weight,
                                minimumWeight,
                                "Examining edge"
                            ));
                        }

                        if (current->weight < minimumWeight)
                        {
                            minimumWeight = current->weight;
                            selectedFrom = i;
                            selectedTo = neighbour;
                            selectedEdgeID = current->edgeID;
                        }
                    }

                    current = current->next;
                }
            }
        }

        if (selectedTo == -1)
        {
            delete[] visited;

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ALGORITHM_FAILED,
                    -1, -1, -1, -1, mstCount, vertexCount - 1,
                    "Prim failed: graph is disconnected"
                ));

            return false;
        }

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

            if (recorder != nullptr)
                recorder->record(AlgorithmEvent(
                    AlgorithmEvent::ALGORITHM_FAILED,
                    -1, selectedEdgeID, -1, -1, 0, 0,
                    "Prim failed: selected edge not found"
                ));

            return false;
        }

        mstEdges[mstCount] = *selectedEdge;

        mstCount++;
        totalWeight += minimumWeight;

        visited[selectedTo] = true;

        if (recorder != nullptr)
        {
            const Vertex* fromVertex = graph.getVertexAt(selectedFrom);
            const Vertex* toVertex = graph.getVertexAt(selectedTo);

            recorder->record(AlgorithmEvent(
                AlgorithmEvent::EDGE_SELECTED,
                toVertex->id,
                selectedEdgeID,
                fromVertex->id,
                toVertex->id,
                minimumWeight,
                totalWeight,
                "Minimum edge selected for MST"
            ));

            recorder->record(AlgorithmEvent(
                AlgorithmEvent::VERTEX_DISCOVERED,
                toVertex->id,
                selectedEdgeID,
                fromVertex->id,
                toVertex->id,
                minimumWeight,
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
    }

    delete[] visited;

    if (recorder != nullptr)
        recorder->record(AlgorithmEvent(
            AlgorithmEvent::ALGORITHM_COMPLETED,
            -1, -1, -1, -1, totalWeight, mstCount,
            "Prim completed"
        ));

    return true;
}