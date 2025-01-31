#include <ui/pages/single_effect.h>
#include "ui/page.h"
#define DISABLE_ALL_LIBRARY_WARNINGS
#include "TFT_eSPI.h"
#include "ui/ui_manager.h"
#include "events/event_manager.h"
#include "input.h"
#include "settings.h"

void SingleEffectPage::draw() {
    // centered text

    tft.fillScreen(TFT_BLACK);

    tft.setTextFont(4);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    if (currentEffect == 8) {
        tft.drawString("Back", tft.width() / 2, tft.height() / 2);

        tft.setTextSize(1);
        // bottom left text ("prev effect")
        tft.setTextDatum(BL_DATUM);
        tft.drawString(String("<-") + effectNames[currentEffect - 1], 10, tft.height() - 10);
    }
    else {
        tft.drawString(Settings::getEffects()[currentEffect].getName(), tft.width() / 2, 40);

        tft.setTextSize(1);

        // display params

        int startY = 85;
        int paramSpacing = 40;

        for (int i = 0; i < 3; i++) { // Assuming each effect has 3 parameters
            if (i == currentParam && editMode == EditMode::PARAM) {
                tft.setTextColor(TFT_GREEN, TFT_BLACK);
            } else {
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
            }

            // parameter name
            String paramName = Settings::getEffects()[currentEffect].getParamName(i);
            tft.drawString(paramName, tft.width() / 2, startY + i * paramSpacing);
            
            // gray background bar
            int barWidth = 120; // help map to progress bar width
            int barX = tft.width() / 2 - barWidth / 2; // center bar horizontally
            tft.fillRect(barX, startY + i * paramSpacing + 10, barWidth, 12, TFT_DARKGREY);

            int progressBarColor = TFT_WHITE;
            if (i == currentParam && editMode == EditMode::VALUE) {
                progressBarColor = TFT_GREEN;
            }
            
            // progress bar
            int progressWidth = map(Settings::getEffects()[currentEffect].params[i], 0, 255, 0, barWidth);
            tft.fillRect(barX, startY + i * paramSpacing + 10, progressWidth, 12, progressBarColor);
        }
        tft.setTextColor(TFT_WHITE);

        // draw prev/next text in the corners

        if (currentEffect > 0) {
            tft.setTextDatum(BL_DATUM);
            tft.drawString(String("<-") + effectNames[currentEffect - 1], 10, tft.height() - 10);
        }

        tft.setTextDatum(BR_DATUM);
        if (currentEffect < sizeof(effectNames) / sizeof(effectNames[0]) - 1) {
            tft.drawString(effectNames[currentEffect + 1] + String("->"), tft.width() - 10, tft.height() - 10);
        }
        else {
            tft.drawString("Back->", tft.width() - 10, tft.height() - 10);
        }
    }
}


void SingleEffectPage::handleEvent(const Event& event) {
    if (event.type == EventType::EVENT_ROTARY_TURN) {
        if (editMode == EditMode::EFFECT) {
            currentEffect = constrain(event.value, 0, 8); // Assuming 8 effects + "Back"
            Input::setRotaryValue(currentEffect);
        }
        else if (editMode == EditMode::PARAM) {
            currentParam = constrain(event.value, 0, 2); // each effect has 3 params
            Input::setRotaryValue(currentParam);
        }
        else if (editMode == EditMode::VALUE) {
            std::vector<Effect> effects = Settings::getEffects();
            int value = constrain(event.value*15, 0, 255);
            effects[currentEffect].params[currentParam] = value;
            Settings::setEffects(effects);

            Input::setRotaryValue(Settings::getEffects()[currentEffect].params[currentParam] / 15);
        }
        draw();
    }
    if (event.type == EventType::EVENT_ROTARY_CLICK && event.value == 1) { // rotary click (press only)
        if (editMode == EditMode::EFFECT) {
            if (currentEffect == 8) { // back
                UIManager::changePage(PageType::MAIN_MENU);
            }
            else {
                editMode = EditMode::PARAM;
                currentParam = 0;
                Input::setRotaryValue(currentParam);
            }
        }
        else if (editMode == EditMode::PARAM) {
            editMode = EditMode::VALUE;
            Input::setRotaryValue(Settings::getEffects()[currentEffect].params[currentParam] / 15);
        }
        else if (editMode == EditMode::VALUE) {
            editMode = EditMode::EFFECT;
            currentParam = -1; // deselect param
            Input::setRotaryValue(currentEffect);
        }
        draw();
    }
}

void SingleEffectPage::enter() {
    currentEffect = 0;
    currentParam = -1;
    editMode = EditMode::EFFECT;
    Input::setRotaryValue(currentEffect);
}