#pragma once

#include "ui/page.h"

class PresetsPage : public Page {
private:
    int currentMenuItem = 0;
public:
    void draw() override;
    void handleEvent(const Event& event) override;
    void enter() override;
};