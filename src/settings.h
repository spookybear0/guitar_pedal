#pragma once

#include <Preferences.h>
#include <ArduinoJson.h>

class Settings {
    public:
        Settings();
        void reset();

        void setString(const char* key, const char* value);
        void setInt(const char* key, int value);
        void setFloat(const char* key, float value);
        void setBool(const char* key, bool value);
        void setJson(const char* key, JsonDocument& json);

        const char* getString(const char* key);
        int getInt(const char* key);
        float getFloat(const char* key);
        bool getBool(const char* key);
        const char* getArray(const char* key, int index);
        JsonDocument getJson(const char* key);
    private:
        void begin();

        Preferences settings;
};