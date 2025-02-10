#pragma once
#include <effect.h>
#include <ArduinoJson.h>
#include <vector>

// preset that includes an ordered list of effects (which contains the params, volume, and mix)

class Preset {
public:
    Preset();
    Preset(std::vector<Effect> effects);
    Preset(std::string name, std::vector<Effect> effects);

    static Preset fromJson(const JsonObject& json);

    std::string getName();
    void setName(std::string name);

    int getId();
    void setId(int id);

    std::vector<Effect> effects;

    void toJson(JsonDocument& doc, bool full = false);
private:
    int id;
    std::string name;
};