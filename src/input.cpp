#include "input.h"
#include <ESP32Encoder.h>
#include <Arduino.h>
#include "events/event_manager.h"

#define ROTARY_PIN_A 21
#define ROTARY_PIN_B 22
#define ROTARY_BUTTON 25

ESP32Encoder Input::rotaryEncoder;
int Input::rotaryValue = 0; // clamped 0-255
int Input::lastRotaryValue = -1;
int Input::lastButtonState = HIGH;
bool Input::buttonPressed = false;
int Input::lastButtonMillis = 0;

void Input::init() {
    // initalize rotary encoder
    pinMode(ROTARY_BUTTON, INPUT_PULLUP);
    ESP32Encoder::useInternalWeakPullResistors=puType::up;
    rotaryEncoder.attachSingleEdge(ROTARY_PIN_A, ROTARY_PIN_B);
    rotaryEncoder.setCount(0);

    // initialize other inputs
    //...
}

void Input::setRotaryValue(int value) {
    rotaryValue = constrain(value, 0, 255);
    rotaryEncoder.setCount(rotaryValue);
}

void Input::handleInput() {
    // rotary encoder (turn)

    // clamp to 0-255

    rotaryValue = rotaryEncoder.getCount();
    if (rotaryValue != lastRotaryValue) {
        Serial.println("Rotary Value: " + String(rotaryValue));
        rotaryValue = constrain(rotaryValue, 0, 255);
        // make sure rotary doesn't go below 0 or above 255
        setRotaryValue(rotaryValue);
        EventManager::handleEvent(Event{EventType::EVENT_ROTARY_TURN, rotaryValue});
    }
    lastRotaryValue = rotaryValue;

    // rotary encoder (click), with debounce, and only considers press, not release
    int buttonState = digitalRead(ROTARY_BUTTON);
    if (buttonState == LOW && lastButtonState == HIGH && millis() - lastButtonMillis > 50) {
        if (buttonPressed) {
            Serial.println("Rotary Pressed");
            EventManager::handleEvent(Event{EventType::EVENT_ROTARY_CLICK, 1});
        }
        else {
            Serial.println("Rotary Released");
            EventManager::handleEvent(Event{EventType::EVENT_ROTARY_CLICK, 0});
        }
        buttonPressed = !buttonPressed;
        lastButtonMillis = millis();
    }
    lastButtonState = buttonState;
}