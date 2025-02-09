#pragma once
#include <map>
#include <string>
#include <ArduinoJson.h>
#include <functional>

void effects_get(JsonDocument& json, JsonDocument& response);
void effects_set(JsonDocument& json, JsonDocument& response);
void ping(JsonDocument& json, JsonDocument& response);
void presets_add(JsonDocument& json, JsonDocument& response);
void presets_edit(JsonDocument& json, JsonDocument& response);
void presets_get(JsonDocument& json, JsonDocument& response);
void presets_remove(JsonDocument& json, JsonDocument& response);
void single_effect_set(JsonDocument& json, JsonDocument& response);

// {path: handler}

std::map<std::string, std::function<void(JsonDocument&, JsonDocument&)>> handlers = {
    {"/effects/get", effects_get},
    {"/effects/set", effects_set},
    {"/ping", ping},
    {"/presets/add", presets_add},
    {"/presets/edit", presets_edit},
    {"/presets/get", presets_get},
    {"/presets/remove", presets_remove},
    {"/single_effect/set", single_effect_set},
};