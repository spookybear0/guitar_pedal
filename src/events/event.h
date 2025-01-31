#pragma once

enum EventType {
    EVENT_ROTARY_TURN,
    EVENT_ROTARY_CLICK,
    EVENT_PASSTHROUGH,
    EVENT_SWITCH1,
    EVENT_SWITCH2,
};

struct Event {
    EventType type;
    int value;
};