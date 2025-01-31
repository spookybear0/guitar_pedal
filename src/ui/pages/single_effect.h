#pragma once

#include "ui/page.h"

enum EditMode {
    EFFECT, // choosing effect
    PARAM, // choosing parameter
    VALUE, // choosing value for parameter
};

class SingleEffectPage : public Page {
private:
    int currentEffect = 0;
    int currentParam = -1;
    EditMode editMode = EditMode::EFFECT;
    
public:
    void draw() override;
    void handleEvent(const Event& event) override;
    void enter() override;
};