#pragma once

#include <stdbool.h>

// Power Class
class Power
{
public:
    // initialize
    void begin(int pinLedPower);
    // LED Power ON
    void turnOnLed();
    // LED Power OFF
    void turnOffLed();
    // detect USB Vbus
    bool detectVbus();
    // monitor Battery Voltage [mV]
    int getVbat();

private:
    int m_pinLedPower; // LED Power ON pin
};
