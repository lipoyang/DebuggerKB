#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "Power.h"

// PIN_VBAT     32     D32 = P0.31 (AIN7_BAT)
// VBAT_ENABLE  14     D14 = P0.14 (READ_BAT) 
#define HICHG   22  // D22 = P0.13 (BQ25100 ISET)

#define VBAT_INTERVAL   1000    // Vbat Interval [msec]
#define LOW_BATT_TIME   5       // Low Battery time [sec]
#define LOW_BATT_MV     3500    // Low Battery threshold [mV]

// initialize
void Power::begin(int pinLedPower)
{
    // Debug Serial (TxD only)
    Serial1.begin(115200);
    NRF_UARTE0->PSEL.RXD = 0xFFFFFFFF; // Disable RxD
    
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

    // initialize to monitor Battery Voltage
    analogReference(AR_INTERNAL_2_4); // VREF = 2.4V
    analogReadResolution(10);         // 10bit A/D
    pinMode(VBAT_ENABLE, OUTPUT);     // READ_BAT pin = LOW
    digitalWrite(VBAT_ENABLE, LOW);
    
    // begin Interval Timer
    timerVbat.set(VBAT_INTERVAL);
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

// monitor Battery Voltage [mV]
int Power::getVbat()
{
    int vbat_raw = analogRead(PIN_VBAT);
    int vbat_mv = vbat_raw * 2400 / 1023; // VREF = 2.4V, 10bit A/D
    vbat_mv = vbat_mv * 1510 / 510;       // 1M + 510k / 510k
    
    return vbat_mv;
}

// check Battery Voltage (Low Battery -> Reset)
void Power::checkVbat()
{
    // Vbat interval
    if(!timerVbat.elapsed()) return;
    
    int vbat = this->getVbat();
    Serial1.print("Vbat = "); Serial1.println(vbat);

    // Battery powered
    if(!this->detectVbus())
    {
        if(vbat < LOW_BATT_MV){
            m_lowBatCnt++;
            if(m_lowBatCnt >= LOW_BATT_TIME){
                // Serial1.println("Reset!");
                // delay(1000);
                NVIC_SystemReset(); // System Reset
            }
        }else{
            m_lowBatCnt = 0;
        }
    }
}
