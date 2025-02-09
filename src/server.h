#pragma once
#include <WebServer.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <string>

// abstract server class for handling server
// and using json to communicate with the app
// after it recieves a request, it then sends it to the appropriate handler
class PedalServer {
private:
    static WebServer server;
public:
    static const char* wifiName;

    static void start();
    static void handleClient();
    static void stop();
};