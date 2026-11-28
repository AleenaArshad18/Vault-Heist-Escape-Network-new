
#include "EventRecorder.h"

EventRecorder::EventRecorder(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        initialCapacity = 128;
    }

    capacity = initialCapacity;
    eventCount = 0;

    events = new AlgorithmEvent[capacity];
}

EventRecorder::~EventRecorder()
{
    delete[] events;
}

void EventRecorder::expand()
{
    int newCapacity = capacity * 2;

    AlgorithmEvent* newEvents =
        new AlgorithmEvent[newCapacity];

    for (int i = 0; i < eventCount; i++)
    {
        newEvents[i] = events[i];
    }

    delete[] events;

    events = newEvents;
    capacity = newCapacity;
}

void EventRecorder::record(const AlgorithmEvent& event)
{
    if (eventCount == capacity)
    {
        expand();
    }

    events[eventCount] = event;
    eventCount++;
}

void EventRecorder::clear()
{
    eventCount = 0;
}

int EventRecorder::getEventCount() const
{
    return eventCount;
}

const AlgorithmEvent* EventRecorder::getEventAt(int index) const
{
    if (index < 0 || index >= eventCount)
    {
        return nullptr;
    }

    return &events[index];
}
