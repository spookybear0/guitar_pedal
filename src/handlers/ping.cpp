#include "ping.h"
#include <ArduinoJson.h>

JsonDocument& ping(JsonDocument& json) {
    Serial.println("ping handler called");

    // create response
    static DynamicJsonDocument response(1024);
    response["status"] = "success";
    response["data"] = "pong";
    
    return response;
}