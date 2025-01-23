#include <Arduino.h>
#include <TFT_eSPI.h>
#include <NimBLEDevice.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include "bluetooth.h"

void setup() {
    Serial.begin(9600);
    Serial.println("Hello World");

    TFT_eSPI tft = TFT_eSPI();
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println("Hello World");
    
    // bluetooth

    
}

void loop() {

}