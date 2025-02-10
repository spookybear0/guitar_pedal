#pragma once

#include "ui/page.h"
#include "../../../preset.h"

class PresetPage : public Page {
private:
    int currentEffect = 0; // this is the index of the effect in the preset
    Preset preset = Preset();
public:
    void draw() override;
    void handleEvent(const Event& event) override;
    void enter() override;

    void setPreset(Preset preset);
};