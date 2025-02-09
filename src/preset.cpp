#include "preset.h"
#include <ArduinoJson.h>
#include "settings.h"

Preset::Preset() {
    this->name = "";
}

Preset::Preset(std::vector<Effect> effects) {
    this->effects = effects;

    // set id to the number of presets
    this->id = Settings::getNextPresetId();
    Settings::setNextPresetId(this->id + 1);
}

Preset::Preset(std::string name, std::vector<Effect> effects) {
    this->name = name;
    this->effects = effects;

    // set id to the number of presets
    this->id = Settings::getNextPresetId();
    Settings::setNextPresetId(this->id + 1);
}

Preset Preset::fromJson(const JsonObject& json) {
    Preset preset;

    preset.id = json["id"];
    preset.name = json["name"].as<std::string>();

    std::vector<Effect> effectsOutput;
    JsonArray effectsArray = json["effects"].as<JsonArray>();
    // loop through each effect
    for (JsonArray::iterator effectIter = effectsArray.begin(); effectIter != effectsArray.end(); ++effectIter) {
        Effect effect = Effect::fromJson(*effectIter);
        effectsOutput.push_back(effect);
    }

    preset.effects = effectsOutput;

    return preset;
}

std::string Preset::getName() {
    return name;
}

void Preset::setName(std::string name) {
    this->name = name;
}

int Preset::getId() {
    return id;
}

void Preset::setId(int id) {
    this->id = id;
}


void Preset::toJson(JsonDocument& doc, bool full) {
    doc["id"] = id;
    doc["name"] = name;
    JsonArray effectsArray = doc["effects"].to<JsonArray>();
    for (int i = 0; i < effects.size(); i++) {
        JsonDocument effectDoc;
        effects[i].toJson(effectDoc, full);
        effectsArray.add(effectDoc);
    }
}