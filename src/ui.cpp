#include "ui.h"
#include <ESP32Encoder.h>

#define ROTARY_PIN_A 21
#define ROTARY_PIN_B 22
#define ROTARY_BUTTON 25

TFT_eSPI tft = TFT_eSPI();

int currentScreen = 0; // 0 = Effect Selection, 1 = Parameter Control, 2 = General Settings
int currentEffect = 0;
int currentParam = 0;
float paramValues[8][3] = {0}; // 8 effects, 3 parameters each
int selectedSetting = 0;

ESP32Encoder encoder;
int rotaryValue = 0;
int lastRotaryValue = -1;

// Effect names
const char *effectNames[] = {"Effect 1", "Effect 2", "Effect 3", "Effect 4", "Effect 5", "Effect 6", "Effect 7", "Effect 8"};

// General Settings options
// TODO: flesh this out
const char *settingsOptions[] = {"Volume", "Mix"};
int settingsValues[] = {50, 50}; // default values

void UI::init() {
    pinMode(ROTARY_BUTTON, INPUT_PULLUP);

    ESP32Encoder::useInternalWeakPullResistors=puType::up;
    encoder.attachSingleEdge(ROTARY_PIN_A, ROTARY_PIN_B);
    encoder.setCount(0);

    // tft
    tft.init();
    tft.setRotation(1);
}

void UI::draw() {
    // Read rotary encoder position
    rotaryValue = encoder.getCount();
    Serial.println(rotaryValue);
    if (rotaryValue != lastRotaryValue) {
        lastRotaryValue = rotaryValue;

        switch (currentScreen) {
        case 0:
            // Navigate effects
            currentEffect = constrain(rotaryValue, 0, 8);
            encoder.setCount(currentEffect);
            UI::drawEffectMenu();
            break;
        case 1:
            // Adjust parameters
            paramValues[currentEffect][currentParam] = constrain(rotaryValue, 0, 100);
            UI::drawParameterMenu();
            break;
        case 2:
            // Adjust general settings
            settingsValues[selectedSetting] = constrain(rotaryValue, 0, 100);
            UI::drawSettingsMenu();
            break;
        }
    }

    // Check button press
    if (digitalRead(ROTARY_BUTTON) == LOW) {
        delay(200); // Debounce
        UI::handleButtonPress();
    }
}

void UI::handleButtonPress() {
    Serial.println("Button pressed");
    switch (currentScreen) {
        case 0:
            if (currentEffect == 8) { // settings
                currentScreen = 2; // Go to General Settings
                encoder.setCount(settingsValues[selectedSetting]);
                UI::drawSettingsMenu();
            } else {
                currentScreen = 1; // Go to Parameter Control
                currentParam = 0;
                encoder.setCount(paramValues[currentEffect][currentParam]);
                UI::drawParameterMenu();
            }
            break;
        case 1:
            currentParam++;
            if (currentParam >= 3) {
                currentScreen = 0; // Back to Effect Selection
                encoder.setCount(currentEffect);
                UI::drawEffectMenu();
            } else {
                encoder.setCount(paramValues[currentEffect][currentParam]);
                UI::drawParameterMenu();
            }
            break;
        case 2:
            selectedSetting++;
            if (selectedSetting >= 3) {
                currentScreen = 0; // Back to Effect Selection
                encoder.setCount(currentEffect);
                UI::drawEffectMenu();
            } else {
                encoder.setCount(settingsValues[selectedSetting]);
                UI::drawSettingsMenu();
            }
            break;
    }
}

void UI::drawEffectMenu() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextFont(2);
    tft.println("Select Effect:");

    for (int i = 0; i < 8; i++) {
        if (i == currentEffect) {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
        } else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        tft.printf("%d: %s\n", i + 1, effectNames[i]);
    }
    if (currentEffect == 8) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    } else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.printf("Settings\n");

    tft.setTextColor(TFT_WHITE);
    tft.printf("\nPress to adjust parameters.");
}

void UI::drawParameterMenu() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextFont(2);
    tft.printf("%s\n", effectNames[currentEffect]);
    tft.println("Adjust Parameters:\n");

    for (int i = 0; i < 3; i++) {
        int color;
        if (i == currentParam) {
            color = TFT_GREEN;
        } else {
            color = TFT_WHITE;
        }
        // TODO: specific param names for certain effects

        // draw a bar graph
        tft.printf("Param%d: \n", i + 1);
        tft.fillRect(60, 52 + i * 15, paramValues[currentEffect][i], 10, color);
    }

    tft.setTextColor(TFT_WHITE);
    // offset the text so it doesn't overlap the bar graph
    tft.setCursor(0, 100);
    tft.printf("\nPress to switch parameter.");
}

void UI::drawSettingsMenu() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextFont(2);
    tft.println("General Settings:");

    int settingsCount = sizeof(settingsOptions) / sizeof(settingsOptions[0]);

    for (int i = 0; i < settingsCount; i++) {
        if (i == selectedSetting) {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
        } else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        tft.printf("%s: %d\n", settingsOptions[i], settingsValues[i]);
    }

    tft.setTextColor(TFT_WHITE);
    tft.printf("\nPress to switch setting.");
}