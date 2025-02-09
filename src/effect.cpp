#include "effect.h"
#include <ArduinoJson.h>

// default names/values for effects
// configured by what's on the 

const char* effectNames[8] = {"Effect 1", "Effect 2", "Effect 3", "Effect 4", "Effect 5", "Effect 6", "Effect 7", "Effect 8"};

const char* effectParamNames[8][3] = {
    "Param 1", "Param 2", "Param 3",
    "Param 1", "Param 2", "Param 3",
    "Param 1", "Param 2", "Param 3",
    "Param 1", "Param 2", "Param 3",
    "Param 1", "Param 2", "Param 3",
    "Param 1", "Param 2", "Param 3",
    "Param 1", "Param 2", "Param 3",
    "Param 1", "Param 2", "Param 3",
};

int effectParamValues[8][3] = {
    {100, 150, 0},
    {0, 50, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
};

Effect::Effect() {}

Effect::Effect(int id) {
    this->id = id;
    params[0] = effectParamValues[id][0];
    params[1] = effectParamValues[id][1];
    params[2] = effectParamValues[id][2];
}

Effect::Effect(const char* name) {
    for (int i = 0; i < 8; i++) {
        if (strcmp(name, effectNames[i]) == 0) {
            id = i;
            params[0] = effectParamValues[i][0];
            params[1] = effectParamValues[i][1];
            params[2] = effectParamValues[i][2];
            break;
        }
    }
}

Effect Effect::fromJson(const JsonObject& json) {
    Effect effect;

    effect.id = json["id"];
    effect.params[0] = json["params"][0];
    effect.params[1] = json["params"][1];
    effect.params[2] = json["params"][2];
    effect.volume = json["volume"];
    effect.mix = json["mix"];

    return effect;
}

const char* Effect::getName() {
    return effectNames[id];
}

const char* Effect::getParamName(int index) {
    return effectParamNames[id][index];
}

void Effect::toJson(JsonDocument& doc, bool full) {
    doc["id"] = id;
    doc["params"][0] = params[0];
    doc["params"][1] = params[1];
    doc["params"][2] = params[2];
    doc["volume"] = volume;
    doc["mix"] = mix;
    if (full) {
        doc["name"] = getName();
        doc["paramNames"][0] = getParamName(0);
        doc["paramNames"][1] = getParamName(1);
        doc["paramNames"][2] = getParamName(2);
    }
}