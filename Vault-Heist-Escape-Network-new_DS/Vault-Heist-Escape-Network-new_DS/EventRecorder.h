#pragma once

#pragma once

#include "AlgorithmEvent.h"

class EventRecorder
{
private:
    AlgorithmEvent* events;

    int eventCount;
    int capacity;

    void expand();

public:
    EventRecorder(int initialCapacity = 128);
    ~EventRecorder();

    EventRecorder(const EventRecorder& other) = delete;
    EventRecorder& operator=(const EventRecorder& other) = delete;

    void record(const AlgorithmEvent& event);

    void clear();

    int getEventCount() const;
    const AlgorithmEvent* getEventAt(int index) const;
};
