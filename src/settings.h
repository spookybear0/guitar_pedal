#pragma once

#include <Preferences.h>
#include <ArduinoJson.h>
#include "effect.h"
#include <vector>

class Settings {
    public:
        static void begin();
        static void reset();
        static void saveValues();

        static int getVolume();
        static void setVolume(int volume);

        static int getMix();
        static void setMix(int mix);

        static std::vector<Effect> getEffects(); // size 8 ONLY
        static void setEffects(std::vector<Effect> effects); // size 8 ONLY

        static std::vector<std::vector<Effect>> getEffectPresets();
        static void setEffectPresets(std::vector<std::vector<Effect>>& presets);
    private:
        // values to save so we don't save to flash constantly
        static bool updateNeeded;// = false;

        static int volume;
        static int mix;
        static std::vector<Effect> effects;
        static std::vector<std::vector<Effect>> effectPresets;

        // getters and setters internal use

        static int getSavedVolume();
        static int getSavedMix();
        static std::vector<Effect> getSavedEffects();
        static std::vector<std::vector<Effect>> getSavedEffectPresets();

        static void setString(const char* key, const char* value);
        static void setInt(const char* key, int value);
        static void setFloat(const char* key, float value);
        static void setBool(const char* key, bool value);
        static void setJson(const char* key, JsonDocument& json);

        static const char* getString(const char* key);
        static int getInt(const char* key);
        static float getFloat(const char* key);
        static bool getBool(const char* key);
        static void getJson(const char* key, JsonDocument& doc);

        static Preferences settings;
};