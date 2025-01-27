#include "settings.h"
#include <Preferences.h>
#include <ArduinoJson.h>

Settings::Settings() {
    begin();
}

void Settings::begin() {
    settings.begin("settings", false);

    bool firstRun = settings.getBool("firstRun", true);

    if (firstRun) {
        // factory defaults
    }
}

void Settings::reset() {
    settings.clear();

    setBool("firstRun", true);

    begin();
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
JsonDocument Settings::getJson(const char* key) {
    String jsonString = settings.getString(key);
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, jsonString);
    return doc;
}