#pragma once
#include <ESP32Encoder.h>

class Input {
private:
    static ESP32Encoder rotaryEncoder;
    static int rotaryValue;// = 0; // clamped 0-255
    static int lastRotaryValue;// = -1;
    static int lastButtonState;// = HIGH;
    static bool buttonPressed;// = false;
    static int lastButtonMillis;// = 0;
public:
    static void init();
    static void handleInput();
    static void setRotaryValue(int value);
};