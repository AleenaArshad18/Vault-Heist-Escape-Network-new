#pragma once
#include "Graph.h"

class CriticalPoints
{
private:
    void DFS(const Graph& graph, int currentIndex, int parentEdgeID, int& timer,
        bool visited[], int discovery[], int low[], Edge bridges[], int& bridgeCount,
        bool articulationFlags[]) const;

public:
    bool findCriticalPoints(const Graph& graph, Edge bridges[], int& bridgeCount,
        int articulationPoints[], int& articulationCount) const;
};