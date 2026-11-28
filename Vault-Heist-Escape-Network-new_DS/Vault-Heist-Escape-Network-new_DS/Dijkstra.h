#pragma once
#include "Graph.h"

class EventRecorder;

class Dijkstra{
public:
    bool findShortestPaths(const Graph& graph, int startVertexID, int distances[], int previous[], EventRecorder* recorder = nullptr) const;
};
