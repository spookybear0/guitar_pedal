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
        void sendJson(JsonDocument& json);

    private:
        NimBLEServer* pServer;
        NimBLEService* pService;
        NimBLECharacteristic* pCharacteristic;


        const char* deviceName = "Vortex Pedal";
        const char* SERVICE_UUID = "3486c460-db62-11ef-a8fa-0800200c9a66";
        const char* CHARACTERISTIC_UUID = "3a4bf870-db62-11ef-a8fa-0800200c9a66";


        class ServerCallbacks : public NimBLEServerCallbacks {
            public:
                ServerCallbacks(Bluetooth* bluetooth) : bluetooth(bluetooth) {}
                void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo);
                void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason);
            private:
                Bluetooth* bluetooth;
        };

        class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
            public:
                CharacteristicCallbacks(Bluetooth* bluetooth) : bluetooth(bluetooth) {}
                void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
                void onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo* connInfo);
            private:
                Bluetooth* bluetooth;
        };

        CharacteristicCallbacks* pCharacteristicCallbacks;
        ServerCallbacks* pServerCallbacks;

};