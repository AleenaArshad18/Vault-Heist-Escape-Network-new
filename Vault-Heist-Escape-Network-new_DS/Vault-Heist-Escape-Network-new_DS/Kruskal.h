#pragma once
#include "Graph.h"

class EventRecorder;

class Kruskal
{
public:
    bool findMST(
        const Graph& graph,
        Edge mstEdges[],
        int& mstCount,
        int& totalWeight,
        EventRecorder* recorder = nullptr
    ) const;
};