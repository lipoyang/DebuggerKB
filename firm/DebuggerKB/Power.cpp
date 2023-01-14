#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "Power.h"

// initialize
void Power::begin(int pinLedPower)
{
    // Power ON for LED
    m_pinLedPower = pinLedPower;
    this->turnOnLed();
    
    // on-board LEDs
    pinMode(LED_RED,    OUTPUT);
    pinMode(LED_GREEN,  OUTPUT);
    pinMode(LED_BLUE,   OUTPUT);
    digitalWrite(LED_RED,   HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE,  HIGH);
}

// LED Power ON
void Power::turnOnLed()
{
    pinMode(m_pinLedPower, OUTPUT);
    digitalWrite(m_pinLedPower, LOW);
}

// LED Power OFF
void Power::turnOffLed()
{
    pinMode(m_pinLedPower, INPUT);  
}

// detect USB Vbus
bool Power::detectVbus()
{
    // first time?
    static bool initFlag = true;
    
    // wait for VBUS detect (?)
    if(initFlag){
        initFlag = false;
        delay(100);
    }
    
    // detect Vbus
    uint32_t usb_reg = NRF_POWER->USBREGSTATUS;
    if (usb_reg & POWER_USBREGSTATUS_VBUSDETECT_Msk){
        return true;
    }else{
        return false;
    }
}
