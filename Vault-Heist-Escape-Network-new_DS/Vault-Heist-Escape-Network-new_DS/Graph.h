#pragma once

#include <string>

using namespace std;

struct EdgeNode
{
    int destinationIndex;
    int edgeID;
    int weight;
    int capacity;
    EdgeNode* next;

    EdgeNode();
};

struct Vertex
{
    int id;
    string name;
    float x;
    float y;
    EdgeNode* head;

    Vertex();
};

struct Edge
{
    int id;
    int fromID;
    int toID;
    int weight;
    int capacity;
    bool directed;

    Edge();
};

class Graph
{
private:
    Vertex* vertices;
    Edge* edges;

    int vertexCount;
    int edgeCount;
    int maxVertices;
    int maxEdges;

    int findVertexIndex(int vertexID) const;
    void clearAdjacencyLists();
    bool hasOnlyUndirectedEdges() const;

public:
    Graph(int maxVertices, int maxEdges);
    ~Graph();

    Graph(const Graph& other) = delete;
    Graph& operator=(const Graph& other) = delete;

    bool addVertex(int id, const string& name, float x, float y);

    bool addEdge(int fromID, int toID, int weight, int capacity, bool directed);

    int getVertexCount() const;
    int getEdgeCount() const;
    int getMaxVertices() const;
    int getMaxEdges() const;

    int getVertexIndexByID(int vertexID) const;

    const Vertex* getVertexAt(int index) const;
    const Edge* getEdgeAt(int index) const;
    const EdgeNode* getAdjacencyHead(int vertexIndex) const;

    void display() const;
    void displayAdjacencyMatrix() const;

    int getDegree(int vertexID) const;
    void getNeighbours(int vertexID, int neighbours[], int& count, int maxSize) const;
    void displayNeighbours(int vertexID) const;

    void BFS(int startVertexID) const;
    void DFS(int startVertexID) const;

    int countConnectedComponents() const;

    void displayConnectedComponents() const;
    bool hasCycleUndirected() const;
    bool hasCycleDFS(int currentIndex, int parentEdgeID, bool visited[]) const;

    void displayEdges() const;
    bool hasNegativeWeight() const;
    bool hasNonNegativeWeights() const;

};
