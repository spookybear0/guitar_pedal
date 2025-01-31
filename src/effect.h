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
    static Effect fromJson(const JsonObject& json);

    int id = 0;
    int params[3] = {0, 0, 0};

    const char* getName();
    const char* getParamName(int index);

    JsonDocument& toJson(bool full = false);
private:
    Effect();
};