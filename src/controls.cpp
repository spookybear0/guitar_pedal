#include "controls.h"
#include <SPI.h>
#include <Arduino.h>

bool Controls::updateNeeded = false;
unsigned char Controls::mixValue = 0;
unsigned char Controls::volumeValue = 0;
unsigned char Controls::param1Value = 0;
unsigned char Controls::param2Value = 0;
unsigned char Controls::param3Value = 0;
unsigned char Controls::effectValue = 0;

void Controls::init(unsigned char mix, unsigned char volume, unsigned char param1, unsigned char param2, unsigned char param3) {
    // initialize the digital potentiometers
    if (MIX_PIN != -1) {
        pinMode(MIX_PIN, OUTPUT);
        setMix(mix);
    }
    if (VOLUME_PIN != -1) {
        pinMode(VOLUME_PIN, OUTPUT);
        setVolume(volume);
    }
    if (PARAM1_PIN != -1) {
        pinMode(PARAM1_PIN, OUTPUT);
        setParam1(param1);
    }
    if (PARAM2_PIN != -1) {
        pinMode(PARAM2_PIN, OUTPUT);
        setParam2(param2);
    }
    if (PARAM3_PIN != -1) {
        pinMode(PARAM3_PIN, OUTPUT);
        setParam3(param3);
    }

    if (DEMUX1_PIN != -1 && DEMUX2_PIN != -1 && DEMUX3_PIN != -1) {
        pinMode(DEMUX1_PIN, OUTPUT);
        pinMode(DEMUX2_PIN, OUTPUT);
        pinMode(DEMUX3_PIN, OUTPUT);
    }
}

void Controls::update() {
    // actually physically update the pots
    if (updateNeeded) { // optimization
        // reset the update flag
        updateNeeded = false;

        // set the demux pins to select the effect (0-7)
        if (DEMUX1_PIN != -1 && DEMUX2_PIN != -1 && DEMUX3_PIN != -1) {
            digitalWrite(DEMUX1_PIN, (effectValue >> 2) & 1);
            digitalWrite(DEMUX2_PIN, (effectValue >> 1) & 1);
            digitalWrite(DEMUX3_PIN, effectValue & 1);
        }

        // pots
        if (MIX_PIN != -1) setPot(MIX_PIN, mixValue);
        if (VOLUME_PIN != -1) setPot(VOLUME_PIN, volumeValue);
        if (PARAM1_PIN != -1) setPot(PARAM1_PIN, param1Value);
        if (PARAM2_PIN != -1) setPot(PARAM2_PIN, param2Value);
        if (PARAM3_PIN != -1) setPot(PARAM3_PIN, param3Value);
    }
}

void Controls::setAllPots(unsigned char param1, unsigned char param2, unsigned char param3, unsigned char volume, unsigned char mix) {
    setMix(mix);
    setVolume(volume);
    setParam1(param1);
    setParam2(param2);
    setParam3(param3);
    updateNeeded = true; // signal that the pots need to be updated using update()
}

void Controls::setAllPots(unsigned char value) {
    setMix(value);
    setVolume(value);
    setParam1(value);
    setParam2(value);
    setParam3(value);
    
    updateNeeded = true;
}

void Controls::setPot(int pin, unsigned char value) {
    if (pin == -1) {
        return;
    }
    digitalWrite(pin, LOW); // enable the pot
    SPI.transfer(0x11); // command to set wiper
    SPI.transfer(value);
    digitalWrite(pin, HIGH); // disable the pot
}


// pot values
void Controls::setMix(unsigned char value) {
    setPot(MIX_PIN, value);
    updateNeeded = true;
}

void Controls::setVolume(unsigned char value) {
    setPot(VOLUME_PIN, value);
    updateNeeded = true;
}

void Controls::setParam1(unsigned char value) {
    param1Value = value;
    updateNeeded = true;
}

void Controls::setParam2(unsigned char value) {
    param2Value = value;
    updateNeeded = true;
}

void Controls::setParam3(unsigned char value) {
    setPot(PARAM3_PIN, value);
    updateNeeded = true;
}

// effect selector (0-7) (demux pins)
void Controls::setEffect(unsigned char effect) {
    effectValue = effect;
    updateNeeded = true;
}