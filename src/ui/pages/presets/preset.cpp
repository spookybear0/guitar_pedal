#include <ui/pages/presets/preset.h>
#include "ui/page.h"
#define DISABLE_ALL_LIBRARY_WARNINGS
#include "TFT_eSPI.h"
#include "ui/ui_manager.h"
#include "events/event_manager.h"
#include "input.h"
#include "../../../settings.h"


void PresetPage::draw() {
    // centered text

    tft.fillScreen(TFT_BLACK);

    tft.setTextFont(4);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    
    tft.drawString(preset.effects[currentEffect].getName(), tft.width() / 2, 40);

    tft.setTextSize(1);

    // display params

    int startY = 85;
    int paramSpacing = 40;

    for (int i = 0; i < 3; i++) { // Assuming each effect has 3 parameters
        // parameter name
        String paramName = preset.effects[currentEffect].getParamName(i);
        tft.drawString(paramName, tft.width() / 2, startY + i * paramSpacing);
        
        // gray background bar
        int barWidth = 120; // help map to progress bar width
        int barX = tft.width() / 2 - barWidth / 2; // center bar horizontally
        tft.fillRect(barX, startY + i * paramSpacing + 10, barWidth, 12, TFT_DARKGREY);

        // progress bar
        int progressWidth = map(preset.effects[currentEffect].params[i], 0, 255, 0, barWidth);
        tft.fillRect(barX, startY + i * paramSpacing + 10, progressWidth, 12, TFT_WHITE);
    }
    tft.setTextColor(TFT_WHITE);

    // draw prev/next text in the corners

    if (currentEffect > 0) {
        tft.setTextDatum(BL_DATUM);
        tft.drawString(String("<-") + String(preset.effects[currentEffect - 1].getName()), 10, tft.height() - 10);
    }

    tft.setTextDatum(BR_DATUM);
    if (currentEffect < preset.effects.size() - 1) {
        tft.drawString(String(preset.effects[currentEffect + 1].getName()) + String("->"), tft.width() - 10, tft.height() - 10);
    }
}

void PresetPage::handleEvent(const Event& event) {
    // handle next and previous effect
    if (event.type == EventType::EVENT_ROTARY_TURN) {
        currentEffect = constrain(event.value, 0, preset.effects.size() - 1);
        Input::setRotaryValue(currentEffect);
    }
    else if (event.type == EventType::EVENT_FOOTSWITCH1 && event.value == 1) { // footswitch 1 pressed
        // go to previous effect
        currentEffect = constrain(currentEffect - 1, 0, preset.effects.size() - 1);
        Input::setRotaryValue(currentEffect);
    }
    else if (event.type == EventType::EVENT_FOOTSWITCH2 && event.value == 1) { // footswitch 2 pressed
        // go to next effect
        currentEffect = constrain(currentEffect + 1, 0, preset.effects.size() - 1);
        Input::setRotaryValue(currentEffect);
    }
    // go back to preset list
    else if (event.type == EventType::EVENT_ROTARY_CLICK && event.value == 1) {
        UIManager::changePage(PageType::PRESET_LIST);
    }
    draw();
}

void PresetPage::enter() {
    currentEffect = 0;
}

void PresetPage::setPreset(Preset preset) {
    this->preset = preset;
}