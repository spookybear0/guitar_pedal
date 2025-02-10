#pragma once

#include "ui/page.h"

class MainMenuPage : public Page {
private:
    const char* menuItems[3] = {"Single Effect", "Presets", "Settings"};
    int currentMenuItem = 0;
public:
    void draw() override;
    void handleEvent(const Event& event) override;
};