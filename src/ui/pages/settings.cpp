#include <ui/pages/settings.h>
#include "ui/page.h"
#define DISABLE_ALL_LIBRARY_WARNINGS
#include "TFT_eSPI.h"
#include "ui/ui_manager.h"
#include "events/event_manager.h"
#include "input.h"
#include "../../settings.h"
#include "controls.h"

void SettingsPage::draw() {
    // centered text

    tft.fillScreen(TFT_BLACK);

    tft.setTextFont(4);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);
    tft.drawString("Settings", tft.width() / 2, 40);

    tft.setTextSize(1);

    int startY = 85;
    int settingSpacing = 40;

    for (int i = 0; i < 2; i++) { // 2 settings + back
        if (i == currentMenuItem) {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
        }
        else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }

        // setting name
        String settingName = menuItems[i];
        tft.drawString(settingName, tft.width() / 2, startY + i * settingSpacing);
        
        // gray background bar
        int barWidth = 120; // help map to progress bar width
        int barX = tft.width() / 2 - barWidth / 2; // center bar horizontally
        tft.fillRect(barX, startY + i * settingSpacing + 10, barWidth, 12, TFT_DARKGREY);

        int progressBarColor = TFT_WHITE;
        if (i == currentMenuItem && editMode) {
            progressBarColor = TFT_GREEN;
        }

        // progress bar
        if (i == 0) { // volume
            int progressWidth = map(Settings::getVolume(), 0, 255, 0, barWidth);
            tft.fillRect(barX, startY + i * settingSpacing + 10, progressWidth, 12, progressBarColor);
        }
        else if (i == 1) { // mix
            int progressWidth = map(Settings::getMix(), 0, 255, 0, barWidth);
            tft.fillRect(barX, startY + i * settingSpacing + 10, progressWidth, 12, progressBarColor);
        }
    }
    if (currentMenuItem == 2) { // Back is selected
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }

    // draw back item

    tft.drawString("Back", tft.width() / 2, startY + 2 * settingSpacing);

    tft.setTextColor(TFT_WHITE);
}


void SettingsPage::handleEvent(const Event& event) {
    // Handle events for the settings menu
    if (event.type == EventType::EVENT_ROTARY_TURN) {
        if (editMode) {
            if (currentMenuItem == 0) { // Volume
                int value = constrain(event.value*15, 0, 255);
                Settings::setVolume(value);
                Controls::setVolume(value);
                Input::setRotaryValue(value / 15);
            }
            else if (currentMenuItem == 1) { // Mix
                int value = constrain(event.value*15, 0, 255);
                Settings::setMix(value);
                Controls::setMix(value);
                Input::setRotaryValue(value / 15);
            }
        }
        else {
            currentMenuItem = constrain(event.value, 0, 2);
            Input::setRotaryValue(currentMenuItem);
        }
        draw();
    }
    if (event.type == EventType::EVENT_ROTARY_CLICK && event.value == 1) { // rotary click (press only)
        if (editMode) {
            editMode = false;
            draw();
        }
        else {
            if (currentMenuItem == 0) { // Volume
                editMode = true;
                Input::setRotaryValue(Settings::getVolume() / 15);
                draw();
            }
            else if (currentMenuItem == 1) { // Mix
                editMode = true;
                Input::setRotaryValue(Settings::getMix() / 15);
                draw();
            }
            else if (currentMenuItem == 2) { // Back
                UIManager::changePage(PageType::MAIN_MENU);
            }
        }
    }
}