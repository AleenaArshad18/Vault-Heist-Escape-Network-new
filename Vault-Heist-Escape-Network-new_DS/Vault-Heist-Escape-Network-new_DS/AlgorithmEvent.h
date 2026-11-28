#pragma once

#pragma once

#include <string>

using namespace std;

struct AlgorithmEvent
{
    // Event types

    static const int ALGORITHM_STARTED = 1;
    static const int VERTEX_DISCOVERED = 2;
    static const int VERTEX_PROCESSED = 3;
    static const int EDGE_EXAMINED = 4;
    static const int EDGE_RELAXED = 5;
    static const int EDGE_SELECTED = 6;
    static const int EDGE_REJECTED = 7;
    static const int VERTEX_COLORED = 8;
    static const int DISTANCE_FINALIZED = 9;
    static const int NEGATIVE_CYCLE_DETECTED = 10;
    static const int BRIDGE_FOUND = 11;
    static const int ARTICULATION_FOUND = 12;
    static const int LOW_LINK_UPDATED = 13;
    static const int AUGMENTING_PATH_FOUND = 14;
    static const int FLOW_UPDATED = 15;
    static const int MIN_CUT_EDGE = 16;
    static const int COMPONENT_STARTED = 17;
    static const int CYCLE_DETECTED = 18;
    static const int ALGORITHM_COMPLETED = 19;
    static const int ALGORITHM_FAILED = 20;

    int type;

    int vertexID;
    int edgeID;

    int fromID;
    int toID;

    int value;
    int auxiliaryValue;

    string message;

    AlgorithmEvent();

    AlgorithmEvent(
        int type,
        int vertexID,
        int edgeID,
        int fromID,
        int toID,
        int value,
        int auxiliaryValue,
        const string& message
    );
};
