#pragma once

#include <stdbool.h>

// Power Class
class Power
{
public:
    // initialize
    void begin(int pinLedPower);
    // detect USB Vbus
    bool detectVbus();

private:
    int m_pinLedPower; // LED Power ON pin
};
