#pragma once
#include <NimBLEDevice.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <string>

// abstract bluetooth class for handling bluetooth
// and using json to communicate with the app
// after it recieves a request, it then sends it to the appropriate handler
class Bluetooth {
    public:
        Bluetooth();
        void start();
        void stop();
        void sendJson(JsonObject& json);

    private:
        NimBLEServer* pServer;
        NimBLEService* pService;
        NimBLECharacteristic* pCharacteristic;


        const char* deviceName = "ESP32";
        const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
        const char* CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";


        class Callbacks : public NimBLECharacteristicCallbacks {
            void onWrite(NimBLECharacteristic* pCharacteristic);
        };

        Callbacks* pCallbacks;

};