#pragma once
#include "Graph.h"

class EventRecorder;

class Kruskal
{
private:
    static void mergeSort(Edge edges[], Edge temp[], int left, int right);

    static void merge(
        Edge edges[],
        Edge temp[],
        int left,
        int middle,
        int right
    );

public:
    bool findMST(
        const Graph& graph,
        Edge mstEdges[],
        int& mstCount,
        int& totalWeight,
        EventRecorder* recorder = nullptr
    ) const;
};