// remove a preset by id
#include "preset.h"
#include "settings.h"
#include "ArduinoJson.h"

void presets_remove(JsonDocument& json, JsonDocument& response) {
    Serial.println("preset_remove handler called");

    if (json["id"].isNull()) {
        response["status"] = "error";
        response["message"] = "preset id is null";
        return;
    }

    Settings::removeEffectPreset(json["id"].as<int>());

    response["status"] = "error";
    response["message"] = "preset not found";
}