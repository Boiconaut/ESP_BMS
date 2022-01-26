#include <WiFi.h>
#include "screen.h"

void Screen::Screen_1(){
    setCursor(1, 1);
    if(date < 10) display.print("0");
    this->print(date);
    this->print("-");
    if(month < 10) display.print("0");
    this->print(month);
    this->print("-");
    this->print(year);
    this->print(" ");
    if(hours < 10) display.print("0");
    this->print(hours);
    this->print(":");
    if(minutes < 10) display.print("0");
    this->print(minutes);
    this->print(":");
    if(seconds < 10) display.print("0");
    this->print(seconds);

    setCursor(1, 10);
    this->print("V=");
    this->print(voltage);
    this->print(" V");

    setCursor(1, 19);
    this->print("I=");
    this->print(voltage);
    this->print(" A");

    setCursor(1, 28);
    this->print("P=");
    this->print(power);
    this->print(" W");

    byte level = SOC / 6;
    for(byte i = 0; i < level; i++){
      fillRect(i * 8, 37, 7, 23, 1);
    }
}

void Screen::Screen_2(ErrorHandler *error, SensorsHandler *sensors){
    setCursor(1, 1);
    this->print("Nominal: ");
    this->print(sensors->GetNomVoltage());
    this->print(" V  ");
    this->print(sensors->GetNomCapacity());
    this->print(" Ah");

    setCursor(1, 10);
    if(sensors->STATE == 1) this->print("On charge: ");
    else if(sensors->STATE == 2) this->print("On discharge: ");
    else this->print("Relaxing: ");
    String h = sensors->GetHours() < 0 ? "0" + String(sensors->GetHours()) : String(sensors->GetHours());
    String m = sensors->GetMinutes() < 0 ? "0" + String(sensors->GetMinutes()) : String(sensors->GetMinutes());
    String s = sensors->GetSeconds() < 0 ? "0" + String(sensors->GetSeconds()) : String(sensors->GetSeconds());
    String state_time = h + ":" + m + ":" + s;
    this->print(state_time);

    setCursor(1, 19);
    this->print("Coulomb: ");
    this->print(sensors->GetCapacity());
    this->print(" mAh");

    setCursor(1, 28);
    WiFi.status() == WL_CONNECTED ? this->print("Wifi connected") : this->print("No Wifi connection");

    if(error->ERROR_CODE){
      uint8_t pos = 37;

      if(bitRead(error->ERROR_CODE, ERROR_LOW_VOLTAGE)){
        setCursor(1, pos);
        this->print("Low voltage!");
        pos += 9;
      }
      if(bitRead(error->ERROR_CODE, ERROR_OVERCURRENT)){
        setCursor(1, pos);
        this->print("Overcurrent!");
        pos += 9;
      }
      if(bitRead(error->ERROR_CODE, ERROR_NO_SD)){
        setCursor(1, pos);
        this->print("No SD card!");
        pos += 9;
      }
    }
}

void Screen::SetupScreen(){
    this->begin(SCREEN_ADDR);
    this->clearDisplay();
}

void Screen::UpdateScreen(){
    clearDisplay();
    setTextSize(0);
    setTextColor(1);
    
    if(millis() % 10000 <= 5000) Screen_1();
    else Screen_2();
    
    display();
}