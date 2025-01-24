#include <Arduino.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include <NimBLEDevice.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include "bluetooth.h"

Bluetooth bluetooth;

void setup() {
    Serial.begin(9600);
    Serial.println("Hello World");


    /*TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set font color to white, background to black
    tft.drawString("Hello World!", 0, 0, 2); // Print text to screen*/


    // bluetooth

    bluetooth.start();
}

void loop() {
    // bluetooth
    // check for any requests from bluetooth and handle them

    bluetooth.receiveData();

    // read inputs (footswitches)

    //...

    // handle ui (update based on rotary encoder)

    //...

    // send values to virtual potentiometers 

    //...
}