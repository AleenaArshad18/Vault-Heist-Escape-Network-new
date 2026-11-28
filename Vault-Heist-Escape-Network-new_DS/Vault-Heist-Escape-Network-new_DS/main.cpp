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
#include "EventRecorder.h"

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

    graph.displayAdjacencyMatrix();

    cout << endl;
    cout << "Degree of Vault: " << graph.getDegree(0) << endl;

    graph.displayNeighbours(0);

    graph.displayNeighbours(1);

    graph.displayNeighbours(2);

    cout << endl;
    graph.BFS(0);

    cout << endl;
    graph.DFS(0);

    cout << endl;

    Graph structureGraph(10, 20);

    structureGraph.addVertex(0, "Vault", 100, 100);
    structureGraph.addVertex(1, "Bank", 300, 100);
    structureGraph.addVertex(2, "Airport", 500, 100);
    structureGraph.addVertex(3, "Museum", 300, 300);
    structureGraph.addVertex(4, "Hotel", 500, 300);

    // First component: Vault, Bank, Airport form a cycle.

    structureGraph.addEdge(0, 1, 5, 10, false);
    structureGraph.addEdge(1, 2, 8, 10, false);
    structureGraph.addEdge(2, 0, 4, 10, false);

    // Second component: Museum and Hotel.

    structureGraph.addEdge(3, 4, 6, 10, false);

    cout << endl;
    cout << "Connected Components Test:" << endl;

    int componentCount = structureGraph.countConnectedComponents();

    cout << "Number of Connected Components: " << componentCount << endl;

    structureGraph.displayConnectedComponents();

    cout << endl;
    cout << "Undirected Cycle Detection Test:" << endl;

    if (structureGraph.hasCycleUndirected())
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


    bool dijkstraSuccess = dijkstra.findShortestPaths(graph, 0, distances, previous);

    if (dijkstraSuccess)
    {
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
    }

    cout << endl;
    cout << "DIJKSTRA EVENT TEST:" << endl;

    EventRecorder dijkstraEvents;

    int eventDistances[10];
    int eventPrevious[10];

    bool eventSuccess = dijkstra.findShortestPaths(
        graph,
        0,
        eventDistances,
        eventPrevious,
        &dijkstraEvents
    );

    if (eventSuccess)
    {
        cout << "Dijkstra event count: "
            << dijkstraEvents.getEventCount() << endl;

        for (int i = 0; i < dijkstraEvents.getEventCount(); i++)
        {
            const AlgorithmEvent* event =
                dijkstraEvents.getEventAt(i);

            cout << "Event " << i
                << " | Type: " << event->type
                << " | Vertex: " << event->vertexID
                << " | Edge: " << event->edgeID
                << " | From: " << event->fromID
                << " | To: " << event->toID
                << " | Value: " << event->value
                << " | Auxiliary: " << event->auxiliaryValue
                << " | " << event->message << endl;
        }
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

            if (bfDistances[i] == 999999)
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


    cout << endl;
    cout << "BELLMAN-FORD EVENT TEST:" << endl;

    EventRecorder bellmanFordEvents;

    int eventBFDistances[10];
    int eventBFPrevious[10];

    bool eventBFSuccess = bellmanFord.findShortestPaths(
        testGraph,
        0,
        eventBFDistances,
        eventBFPrevious,
        &bellmanFordEvents
    );

    if (eventBFSuccess)
    {
        cout << "Bellman-Ford event count: "
            << bellmanFordEvents.getEventCount() << endl;

        for (int i = 0; i < bellmanFordEvents.getEventCount(); i++)
        {
            const AlgorithmEvent* event =
                bellmanFordEvents.getEventAt(i);

            cout << "Event " << i
                << " | Type: " << event->type
                << " | Vertex: " << event->vertexID
                << " | Edge: " << event->edgeID
                << " | From: " << event->fromID
                << " | To: " << event->toID
                << " | Value: " << event->value
                << " | Auxiliary: " << event->auxiliaryValue
                << " | " << event->message << endl;
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

    // Stores the final flow on each original edge.
    int edgeFlows[20];

    bool flowSuccess = edmondsKarp.findMaxFlow(
        flowGraph,
        0,
        5,
        maxFlow,
        minCutEdges,
        minCutCount,
        edgeFlows
    );

    if (flowSuccess)
    {
        cout << endl;
        cout << "Maximum Flow: " << maxFlow << endl;

        // Display flow on every original edge.

        cout << endl;
        cout << "Flow on Original Edges:" << endl;

        for (int i = 0; i < flowGraph.getEdgeCount(); i++)
        {
            const Edge* edge = flowGraph.getEdgeAt(i);

            cout << "Edge ID " << edge->id
                << " (" << edge->fromID << " -> " << edge->toID << ")"
                << ": " << edgeFlows[i]
                << " / " << edge->capacity << endl;
        }

        // Display minimum cut edges.

        cout << endl;
        cout << "Minimum Cut Edges:" << endl;

        int cutCapacity = 0;

        for (int i = 0; i < minCutCount; i++)
        {
            cout << minCutEdges[i].fromID << " -> "
                << minCutEdges[i].toID;

            cout << " Capacity: "
                << minCutEdges[i].capacity << endl;

            cutCapacity += minCutEdges[i].capacity;
        }

        cout << "Minimum Cut Capacity: " << cutCapacity << endl;
    }
    else
    {
        cout << "Invalid graph or source/destination." << endl;
    }


    

    bipartiteGraph.addEdge(0, 1, 1, 10, false);
    bipartiteGraph.addEdge(0, 3, 1, 10, false);
    bipartiteGraph.addEdge(2, 1, 1, 10, false);
    bipartiteGraph.addEdge(2, 3, 1, 10, false);


    cout << endl;
    cout << "========== PHASE 1: EDGE CASE TESTS ==========" << endl;

    // TEST 1: Dijkstra with a negative edge.

    cout << endl << "TEST 1: Dijkstra with negative weight" << endl;

    Graph negativeDijkstraGraph(5, 10);

    negativeDijkstraGraph.addVertex(0, "A", 100, 100);
    negativeDijkstraGraph.addVertex(1, "B", 200, 100);
    negativeDijkstraGraph.addEdge(0, 1, -5, 10, true);

    int testDistances[10];
    int testPrevious[10];

    bool testDijkstra = dijkstra.findShortestPaths(
        negativeDijkstraGraph, 0, testDistances, testPrevious
    );

    cout << "Expected: Rejected" << endl;
    cout << "Actual: " << (testDijkstra ? "Accepted" : "Rejected") << endl;


    // TEST 2: Dijkstra with an invalid starting vertex.

    cout << endl << "TEST 2: Dijkstra with invalid start vertex" << endl;

    bool invalidStart = dijkstra.findShortestPaths(
        negativeDijkstraGraph, 99, testDistances, testPrevious
    );

    cout << "Expected: Rejected" << endl;
    cout << "Actual: " << (invalidStart ? "Accepted" : "Rejected") << endl;


    // TEST 3: Bellman-Ford with a reachable negative cycle.

    cout << endl << "TEST 3: Bellman-Ford negative cycle" << endl;

    Graph negativeCycleGraph(5, 10);

    negativeCycleGraph.addVertex(0, "A", 100, 100);
    negativeCycleGraph.addVertex(1, "B", 200, 100);
    negativeCycleGraph.addVertex(2, "C", 300, 100);

    negativeCycleGraph.addEdge(0, 1, 1, 10, true);
    negativeCycleGraph.addEdge(1, 2, -2, 10, true);
    negativeCycleGraph.addEdge(2, 0, 0, 10, true);

    bool testBellmanFord = bellmanFord.findShortestPaths(
        negativeCycleGraph, 0, bfDistances, bfPrevious
    );

    cout << "Expected: Rejected (negative cycle)" << endl;
    cout << "Actual: " << (testBellmanFord ? "Accepted" : "Rejected") << endl;


    // TEST 4: Kruskal with a disconnected graph.

    cout << endl << "TEST 4: Kruskal with disconnected graph" << endl;

    Graph disconnectedMSTGraph(5, 10);

    disconnectedMSTGraph.addVertex(0, "A", 100, 100);
    disconnectedMSTGraph.addVertex(1, "B", 200, 100);
    disconnectedMSTGraph.addVertex(2, "C", 300, 100);
    disconnectedMSTGraph.addVertex(3, "D", 400, 100);

    disconnectedMSTGraph.addEdge(0, 1, 5, 10, false);
    disconnectedMSTGraph.addEdge(2, 3, 7, 10, false);

    Edge testMSTEdges[10];
    int testMSTCount;
    int testMSTWeight;

    bool testKruskal = kruskal.findMST(
        disconnectedMSTGraph, testMSTEdges, testMSTCount, testMSTWeight
    );

    cout << "Expected: Rejected (no complete MST)" << endl;
    cout << "Actual: " << (testKruskal ? "Accepted" : "Rejected") << endl;


    // TEST 5: Prim with a disconnected graph.

    cout << endl << "TEST 5: Prim with disconnected graph" << endl;

    bool testPrim = prim.findMST(
        disconnectedMSTGraph, 0, testMSTEdges, testMSTCount, testMSTWeight
    );

    cout << "Expected: Rejected (no complete MST)" << endl;
    cout << "Actual: " << (testPrim ? "Accepted" : "Rejected") << endl;


    // TEST 6: Topological Sort with a directed cycle.

    cout << endl << "TEST 6: Topological Sort with directed cycle" << endl;

    Graph cyclicTopoGraph(5, 10);

    cyclicTopoGraph.addVertex(0, "A", 100, 100);
    cyclicTopoGraph.addVertex(1, "B", 200, 100);
    cyclicTopoGraph.addVertex(2, "C", 300, 100);

    cyclicTopoGraph.addEdge(0, 1, 1, 10, true);
    cyclicTopoGraph.addEdge(1, 2, 1, 10, true);
    cyclicTopoGraph.addEdge(2, 0, 1, 10, true);

    int testOrdering[10];
    int testOrderingCount;

    bool testTopo = topo.kahnSort(
        cyclicTopoGraph, testOrdering, testOrderingCount
    );

    cout << "Expected: Rejected (cycle)" << endl;
    cout << "Actual: " << (testTopo ? "Accepted" : "Rejected") << endl;


    // TEST 7: Bipartite checking with a triangle.

    cout << endl << "TEST 7: Bipartite checking with triangle" << endl;

    Graph triangleGraph(5, 10);

    triangleGraph.addVertex(0, "A", 100, 100);
    triangleGraph.addVertex(1, "B", 200, 100);
    triangleGraph.addVertex(2, "C", 300, 100);

    triangleGraph.addEdge(0, 1, 1, 10, false);
    triangleGraph.addEdge(1, 2, 1, 10, false);
    triangleGraph.addEdge(2, 0, 1, 10, false);

    int testColours[10];

    bool testBipartite = bipartite.checkBipartite(
        triangleGraph, testColours
    );

    cout << "Expected: Not Bipartite" << endl;
    cout << "Actual: " << (testBipartite ? "Bipartite" : "Not Bipartite") << endl;


    // TEST 8: Critical Points with a directed edge.

    cout << endl << "TEST 8: Critical Points with directed graph" << endl;

    Graph directedCriticalGraph(5, 10);

    directedCriticalGraph.addVertex(0, "A", 100, 100);
    directedCriticalGraph.addVertex(1, "B", 200, 100);

    directedCriticalGraph.addEdge(0, 1, 5, 10, true);

    Edge testBridges[10];
    int testBridgeCount;

    int testArticulationPoints[10];
    int testArticulationCount;

    bool testCritical = criticalPoints.findCriticalPoints(
        directedCriticalGraph,
        testBridges, testBridgeCount,
        testArticulationPoints, testArticulationCount
    );

    cout << "Expected: Rejected" << endl;
    cout << "Actual: " << (testCritical ? "Accepted" : "Rejected") << endl;


    // TEST 9: Edmonds-Karp with an undirected edge.

    cout << endl << "TEST 9: Edmonds-Karp with undirected graph" << endl;

    Graph undirectedFlowGraph(5, 10);

    undirectedFlowGraph.addVertex(0, "Source", 100, 100);
    undirectedFlowGraph.addVertex(1, "Sink", 200, 100);

    undirectedFlowGraph.addEdge(0, 1, 0, 10, false);

    int testMaxFlow;
    Edge testCutEdges[10];
    int testCutCount;

    bool testFlow = edmondsKarp.findMaxFlow(
        undirectedFlowGraph, 0, 1,
        testMaxFlow, testCutEdges, testCutCount
    );

    cout << "Expected: Rejected" << endl;
    cout << "Actual: " << (testFlow ? "Accepted" : "Rejected") << endl;


    // TEST 10: Edmonds-Karp with disconnected source and sink.

    cout << endl << "TEST 10: Edmonds-Karp with no connecting path" << endl;

    Graph disconnectedFlowGraph(5, 10);

    disconnectedFlowGraph.addVertex(0, "Source", 100, 100);
    disconnectedFlowGraph.addVertex(1, "Middle", 200, 100);
    disconnectedFlowGraph.addVertex(2, "Sink", 300, 100);

    disconnectedFlowGraph.addEdge(0, 1, 0, 10, true);

    bool noPathFlow = edmondsKarp.findMaxFlow(
        disconnectedFlowGraph, 0, 2,
        testMaxFlow, testCutEdges, testCutCount
    );

    cout << "Expected: Accepted with maximum flow 0" << endl;
    cout << "Actual: "
        << (noPathFlow ? "Accepted" : "Rejected")
        << ", Maximum Flow: " << testMaxFlow << endl;

    cout << endl << "========== EDGE CASE TESTS FINISHED ==========" << endl;


    cout << endl;
    cout << "TEST 11: Edmonds-Karp with Parallel Edges" << endl;

    Graph parallelGraph(5, 10);

    parallelGraph.addVertex(0, "Source", 100, 100);
    parallelGraph.addVertex(1, "Middle", 300, 100);
    parallelGraph.addVertex(2, "Sink", 500, 100);

    parallelGraph.addEdge(0, 1, 0, 3, true);
    parallelGraph.addEdge(0, 1, 0, 4, true);
    parallelGraph.addEdge(1, 2, 0, 7, true);

    EdmondsKarp parallelEK;

    int parallelMaxFlow;
    int parallelMinCutCount;
    Edge parallelMinCutEdges[10];
    int parallelEdgeFlows[10];

    bool parallelSuccess = parallelEK.findMaxFlow(
        parallelGraph, 0, 2,
        parallelMaxFlow,
        parallelMinCutEdges,
        parallelMinCutCount,
        parallelEdgeFlows
    );

    if (parallelSuccess)
    {
        cout << "Maximum Flow: " << parallelMaxFlow << endl;

        for (int i = 0; i < parallelGraph.getEdgeCount(); i++)
        {
            const Edge* edge = parallelGraph.getEdgeAt(i);

            cout << "Edge " << edge->id << ": "
                << parallelEdgeFlows[i] << " / "
                << edge->capacity << endl;
        }

        cout << "Expected Maximum Flow: 7" << endl;
    }
    else
    {
        cout << "Test failed!" << endl;
    }

    cout << endl;
    cout << "TEST 12: Edmonds-Karp with Opposite-Direction Edges" << endl;

    Graph oppositeGraph(5, 10);

    oppositeGraph.addVertex(0, "Source", 100, 100);
    oppositeGraph.addVertex(1, "Middle", 300, 100);
    oppositeGraph.addVertex(2, "Sink", 500, 100);

    oppositeGraph.addEdge(0, 1, 0, 5, true);
    oppositeGraph.addEdge(1, 0, 0, 9, true);
    oppositeGraph.addEdge(1, 2, 0, 5, true);

    EdmondsKarp oppositeEK;

    int oppositeMaxFlow;
    int oppositeMinCutCount;
    Edge oppositeMinCutEdges[10];
    int oppositeEdgeFlows[10];

    bool oppositeSuccess = oppositeEK.findMaxFlow(
        oppositeGraph, 0, 2,
        oppositeMaxFlow,
        oppositeMinCutEdges,
        oppositeMinCutCount,
        oppositeEdgeFlows
    );

    if (oppositeSuccess)
    {
        cout << "Maximum Flow: " << oppositeMaxFlow << endl;

        for (int i = 0; i < oppositeGraph.getEdgeCount(); i++)
        {
            const Edge* edge = oppositeGraph.getEdgeAt(i);

            cout << "Edge " << edge->id << ": "
                << oppositeEdgeFlows[i] << " / "
                << edge->capacity << endl;
        }

        cout << "Expected Maximum Flow: 5" << endl;
    }
    else
    {
        cout << "Test failed!" << endl;
    }

    return 0;
}
