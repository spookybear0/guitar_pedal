#pragma once

#include "event.h"
#include <vector>

// can subscribe to events
class EventManager {
private:
    static std::vector<void (*)(const Event& event)> callbacks;
public:
    static void handleEvent(const Event& event);
    static void subscribe(void (*callback)(const Event& event));
};