#include "UnionFind.h"

using namespace std;

UnionFind::UnionFind(int size)
{
    this->size = size;

    parent = new int[size];
    rank = new int[size];

    for (int i = 0; i < size; i++)
    {
        parent[i] = i;
        rank[i] = 0;
    }
}

UnionFind::~UnionFind()
{
    delete[] parent;
    delete[] rank;
}

int UnionFind::find(int vertex)
{
    if (parent[vertex] != vertex)
    {
        parent[vertex] = find(parent[vertex]);
    }

    return parent[vertex];
}

bool UnionFind::unite(int vertex1, int vertex2)
{
    int root1 = find(vertex1);
    int root2 = find(vertex2);

    if (root1 == root2)
    {
        return false;
    }

    if (rank[root1] < rank[root2])
    {
        parent[root1] = root2;
    }
    else if (rank[root1] > rank[root2])
    {
        parent[root2] = root1;
    }
    else
    {
        parent[root2] = root1;
        rank[root1]++;
    }

    return true;
}