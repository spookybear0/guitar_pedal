#pragma once
#include <map>
#include <string>
#include <ArduinoJson.h>
#include <functional>

void effects_get(JsonDocument& json, JsonDocument& response);
void ping(JsonDocument& json, JsonDocument& response);

// {path: handler}

std::map<std::string, std::function<void(JsonDocument&, JsonDocument&)>> handlers = {
    {"effects_get", effects_get},
    {"ping", ping},
};