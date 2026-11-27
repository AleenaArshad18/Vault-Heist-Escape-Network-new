#pragma once
#include "Graph.h"

class Kruskal
{
public:
    bool findMST(const Graph& graph, Edge mstEdges[], int& mstCount, int& totalWeight) const;
};
