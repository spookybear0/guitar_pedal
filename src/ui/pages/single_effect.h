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
    int initalEffect = -1; // used to set the effect when entering the page
    
public:
    void draw() override;
    void handleEvent(const Event& event) override;
    void enter() override;

    void setInitialEffect(int effect);
    int getEffect();
};