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

    
    tft.drawString(preset->effects[currentEffect].getName(), tft.width() / 2, 40);

    tft.setTextSize(1);

    int startY = 80;
    int paramSpacing = 25;
    int barWidth = 120; // help map to progress bar width
    int textWidth = 50; // Estimated text width
    int totalWidth = textWidth + barWidth + 10; // 10px spacing between text and bar
    int centerX = tft.width() / 2 - totalWidth / 2; // Center everything horizontally
    int textX = centerX + 25; // Position text with some padding
    int barX = textX + textWidth + 10; // Position bar right next to text

    // draw params
    for (int i = 0; i < 3; i++) { // Assuming each effect has 3 parameters
        // parameter name
        String paramName = preset->effects[currentEffect].getParamName(i);
        tft.drawString(paramName, textX, startY + i * paramSpacing);

        // gray background bar
        tft.fillRect(barX, startY + i * paramSpacing - 8, barWidth, 12, TFT_DARKGREY);

        // progress bar
        int progressWidth = map(preset->effects[currentEffect].params[i], 0, 255, 0, barWidth);
        tft.fillRect(barX, startY + i * paramSpacing - 8, progressWidth, 12, TFT_WHITE);
    }

    // draw volume
    tft.drawString("Volume", textX, startY + 3 * paramSpacing);

    tft.fillRect(barX, startY + 3 * paramSpacing - 8, barWidth, 12, TFT_DARKGREY);
    int progressWidth = map(preset->effects[currentEffect].volume, 0, 255, 0, barWidth);
    tft.fillRect(barX, startY + 3 * paramSpacing - 8, progressWidth, 12, TFT_WHITE);

    // draw mix
    tft.drawString("Mix", textX, startY + 4 * paramSpacing);

    tft.fillRect(barX, startY + 4 * paramSpacing - 8, barWidth, 12, TFT_DARKGREY);
    progressWidth = map(preset->effects[currentEffect].mix, 0, 255, 0, barWidth);
    tft.fillRect(barX, startY + 4 * paramSpacing - 8, progressWidth, 12, TFT_WHITE);

    tft.setTextColor(TFT_WHITE);


    // draw prev/next text in the corners

    if (currentEffect > 0) {
        tft.setTextDatum(BL_DATUM);
        tft.drawString(String("<-") + String(preset->effects[currentEffect - 1].getName()), 10, tft.height() - 10);
    }

    tft.setTextDatum(BR_DATUM);
    if (currentEffect < preset->effects.size() - 1) {
        tft.drawString(String(preset->effects[currentEffect + 1].getName()) + String("->"), tft.width() - 10, tft.height() - 10);
    }
}

void PresetPage::handleEvent(const Event& event) {
    // handle next and previous effect
    if (event.type == EventType::EVENT_ROTARY_TURN) {
        currentEffect = constrain(event.value, 0, preset->effects.size() - 1);
        Input::setRotaryValue(currentEffect);
        draw();
    }
    else if (event.type == EventType::EVENT_FOOTSWITCH1 && event.value == 1) { // footswitch 1 pressed
        // go to previous effect
        currentEffect = constrain(currentEffect - 1, 0, preset->effects.size() - 1);
        Input::setRotaryValue(currentEffect);
        draw();
    }
    else if (event.type == EventType::EVENT_FOOTSWITCH2 && event.value == 1) { // footswitch 2 pressed
        // go to next effect
        currentEffect = constrain(currentEffect + 1, 0, preset->effects.size() - 1);
        Input::setRotaryValue(currentEffect);
        draw();
    }
    // go back to preset list
    else if (event.type == EventType::EVENT_ROTARY_CLICK && event.value == 1) {
        UIManager::changePage(PageType::PRESETS);
    }
}

void PresetPage::enter() {
    currentEffect = 0;
}

void PresetPage::setPreset(Preset& preset) {
    this->preset = &preset;
}