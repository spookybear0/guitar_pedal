#include <ArduinoJson.h>

void ping(JsonDocument& json, JsonDocument& response) {
    Serial.println("ping handler called");

    // create response
    response["status"] = "success";
    response["data"] = "pong";
}