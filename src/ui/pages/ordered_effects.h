#pragma once

#include "ui/page.h"

class OrderedEffectsPage : public Page {
private:
public:
    void draw() override;
    void handleEvent(const Event& event) override;
    void enter() override;
};