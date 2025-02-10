#include <ui/pages/presets/preset_list.h>
#include "ui/page.h"
#define DISABLE_ALL_LIBRARY_WARNINGS
#include "TFT_eSPI.h"
#include "ui/ui_manager.h"
#include "events/event_manager.h"
#include "input.h"
#include "../../../settings.h"
#include "ui/pages/presets/preset.h"

void PresetListPage::draw() {
    // centered text

    tft.fillScreen(TFT_BLACK);

    tft.setTextFont(4);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);
    tft.drawString("Presets", tft.width() / 2, 40);

    tft.setTextSize(1);

    tft.setTextColor(TFT_WHITE);

    // draw presets
    
    int startY = 85;
    int presetSpacing = 40;

    std::vector<Preset>& presets = Settings::getEffectPresets();
    for (int i = 0; i < presets.size(); i++) {
        if (i == currentMenuItem) {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
        }
        else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        tft.drawString(presets[i].getName().c_str(), tft.width() / 2, startY + i * presetSpacing);
    }

    // back

    if (currentMenuItem == presets.size()) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.drawString("Back", tft.width() / 2, startY + presets.size() * presetSpacing);

    tft.setTextColor(TFT_WHITE);
}

void PresetListPage::handleEvent(const Event& event) {
    if (event.type == EventType::EVENT_ROTARY_TURN) {
        currentMenuItem = constrain(event.value, 0, Settings::getEffectPresets().size());
        Input::setRotaryValue(currentMenuItem);
        draw();
    }
    if (event.type == EventType::EVENT_ROTARY_CLICK && event.value == 1) { // rotary click (press only)
        if (currentMenuItem == Settings::getEffectPresets().size()) { // back
            UIManager::changePage(PageType::MAIN_MENU);
        }
        else {
            // navigate to preset page
            UIManager::changePageDeferred(PageType::PRESET);
            // pass preset id
            PresetPage* page = (PresetPage*)UIManager::pages[PageType::PRESET];
            page->setPreset(Settings::getEffectPresets()[currentMenuItem]);
        }
    }
}

void PresetListPage::enter() {

}