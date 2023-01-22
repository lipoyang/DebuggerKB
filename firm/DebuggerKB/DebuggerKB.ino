#include <Adafruit_TinyUSB.h>
#include <Adafruit_NeoPixel.h>
#include "UsbKeyboard.h"
#include "BleKeyboard.h"
#include "SerialCommand.h"
#include "KeyMatrix.h"
#include "Power.h"
#include "Switch.h"
#include "ColorLed.h"
#include "PollingTimer.h"

#define PIN_PAGE_SW      9  // Page Swith pin
#define PIN_LED_POWER    7  // LED Power ON pin
#define PIN_PAGE_LED    10  // LED pin

#define KEY_INTERVAL     5  // Key Interval [msec]

// Page Switch
Switch pageSwitch;
// Page LED
ColorLed pageLed(PIN_PAGE_LED);
// Power Controller
Power power;

// USB Keyboard
UsbKeyboard usbKeyboard;
// BLE Keyboard
BleKeyboard bleKeyboard;
// USB or BLE Keyboard
Keyboard *keyboard;

// Key Matrix
KeyMatrix keyMatrix;
// Serial Command
SerialCommand serialCommand;
// Keymap Storage
KeyMapStorage keyMapStorage;

// Output Pins (for Key Matrix)
int outPin[ROW_NUM] = { 0, 1 };
// Input Pins  (for Key Matrix)
int inPin [COL_NUM] = { 2, 3, 4, 5, 8 };
// Keycode 
uint8_t keyTable[KEY_MAX][KEY_COMBI_MAX];

// is USB connected?
bool isUsbConnected;

// Interval Timer
IntervalTimer interval;

// Initialize
void setup()
{
    // Power
    power.begin(PIN_LED_POWER, PIN_PAGE_SW);
    
    // Page Switch
    pageSwitch.begin(PIN_PAGE_SW);
    
    // Page LED
    if (power.detectVbus()) {
        pageLed.begin(LED_BUS_POWERED);
    } else {
        pageLed.begin(LED_BAT_POWERED);
    }
    
    // detect VBUS
    if (power.detectVbus()) {
        // USB Keyboard
        digitalWrite(LED_GREEN, LOW);
        keyboard = &usbKeyboard;
        isUsbConnected = true;
    } else {
        // BLE Keyboard
        digitalWrite(LED_BLUE,  LOW);
        keyboard = &bleKeyboard;
        isUsbConnected = false;
    }

    // begin HID (USB of BLE)
    keyboard->begin();
    
    // begin Serial Command
    serialCommand.keyMaps = keyMapStorage.keyMaps;
    if(isUsbConnected) serialCommand.begin();
    
    // Keymap Strage
    keyMapStorage.begin();
    keyMapStorage.getKeyTable(keyTable);
    
    // begin Key Matrix
    keyMatrix.begin(outPin, inPin);
    keyMatrix.setKeyTable(keyTable);

    // show Page LED
    pageLed.setColor(keyMapStorage.getLedColor());
    
    // begin Interval Timer
    interval.set(KEY_INTERVAL);
}

// Main Loop
void loop()
{
    // key interval
    if(!interval.elapsed()) return;

    // page LED control
    pageLed.task();
    
    // check low battery voltage
    bool lowBattery = power.checkLowBattery();
    // check no operation
    bool noOperation = power.checkNoOperation();
    // sleep?
    if(lowBattery || noOperation){
        pageLed.turnOff();
        power.sleep();
    }
    
    // serial command task
    if(isUsbConnected){
        int ret = serialCommand.task();
        if(ret == RET_WRITE){
            keyMapStorage.save();
            keyMapStorage.changePage(false);
            keyMapStorage.getKeyTable(keyTable);
            keyMatrix.setKeyTable(keyTable);
            
            pageLed.setColor(keyMapStorage.getLedColor());
        }
    }
    
    // get page switch event
    int event = pageSwitch.get();
    if(event == SW_EVENT_SHORT){
        keyMapStorage.changePage(true);
        keyMapStorage.getKeyTable(keyTable);
        keyMatrix.setKeyTable(keyTable);
        
        pageLed.setColor(keyMapStorage.getLedColor());
        power.kick();
    }
    else if(event == SW_EVENT_LONG){
        pageLed.turnOnWhile();
        power.kick();
    }

    // skip if hid is not ready
    if ( !keyboard->ready() ) return;

    // scan key matrix
    if(keyMatrix.scan()){
        // press any key?
        if(keyMatrix.pressAnyKey()){
            // wake up
            keyboard->wakeup();
            // get key code
            uint8_t keycode[KEY_CODE_MAX] = { 0 };
            keyMatrix.getKey(keycode);
            
            // keyboard report
            if(keycode[0] != 0){
                keyboard->keyboardReport(keycode);
                
                pageLed.turnOnMoment();
                power.kick();
            }
        }else{
            // key release
            keyboard->keyboardRelease();
        }
    }
}
