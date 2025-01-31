#include "settings.h"
#include <Preferences.h>
#include <ArduinoJson.h>
#include <vector>
#include "effect.h"


Preferences Settings::settings;
bool Settings::updateNeeded = false;
int Settings::volume = 50;
int Settings::mix = 100;
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
std::vector<std::vector<Effect>> Settings::effectPresets = {};

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
    }
}

void Settings::reset() {
    settings.clear();

    setBool("firstRun", false);
    
    setInt("volume", 50);
    setInt("mix", 100);

    // set default effects to default values
    JsonDocument effectsDoc;
    JsonArray effectsArray = effectsDoc["effects"].to<JsonArray>();

    for (int i = 0; i < 8; i++) {
        Effect effect(i);
        effectsArray.add(effect.toJson());
    }

    setJson("effects", effectsDoc);


    setString("presets", "{\"presets\": []}");


    // save values in variables
    volume = getSavedVolume();
    mix = getSavedMix();
    effects = getSavedEffects();
    effectPresets = getSavedEffectPresets();
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
            effectsArray.add(effects[i].toJson());
        }

        setJson("effects", effectDoc);

        // update presets

        JsonDocument presetDoc;
        // save as {"presets": [[Effect, Effect, ...], [Effect, Effect, ...], ...]}

        JsonArray presetsArray = presetDoc["presets"].to<JsonArray>();
        for (auto& preset : effectPresets) {
            JsonArray presetArray = presetsArray.createNestedArray();
            for (auto& effect : preset) {
                presetArray.add(effect.toJson());
            }
        }

        setJson("presets", presetDoc);

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

std::vector<std::vector<Effect>> Settings::getEffectPresets() {
    return effectPresets;
}

void Settings::setEffectPresets(std::vector<std::vector<Effect>>& presets) {
    Settings::effectPresets = presets;
    updateNeeded = true;
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

std::vector<std::vector<Effect>> Settings::getSavedEffectPresets() {
    std::vector<std::vector<Effect>> presets;

    JsonDocument doc;
    getJson("presets", doc);

    JsonArray presetsArray = doc["presets"].as<JsonArray>();

    // loop through each preset
    for (JsonArray::iterator presetIter = presetsArray.begin(); presetIter != presetsArray.end(); ++presetIter) {
        JsonArray presetArray = *presetIter; // get the preset array

        std::vector<Effect> preset; // store effects in preset
        for (JsonArray::iterator effectIter = presetArray.begin(); effectIter != presetArray.end(); ++effectIter) {
            Effect effect = Effect::fromJson(*effectIter);
            preset.push_back(effect);
        }

        presets.push_back(preset); // add preset to presets
    }

    return presets;
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