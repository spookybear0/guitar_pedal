#include "controls.h"
#include <SPI.h>
#include <Arduino.h>

void Controls::init(unsigned char param1, unsigned char param2, unsigned char param3, unsigned char volume, unsigned char mix) {
    // initialize the digital potentiometers
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
    if (VOLUME_PIN != -1) {
        pinMode(VOLUME_PIN, OUTPUT);
        setVolume(volume);
    }
    if (MIX_PIN != -1) {
        pinMode(MIX_PIN, OUTPUT);
        setMix(mix);
    }
}

void Controls::update() {
    // actually physically update the pots
    if (updateNeeded) { // optimization
        setPot(PARAM1_PIN, param1Value);
        setPot(PARAM2_PIN, param2Value);
        setPot(PARAM3_PIN, param3Value);
        setPot(VOLUME_PIN, volumeValue);
        setPot(MIX_PIN, mixValue);
        updateNeeded = false;
    }
}

void Controls::setAll(unsigned char param1, unsigned char param2, unsigned char param3, unsigned char volume, unsigned char mix) {
    setParam1(param1);
    setParam2(param2);
    setParam3(param3);
    setVolume(volume);
    setMix(mix);
    updateNeeded = true; // signal that the pots need to be updated using update()
}

void Controls::setAll(unsigned char value) {
    setParam1(value);
    setParam2(value);
    setParam3(value);
    setVolume(value);
    setMix(value);
    updateNeeded = true;
}

void Controls::setPot(int pin, unsigned char value) {
    if (pin == -1) {
        return;
    }
    digitalWrite(pin, LOW); // enable the pot
    SPI.transfer(value);
    digitalWrite(pin, HIGH); // disable the pot
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

void Controls::setVolume(unsigned char value) {
    setPot(VOLUME_PIN, value);
    updateNeeded = true;
}

void Controls::setMix(unsigned char value) {
    setPot(MIX_PIN, value);
    updateNeeded = true;
}