#pragma once
#include "Graph.h"

class EventRecorder;

class TopologicalSort
{
public:
    bool kahnSort(const Graph& graph, int result[], int& resultCount) const;
};