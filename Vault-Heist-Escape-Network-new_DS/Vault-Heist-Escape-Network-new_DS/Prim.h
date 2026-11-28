#pragma once
#include "Graph.h"

class EventRecorder;

class Prim
{
public:
    bool findMST(
        const Graph& graph,
        int startVertexID,
        Edge mstEdges[],
        int& mstCount,
        int& totalWeight,
        EventRecorder* recorder = nullptr
    ) const;
};