#include <iostream>
#include "Graph.h"
#include "Dijkstra.h"
#include "BellmanFord.h"
#include "Kruskal.h"
#include "Prim.h"
#include "TopologicalSort.h"
#include "Bipartite.h"
#include "CriticalPoints.h"
#include "EdmondsKarp.h"

using namespace std;

int main()
{
    Graph graph(10, 20);

    graph.addVertex(0, "Vault", 100, 100);
    graph.addVertex(1, "Bank", 300, 100);
    graph.addVertex(2, "Airport", 500, 100);

    graph.addEdge(0, 1, 5, 10, false);
    graph.addEdge(1, 2, 8, 15, false);
    graph.addEdge(0, 2, 12, 5, true);

    graph.display();

    graph.display();

    graph.displayAdjacencyMatrix();

    cout << "\nDegree of Vault: " << graph.getDegree(0) << endl;

    graph.displayNeighbours(0);

    graph.displayNeighbours(1);

    graph.displayNeighbours(2);

    cout << endl;
    graph.BFS(0);

    cout << endl;
    graph.DFS(0);

    cout << endl;

    cout << "Number of Connected Components: "<< graph.countConnectedComponents() << endl;

    graph.displayConnectedComponents();

    cout << endl;

    if (graph.hasCycleUndirected())
    {
        cout << "Graph contains a cycle." << endl;
    }
    else
    {
        cout << "Graph does not contain a cycle." << endl;
    }

    cout << endl;

    graph.displayEdges();

    cout << endl;

    if (graph.hasNegativeWeight())
    {
        cout << "Graph has negative weights." << endl;
    }
    else
    {
        cout << "Graph has no negative weights." << endl;
    }

    Dijkstra dijkstra;

    int distances[10];
    int previous[10];

    dijkstra.findShortestPaths(graph, 0, distances, previous);

    cout << endl;
    cout << "Dijkstra Shortest Distances:" << endl;

    for (int i = 0; i < graph.getVertexCount(); i++)
    {
        const Vertex* vertex = graph.getVertexAt(i);

        cout << "Vertex " << vertex->id << ": ";

        if (distances[i] == 1000000000)
        {
            cout << "Unreachable";
        }
        else
        {
            cout << distances[i];
        }

        cout << endl;
    }

    Graph testGraph(10, 20);

    testGraph.addVertex(0, "Vault", 100, 100);
    testGraph.addVertex(1, "Bank", 300, 100);
    testGraph.addVertex(2, "Airport", 500, 100);

    testGraph.addEdge(0, 1, 4, 10, true);
    testGraph.addEdge(0, 2, 5, 10, true);
    testGraph.addEdge(1, 2, -2, 10, true);

    BellmanFord bellmanFord;

    int bfDistances[10];
    int bfPrevious[10];

    bool success = bellmanFord.findShortestPaths(testGraph, 0, bfDistances, bfPrevious);

    if (success)
    {
        cout << endl;
        cout << "Bellman-Ford Shortest Distances:" << endl;

        for (int i = 0; i < testGraph.getVertexCount(); i++)
        {
            const Vertex* vertex = testGraph.getVertexAt(i);

            cout << "Vertex " << vertex->id << ": ";

            if (bfDistances[i] == 1000000000)
            {
                cout << "Unreachable";
            }
            else
            {
                cout << bfDistances[i];
            }

            cout << endl;
        }
    }

    Graph mstGraph(10, 20);

    mstGraph.addVertex(0, "Vault", 100, 100);
    mstGraph.addVertex(1, "Bank", 300, 100);
    mstGraph.addVertex(2, "Airport", 500, 100);
    mstGraph.addVertex(3, "Museum", 300, 300);

    mstGraph.addEdge(0, 1, 10, 10, false);
    mstGraph.addEdge(0, 2, 6, 10, false);
    mstGraph.addEdge(0, 3, 5, 10, false);
    mstGraph.addEdge(1, 3, 15, 10, false);
    mstGraph.addEdge(2, 3, 4, 10, false);

    Kruskal kruskal;

    Edge mstEdges[10];
    int mstCount;
    int totalWeight;

    bool success1 = kruskal.findMST(mstGraph, mstEdges, mstCount, totalWeight);

    if (success1)
    {
        cout << endl;
        cout << "Kruskal Minimum Spanning Tree:" << endl;

        for (int i = 0; i < mstCount; i++)
        {
            cout << mstEdges[i].fromID << " -> " << mstEdges[i].toID;
            cout << " Weight: " << mstEdges[i].weight << endl;
        }

        cout << "Total MST Weight: " << totalWeight << endl;
    }
    else
    {
        cout << "Graph does not have a complete Minimum Spanning Tree." << endl;
    }

    Graph primGraph(10, 20);

    primGraph.addVertex(0, "Vault", 100, 100);
    primGraph.addVertex(1, "Bank", 300, 100);
    primGraph.addVertex(2, "Airport", 500, 100);
    primGraph.addVertex(3, "Museum", 300, 300);

    primGraph.addEdge(0, 1, 10, 10, false);
    primGraph.addEdge(0, 2, 6, 10, false);
    primGraph.addEdge(0, 3, 5, 10, false);
    primGraph.addEdge(1, 3, 15, 10, false);
    primGraph.addEdge(2, 3, 4, 10, false);

    Prim prim;

    Edge primEdges[10];
    int primCount;
    int primTotalWeight;

    bool primSuccess = prim.findMST(primGraph, 0, primEdges, primCount, primTotalWeight);

    if (primSuccess)
    {
        cout << endl;
        cout << "Prim Minimum Spanning Tree:" << endl;

        for (int i = 0; i < primCount; i++)
        {
            cout << primEdges[i].fromID << " -> " << primEdges[i].toID;
            cout << " Weight: " << primEdges[i].weight << endl;
        }

        cout << "Total MST Weight: " << primTotalWeight << endl;
    }
    else
    {
        cout << "Graph does not have a complete Minimum Spanning Tree." << endl;
    }

    Graph topoGraph(10, 20);

    topoGraph.addVertex(0, "Plan", 100, 100);
    topoGraph.addVertex(1, "Disable Alarm", 300, 100);
    topoGraph.addVertex(2, "Enter Vault", 500, 100);
    topoGraph.addVertex(3, "Escape", 700, 100);
    topoGraph.addVertex(4, "Collect Tools", 100, 300);

    topoGraph.addEdge(0, 1, 1, 10, true);
    topoGraph.addEdge(1, 2, 1, 10, true);
    topoGraph.addEdge(2, 3, 1, 10, true);
    topoGraph.addEdge(4, 2, 1, 10, true);

    TopologicalSort topo;

    int ordering[10];
    int orderingCount;

    bool success2 = topo.kahnSort(topoGraph, ordering, orderingCount);

    if (success2)
    {
        cout << endl;
        cout << "Topological Ordering:" << endl;

        for (int i = 0; i < orderingCount; i++)
        {
            const Vertex* vertex = topoGraph.getVertexAt(topoGraph.getVertexIndexByID(ordering[i]));

            cout << vertex->name << endl;
        }
    }
    else
    {
        cout << "Graph contains a cycle. Topological sorting is not possible." << endl;
    }

    Graph bipartiteGraph(10, 20);

    bipartiteGraph.addVertex(0, "Vault", 100, 100);
    bipartiteGraph.addVertex(1, "Bank", 300, 100);
    bipartiteGraph.addVertex(2, "Airport", 500, 100);
    bipartiteGraph.addVertex(3, "Museum", 300, 300);

    bipartiteGraph.addEdge(0, 1, 1, 10, false);
    bipartiteGraph.addEdge(0, 3, 1, 10, false);
    bipartiteGraph.addEdge(2, 1, 1, 10, false);
    bipartiteGraph.addEdge(2, 3, 1, 10, false);

    Bipartite bipartite;

    int colours[10];

    bool isBipartite = bipartite.checkBipartite(bipartiteGraph, colours);

    if (isBipartite)
    {
        cout << endl;
        cout << "Graph is Bipartite." << endl;

        for (int i = 0; i < bipartiteGraph.getVertexCount(); i++)
        {
            const Vertex* vertex = bipartiteGraph.getVertexAt(i);

            cout << vertex->name << " - Group " << colours[i] << endl;
        }
    }
    else
    {
        cout << "Graph is NOT Bipartite." << endl;
    }

    Graph criticalGraph(10, 20);

    criticalGraph.addVertex(0, "Vault", 100, 100);
    criticalGraph.addVertex(1, "Bank", 300, 100);
    criticalGraph.addVertex(2, "Airport", 500, 100);
    criticalGraph.addVertex(3, "Museum", 300, 300);
    criticalGraph.addVertex(4, "Hotel", 500, 300);

    // Triangle: 0, 1, 2

    criticalGraph.addEdge(0, 1, 5, 10, false);
    criticalGraph.addEdge(1, 2, 7, 10, false);
    criticalGraph.addEdge(2, 0, 4, 10, false);

    // Chain connected to the triangle

    criticalGraph.addEdge(1, 3, 3, 10, false);
    criticalGraph.addEdge(3, 4, 6, 10, false);

    CriticalPoints criticalPoints;

    Edge bridges[20];
    int bridgeCount;

    int articulationPoints[10];
    int articulationCount;

    bool success4 = criticalPoints.findCriticalPoints(
        criticalGraph, bridges, bridgeCount,
        articulationPoints, articulationCount
    );

    if (success4)
    {
        cout << endl;
        cout << "Bridges:" << endl;

        for (int i = 0; i < bridgeCount; i++)
        {
            cout << bridges[i].fromID << " -> " << bridges[i].toID;
            cout << " Weight: " << bridges[i].weight << endl;
        }

        cout << endl;
        cout << "Articulation Points:" << endl;

        for (int i = 0; i < articulationCount; i++)
        {
            cout << articulationPoints[i] << endl;
        }
    }

    Graph flowGraph(10, 20);

    flowGraph.addVertex(0, "Vault", 100, 100);
    flowGraph.addVertex(1, "Bank", 300, 100);
    flowGraph.addVertex(2, "Airport", 500, 100);
    flowGraph.addVertex(3, "Museum", 300, 300);
    flowGraph.addVertex(4, "Hotel", 500, 300);
    flowGraph.addVertex(5, "Escape", 700, 200);

    flowGraph.addEdge(0, 1, 0, 16, true);
    flowGraph.addEdge(0, 2, 0, 13, true);
    flowGraph.addEdge(1, 2, 0, 10, true);
    flowGraph.addEdge(2, 1, 0, 4, true);
    flowGraph.addEdge(1, 3, 0, 12, true);
    flowGraph.addEdge(3, 2, 0, 9, true);
    flowGraph.addEdge(2, 4, 0, 14, true);
    flowGraph.addEdge(4, 3, 0, 7, true);
    flowGraph.addEdge(3, 5, 0, 20, true);
    flowGraph.addEdge(4, 5, 0, 4, true);

    EdmondsKarp edmondsKarp;

    int maxFlow;
    Edge minCutEdges[20];
    int minCutCount;

    bool flowSuccess = edmondsKarp.findMaxFlow(
        flowGraph, 0, 5, maxFlow, minCutEdges, minCutCount
    );

    if (flowSuccess)
    {
        cout << endl;
        cout << "Maximum Flow: " << maxFlow << endl;

        cout << endl;
        cout << "Minimum Cut Edges:" << endl;

        int cutCapacity = 0;

        for (int i = 0; i < minCutCount; i++)
        {
            cout << minCutEdges[i].fromID << " -> " << minCutEdges[i].toID;
            cout << " Capacity: " << minCutEdges[i].capacity << endl;

            cutCapacity += minCutEdges[i].capacity;
        }

        cout << "Minimum Cut Capacity: " << cutCapacity << endl;
    }
    else
    {
        cout << "Invalid graph or source/destination." << endl;
    }

    return 0;
}
