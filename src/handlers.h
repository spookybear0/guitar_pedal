#pragma once
#include <map>
#include <string>
#include <ArduinoJson.h>

// {path: handler}

std::map<std::string, void (*)(JsonDocument&)> handlers = {
    {"hello", [](JsonDocument& json) {
        Serial.println("Hello");
    }},
    {"world", [](JsonDocument& json) {
        Serial.println("World");
    }}
};