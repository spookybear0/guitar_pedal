#pragma once
#include <map>
#include <string>
#include <ArduinoJson.h>
#include <functional>

JsonDocument& ping(JsonDocument& json);

// {path: handler}

std::map<std::string, std::function<JsonDocument&(JsonDocument&)>> handlers = {
    {"ping", ping}
};