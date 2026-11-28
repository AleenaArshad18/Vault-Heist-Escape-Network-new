#include "LevelManager.h"

LevelManager::LevelManager()
{
    currentGraph = nullptr;
    currentMissionID = 0;

    initializeMissionInfo();
}

LevelManager::~LevelManager()
{
    delete currentGraph;
    currentGraph = nullptr;
}

void LevelManager::initializeMissionInfo()
{
    missions[0] = {
        1, 1, 1,
        "Scout the Perimeter",
        "Breadth-First Search",
        "Start at the Vault and discover every reachable location level by level.",
        "BFS traversal from vertex 0 visits all reachable vertices.",
        0
    };

    missions[1] = {
        2, 1, 2,
        "Trace the Hidden Route",
        "Depth-First Search",
        "Start at the Vault and explore each available route as deeply as possible.",
        "DFS traversal from vertex 0 visits all reachable vertices.",
        0
    };

    missions[2] = {
        3, 2, 1,
        "Find the Cheapest Route",
        "Dijkstra",
        "Find the minimum travel cost from the Vault to every reachable location. All edge weights are non-negative.",
        "Shortest distances from vertex 0: 0, 4, 7, 8, 10.",
        0
    };

    missions[3] = {
        4, 2, 2,
        "Navigate the Risky Network",
        "Bellman-Ford",
        "Find shortest distances from the Vault. This mission includes a negative edge but no reachable negative cycle.",
        "Shortest distances from vertex 0: 0, 4, 2, 5, 8.",
        0
    };

    missions[4] = {
        5, 3, 1,
        "Build the Cheapest Network",
        "Kruskal",
        "Connect every location using the minimum possible total edge weight.",
        "A minimum spanning tree has 4 edges and total weight 22.",
        0
    };

    missions[5] = {
        6, 3, 2,
        "Secure the Network",
        "Prim",
        "Grow a minimum spanning tree from the Vault, selecting the cheapest connecting edge each time.",
        "A minimum spanning tree has 4 edges and total weight 22.",
        0
    };

    missions[6] = {
        7, 4, 1,
        "Schedule the Heist",
        "Topological Sort (Kahn)",
        "Find an order that respects every directed prerequisite. If the graph contains a directed cycle, no valid order exists.",
        "A valid topological ordering contains all 5 vertices.",
        0
    };

    missions[7] = {
        8, 4, 2,
        "Crew Assignment",
        "Bipartite Check",
        "Assign connected locations to two groups so that every undirected edge joins different groups.",
        "The graph is bipartite and can be divided into two groups.",
        0
    };

    missions[8] = {
        9, 5, 1,
        "Find the Weak Points",
        "Bridges and Articulation Points",
        "Identify every bridge and articulation point in the undirected security network.",
        "Bridges: 1-3 and 3-4. Articulation points: 1 and 3.",
        0
    };

    missions[9] = {
        10, 5, 2,
        "Escape Capacity",
        "Edmonds-Karp Maximum Flow",
        "Calculate the maximum amount of flow that can travel from the Vault (source) to the Escape Point (sink).",
        "Maximum flow is 23 and equals the minimum cut capacity.",
        0
    };
}

int LevelManager::getMissionCount() const
{
    return TOTAL_MISSIONS;
}

const MissionInfo* LevelManager::getMissionInfo(int missionID) const
{
    if (missionID < 1 || missionID > TOTAL_MISSIONS)
        return nullptr;

    return &missions[missionID - 1];
}

bool LevelManager::loadMission(int missionID)
{
    if (missionID < 1 || missionID > TOTAL_MISSIONS)
        return false;

    delete currentGraph;
    currentGraph = nullptr;
    currentMissionID = 0;

    if (missionID == 1)
        currentGraph = new Graph(6, 8);
    else if (missionID == 2)
        currentGraph = new Graph(6, 8);
    else if (missionID == 3)
        currentGraph = new Graph(6, 10);
    else if (missionID == 4)
        currentGraph = new Graph(6, 10);
    else if (missionID == 5)
        currentGraph = new Graph(6, 10);
    else if (missionID == 6)
        currentGraph = new Graph(6, 10);
    else if (missionID == 7)
        currentGraph = new Graph(6, 10);
    else if (missionID == 8)
        currentGraph = new Graph(6, 10);
    else if (missionID == 9)
        currentGraph = new Graph(6, 10);
    else if (missionID == 10)
        currentGraph = new Graph(6, 12);

    if (currentGraph == nullptr)
        return false;

    if (!buildMissionGraph(missionID))
    {
        delete currentGraph;
        currentGraph = nullptr;
        return false;
    }

    currentMissionID = missionID;
    return true;
}

bool LevelManager::buildMissionGraph(int missionID)
{
    if (currentGraph == nullptr)
        return false;

    bool success = true;

    if (missionID == 1 || missionID == 2)
    {
        success = currentGraph->addVertex(0, "Vault", 100, 250) &&
            currentGraph->addVertex(1, "Bank", 280, 120) &&
            currentGraph->addVertex(2, "Airport", 460, 250) &&
            currentGraph->addVertex(3, "Museum", 280, 390) &&
            currentGraph->addVertex(4, "Safehouse", 620, 120);

        success = success && currentGraph->addEdge(0, 1, 2, 10, false);
        success = success && currentGraph->addEdge(0, 2, 4, 10, false);
        success = success && currentGraph->addEdge(0, 3, 3, 10, false);
        success = success && currentGraph->addEdge(1, 4, 5, 10, false);
        success = success && currentGraph->addEdge(2, 4, 2, 10, false);
        success = success && currentGraph->addEdge(3, 4, 4, 10, false);
    }
    else if (missionID == 3)
    {
        success = currentGraph->addVertex(0, "Vault", 100, 250) &&
            currentGraph->addVertex(1, "Bank", 260, 100) &&
            currentGraph->addVertex(2, "Airport", 450, 100) &&
            currentGraph->addVertex(3, "Museum", 260, 390) &&
            currentGraph->addVertex(4, "Safehouse", 450, 390);

        success = success && currentGraph->addEdge(0, 1, 4, 10, false);
        success = success && currentGraph->addEdge(0, 2, 7, 10, false);
        success = success && currentGraph->addEdge(1, 2, 3, 10, false);
        success = success && currentGraph->addEdge(1, 3, 4, 10, false);
        success = success && currentGraph->addEdge(2, 3, 2, 10, false);
        success = success && currentGraph->addEdge(2, 4, 3, 10, false);
        success = success && currentGraph->addEdge(3, 4, 2, 10, false);
    }
    else if (missionID == 4)
    {
        success = currentGraph->addVertex(0, "Vault", 100, 250) &&
            currentGraph->addVertex(1, "Bank", 260, 100) &&
            currentGraph->addVertex(2, "Airport", 260, 390) &&
            currentGraph->addVertex(3, "Museum", 460, 100) &&
            currentGraph->addVertex(4, "Safehouse", 460, 390);

        success = success && currentGraph->addEdge(0, 1, 4, 10, true);
        success = success && currentGraph->addEdge(0, 2, 5, 10, true);
        success = success && currentGraph->addEdge(1, 2, -2, 10, true);
        success = success && currentGraph->addEdge(1, 3, 3, 10, true);
        success = success && currentGraph->addEdge(2, 3, 3, 10, true);
        success = success && currentGraph->addEdge(3, 4, 3, 10, true);
    }
    else if (missionID == 5 || missionID == 6)
    {
        success = currentGraph->addVertex(0, "Vault", 100, 250) &&
            currentGraph->addVertex(1, "Bank", 260, 100) &&
            currentGraph->addVertex(2, "Airport", 260, 390) &&
            currentGraph->addVertex(3, "Museum", 460, 100) &&
            currentGraph->addVertex(4, "Safehouse", 460, 390);

        success = success && currentGraph->addEdge(0, 1, 10, 10, false);
        success = success && currentGraph->addEdge(0, 2, 5, 10, false);
        success = success && currentGraph->addEdge(0, 3, 6, 10, false);
        success = success && currentGraph->addEdge(1, 2, 6, 10, false);
        success = success && currentGraph->addEdge(1, 3, 15, 10, false);
        success = success && currentGraph->addEdge(2, 3, 4, 10, false);
        success = success && currentGraph->addEdge(2, 4, 8, 10, false);
        success = success && currentGraph->addEdge(3, 4, 7, 10, false);
    }
    else if (missionID == 7)
    {
        success = currentGraph->addVertex(0, "Plan", 100, 250) &&
            currentGraph->addVertex(1, "Collect Tools", 270, 100) &&
            currentGraph->addVertex(2, "Disable Alarm", 270, 390) &&
            currentGraph->addVertex(3, "Enter Vault", 470, 250) &&
            currentGraph->addVertex(4, "Escape", 650, 250);

        success = success && currentGraph->addEdge(0, 1, 1, 10, true);
        success = success && currentGraph->addEdge(0, 2, 1, 10, true);
        success = success && currentGraph->addEdge(1, 3, 1, 10, true);
        success = success && currentGraph->addEdge(2, 3, 1, 10, true);
        success = success && currentGraph->addEdge(3, 4, 1, 10, true);
    }
    else if (missionID == 8)
    {
        success = currentGraph->addVertex(0, "Vault", 100, 250) &&
            currentGraph->addVertex(1, "Bank", 280, 100) &&
            currentGraph->addVertex(2, "Airport", 460, 250) &&
            currentGraph->addVertex(3, "Museum", 280, 400);

        success = success && currentGraph->addEdge(0, 1, 1, 10, false);
        success = success && currentGraph->addEdge(1, 2, 1, 10, false);
        success = success && currentGraph->addEdge(2, 3, 1, 10, false);
        success = success && currentGraph->addEdge(3, 0, 1, 10, false);
    }
    else if (missionID == 9)
    {
        success = currentGraph->addVertex(0, "Vault", 100, 250) &&
            currentGraph->addVertex(1, "Bank", 260, 100) &&
            currentGraph->addVertex(2, "Airport", 260, 390) &&
            currentGraph->addVertex(3, "Museum", 460, 250) &&
            currentGraph->addVertex(4, "Safehouse", 650, 250);

        success = success && currentGraph->addEdge(0, 1, 5, 10, false);
        success = success && currentGraph->addEdge(0, 2, 4, 10, false);
        success = success && currentGraph->addEdge(1, 2, 7, 10, false);
        success = success && currentGraph->addEdge(1, 3, 3, 10, false);
        success = success && currentGraph->addEdge(3, 4, 6, 10, false);
    }
    else if (missionID == 10)
    {
        success = currentGraph->addVertex(0, "Vault", 100, 250) &&
            currentGraph->addVertex(1, "Bank", 260, 100) &&
            currentGraph->addVertex(2, "Airport", 260, 390) &&
            currentGraph->addVertex(3, "Museum", 460, 100) &&
            currentGraph->addVertex(4, "Safehouse", 460, 390) &&
            currentGraph->addVertex(5, "Escape Point", 650, 250);

        success = success && currentGraph->addEdge(0, 1, 0, 16, true);
        success = success && currentGraph->addEdge(0, 2, 0, 13, true);
        success = success && currentGraph->addEdge(1, 2, 0, 10, true);
        success = success && currentGraph->addEdge(2, 1, 0, 4, true);
        success = success && currentGraph->addEdge(1, 3, 0, 12, true);
        success = success && currentGraph->addEdge(3, 2, 0, 9, true);
        success = success && currentGraph->addEdge(2, 4, 0, 14, true);
        success = success && currentGraph->addEdge(4, 3, 0, 7, true);
        success = success && currentGraph->addEdge(3, 5, 0, 20, true);
        success = success && currentGraph->addEdge(4, 5, 0, 4, true);
    }

    return success;
}

void LevelManager::restartMission()
{
    if (currentMissionID != 0)
        loadMission(currentMissionID);
}

const Graph* LevelManager::getCurrentGraph() const
{
    return currentGraph;
}

int LevelManager::getCurrentMissionID() const
{
    return currentMissionID;
}

bool LevelManager::isMissionLoaded() const
{
    return currentGraph != nullptr && currentMissionID != 0;
}
