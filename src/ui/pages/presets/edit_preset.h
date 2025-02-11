#pragma once

#include "ui/page.h"
#include "../../../preset.h"

// duplicate from ui/pages/single_effect.h
enum EditPresetMode {
    EFFECT,
    PARAM,
    VALUE
};

class EditPresetPage : public Page {
private:
    int currentEffect = 0; // this is the index of the effect in the preset
    int currentParam = 0;
    EditPresetMode editMode = EditPresetMode::EFFECT;
    Preset* preset;
public:
    void draw() override;
    void handleEvent(const Event& event) override;
    void enter() override;

    void setPreset(Preset& preset);
};