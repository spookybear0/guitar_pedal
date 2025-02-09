#include "server.h"
#include <WebServer.h>
#include "handlers.h"
#include <ArduinoJson.h>

const char* PedalServer::wifiName = "Vortex Pedal";
WebServer PedalServer::server(80);

void PedalServer::start() {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
    WiFi.softAP(wifiName);

    for (auto& handler : handlers) {
        server.on(handler.first.c_str(), HTTP_POST, [&handler]() {
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, server.arg("plain"));
            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                server.send(400, "application/json", "{\"error\": \"Invalid JSON\"}");
                return;
            }

            Serial.println("request: " + doc.as<String>());

            JsonDocument response;
            handler.second(doc, response);

            String responseString;
            serializeJson(response, responseString);

            Serial.println("response: " + responseString);

            server.send(200, "application/json", responseString);
        });
    }

    server.onNotFound([]() {
        server.send(404, "application/json", "{\"status\": \"error\", \"error\": \"Not found\"}");
    });

    server.begin();
}

void PedalServer::handleClient() {
    server.handleClient();
}

void PedalServer::stop() {
    WiFi.softAPdisconnect(true);
}