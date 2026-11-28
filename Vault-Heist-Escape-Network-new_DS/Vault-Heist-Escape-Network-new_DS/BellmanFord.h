
#pragma once
#include "Graph.h"

class EventRecorder;

class BellmanFord
{
public:
    bool findShortestPaths(
        const Graph& graph,
        int startVertexID,
        int distances[],
        int previous[],
        EventRecorder* recorder = nullptr
    ) const;
};
