#pragma once
#include "Graph.h"

class EventRecorder;

class Bipartite
{
public:
    bool checkBipartite(const Graph& graph, int colours[]) const;
};
