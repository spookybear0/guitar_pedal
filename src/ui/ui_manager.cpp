#include "ui_manager.h"
#include <ESP32Encoder.h>
#include <Arduino.h>
#include "events/event_manager.h"
#include "input.h"

#include "ui/page.h"

#include "ui/pages/main_menu.h"
#include "ui/pages/single_effect.h"

#define ROTARY_PIN_A 21
#define ROTARY_PIN_B 22
#define ROTARY_BUTTON 25

TFT_eSPI tft = TFT_eSPI();

Page* UIManager::pages[] = {
    new MainMenuPage(),
    new SingleEffectPage(),
};
PageType UIManager::currentPage = PageType::MAIN_MENU; // corresponds to the index of the page in the pages array

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

void UIManager::changePage(PageType page) {
    // check page number validity
    if (page < 0 || page >= sizeof(pages) / sizeof(pages[0])) {
        Serial.println("ERROR: Invalid page number, restarting...");
        ESP.restart();
        return;
    }

    currentPage = page;
    Input::setRotaryValue(0);
    pages[currentPage]->enter();
    pages[currentPage]->draw();
}