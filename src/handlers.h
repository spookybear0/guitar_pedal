#pragma once
#include <map>
#include <string>
#include <ArduinoJson.h>
#include "handlers/ping.h"
#include <functional>

// {path: handler}

std::map<std::string, std::function<JsonDocument&(JsonDocument&)>> handlers = {
    {"ping", ping}
};