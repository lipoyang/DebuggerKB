#include <Arduino.h>
#include "Switch.h"

// Time threshold
#define T1  20      // short push
#define T2  1000    // long push

// initialize
void Switch::begin(int pin)
{
    m_pin = pin;
    m_state = HIGH;
    
    pinMode(m_pin, INPUT_PULLUP);
    delay(100);
}

// get switch event
int Switch::get()
{
    int state = digitalRead(m_pin);
    
    if(m_state == HIGH){
        if(state == LOW){
            m_state = LOW;
            m_t0 = millis();
        }
    }else{
        if(state == HIGH){
            m_state = HIGH;
            uint32_t now = millis();
            uint32_t elapsed = now - m_t0;
            if(elapsed >= T2){
                return SW_EVENT_LONG;
            }else if(elapsed >= T1){
                return SW_EVENT_SHORT;
            }
        }
    }
    return SW_EVENT_NONE;
}
