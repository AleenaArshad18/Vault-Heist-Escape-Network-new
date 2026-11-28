
#include "MinHeap.h"

HeapNode::HeapNode()
{
    priority = 0;
    vertexIndex = -1;
    parentIndex = -1;
    edgeID = -1;
}

MinHeap::MinHeap(int initialCapacity)
{
    if (initialCapacity < 1)
        initialCapacity = 1;

    capacity = initialCapacity;
    size = 0;

    heap = new HeapNode[capacity];
}

MinHeap::~MinHeap()
{
    delete[] heap;
    heap = nullptr;
}

void MinHeap::expand()
{
    int newCapacity = capacity * 2;

    HeapNode* newHeap = new HeapNode[newCapacity];

    for (int i = 0; i < size; i++)
        newHeap[i] = heap[i];

    delete[] heap;

    heap = newHeap;
    capacity = newCapacity;
}

void MinHeap::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap[parent].priority <= heap[index].priority)
            break;

        HeapNode temp = heap[parent];
        heap[parent] = heap[index];
        heap[index] = temp;

        index = parent;
    }
}

void MinHeap::heapifyDown(int index)
{
    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size &&
            heap[left].priority < heap[smallest].priority)
        {
            smallest = left;
        }

        if (right < size &&
            heap[right].priority < heap[smallest].priority)
        {
            smallest = right;
        }

        if (smallest == index)
            break;

        HeapNode temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;

        index = smallest;
    }
}

void MinHeap::insert(
    int priority,
    int vertexIndex,
    int parentIndex,
    int edgeID)
{
    if (size == capacity)
        expand();

    heap[size].priority = priority;
    heap[size].vertexIndex = vertexIndex;
    heap[size].parentIndex = parentIndex;
    heap[size].edgeID = edgeID;

    heapifyUp(size);

    size++;
}

bool MinHeap::extractMin(HeapNode& result)
{
    if (size == 0)
        return false;

    result = heap[0];

    size--;

    if (size > 0)
    {
        heap[0] = heap[size];
        heapifyDown(0);
    }

    return true;
}

bool MinHeap::isEmpty() const
{
    return size == 0;
}

int MinHeap::getSize() const
{
    return size;
}

void MinHeap::clear()
{
    size = 0;
}
