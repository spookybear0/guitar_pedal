#include "settings.h"
#include <Preferences.h>
#include <ArduinoJson.h>
#include <vector>
#include "effect.h"
#include "preset.h"


Preferences Settings::settings;
bool Settings::updateNeeded = false;
int Settings::volume = 128;
int Settings::mix = 255;
std::vector<Effect> Settings::effects = {
    Effect(0),
    Effect(1),
    Effect(2),
    Effect(3),
    Effect(4),
    Effect(5),
    Effect(6),
    Effect(7)
};
std::vector<Preset>& Settings::effectPresets = *new std::vector<Preset>();
int Settings::nextPresetId = 0;

void Settings::begin() {
    settings.begin("settings", false);

    bool firstRun = settings.getBool("firstRun", true);

    if (firstRun) {
        Serial.println("First run, resetting settings to factory defaults");
        // factory defaults
        reset();
    }
    else {
        // load values in variables
        volume = getSavedVolume();
        mix = getSavedMix();
        effects = getSavedEffects();
        effectPresets = getSavedEffectPresets();
        nextPresetId = getNextPresetId();
    }
}

void Settings::reset() {
    settings.clear();

    setBool("firstRun", false);
    
    setInt("volume", 128);
    setInt("mix", 255);

    // set default effects to default values
    JsonDocument effectsDoc;
    JsonArray effectsArray = effectsDoc["effects"].to<JsonArray>();

    for (int i = 0; i < 8; i++) {
        Effect effect(i);
        JsonDocument effectDoc;
        effect.toJson(effectDoc, false);
        effectsArray.add(effectDoc);
    }

    setJson("effects", effectsDoc);


    setString("presets", "{\"presets\": []}");

    setInt("nextPresetId", 0);


    // save values in variables
    volume = getSavedVolume();
    mix = getSavedMix();
    effects = getSavedEffects();
    effectPresets = getSavedEffectPresets();
    nextPresetId = getNextPresetId();
}

void Settings::saveValues() {
    // update value in settings (try to use this on an interval)

    if (updateNeeded) {
        settings.putInt("volume", volume);
        settings.putInt("mix", mix);

        // update effects
        JsonDocument effectDoc;
        JsonArray effectsArray = effectDoc["effects"].to<JsonArray>();

        for (int i = 0; i < 8; i++) {
            JsonDocument effectDoc;
            effects[i].toJson(effectDoc, false);
            effectsArray.add(effectDoc);
        }

        setJson("effects", effectDoc);

        // update presets

        JsonDocument presetDoc;
        // save as {"presets": [[Effect, Effect, ...], [Effect, Effect, ...], ...]}

        JsonArray presetsArray = presetDoc["presets"].to<JsonArray>();
        for (int i = 0; i < effectPresets.size(); i++) {
            JsonDocument presetDoc;
            effectPresets[i].toJson(presetDoc);
            presetsArray.add(presetDoc);
        }

        setJson("presets", presetDoc);

        settings.putInt("nextPresetId", nextPresetId);

        updateNeeded = false;

        Serial.println("Settings saved!");
    }
}

int Settings::getVolume() {
    return volume;
}

void Settings::setVolume(int volume) {
    Settings::volume = volume;
    updateNeeded = true;
}

int Settings::getMix() {
    return mix;
}

void Settings::setMix(int mix) {
    Settings::mix = mix;
    updateNeeded = true;
}

std::vector<Effect> Settings::getEffects() {
    return effects;
}

void Settings::setEffects(std::vector<Effect> effects) {
    Settings::effects = effects;
    updateNeeded = true;
}

std::vector<Preset>& Settings::getEffectPresets() {
    return effectPresets;
}

void Settings::setEffectPresets(std::vector<Preset>& presets) {
    Settings::effectPresets = presets;
    updateNeeded = true;
}

void Settings::addEffectPreset(Preset preset) {
    effectPresets.push_back(preset);
    updateNeeded = true;
}

void Settings::removeEffectPreset(int id) {
    Serial.println("Removing preset " + String(id));
    for (int i = 0; i < effectPresets.size(); i++) {
        if (effectPresets[i].getId() == id) {
            Serial.println("actually Removing preset " + String(id));
            effectPresets.erase(effectPresets.begin() + i);
            break;
        }
    }
    updateNeeded = true;
}

int Settings::getNextPresetId() {
    return nextPresetId;
}

void Settings::setNextPresetId(int id) {
    setInt("nextPresetId", id);
}


// getters and setters internal use


// "saved" ones are the ones that are saved in flash
// as opposed to the ones in memory

int Settings::getSavedVolume() {
    return getInt("volume");
}

int Settings::getSavedMix() {
    return getInt("mix");
}

std::vector<Effect> Settings::getSavedEffects() {
    std::vector<Effect> effectsOutput;

    JsonDocument doc;
    getJson("effects", doc);
    
    JsonArray effectsArray = doc["effects"].as<JsonArray>();
    // loop through each effect
    for (JsonArray::iterator effectIter = effectsArray.begin(); effectIter != effectsArray.end(); ++effectIter) {
        Effect effect = Effect::fromJson(*effectIter);
        effectsOutput.push_back(effect);
    }

    return effectsOutput;
}

std::vector<Preset>& Settings::getSavedEffectPresets() {
    std::vector<Preset>& presets = *new std::vector<Preset>();

    JsonDocument doc;
    getJson("presets", doc);

    JsonArray presetsArray = doc["presets"].as<JsonArray>();

    // loop through each preset
    for (JsonArray::iterator presetIter = presetsArray.begin(); presetIter != presetsArray.end(); ++presetIter) {
        Preset preset = Preset::fromJson(*presetIter);
        presets.push_back(preset);
    }

    return presets;
}

int Settings::getSavedNextPresetId() {
    return getInt("nextPresetId");
}

void Settings::setString(const char* key, const char* value) {
    settings.putString(key, value);
}
void Settings::setInt(const char* key, int value) {
    settings.putInt(key, value);
}
void Settings::setFloat(const char* key, float value) {
    settings.putFloat(key, value);
}
void Settings::setBool(const char* key, bool value) {
    settings.putBool(key, value);
}
void Settings::setJson(const char* key, JsonDocument& json) {
    String jsonString;
    serializeJson(json, jsonString);
    settings.putString(key, jsonString);
}

const char* Settings::getString(const char* key) {
    return settings.getString(key).c_str();
}
int Settings::getInt(const char* key) {
    return settings.getInt(key);
}
float Settings::getFloat(const char* key) {
    return settings.getFloat(key);
}
bool Settings::getBool(const char* key) {
    return settings.getBool(key);
}
void Settings::getJson(const char* key, JsonDocument& doc) {
    String jsonString = settings.getString(key);

    deserializeJson(doc, jsonString);
}