#include "event_manager.h"
#include "ui/ui_manager.h"

std::vector<void (*)(const Event& event)> EventManager::callbacks = std::vector<void (*)(const Event& event)>();

void EventManager::handleEvent(const Event& event) {
    for (auto& callback : callbacks) {
        Serial.println("Calling callback");
        callback(event);
    }
}

void EventManager::subscribe(void (*callback)(const Event& event)) {
    callbacks.push_back(callback);
}