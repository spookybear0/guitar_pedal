#include <Arduino.h>
#define DISABLE_ALL_LIBRARY_WARNINGS
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <ESP32Encoder.h>
#include "server.h"
#include "settings.h"
#include "ui/ui_manager.h"
#include "controls.h"
#include "events/event_manager.h"
#include "input.h"

uint32_t settingsSaveNextMillis = 0;
int settingsSaveInterval = 10000; // 10 seconds

// TODO: maybe make it so if nothing changes on the screen after an input, don't update it (to prevent flickering)
// TODO: effects that have less than 3 params should not show empty params

void setup() {
    Serial.begin(115200);
    Serial.println("Hello World");

    // initalize inputs
    Input::init();

    Controls::init(100, 50, 0, 0, 0);

    UIManager::init();

    Settings::begin();

    PedalServer::start();
}

void loop() {
    // handle server requests

    PedalServer::handleClient();

    // read inputs and handle events

    Input::handleInput();

    // ui update

    UIManager::update();

    // send values to virtual potentiometers and demux

    Controls::update();

    // save settings if needed (every 30 seconds for optimization and flash wear)
    if (millis() - settingsSaveNextMillis > settingsSaveInterval) {
        Settings::saveValues();
        settingsSaveNextMillis += settingsSaveInterval;
    }
}