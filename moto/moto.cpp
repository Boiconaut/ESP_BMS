#include <EEPROM.h>
#include "moto.h"

MotoHandler::MotoHandler(){
    loadCurrent = LOAD_CURRENT;
    min_cell = MIN_CELL; 
    hour_low_cell = HOUR_LOW_CELL; 
    hour_high_cell = HOUR_HIGH_CELL; 
}

void MotoHandler::read(){
    motomin = EEPROM.read(min_cell);
    uint8_t low = EEPROM.read(hour_low_cell);
    uint8_t high = EEPROM.read(hour_high_cell);
    motohour = word(high, low);
}

void MotoHandler::SetupMotohours(){
    EEPROM.begin(512);
    read();
}

void MotoHandler::Reset(){
    EEPROM.write(min_cell, 0);
    EEPROM.write(hour_low-cell, 0);
    EEPROM.write(hour_high_cell, 0);
    EEPROM.commit();
}

void MotoHandler::Tick(){
    motosec++;
    if(motosec > 59){
        motomin++;
        motosec = 0;
    }
    if(motomin > 59){
        motohour++;
        motomin = 0;
    }
}

void MotoHandler::Save(){
    uint8_t high = highByte(motohour);
    uint8_t low = lowByte(motohour);
    EEPROM.put(min_cell, motomin);
    EEPROM.put(hour_low_cell, low);
    EEPROM.put(hour_high_cell, high);
    EEPROM.commit();
}

uint16_t MotoHandler::GetHours(){
    return motohour;
}

uint8_t MotoHandler::GetMinutes(){
    return motomin;
}

uint8_t MotoHandler::GetSeconds(){
    return motosec;
}