// remove a preset by id
#include "preset.h"
#include "controls.h"
#include "ArduinoJson.h"
#include "ui/ui_manager.h"
#include "ui/pages/single_effect.h"

void single_effect_set(JsonDocument& json, JsonDocument& response) {
    Serial.println("single_effect_set handler called");

    // sets the currently selected effect (0-7), so we have to switch to that effect and single effect page

    if (json["effect_id"].isNull()) {
        Serial.println("Effect id is null!");
        response["status"] = "error";
        response["message"] = "effect id is null";
        return;
    }

    int effectId = json["effect_id"].as<int>();

    // set the effect

    UIManager::changePageDeferred(PageType::SINGLE_EFFECT);
    SingleEffectPage* page = (SingleEffectPage*)UIManager::pages[PageType::SINGLE_EFFECT];

    page->setInitialEffect(effectId);

    // create response
    response["status"] = "success";
}