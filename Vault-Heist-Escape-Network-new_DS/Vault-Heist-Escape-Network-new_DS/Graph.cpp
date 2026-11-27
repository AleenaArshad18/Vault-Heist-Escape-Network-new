#include "Graph.h"
#include <iostream>

using namespace std;

EdgeNode::EdgeNode()
{
    destinationIndex = -1;
    edgeID = -1;
    weight = 0;
    capacity = 0;
    next = nullptr;
}

Vertex::Vertex()
{
    id = -1;
    name = "";
    x = 0.0f;
    y = 0.0f;
    head = nullptr;
}

Edge::Edge()
{
    id = -1;
    fromID = -1;
    toID = -1;
    weight = 0;
    capacity = 0;
    directed = false;
}

Graph::Graph(int maxVertices, int maxEdges)
{
    if (maxVertices < 0)
    {
        maxVertices = 0;
    }

    if (maxEdges < 0)
    {
        maxEdges = 0;
    }

    this->maxVertices = maxVertices;
    this->maxEdges = maxEdges;

    vertexCount = 0;
    edgeCount = 0;

    vertices = new Vertex[this->maxVertices];
    edges = new Edge[this->maxEdges];
}

Graph::~Graph()
{
    clearAdjacencyLists();

    delete[] vertices;
    delete[] edges;

    vertices = nullptr;
    edges = nullptr;
}

void Graph::clearAdjacencyLists()
{
    for (int i = 0; i < vertexCount; i++)
    {
        EdgeNode* current = vertices[i].head;

        while (current != nullptr)
        {
            EdgeNode* nodeToDelete = current;
            current = current->next;
            delete nodeToDelete;
        }

        vertices[i].head = nullptr;
    }
}

int Graph::findVertexIndex(int vertexID) const
{
    for (int i = 0; i < vertexCount; i++)
    {
        if (vertices[i].id == vertexID)
        {
            return i;
        }
    }

    return -1;
}

bool Graph::addVertex(int id, const string& name, float x, float y)
{
    if (vertexCount >= maxVertices)
    {
        return false;
    }

    if (findVertexIndex(id) != -1)
    {
        return false;
    }

    vertices[vertexCount].id = id;
    vertices[vertexCount].name = name;
    vertices[vertexCount].x = x;
    vertices[vertexCount].y = y;
    vertices[vertexCount].head = nullptr;

    vertexCount++;

    return true;
}

bool Graph::addEdge(int fromID, int toID, int weight, int capacity, bool directed)
{
    if (edgeCount >= maxEdges || capacity < 0)
    {
        return false;
    }

    int fromIndex = findVertexIndex(fromID);
    int toIndex = findVertexIndex(toID);

    if (fromIndex == -1 || toIndex == -1)
    {
        return false;
    }

    int newEdgeID = edgeCount;

    EdgeNode* forwardNode = new EdgeNode;

    forwardNode->destinationIndex = toIndex;
    forwardNode->edgeID = newEdgeID;
    forwardNode->weight = weight;
    forwardNode->capacity = capacity;

    EdgeNode* reverseNode = nullptr;

    if (!directed)
    {
        reverseNode = new EdgeNode;

        reverseNode->destinationIndex = fromIndex;
        reverseNode->edgeID = newEdgeID;
        reverseNode->weight = weight;
        reverseNode->capacity = capacity;
    }

    forwardNode->next = vertices[fromIndex].head;
    vertices[fromIndex].head = forwardNode;

    if (!directed)
    {
        reverseNode->next = vertices[toIndex].head;
        vertices[toIndex].head = reverseNode;
    }

    edges[edgeCount].id = newEdgeID;
    edges[edgeCount].fromID = fromID;
    edges[edgeCount].toID = toID;
    edges[edgeCount].weight = weight;
    edges[edgeCount].capacity = capacity;
    edges[edgeCount].directed = directed;

    edgeCount++;

    return true;
}

int Graph::getVertexCount() const
{
    return vertexCount;
}

int Graph::getEdgeCount() const
{
    return edgeCount;
}

int Graph::getMaxVertices() const
{
    return maxVertices;
}

int Graph::getMaxEdges() const
{
    return maxEdges;
}

int Graph::getVertexIndexByID(int vertexID) const
{
    return findVertexIndex(vertexID);
}

const Vertex* Graph::getVertexAt(int index) const
{
    if (index < 0 || index >= vertexCount)
    {
        return nullptr;
    }

    return &vertices[index];
}

const Edge* Graph::getEdgeAt(int index) const
{
    if (index < 0 || index >= edgeCount)
    {
        return nullptr;
    }

    return &edges[index];
}

const EdgeNode* Graph::getAdjacencyHead(int vertexIndex) const
{
    if (vertexIndex < 0 || vertexIndex >= vertexCount)
    {
        return nullptr;
    }

    return vertices[vertexIndex].head;
}

void Graph::display() const
{
    cout << "Graph adjacency list" << endl;
    cout << "Vertices: " << vertexCount << endl;
    cout << "Edges: " << edgeCount << endl;
    cout << "------------------------" << endl;

    for (int i = 0; i < vertexCount; i++)
    {
        cout << vertices[i].id << " (" << vertices[i].name << "): ";

        const EdgeNode* current = vertices[i].head;

        if (current == nullptr)
        {
            cout << "No connections";
        }

        while (current != nullptr)
        {
            int destinationID = vertices[current->destinationIndex].id;

            cout << destinationID
                << "[weight=" << current->weight
                << ", capacity=" << current->capacity
                << ", edgeID=" << current->edgeID << "]";

            if (current->next != nullptr)
            {
                cout << " -> ";
            }

            current = current->next;
        }

        cout << endl;
    }
}

void Graph::displayAdjacencyMatrix() const
{
    int** matrix = new int* [vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        matrix[i] = new int[vertexCount];

        for (int j = 0; j < vertexCount; j++)
        {
            matrix[i][j] = -1;
        }
    }

    for (int i = 0; i < edgeCount; i++)
    {
        int fromIndex = findVertexIndex(edges[i].fromID);
        int toIndex = findVertexIndex(edges[i].toID);

        matrix[fromIndex][toIndex] = edges[i].weight;

        if (!edges[i].directed)
        {
            matrix[toIndex][fromIndex] = edges[i].weight;
        }
    }

    cout << "\nAdjacency Matrix\n\n";

    cout << "    ";

    for (int i = 0; i < vertexCount; i++)
    {
        cout << i << "   ";
    }

    cout << endl;

    for (int i = 0; i < vertexCount; i++)
    {
        cout << i << "   ";

        for (int j = 0; j < vertexCount; j++)
        {
            if (matrix[i][j] == -1)
            {
                cout << "-   ";
            }
            else
            {
                cout << matrix[i][j] << "   ";
            }
        }

        cout << endl;
    }

    for (int i = 0; i < vertexCount; i++)
    {
        delete[] matrix[i];
    }

    delete[] matrix;
}

int Graph::getDegree(int vertexID) const
{
    int index = findVertexIndex(vertexID);

    if (index == -1)
    {
        return -1;
    }

    int degree = 0;

    const EdgeNode* current = vertices[index].head;

    while (current != nullptr)
    {
        degree++;
        current = current->next;
    }

    return degree;
}

void Graph::getNeighbours(int vertexID, int neighbours[], int& count) const
{
    count = 0;

    int index = findVertexIndex(vertexID);

    if (index == -1)
    {
        return;
    }

    const EdgeNode* current = vertices[index].head;

    while (current != nullptr)
    {
        neighbours[count] = vertices[current->destinationIndex].id;
        count++;

        current = current->next;
    }
}

void Graph::displayNeighbours(int vertexID) const
{
    int neighbours[100];
    int count = 0;

    getNeighbours(vertexID, neighbours, count);

    if (findVertexIndex(vertexID) == -1)
    {
        cout << "Vertex not found!" << endl;
        return;
    }

    cout << "Neighbours of " << vertexID << ": ";

    if (count == 0)
    {
        cout << "No neighbours";
    }

    for (int i = 0; i < count; i++)
    {
        cout << neighbours[i] << " ";
    }

    cout << endl;
}

void Graph::BFS(int startVertexID) const
{
    int startIndex = findVertexIndex(startVertexID);

    if (startIndex == -1)
    {
        cout << "Starting vertex not found!" << endl;
        return;
    }

    bool* visited = new bool[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    int* queue = new int[vertexCount];

    int front = 0;
    int rear = 0;

    visited[startIndex] = true;
    queue[rear] = startIndex;
    rear++;

    cout << "BFS Traversal: ";

    while (front < rear)
    {
        int currentIndex = queue[front];
        front++;

        cout << vertices[currentIndex].id << " ";

        const EdgeNode* current = vertices[currentIndex].head;

        while (current != nullptr)
        {
            int neighbourIndex = current->destinationIndex;

            if (!visited[neighbourIndex])
            {
                visited[neighbourIndex] = true;

                queue[rear] = neighbourIndex;
                rear++;
            }

            current = current->next;
        }
    }

    cout << endl;

    delete[] visited;
    delete[] queue;
}

void Graph::DFS(int startVertexID) const
{
    int startIndex = findVertexIndex(startVertexID);

    if (startIndex == -1)
    {
        cout << "Starting vertex not found!" << endl;
        return;
    }

    bool* visited = new bool[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    int* stack = new int[vertexCount];

    int top = -1;

    stack[++top] = startIndex;

    cout << "DFS Traversal: ";

    while (top != -1)
    {
        int currentIndex = stack[top--];

        if (!visited[currentIndex])
        {
            visited[currentIndex] = true;

            cout << vertices[currentIndex].id << " ";

            const EdgeNode* current = vertices[currentIndex].head;

            int neighbours[100];
            int count = 0;

            while (current != nullptr)
            {
                neighbours[count] = current->destinationIndex;
                count++;

                current = current->next;
            }

            for (int i = count - 1; i >= 0; i--)
            {
                if (!visited[neighbours[i]])
                {
                    stack[++top] = neighbours[i];
                }
            }
        }
    }

    cout << endl;

    delete[] visited;
    delete[] stack;
}

int Graph::countConnectedComponents() const
{
    bool* visited = new bool[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    int components = 0;

    int* stack = new int[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        if (!visited[i])
        {
            components++;

            int top = -1;

            stack[++top] = i;
            visited[i] = true;

            while (top != -1)
            {
                int currentIndex = stack[top--];

                const EdgeNode* current = vertices[currentIndex].head;

                while (current != nullptr)
                {
                    int neighbourIndex = current->destinationIndex;

                    if (!visited[neighbourIndex])
                    {
                        visited[neighbourIndex] = true;
                        stack[++top] = neighbourIndex;
                    }

                    current = current->next;
                }
            }
        }
    }

    delete[] visited;
    delete[] stack;

    return components;
}

void Graph::displayConnectedComponents() const
{
    bool* visited = new bool[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    int* stack = new int[vertexCount];

    int componentNumber = 0;

    cout << "Connected Components:" << endl;

    for (int i = 0; i < vertexCount; i++)
    {
        if (!visited[i])
        {
            componentNumber++;

            cout << "Component " << componentNumber << ": ";

            int top = -1;

            stack[++top] = i;
            visited[i] = true;

            while (top != -1)
            {
                int currentIndex = stack[top--];

                cout << vertices[currentIndex].id << " ";

                const EdgeNode* current = vertices[currentIndex].head;

                while (current != nullptr)
                {
                    int neighbourIndex = current->destinationIndex;

                    if (!visited[neighbourIndex])
                    {
                        visited[neighbourIndex] = true;
                        stack[++top] = neighbourIndex;
                    }

                    current = current->next;
                }
            }

            cout << endl;
        }
    }

    cout << "Total Components: " << componentNumber << endl;

    delete[] visited;
    delete[] stack;
}

bool Graph::hasCycleDFS(int currentIndex, int parentIndex, bool visited[]) const
{
    visited[currentIndex] = true;

    const EdgeNode* current = vertices[currentIndex].head;

    while (current != nullptr)
    {
        int neighbourIndex = current->destinationIndex;

        if (!visited[neighbourIndex])
        {
            if (hasCycleDFS(neighbourIndex, currentIndex, visited))
            {
                return true;
            }
        }
        else if (neighbourIndex != parentIndex)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool Graph::hasCycleUndirected() const
{
    bool* visited = new bool[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    for (int i = 0; i < vertexCount; i++)
    {
        if (!visited[i])
        {
            if (hasCycleDFS(i, -1, visited))
            {
                delete[] visited;
                return true;
            }
        }
    }

    delete[] visited;

    return false;
}

void Graph::displayEdges() const
{
    cout << "Graph Edges:" << endl;

    for (int i = 0; i < edgeCount; i++)
    {
        cout << edges[i].fromID << " -> " << edges[i].toID
            << " | Weight: " << edges[i].weight
            << " | Capacity: " << edges[i].capacity
            << " | Directed: ";

        if (edges[i].directed)
        {
            cout << "Yes";
        }
        else
        {
            cout << "No";
        }

        cout << endl;
    }
}

bool Graph::hasNegativeWeight() const
{
    for (int i = 0; i < edgeCount; i++)
    {
        if (edges[i].weight < 0)
        {
            return true;
        }
    }

    return false;
}

bool Graph::hasNonNegativeWeights() const
{
    return !hasNegativeWeight();
}