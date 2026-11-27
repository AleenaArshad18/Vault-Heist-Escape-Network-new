#pragma once
class UnionFind
{
private:
    int* parent;
    int* rank;
    int size;

public:
    UnionFind(int size);
    ~UnionFind();

    int find(int vertex);
    bool unite(int vertex1, int vertex2);
};