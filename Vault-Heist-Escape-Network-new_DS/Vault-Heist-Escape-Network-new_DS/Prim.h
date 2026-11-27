#pragma once
#include "Graph.h"

class Prim
{
public:
    bool findMST(const Graph& graph, int startVertexID, Edge mstEdges[], int& mstCount, int& totalWeight) const;
};