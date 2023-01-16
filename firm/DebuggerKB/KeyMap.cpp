#include <Arduino.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include "KeyMap.h"

using namespace Adafruit_LittleFS_Namespace;

// internal storage file name
#define FILENAME    "/setting.bin"

// bit mask for modifier keys
#define ALT_MASK    0x04    // Alt
#define CTRL_MASK   0x02    // Ctrl
#define SHIFT_MASK  0x01    // Shift

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
    // TODO  仮の初期値
    for(int page = 0; page < PAGE_MAX; page++){
        // Enabled
        keyMaps[page].Enabled = (page % 2 == 0) ? true : false;
        // Name
        memset(keyMaps[page].Name, 0x00, 20);
        sprintf((char*)keyMaps[page].Name, "Piyo Piyo %d", page);
        // Led
        keyMaps[page].Led[0] = ((page & 0x01) == 0) ? 0xFF : 0x00;
        keyMaps[page].Led[1] = ((page & 0x02) == 0) ? 0xFF : 0x00;
        keyMaps[page].Led[2] = ((page & 0x04) == 0) ? 0xFF : 0x00;
        // each Keys
        for(int key = 0; key < KEY_MAX; key++){
            keyMaps[page].Modifiers[key] = page;
            keyMaps[page].KeyCodes[key]  = 0x04 + page + key;
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
        if(modifiers & ALT_MASK) {
            keyTable[i][j] = HID_KEY_ALT_LEFT;
            j++;
        }
        // Ctrl
        if(modifiers & CTRL_MASK) {
            keyTable[i][j] = HID_KEY_CONTROL_RIGHT;
            j++;
        }
        // Shift
        if(modifiers & SHIFT_MASK) {
            keyTable[i][j] = HID_KEY_SHIFT_LEFT;
            j++;
        }
        // Key
        keyTable[i][j] = keyMaps[m_page].KeyCodes[i];
    }
}
