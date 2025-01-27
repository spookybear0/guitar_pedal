#pragma once
#define DISABLE_ALL_LIBRARY_WARNINGS
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

class UI {
public:
    static void init();
    static void draw();
    static void handleButtonPress();
    static void drawEffectMenu();
    static void drawParameterMenu();
    static void drawSettingsMenu();
};