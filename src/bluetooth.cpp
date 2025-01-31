#include "bluetooth.h"
#include <NimBLEDevice.h>
#include "handlers.h"

Bluetooth::Bluetooth() {
    // do nothing
}

void Bluetooth::start() {
    // start bluetooth server and check for errors

    NimBLEDevice::init(deviceName);

    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks(this));

    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::WRITE);
    pCharacteristic->setCallbacks(new CharacteristicCallbacks(this));
    pCharacteristic->setValue("Hello World");
    pService->start();


    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->setName(deviceName);
    pAdvertising->addServiceUUID(SERVICE_UUID);

    pAdvertising->enableScanResponse(true);
    pAdvertising->start();
    Serial.println("Bluetooth started");
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

void Bluetooth::CharacteristicCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    Serial.println("Characteristic written");
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

    // get request id
    int requestId = doc["requestId"];

    // get the handler

    if (handlers.find(path) != handlers.end()) {
        // call the handler
        JsonDocument response;
        handlers[path](doc, response);

        // add the request id
        response["requestId"] = requestId;

        bluetooth->sendJson(response);
    } else {
        Serial.println("Handler not found");
        // TODO: handle error
    }
}

void Bluetooth::CharacteristicCallbacks::onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo* connInfo) {
    Serial.println("Characteristic read");
    std::string value = pCharacteristic->getValue();
    Serial.println(value.c_str());
}

void Bluetooth::ServerCallbacks::onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) {
    Serial.println("Client connected");
    pServer->updateConnParams(connInfo.getConnHandle(), 100, 100, 0, 600);
}

void Bluetooth::ServerCallbacks::onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
    Serial.println("Client disconnected");

    // restart advertising
    bluetooth->pServer->startAdvertising();
}