#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

// first time?
static bool initFlag = true;

// detect USB Vbus
bool VbusDetect()
{
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

