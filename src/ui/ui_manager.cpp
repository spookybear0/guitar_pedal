#include "ui_manager.h"
#include <ESP32Encoder.h>

#include "ui/page.h"

#include "ui/pages/main_menu.h"

#define ROTARY_PIN_A 21
#define ROTARY_PIN_B 22
#define ROTARY_BUTTON 25

TFT_eSPI tft = TFT_eSPI();

Page* UIManager::pages[] = {
    new MainMenuPage(),
};
int UIManager::currentPage = 0; // corresponds to the index of the page in the pages array

void UIManager::init() {
    // tft
    tft.init();
    tft.setRotation(1);

    // subscribe to events
    EventManager::subscribe(&UIManager::handleEvent);
}

void UIManager::handleEvent(const Event& event) {
    // check page number validity
    if (currentPage < 0 || currentPage >= sizeof(pages) / sizeof(pages[0])) {
        Serial.println("ERROR: Invalid page number, restarting...");
        ESP.restart();
        return;
    }

    // set chip select pin to low to start communication with the display
    digitalWrite(TFT_CS, LOW);

    // send the event to the current page
    pages[currentPage]->handleEvent(event);

    // end communication with the display
    digitalWrite(TFT_CS, HIGH);
}