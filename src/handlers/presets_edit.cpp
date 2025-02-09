// change preset by changing/adding effects and modifying the parameters of those effects by id
#include "preset.h"
#include "settings.h"
#include "ArduinoJson.h"

void presets_edit(JsonDocument& json, JsonDocument& response) {
    Serial.println("presets_edit handler called");

    if (json["id"].isNull()) {
        response["status"] = "error";
        response["message"] = "preset id is null";
        return;
    }

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

    Serial.println("ID: " + String(json["id"].as<int>()));
    Serial.println("Name: " + String(json["name"].as<String>().c_str()));
    Serial.println("Effects: " + String(json["effects"].as<String>().c_str()));

    std::vector<Effect> effects;
    JsonArray effectsArray = json["effects"].as<JsonArray>();
    // loop through each effect
    for (JsonArray::iterator effectIter = effectsArray.begin(); effectIter != effectsArray.end(); ++effectIter) {
        Effect effect = Effect::fromJson(*effectIter);
        effects.push_back(effect);
    }

    std::vector<Preset>& presets = Settings::getEffectPresets();
    for (int i = 0; i < presets.size(); i++) {
        // check id
        if (presets[i].getId() == json["id"].as<int>()) {
            presets[i].setName(json["name"].as<String>().c_str());
            presets[i].effects = effects;
            Settings::setEffectPresets(presets);
            response["status"] = "success";
            return;
        }
    }

    // print out the presets

    response["status"] = "success";
}