#pragma once
#include "Graph.h"

class BellmanFord
{
public:
    bool findShortestPaths(const Graph& graph, int startVertexID, int distances[], int previous[]) const;
};