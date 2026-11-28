
#include "AlgorithmEvent.h"

AlgorithmEvent::AlgorithmEvent()
{
    type = 0;

    vertexID = -1;
    edgeID = -1;

    fromID = -1;
    toID = -1;

    value = 0;
    auxiliaryValue = 0;

    message = "";
}

AlgorithmEvent::AlgorithmEvent(
    int type,
    int vertexID,
    int edgeID,
    int fromID,
    int toID,
    int value,
    int auxiliaryValue,
    const string& message)
{
    this->type = type;

    this->vertexID = vertexID;
    this->edgeID = edgeID;

    this->fromID = fromID;
    this->toID = toID;

    this->value = value;
    this->auxiliaryValue = auxiliaryValue;

    this->message = message;
}
