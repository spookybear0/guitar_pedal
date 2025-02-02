#pragma once
#include <ESP32Encoder.h>

class Input {
private:
    static ESP32Encoder rotaryEncoder;
    static int rotaryValue;// = 0; // clamped 0-255
    static int lastRotaryValue;// = -1;
    static int lastRotaryButtonState;// = HIGH;
    static int lastRotaryButtonMillis;// = 0;

    static int lastFootswitch1State;// = HIGH;
    static int lastFootswitch1Millis;// = 0;
    static int lastFootswitch2State;// = HIGH;
    static int lastFootswitch2Millis;// = 0;
public:
    static void init();
    static void handleInput();
    static void setRotaryValue(int value);
};