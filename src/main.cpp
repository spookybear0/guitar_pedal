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
#include "ui.h"
#include "controls.h"

Bluetooth bluetooth;
Settings settings;


void handleButtonPress();

void setup() {
    Serial.begin(9600);
    Serial.println("Hello World");

    Controls::init(0, 0, 0, 0, 0);

    UI::init();

    bluetooth.start();
}

void loop() {
    // bluetooth
    // check for any requests from bluetooth and handle them

    // read inputs (footswitches)

    //...

    // handle ui (update based on rotary encoder)

    UI::draw();

    // send values to virtual potentiometers 

    Controls::update();
}