#pragma once

#include "ui/page.h"

class PresetListPage : public Page {
private:
    int currentMenuItem = 0;
public:
    void draw() override;
    void handleEvent(const Event& event) override;
    void enter() override;
};