#include <Arduino.h>
#define DISABLE_ALL_LIBRARY_WARNINGS
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <ESP32Encoder.h>
#include "bluetooth.h"
#include "settings.h"
#include "ui/ui_manager.h"
#include "controls.h"
#include "events/event_manager.h"
#include "input.h"

Bluetooth bluetooth;
Settings settings;

void setup() {
    Serial.begin(9600);
    Serial.println("Hello World");

    // initalize inputs
    Input::init();

    Controls::init(0, 0, 0, 0, 0);

    UIManager::init();

    bluetooth.start();
}

void loop() {
    // read inputs and handle events

    Input::handleInput();

    // send values to virtual potentiometers 

    //Controls::update();
}