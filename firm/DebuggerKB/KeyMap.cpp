#include <Arduino.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <Adafruit_TinyUSB.h>
#include "KeyMap.h"

using namespace Adafruit_LittleFS_Namespace;

// internal storage file name
#define FILENAME    "/setting.bin"

// bit mask for modifier keys
#define ALT_KEY    0x04    // Alt
#define CTRL_KEY   0x02    // Ctrl
#define SHIFT_KEY  0x01    // Shift

// modifier key code
// https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h
#define HID_KEY_ALT_LEFT        0xE2
#define HID_KEY_CONTROL_RIGHT   0xE4 // Some people swap left Ctrl and CapsLock.
#define HID_KEY_SHIFT_LEFT      0xE1

// internal storage file
static File g_file(InternalFS);

// initialize
void KeyMapStorage::begin()
{
    // begin internal file system
    InternalFS.begin();
    
    // load Keymap data from Data Flash
    this->load();
}

// change Keymap Page
void KeyMapStorage::changePage(bool change)
{
    int cnt = 0;
    
    while(true){
        if(change) m_page++;
        if(m_page >= PAGE_MAX) m_page = 0;
        if(keyMaps[m_page].Enabled) break;
        cnt++;
        if(cnt >= PAGE_MAX ) break;
    }
//  Serial1.print("page = "); Serial1.println(m_page); 
}

// set default value
void KeyMapStorage::setDefaultValue()
{
    int page;
    
    //********** layer 0 : Visual Studio **********
    page = 0;
    keyMaps[page].Enabled = true;
    // Name
    memset(keyMaps[page].Name, 0x00, 20);
    sprintf((char*)keyMaps[page].Name, "Visual Studio");
    // Led
    keyMaps[page].Led[0] = 135;
    keyMaps[page].Led[1] = 80;
    keyMaps[page].Led[2] = 200;
    // Key 0 : Start
    keyMaps[page].Modifiers[0] = 0;
    keyMaps[page].KeyCodes [0] = HID_KEY_F5;
    // Key 1 : Start Without Debugging
    keyMaps[page].Modifiers[1] = CTRL_KEY;
    keyMaps[page].KeyCodes [1] = HID_KEY_F5;
    // Key 2 : Continue
    keyMaps[page].Modifiers[2] = 0;
    keyMaps[page].KeyCodes [2] = HID_KEY_F5;
    // Key 3 : Break All
    keyMaps[page].Modifiers[3] = CTRL_KEY + ALT_KEY;
    keyMaps[page].KeyCodes [3] = HID_KEY_CANCEL; // Break key
    // Key 4 : Stop Debugging
    keyMaps[page].Modifiers[4] = SHIFT_KEY;
    keyMaps[page].KeyCodes [4] = HID_KEY_F5;
    // Key 5 : Step Over
    keyMaps[page].Modifiers[5] = 0;
    keyMaps[page].KeyCodes [5] = HID_KEY_F10;
    // Key 6 : Step Into
    keyMaps[page].Modifiers[6] = 0;
    keyMaps[page].KeyCodes [6] = HID_KEY_F11;
    // Key 7 : Step Out
    keyMaps[page].Modifiers[7] = SHIFT_KEY;
    keyMaps[page].KeyCodes [7] = HID_KEY_F11;
    // Key 8 : Run To Cursor
    keyMaps[page].Modifiers[8] = CTRL_KEY;
    keyMaps[page].KeyCodes [8] = HID_KEY_F10;
    // Key 9 : Restart
    keyMaps[page].Modifiers[9] = CTRL_KEY + SHIFT_KEY;
    keyMaps[page].KeyCodes [9] = HID_KEY_F5;

    //********** layer 1 : Eclipse **********
    page = 1;
    keyMaps[page].Enabled = true;
    // Name
    memset(keyMaps[page].Name, 0x00, 20);
    sprintf((char*)keyMaps[page].Name, "Eclipse");
    // Led
    keyMaps[page].Led[0] = 247;
    keyMaps[page].Led[1] = 148;
    keyMaps[page].Led[2] = 30;
    // Key 0 : Debug
    keyMaps[page].Modifiers[0] = 0;
    keyMaps[page].KeyCodes [0] = HID_KEY_F11;
    // Key 1 : Run
    keyMaps[page].Modifiers[1] = CTRL_KEY;
    keyMaps[page].KeyCodes [1] = HID_KEY_F11;
    // Key 2 : Resume
    keyMaps[page].Modifiers[2] = 0;
    keyMaps[page].KeyCodes [2] = HID_KEY_F8;
    // Key 3 : Suspend
    keyMaps[page].Modifiers[3] = 0;
    keyMaps[page].KeyCodes [3] = 0; // not assigned
    // Key 4 : Terminate
    keyMaps[page].Modifiers[4] = CTRL_KEY;
    keyMaps[page].KeyCodes [4] = HID_KEY_F2;
    // Key 5 : Step Over
    keyMaps[page].Modifiers[5] = 0;
    keyMaps[page].KeyCodes [5] = HID_KEY_F6;
    // Key 6 : Step Into
    keyMaps[page].Modifiers[6] = 0;
    keyMaps[page].KeyCodes [6] = HID_KEY_F5;
    // Key 7 : Step Return
    keyMaps[page].Modifiers[7] = 0;
    keyMaps[page].KeyCodes [7] = HID_KEY_F7;
    // Key 8 : Run to Line
    keyMaps[page].Modifiers[8] = CTRL_KEY;
    keyMaps[page].KeyCodes [8] = HID_KEY_R;
    // Key 9 : Restart
    keyMaps[page].Modifiers[9] = 0;
    keyMaps[page].KeyCodes [9] = 0; // not assigned

    //********** layer 2 : Android Studio **********
    page = 2;
    keyMaps[page].Enabled = true;
    // Name
    memset(keyMaps[page].Name, 0x00, 20);
    sprintf((char*)keyMaps[page].Name, "Android Studio", page);
    // Led
    keyMaps[page].Led[0] = 61;
    keyMaps[page].Led[1] = 220;
    keyMaps[page].Led[2] = 132;
    // Key 0 : Debug
    keyMaps[page].Modifiers[0] = SHIFT_KEY;
    keyMaps[page].KeyCodes [0] = HID_KEY_F9;
    // Key 1 : Run
    keyMaps[page].Modifiers[1] = SHIFT_KEY;
    keyMaps[page].KeyCodes [1] = HID_KEY_F10;
    // Key 2 : Resume Program
    keyMaps[page].Modifiers[2] = 0;
    keyMaps[page].KeyCodes [2] = HID_KEY_F9;
    // Key 3 : Pause
    keyMaps[page].Modifiers[3] = 0;
    keyMaps[page].KeyCodes [3] = 0; // not assigned
    // Key 4 : Stop
    keyMaps[page].Modifiers[4] = CTRL_KEY;
    keyMaps[page].KeyCodes [4] = HID_KEY_F2;
    // Key 5 : Step Over
    keyMaps[page].Modifiers[5] = 0;
    keyMaps[page].KeyCodes [5] = HID_KEY_F8;
    // Key 6 : Step Into
    keyMaps[page].Modifiers[6] = 0;
    keyMaps[page].KeyCodes [6] = HID_KEY_F7;
    // Key 7 : Step Out
    keyMaps[page].Modifiers[7] = SHIFT_KEY;
    keyMaps[page].KeyCodes [7] = HID_KEY_F8;
    // Key 8 : Run to Cursor
    keyMaps[page].Modifiers[8] = ALT_KEY;
    keyMaps[page].KeyCodes [8] = HID_KEY_F9;
    // Key 9 : Rerun
    keyMaps[page].Modifiers[9] = 0;
    keyMaps[page].KeyCodes [9] = 0; // not assigned

    //********** layer 3 : MPLAB X **********
    page = 3;
    keyMaps[page].Enabled = true;
    // Name
    memset(keyMaps[page].Name, 0x00, 20);
    sprintf((char*)keyMaps[page].Name, "MPLAB X");
    // Led
    keyMaps[page].Led[0] = 237;
    keyMaps[page].Led[1] = 39;
    keyMaps[page].Led[2] = 36;
    // Key 0 : Debug Main Project
    keyMaps[page].Modifiers[0] = 0;
    keyMaps[page].KeyCodes [0] = 0; // not assigned
    // Key 1 : Run Main Project
    keyMaps[page].Modifiers[1] = 0;
    keyMaps[page].KeyCodes [1] = 0; // not assigned
    // Key 2 : Continue
    keyMaps[page].Modifiers[2] = 0;
    keyMaps[page].KeyCodes [2] = HID_KEY_F5;
    // Key 3 : Pause
    keyMaps[page].Modifiers[3] = CTRL_KEY + ALT_KEY;
    keyMaps[page].KeyCodes [3] = HID_KEY_8;
    // Key 4 : Finish Debugger Session
    keyMaps[page].Modifiers[4] = SHIFT_KEY;
    keyMaps[page].KeyCodes [4] = HID_KEY_F5;
    // Key 5 : Step Over
    keyMaps[page].Modifiers[5] = 0;
    keyMaps[page].KeyCodes [5] = HID_KEY_F8;
    // Key 6 : Step Into
    keyMaps[page].Modifiers[6] = 0;
    keyMaps[page].KeyCodes [6] = HID_KEY_F7;
    // Key 7 : Step Out
    keyMaps[page].Modifiers[7] = CTRL_KEY;
    keyMaps[page].KeyCodes [7] = HID_KEY_F7;
    // Key 8 : Run to Cursor
    keyMaps[page].Modifiers[8] = 0;
    keyMaps[page].KeyCodes [8] = HID_KEY_F4;
    // Key 9 : Reset
    keyMaps[page].Modifiers[9] = 0;
    keyMaps[page].KeyCodes [9] = 0; // not assigned

    // layer 4-7 : not used
    for(page = 4; page < PAGE_MAX; page++){
        // Enabled
        keyMaps[page].Enabled = false;
        // Name
        memset(keyMaps[page].Name, 0x00, 20);
        sprintf((char*)keyMaps[page].Name, "Not Used");
        // Led
        keyMaps[page].Led[0] = 0x00;
        keyMaps[page].Led[1] = 0x00;
        keyMaps[page].Led[2] = 0x00;
        // each Keys
        for(int key = 0; key < KEY_MAX; key++){
            keyMaps[page].Modifiers[key] = 0;
            keyMaps[page].KeyCodes [key] = HID_KEY_A + key;
        }
    }
    
    // initialize page number
    m_page = 0;
    
    // save to data Flash
    this->save();
}

// load Keymap data from data Flash
void KeyMapStorage::load()
{
    if (g_file.open(FILENAME, FILE_O_READ))
    {
        int len = sizeof(keyMaps);
        int readlen = g_file.read((uint8_t*)keyMaps, len);
//      int pos  = g_file.position();
//      int size = g_file.size();
        g_file.close();
        
        if(readlen == len) {
//          Serial1.println("read OK!");
//          Serial1.print("len  = "); Serial1.println(readlen);
//          Serial1.print("pos  = "); Serial1.println(pos);
//          Serial1.print("size = "); Serial1.println(size);
//          Serial1.println((char*)keyMaps[0].Name);

            // initialize page number
            m_page = 0;
            changePage(false);
        }else{
//          Serial1.println("read ERROR!");
            digitalWrite(LED_RED, LOW);
            this->setDefaultValue(); // set default value
        }
    }else{
//      Serial1.println("open(read) ERROR !");
        digitalWrite(LED_RED, LOW);
        this->setDefaultValue(); // set default value
    }
}

// save Keymap data to data Flash
void KeyMapStorage::save()
{
    if( g_file.open(FILENAME, FILE_O_WRITE) )
    {
        g_file.seek(0); // (!!!) must seek(0) to overwrite file 
        int len = sizeof(keyMaps);
        int writtenlen = g_file.write((uint8_t*)keyMaps, len);
//      int pos  = g_file.position();
//      int size = g_file.size();
        g_file.close();

        if(writtenlen == len) {
//          Serial1.println("write OK!");
//          Serial1.print("len  = "); Serial1.println(writtenlen);
//          Serial1.print("pos  = "); Serial1.println(pos);
//          Serial1.print("size = "); Serial1.println(size);
//          Serial1.println((char*)keyMaps[0].Name);
        }else{
//          Serial1.println("write ERROR!");
            digitalWrite(LED_RED, LOW);
        }
    }else{
//      Serial1.println("open(write) ERROR!");
        digitalWrite(LED_RED, LOW);
    }
}

// get LED color
uint32_t KeyMapStorage::getLedColor()
{
    uint8_t r = keyMaps[m_page].Led[0];
    uint8_t g = keyMaps[m_page].Led[1];
    uint8_t b = keyMaps[m_page].Led[2];
    
    uint32_t color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    
    return color;
}

// get Key matrix table
void KeyMapStorage::getKeyTable(uint8_t keyTable[][KEY_COMBI_MAX] )
{
    for(int i = 0; i < KEY_MAX; i++){
        // zero clear
        for(int j = 0; j < KEY_COMBI_MAX; j++){
            keyTable[i][j] = 0;
        }
        int j = 0;
        uint8_t modifiers = keyMaps[m_page].Modifiers[i];
        // Alt
        if(modifiers & ALT_KEY) {
            keyTable[i][j] = HID_KEY_ALT_LEFT;
            j++;
        }
        // Ctrl
        if(modifiers & CTRL_KEY) {
            keyTable[i][j] = HID_KEY_CONTROL_RIGHT;
            j++;
        }
        // Shift
        if(modifiers & SHIFT_KEY) {
            keyTable[i][j] = HID_KEY_SHIFT_LEFT;
            j++;
        }
        // Key
        keyTable[i][j] = keyMaps[m_page].KeyCodes[i];
    }
}

// execute factory reset
void KeyMapStorage::factoryReset()
{
    // Format
    InternalFS.begin();
    InternalFS.format();
}
