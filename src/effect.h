#pragma once
#include <ArduinoJson.h>

// default values
extern const char* effectNames[8];
extern const char* effectParamNames[8][3];
extern int effectParamValues[8][3];

class Effect {
public:
    // TODO: maybe store volume?

    Effect(int id);
    Effect(const char* name);

    static Effect fromJson(const JsonObject& json);

    int id = 0;
    int params[3] = {0, 0, 0};
    // stored for presets. these values are *averaged* with the overall settings
    int volume = 255;
    int mix = 255;

    const char* getName();
    const char* getParamName(int index);

    void toJson(JsonDocument& doc, bool full = false);
private:
    Effect();
};