#ifndef CLOCK_H
#define CLOCK_H

#define DS3231_I2C_ADDRESS 0x68
#include <NTPClient.h>

class Clock{
private:
    static uint8_t seconds, minutes, hours, day, date, month, year;  
    static uint8_t sec_before; 
    static uint16_t millisec;  
    static char weekDay[4];  
    static uint8_t tMSB, tLSB;
    static float temp3231;

    void decToBcd(uint8_t val);
    void watchConsole();
    uint8_t getRegister(uint8_t regNo);
    void setRegister(uint8_t regNo, uint8_t value);
    void getDateTime();
    String getFormattedDate(NTPClient *timeClient);
    uint16_t getYear(NTPClient *timeClient);
    uint16_t GetMonth(NTPClient *timeClient);
    uint16_t getDate(NTPClient *timeClient);
public:
    Clock();
    ~Clock();
    
    void SetupClock();
    void SetDateTime(uint8_t _date, uint8_t _month, uint8_t _year, 
                     uint8_t _hours, uint8_t _minutes, uint8_t _seconds);
    float GetTemperature();
    void SyncTime(NTPClient *timeClient);
    
    uint8_t GetDay();
    uint8_t GetMonth();
    uint8_t GetYear();
    uint8_t GetHours();
    uint8_t GetMinutes();
    uint8_t GetSeconds();
};

#endif