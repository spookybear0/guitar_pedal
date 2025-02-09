// gets all presets
#include "preset.h"
#include "settings.h"
#include "ArduinoJson.h"

void presets_get(JsonDocument& json, JsonDocument& response) {
    try {
    Serial.println("presets_get handler called");

    if (json["name"].isNull()) { // get all presets
        std::vector<Preset>& presets = Settings::getEffectPresets();
        JsonArray presetsArray = response["data"]["presets"].to<JsonArray>();

        for (int i = 0; i < presets.size(); i++) {
            JsonDocument presetDoc;
            presets[i].toJson(presetDoc, true);
            presetsArray.add(presetDoc);
        }

        // create response
        response["status"] = "success";

        return;
    }

    if (json["name"].is<String>()) { // get preset by name
        std::vector<Preset>& presets = Settings::getEffectPresets();
        for (int i = 0; i < presets.size(); i++) {
            if (strcmp(presets[i].getName().c_str(), json["name"]) == 0) {
                response["status"] = "success";
                JsonDocument presetDoc;
                presets[i].toJson(presetDoc, true);
                response["data"]["preset"] = presetDoc;
                return;
            }
        }

        response["status"] = "error";
        response["message"] = "preset not found";
    }
    else {
        response["status"] = "error";
        response["message"] = "preset name invalid";
    }
    }
    catch (const std::exception& e) {
        Serial.println("Error in presets_get handler: " + String(e.what()));
        response["status"] = "error";
        response["message"] = "internal server error";
    }
}