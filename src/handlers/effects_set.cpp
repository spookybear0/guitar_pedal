#include "ArduinoJson.h"
#include "ui/ui_manager.h"
#include "settings.h"
// set parameters for single effect mode

void effects_set(JsonDocument& json, JsonDocument& response) {
    Serial.println("effects_set handler called");

    if (json["effect"].isNull()) {
        response["status"] = "error";
        response["message"] = "effect is null";
        return;
    }

    if (json["effect"]["id"].isNull()) {
        response["status"] = "error";
        response["message"] = "effect name is null";
        return;
    }

    if (json["effect"]["params"].isNull()) {
        response["status"] = "error";
        response["message"] = "effect params is null";
        return;
    }

    int effectId = json["effect"]["id"].as<int>();

    std::vector<Effect> effects = Settings::getEffects();
    Effect effect = Effect(effectId);
    effect.params[0] = json["effect"]["params"][0];
    effect.params[1] = json["effect"]["params"][1];
    effect.params[2] = json["effect"]["params"][2];
    effects[effectId] = effect;
    Settings::setEffects(effects);

    if (!json["draw"].isNull() && json["draw"].as<bool>()) {
        UIManager::forceDraw();
    }

    // create response
    response["status"] = "success";
}