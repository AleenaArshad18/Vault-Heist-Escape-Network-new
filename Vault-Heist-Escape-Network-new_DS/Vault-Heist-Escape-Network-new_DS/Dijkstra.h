#pragma once
#include "Graph.h"

class Dijkstra
{
public:
    void findShortestPaths(const Graph& graph, int startVertexID, int distances[], int previous[]) const;
};