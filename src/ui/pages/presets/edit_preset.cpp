#include <ui/pages/presets/edit_preset.h>
#include "ui/page.h"
#define DISABLE_ALL_LIBRARY_WARNINGS
#include "TFT_eSPI.h"
#include "ui/ui_manager.h"
#include "events/event_manager.h"
#include "input.h"
#include "../../../settings.h"


void EditPresetPage::draw() {
    tft.fillScreen(TFT_BLACK);

    tft.setTextFont(4);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    if (editMode == EditPresetMode::EFFECT) {
        tft.drawString(String(preset->getName().c_str()), tft.width() / 2, 40);
        tft.setTextSize(1);

        // display list of effects (that are in the preset)

        int startY = 85;
        int effectSpacing = 30;

        for (int i = 0; i < preset->effects.size(); i++) {
            if (i == currentEffect) {
                tft.setTextColor(TFT_GREEN, TFT_BLACK);
            }
            else {
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
            }
            tft.drawString(preset->effects[i].getName(), tft.width() / 2, startY + i * effectSpacing);
        }

        // draw back

        if (currentEffect == preset->effects.size()) {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
        }
        else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }

        tft.drawString("Back", tft.width() / 2, startY + preset->effects.size() * effectSpacing);

        tft.setTextColor(TFT_WHITE);
    }
    else {
        tft.drawString(preset->effects[currentEffect].getName(), tft.width() / 2, 40);
        tft.setTextSize(1);

        // display params of current effect

        int startY = 85;
        int paramSpacing = 30;
        int barWidth = 120; // help map to progress bar width
        int textWidth = 50; // Estimated text width
        int totalWidth = textWidth + barWidth + 10; // 10px spacing between text and bar
        int centerX = tft.width() / 2 - totalWidth / 2; // Center everything horizontally
        int textX = centerX + 25; // Position text with some padding
        int barX = textX + textWidth + 10; // Position bar right next to text

        // draw params
        for (int i = 0; i < 3; i++) { // Assuming each effect has 3 parameters
            if (i == currentParam && editMode == EditPresetMode::PARAM) {
                tft.setTextColor(TFT_GREEN, TFT_BLACK);
            } else {
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
            }

            // parameter name
            String paramName = preset->effects[currentEffect].getParamName(i);
            tft.drawString(paramName, textX, startY + i * paramSpacing);

            // gray background bar
            tft.fillRect(barX, startY + i * paramSpacing - 8, barWidth, 12, TFT_DARKGREY);

            int progressBarColor = (i == currentParam && editMode == EditPresetMode::VALUE) ? TFT_GREEN : TFT_WHITE;

            // progress bar
            int progressWidth = map(preset->effects[currentEffect].params[i], 0, 255, 0, barWidth);
            tft.fillRect(barX, startY + i * paramSpacing - 8, progressWidth, 12, progressBarColor);
        }

        // draw volume
        tft.setTextColor(currentParam == 3 ? TFT_GREEN : TFT_WHITE, TFT_BLACK);
        tft.drawString("Volume", textX, startY + 3 * paramSpacing);

        tft.fillRect(barX, startY + 3 * paramSpacing - 8, barWidth, 12, TFT_DARKGREY);
        int progressBarColor = (currentParam == 3 && editMode == EditPresetMode::VALUE) ? TFT_GREEN : TFT_WHITE;
        int progressWidth = map(preset->effects[currentEffect].volume, 0, 255, 0, barWidth);
        tft.fillRect(barX, startY + 3 * paramSpacing - 8, progressWidth, 12, progressBarColor);

        // draw mix
        tft.setTextColor(currentParam == 4 ? TFT_GREEN : TFT_WHITE, TFT_BLACK);
        tft.drawString("Mix", textX, startY + 4 * paramSpacing);

        tft.fillRect(barX, startY + 4 * paramSpacing - 8, barWidth, 12, TFT_DARKGREY);
        progressBarColor = (currentParam == 4 && editMode == EditPresetMode::VALUE) ? TFT_GREEN : TFT_WHITE;
        progressWidth = map(preset->effects[currentEffect].mix, 0, 255, 0, barWidth);
        tft.fillRect(barX, startY + 4 * paramSpacing - 8, progressWidth, 12, progressBarColor);

        tft.setTextColor(TFT_WHITE);
    }
}

void EditPresetPage::handleEvent(const Event& event) {
    if (event.type == EventType::EVENT_ROTARY_TURN) {
        if (editMode == EditPresetMode::EFFECT) {
            currentEffect = constrain(event.value, 0, preset->effects.size()); // preset effects size + "Back"
            Input::setRotaryValue(currentEffect);
            draw();
        }
        else if (editMode == EditPresetMode::PARAM) {
            currentParam = constrain(event.value, 0, 4); // 3 params + volume + mix
            Input::setRotaryValue(currentParam);
        }
        else if (editMode == EditPresetMode::VALUE) {
            int value = constrain(event.value*15, 0, 255);
            if (currentParam < 3) {
                // params (1,2,3)
                preset->effects[currentEffect].params[currentParam] = value;
            }
            else if (currentParam == 3) {
                // volume
                preset->effects[currentEffect].volume = value;
            }
            else if (currentParam == 4) {
                // mix
                preset->effects[currentEffect].mix = value;
            }

            Settings::setEffectPresets(Settings::getEffectPresets());

            // TODO: maybe update controls here
            
            Input::setRotaryValue(value / 15);
        }
        draw();
    }
    else if (event.type == EventType::EVENT_ROTARY_CLICK && event.value == 1) { // rotary click (press only)
        if (editMode == EditPresetMode::EFFECT) {
            if (currentEffect == preset->effects.size()) { // back
                UIManager::changePage(PageType::EDIT_PRESETS);
            }
            else {
                editMode = EditPresetMode::PARAM;
                draw();
            }
        }
        else if (editMode == EditPresetMode::PARAM) {
            editMode = EditPresetMode::VALUE;
            draw();
        }
        else if (editMode == EditPresetMode::VALUE) {
            editMode = EditPresetMode::EFFECT;

            if (currentParam < 3) {
                // params (1,2,3)
                Input::setRotaryValue(preset->effects[currentEffect].params[currentParam] / 15);
            }
            else if (currentParam == 3) {
                // volume
                Input::setRotaryValue(preset->effects[currentEffect].volume / 15);
            }
            else if (currentParam == 4) {
                // mix
                Input::setRotaryValue(preset->effects[currentEffect].mix / 15);
            }
            draw();
        }
    }
}

void EditPresetPage::enter() {
    
}

void EditPresetPage::setPreset(Preset& preset) {
    this->preset = &preset;
}