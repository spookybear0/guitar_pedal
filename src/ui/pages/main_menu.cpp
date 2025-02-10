#include <ui/pages/main_menu.h>
#include "ui/page.h"
#define DISABLE_ALL_LIBRARY_WARNINGS
#include "TFT_eSPI.h"
#include "ui/ui_manager.h"
#include "events/event_manager.h"
#include "input.h"

void MainMenuPage::draw() {
    // centered text

    tft.fillScreen(TFT_BLACK);

    tft.setTextFont(4);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);
    tft.drawString("Main Menu", tft.width() / 2, 40);

    tft.setTextSize(1);
    for (int i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++) {
        if (i == currentMenuItem) {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
        } else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        tft.drawString(menuItems[i], tft.width() / 2, tft.height() / 2 + i * 30);
    }

    tft.setTextColor(TFT_WHITE);
}


void MainMenuPage::handleEvent(const Event& event) {
    // Handle events for the main menu
    if (event.type == EventType::EVENT_ROTARY_TURN) {
        currentMenuItem = constrain(event.value, 0, sizeof(menuItems) / sizeof(menuItems[0]) - 1);
        Input::setRotaryValue(currentMenuItem);
        draw();
    }
    if (event.type == EventType::EVENT_ROTARY_CLICK && event.value == 1) { // rotary click (press only)
        // check page and navigate
        if (currentMenuItem == 0) { // Single Effect
            UIManager::changePage(PageType::SINGLE_EFFECT);
        }
        else if (currentMenuItem == 1) { // Preset List
            UIManager::changePage(PageType::PRESET_LIST);
        }
        else if (currentMenuItem == 2) { // Settings
            UIManager::changePage(PageType::SETTINGS);
        }
    }
}