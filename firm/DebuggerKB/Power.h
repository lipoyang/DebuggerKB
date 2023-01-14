#pragma once

#include <stdbool.h>
#include "PollingTimer.h"

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
    // check Battery Voltage (Low Battery -> Reset)
    void checkVbat();

private:
    int m_pinLedPower;       // LED Power ON pin
    int m_lowBatCnt;         // Low Battery Count
    IntervalTimer timerVbat; // Interval Timer for Vbat
};
