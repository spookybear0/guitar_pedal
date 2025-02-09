#pragma once

#include <Preferences.h>
#include <ArduinoJson.h>
#include "effect.h"
#include <vector>
#include "preset.h"

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

        static std::vector<Preset>& getEffectPresets();
        static void setEffectPresets(std::vector<Preset>& presets);

        static void addEffectPreset(Preset preset);
        static void removeEffectPreset(int id);

        static int getNextPresetId();
        static void setNextPresetId(int id);
    private:
        // values to save so we don't save to flash constantly
        static bool updateNeeded;// = false;

        static int volume;
        static int mix;
        static std::vector<Effect> effects;
        static std::vector<Preset>& effectPresets;
        static int nextPresetId;

        // getters and setters internal use

        static int getSavedVolume();
        static int getSavedMix();
        static std::vector<Effect> getSavedEffects();
        static std::vector<Preset>& getSavedEffectPresets();
        static int getSavedNextPresetId();


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