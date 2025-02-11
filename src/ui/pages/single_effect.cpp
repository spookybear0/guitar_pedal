#include <ui/pages/single_effect.h>
#include "ui/page.h"
#define DISABLE_ALL_LIBRARY_WARNINGS
#include "TFT_eSPI.h"
#include "ui/ui_manager.h"
#include "events/event_manager.h"
#include "input.h"
#include "settings.h"
#include "controls.h"
#include "../../settings.h"

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
        tft.drawString(String("<-") + Settings::getEffects()[currentEffect - 1].getName(), 10, tft.height() - 10);
    }
    else {
        tft.drawString(Settings::getEffects()[currentEffect].getName(), tft.width() / 2, 40);

        tft.setTextSize(1);

        // display params

        int startY = 85;
        int paramSpacing = 40;

        for (int i = 0; i < 3; i++) { // Assuming each effect has 3 parameters
            if (i == currentParam && editMode == EffectEditMode::PARAM) {
                tft.setTextColor(TFT_GREEN, TFT_BLACK);
            }
            else {
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
            if (i == currentParam && editMode == EffectEditMode::VALUE) {
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
            tft.drawString(String("<-") + String(Settings::getEffects()[currentEffect - 1].getName()), 10, tft.height() - 10);
        }

        tft.setTextDatum(BR_DATUM);
        if (currentEffect < 7) {
            tft.drawString(String(Settings::getEffects()[currentEffect + 1].getName()) + String("->"), tft.width() - 10, tft.height() - 10);
        }
        else {
            tft.drawString("Back->", tft.width() - 10, tft.height() - 10);
        }
    }
}


void SingleEffectPage::handleEvent(const Event& event) {
    if (event.type == EventType::EVENT_ROTARY_TURN) {
        if (editMode == EffectEditMode::EFFECT) {
            currentEffect = constrain(event.value, 0, 8); // Assuming 8 effects + "Back"
            Input::setRotaryValue(currentEffect);

            // update controls
            Controls::setEffect(currentEffect);
            Effect effect = Settings::getEffects()[currentEffect];
            Controls::setParam1(effect.params[0]);
            Controls::setParam2(effect.params[1]);
            Controls::setParam3(effect.params[2]);
        }
        else if (editMode == EffectEditMode::PARAM) {
            currentParam = constrain(event.value, 0, 2); // each effect has 3 params
            Input::setRotaryValue(currentParam);
        }
        else if (editMode == EffectEditMode::VALUE) {
            std::vector<Effect> effects = Settings::getEffects();
            int value = constrain(event.value*15, 0, 255);
            effects[currentEffect].params[currentParam] = value;
            Settings::setEffects(effects);

            if (currentParam == 0) {
                Controls::setParam1(value);
            }
            else if (currentParam == 1) {
                Controls::setParam2(value);
            }
            else if (currentParam == 2) {
                Controls::setParam3(value);
            }
            
            Input::setRotaryValue(value / 15);
        }
        draw();
    }
    if (event.type == EventType::EVENT_ROTARY_CLICK && event.value == 1) { // rotary click (press only)
        if (editMode == EffectEditMode::EFFECT) {
            if (currentEffect == 8) { // back
                UIManager::changePage(PageType::MAIN_MENU);
            }
            else {
                editMode = EffectEditMode::PARAM;
                currentParam = 0;
                Input::setRotaryValue(currentParam);
            }
        }
        else if (editMode == EffectEditMode::PARAM) {
            editMode = EffectEditMode::VALUE;
            Input::setRotaryValue(Settings::getEffects()[currentEffect].params[currentParam] / 15);
        }
        else if (editMode == EffectEditMode::VALUE) {
            editMode = EffectEditMode::EFFECT;
            currentParam = -1; // deselect param
            Input::setRotaryValue(currentEffect);
        }
        draw();
    }
}

void SingleEffectPage::enter() {
    if (initalEffect == -1) {
        currentEffect = 0;

    }
    else {
        currentEffect = initalEffect;
    }
    currentParam = -1;
    editMode = EffectEditMode::EFFECT;
    Input::setRotaryValue(currentEffect);
}

void SingleEffectPage::setInitialEffect(int effect) {
    initalEffect = effect;
}

int SingleEffectPage::getEffect() {
    return currentEffect;
}