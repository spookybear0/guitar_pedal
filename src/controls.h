#pragma once
#include <SPI.h>

// control 5 different analog potentiometers, param1, param2, param3, volume and mix


// chip select (CS) pin for the digital potentiometers
#define MIX_PIN -1 // 10K potentiometer
#define VOLUME_PIN -1 // 100K potentiometer
#define PARAM1_PIN -1 // 100K potentiometer
#define PARAM2_PIN -1 // 100K potentiometer
#define PARAM3_PIN -1 // 100K potentiometer

// effect selector demux pins 3 to 8
#define DEMUX1_PIN -1
#define DEMUX2_PIN -1
#define DEMUX3_PIN -1

// 0-255
class Controls {
public:
    // to use: set the values of the pots and then call update()
    // when after done updating UI to prevent SPI bus contention

    static void init(unsigned char mix, unsigned char volume, unsigned char param1, unsigned char param2, unsigned char param3);
    static void setMix(unsigned char value);
    static void setVolume(unsigned char value);
    static void setParam1(unsigned char value);
    static void setParam2(unsigned char value);
    static void setParam3(unsigned char value);
    static void setAllPots(unsigned char param1, unsigned char param2, unsigned char param3, unsigned char volume, unsigned char mix);
    static void setAllPots(unsigned char value); // set all pots to the same value
    static void setEffect(unsigned char effect); // (0-7) set the effect using the demux pins
    static void update(); // update the pots with the current values in the class
private:
    static bool updateNeeded;

    static unsigned char mixValue;
    static unsigned char volumeValue;
    static unsigned char param1Value;
    static unsigned char param2Value;
    static unsigned char param3Value;

    static unsigned char effectValue;

    static void setPot(int pin, unsigned char value);
};