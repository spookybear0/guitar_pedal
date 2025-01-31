#include "ArduinoJson.h"
#include "settings.h"
// give info about all effects including the name and the paramaters

void effects_get(JsonDocument& json, JsonDocument& response) {
    Serial.println("effects_get handler called");

    std::vector<Effect> effects = Settings::getEffects();

    JsonDocument effectDoc;
    JsonArray effectsArray = effectDoc["effects"].to<JsonArray>();

    for (int i = 0; i < 8; i++) {
        effectsArray.add(effects[i].toJson(true));
    }

    // create response
    response["status"] = "success";
    response["data"] = effectDoc;

    Serial.println(response.as<String>());
}