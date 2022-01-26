#ifndef MOTO_H
#define MOTO_H

#define LOAD_CURRENT 5.00

#define MIN_CELL 0
#define HOUR_LOW_CELL 1
#define HOUR_HIGH_CELL 2

class MotoHandler{
private:
    static uint16_t motohour;
    static uint8_t motomin;
    static uint8_t motosec;

    static uint8_t min_cell; 
    static uint8_t hour_low_cell; 
    static uint8_t hour_high_cell; 

    static float loadCurrent;
    void read();
public:
    MotoHandler();
    ~MotoHandler();
    void SetupMotohours();
    void Reset();
    void Tick();
    void Save();

    uint16_t GetHours();
    uint8_t GetMinutes();
    uint8_t GetSeconds();
};

#endif