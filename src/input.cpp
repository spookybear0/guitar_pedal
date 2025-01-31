#include "input.h"
#include <ESP32Encoder.h>
#include <Arduino.h>
#include "events/event_manager.h"

#define ROTARY_PIN_A 21
#define ROTARY_PIN_B 22
#define ROTARY_BUTTON 25

int Input::rotaryValue = 0; // clamped 0-255
int Input::lastRotaryValue = -1;

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
    int newRotaryValue = constrain(rotaryEncoder.getCount(), 0, 255);
    if (newRotaryValue != lastRotaryValue) {
        Serial.print("Rotary: ");
        Serial.println(newRotaryValue);
        EventManager::handleEvent(Event{EventType::EVENT_ROTARY_TURN, newRotaryValue});
        lastRotaryValue = newRotaryValue;
    }
    // rotary encoder (click)
    if (digitalRead(ROTARY_BUTTON) == LOW) {
        Serial.println("Rotary Click");
        EventManager::handleEvent(Event{EventType::EVENT_ROTARY_CLICK, -1});
    }
}