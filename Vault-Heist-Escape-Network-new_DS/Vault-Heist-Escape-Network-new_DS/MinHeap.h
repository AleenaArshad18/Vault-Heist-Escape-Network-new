
#pragma once

struct HeapNode
{
    int priority;
    int vertexIndex;
    int parentIndex;
    int edgeID;

    HeapNode();
};

class MinHeap
{
private:
    HeapNode* heap;
    int size;
    int capacity;

    void heapifyUp(int index);
    void heapifyDown(int index);
    void expand();

public:
    MinHeap(int initialCapacity);
    ~MinHeap();

    MinHeap(const MinHeap& other) = delete;
    MinHeap& operator=(const MinHeap& other) = delete;

    void insert(int priority, int vertexIndex, int parentIndex = -1, int edgeID = -1);
    bool extractMin(HeapNode& result);

    bool isEmpty() const;
    int getSize() const;
    void clear();
};
