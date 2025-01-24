#pragma once
#include <Preferences.h>
#include <ArduinoJson.h>
#include <string>
#include <BluetoothSerial.h>

class Bluetooth {
    public:
        Bluetooth();
        void start();
        void stop();
        void sendJson(JsonDocument& json);
        // function to be scheduled to be called every 10ms to recieive data from bluetooth and call the appropriate handler
        void receiveData();
    private:
        BluetoothSerial SerialBT;
        const char* deviceName = "ESP32";
};