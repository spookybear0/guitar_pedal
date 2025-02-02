#pragma once

#include "ui/page.h"

class SettingsPage : public Page {
private:
    const char* menuItems[3] = {"Volume", "Mix", "Back"};
    int currentMenuItem = 0;
    bool editMode = false;
public:
    void draw() override;
    void handleEvent(const Event& event) override;
};