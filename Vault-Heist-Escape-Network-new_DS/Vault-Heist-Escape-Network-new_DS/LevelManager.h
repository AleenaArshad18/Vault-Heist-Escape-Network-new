#pragma once

#include "Graph.h"
#include <string>

using namespace std;

struct MissionInfo
{
    int missionID;
    int actNumber;
    int missionNumber;
    string title;
    string algorithmName;
    string instructions;
    string expectedOutcome;
    int startVertexID;
};

class LevelManager
{
private:
    static const int TOTAL_MISSIONS = 10;

    MissionInfo missions[TOTAL_MISSIONS];
    Graph* currentGraph;
    int currentMissionID;

    void initializeMissionInfo();
    bool buildMissionGraph(int missionID);

public:
    LevelManager();
    ~LevelManager();

    LevelManager(const LevelManager& other) = delete;
    LevelManager& operator=(const LevelManager& other) = delete;

    int getMissionCount() const;
    const MissionInfo* getMissionInfo(int missionID) const;

    bool loadMission(int missionID);
    void restartMission();

    const Graph* getCurrentGraph() const;
    int getCurrentMissionID() const;
    bool isMissionLoaded() const;
};
