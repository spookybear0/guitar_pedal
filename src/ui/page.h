#pragma once
#include "events/event.h"
#include <events/event_manager.h>

class Page {
public:
    virtual void draw() = 0;
    virtual void handleEvent(const Event& event) = 0;
    virtual void enter() { }; // default does nothing since this is optional
};