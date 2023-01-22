#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <Adafruit_NeoPixel.h>
#include "PollingTimer.h"

#define LED_BAT_POWERED     0
#define LED_BUS_POWERED     1

// Color LED Class
class ColorLed
{
public:
    ColorLed(int pin);
    void begin(int mode);
    void setColor(uint32_t color);
    void turnOff();
    void turnOnWhile();
    void turnOnMoment();
    void endTempOn();
    void task();

private:
    int               m_mode;
    Adafruit_NeoPixel m_neoPixel;
    uint32_t          m_color;
    OneShotTimer      m_timer;
    bool              m_turnedOn;
};
