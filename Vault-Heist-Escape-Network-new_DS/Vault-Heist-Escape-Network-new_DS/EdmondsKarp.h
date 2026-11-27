#pragma once
#include "Graph.h"

class EdmondsKarp
{
public:
    bool findMaxFlow(const Graph& graph, int sourceID, int sinkID,
        int& maxFlow, Edge minCutEdges[], int& minCutCount) const;
};