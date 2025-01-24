#include "bluetooth.h"
#include "handlers.h"

Bluetooth::Bluetooth() {
    // do nothing
}

void Bluetooth::start() {
    SerialBT.begin(deviceName);

    Serial.println("The device started, now you can pair it with bluetooth!");
}

void Bluetooth::stop() {
    SerialBT.end();
}

void Bluetooth::sendJson(JsonDocument& json) {
    String jsonString;
    serializeJson(json, jsonString);
    SerialBT.println(jsonString);
}

void Bluetooth::receiveData() {
    if (SerialBT.available()) {
        String data = SerialBT.readStringUntil('\n');
        Serial.println(data);
        // parse json
        JsonDocument doc;

        DeserializationError error = deserializeJson(doc, data.c_str());

        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }

        // get the path
        std::string path = doc["path"];

        // get the handler

        if (handlers.find(path) != handlers.end()) {
            // call the handler
            JsonDocument& response = handlers[path](doc);
            sendJson(response);
        } else {
            Serial.println("Handler not found");
            // TODO: handle
        }
    }
}