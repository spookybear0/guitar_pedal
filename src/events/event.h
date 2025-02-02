#pragma once

enum EventType {
    EVENT_ROTARY_TURN,
    EVENT_ROTARY_CLICK,
    EVENT_PASSTHROUGH,
    EVENT_FOOTSWITCH1,
    EVENT_FOOTSWITCH2,
};

struct Event {
    EventType type;
    int value;
};