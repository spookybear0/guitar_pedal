#include "input.h"
#include <ESP32Encoder.h>
#include <Arduino.h>
#include "events/event_manager.h"

#define ROTARY_PIN_A 21
#define ROTARY_PIN_B 22
#define ROTARY_BUTTON 25
#define FOOTSWITCH1_PIN -1
#define FOOTSWITCH2_PIN -1

ESP32Encoder Input::rotaryEncoder;
int Input::rotaryValue = 0; // clamped 0-255
int Input::lastRotaryValue = -1;
int Input::lastRotaryButtonState = HIGH;
int Input::lastRotaryButtonMillis = 0;

int Input::lastFootswitch1State = HIGH;
int Input::lastFootswitch1Millis = 0;
int Input::lastFootswitch2State = HIGH;
int Input::lastFootswitch2Millis = 0;

void Input::init() {
    // initalize rotary encoder
    pinMode(ROTARY_BUTTON, INPUT_PULLUP);
    ESP32Encoder::useInternalWeakPullResistors=puType::up;
    rotaryEncoder.attachSingleEdge(ROTARY_PIN_A, ROTARY_PIN_B);
    rotaryEncoder.setCount(0);

    // initialize other inputs
    //pinMode(FOOTSWITCH1_PIN, INPUT_PULLUP);
    //pinMode(FOOTSWITCH2_PIN, INPUT_PULLUP);
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
    if (buttonState == LOW && lastRotaryButtonState == HIGH && millis() - lastRotaryButtonMillis > 50) {
        Serial.println("Rotary Button Pressed");
        EventManager::handleEvent(Event{EventType::EVENT_ROTARY_CLICK, 1});
        lastRotaryButtonMillis = millis();
    }
    else if (buttonState == HIGH && lastRotaryButtonState == LOW && millis() - lastRotaryButtonMillis > 50) {
        Serial.println("Rotary Button Released");
        EventManager::handleEvent(Event{EventType::EVENT_ROTARY_CLICK, 0});
        lastRotaryButtonMillis = millis();
    }
    lastRotaryButtonState = buttonState;

    // footswitches

    // only send event if pressed or released
    // use a debounce time of 50ms

    /*
    int footswitch1State = digitalRead(FOOTSWITCH1_PIN);
    if (footswitch1State == LOW && lastFootswitch1State == HIGH && millis() - lastFootswitch1Millis > 50) {
        Serial.println("Footswitch 1 Pressed");
        EventManager::handleEvent(Event{EventType::EVENT_FOOTSWITCH1, 1});
        lastFootswitch1Millis = millis();
    }
    else if (footswitch1State == HIGH && lastFootswitch1State == LOW && millis() - lastFootswitch1Millis > 50) {
        Serial.println("Footswitch 1 Released");
        EventManager::handleEvent(Event{EventType::EVENT_FOOTSWITCH1, 0});
        lastFootswitch1Millis = millis();
    }
    lastFootswitch1State = footswitch1State;
    
    int footswitch2State = digitalRead(FOOTSWITCH2_PIN);
    if (footswitch2State == LOW && lastFootswitch2State == HIGH && millis() - lastFootswitch2Millis > 50) {
        Serial.println("Footswitch 2 Pressed");
        EventManager::handleEvent(Event{EventType::EVENT_FOOTSWITCH2, 1});
        lastFootswitch2Millis = millis();
    }
    else if (footswitch2State == HIGH && lastFootswitch2State == LOW && millis() - lastFootswitch2Millis > 50) {
        Serial.println("Footswitch 2 Released");
        EventManager::handleEvent(Event{EventType::EVENT_FOOTSWITCH2, 0});
        lastFootswitch2Millis = millis();
    }
    
    */
}