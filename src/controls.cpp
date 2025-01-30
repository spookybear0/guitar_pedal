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

void Controls::setPot(int pin, unsigned char value) {
    if (pin == -1) {
        return;
    }
    digitalWrite(pin, LOW); // enable the pot
    SPI.transfer(value);
    digitalWrite(pin, HIGH); // disable the pot
}

void Controls::setParam1(unsigned char value) {
    setPot(PARAM1_PIN, value);
    param1Value = value;
}

void Controls::setParam2(unsigned char value) {
    setPot(PARAM2_PIN, value);
    param2Value = value;
}

void Controls::setParam3(unsigned char value) {
    setPot(PARAM3_PIN, value);
    param3Value = value;
}

void Controls::setVolume(unsigned char value) {
    setPot(VOLUME_PIN, value);
    volumeValue = value;
}

void Controls::setMix(unsigned char value) {
    setPot(MIX_PIN, value);
    mixValue = value;
}