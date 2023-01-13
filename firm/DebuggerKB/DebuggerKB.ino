#include <Adafruit_TinyUSB.h>
#include <Adafruit_NeoPixel.h>
#include "UsbKeyboard.h"
#include "BleKeyboard.h"
#include "SerialCommand.h"
#include "KeyMatrix.h"
#include "VbusDetect.h"
#include "Switch.h"

#define PIN_PAGE_SW      9  // Page Swith pin
#define PIN_PAGE_LED    10  // LED pin
#define LED_BRIGHTNESS  32  // LED brightness

// Page Switch
Switch pageSwitch;
// Page LED
Adafruit_NeoPixel pageLed(1, PIN_PAGE_LED, NEO_GRB + NEO_KHZ800);

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

// Initialize
void setup()
{
    pinMode(LED_RED,    OUTPUT);
    pinMode(LED_GREEN,  OUTPUT);
    pinMode(LED_BLUE,   OUTPUT);
    digitalWrite(LED_RED,   HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE,  HIGH);
    
    // Page Switch
    pageSwitch.begin(PIN_PAGE_SW);
    
    // Page LED
    pageLed.begin();
    pageLed.setBrightness(LED_BRIGHTNESS);
    
    // detect VBUS
    if (VbusDetect()) {
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
    pageLed.setPixelColor(0, keyMapStorage.getLedColor());
    pageLed.show();
}

// Main Loop
void loop()
{
    delay(2);
    
    // serial command task
    if(isUsbConnected){
        int ret = serialCommand.task();
        if(ret == RET_WRITE){
            keyMapStorage.save();
            keyMapStorage.changePage(false);
            keyMapStorage.getKeyTable(keyTable);
            keyMatrix.setKeyTable(keyTable);
            
            pageLed.setPixelColor(0, keyMapStorage.getLedColor());
            pageLed.show();
        }
    }
    
    // get page switch event
    int event = pageSwitch.get();
    if(event == SW_EVENT_SHORT){
        keyMapStorage.changePage(true);
        keyMapStorage.getKeyTable(keyTable);
        keyMatrix.setKeyTable(keyTable);
        
        pageLed.setPixelColor(0, keyMapStorage.getLedColor());
        pageLed.show();
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
            }
        }else{
            // key release
            keyboard->keyboardRelease();
        }
    }
}
