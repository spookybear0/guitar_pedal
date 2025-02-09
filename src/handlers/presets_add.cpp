#include "ArduinoJson.h"
#include "settings.h"
#include "preset.h"

void presets_add(JsonDocument& json, JsonDocument& response) {
    Serial.println("preset_add handler called");

    if (json["name"].isNull()) {
        response["status"] = "error";
        response["message"] = "preset name is null";
        return;
    }

    if (json["effects"].isNull()) {
        response["status"] = "error";
        response["message"] = "preset effects is null";
        return;
    }

    std::vector<Effect> effects;
    JsonArray effectsArray = json["effects"].as<JsonArray>();
    // loop through each effect
    for (JsonArray::iterator effectIter = effectsArray.begin(); effectIter != effectsArray.end(); ++effectIter) {
        Effect effect = Effect::fromJson(*effectIter);
        effects.push_back(effect);
    }
    Preset preset = Preset(json["name"], effects);

    // add preset to settings

    Settings::addEffectPreset(preset);

    // create response
    response["status"] = "success";
}