#include "bluetooth.h"
#include <NimBLEDevice.h>
#include "handlers.h"

Bluetooth::Bluetooth() {
    // do nothing
}

void Bluetooth::start() {
        NimBLEDevice::init(deviceName);
        pServer = NimBLEDevice::createServer();
        pService = pServer->createService(SERVICE_UUID);

        pCharacteristic = pService->createCharacteristic(
                                CHARACTERISTIC_UUID,
                                NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
                            );
        
        pCharacteristic->setCallbacks(new Callbacks(this));
        pService->start();
        
        NimBLEAdvertising* pAdvertising = pServer->getAdvertising();
        pAdvertising->start();
        Serial.println("BLE server started and advertising.");
}

void Bluetooth::stop() {
    NimBLEDevice::deinit(true);
}

void Bluetooth::sendJson(JsonDocument& json) {
    // convert json to string
    String jsonString;
    serializeJson(json, jsonString);
    pCharacteristic->setValue(jsonString.c_str());
    pCharacteristic->notify();
}

void Bluetooth::Callbacks::onWrite(NimBLECharacteristic* pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    Serial.println(value.c_str());

    // parse json
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, value.c_str());

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
        bluetooth->sendJson(response);
    } else {
        Serial.println("Handler not found");
        // TODO: handle
    }
}