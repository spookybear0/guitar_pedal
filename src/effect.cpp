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

Effect Effect::fromJson(const JsonObject& json) {
    Effect effect;

    effect.id = json["id"];
    effect.params[0] = json["params"][0];
    effect.params[1] = json["params"][1];
    effect.params[2] = json["params"][2];
    return effect;
}

const char* Effect::getName() {
    return effectNames[id];
}

const char* Effect::getParamName(int index) {
    return effectParamNames[id][index];
}

JsonDocument& Effect::toJson() {
    static DynamicJsonDocument doc(1024);
    doc["id"] = id;
    doc["params"][0] = params[0];
    doc["params"][1] = params[1];
    doc["params"][2] = params[2];
    return doc;
}