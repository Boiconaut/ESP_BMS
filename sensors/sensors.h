#ifndef SENSORS_H
#define SENSORS_H

#define VoltPin 36            
#define DischargeCurrPin 39   
#define ChargeCurrPin 34   

#define SENSOR_UPDATE_PERIOD 100

#define STATE_REST 0       
#define STATE_CHARGE 1      
#define STATE_DISCHARGE 2 

#define VMAX 3.4     
#define V90 3.23      
#define V80 3.22
#define V70 3.21
#define V60 3.19
#define V50 3.18
#define V40 3.16
#define V30 3.13
#define V20 3.08
#define V10 2.96
#define VMIN 2.20

#define kVD 22
#define kI 30.3
#define kVSOC_INIT 4.4

class SensorsHandler{
private:
    static float _capacity;
    static uint8_t STATE_BEFORE;
    static uint8_t NOM_VOLTAGE;
    static uint8_t NOM_CAPACITY;
        
    static float kVSOC;   //For calculating SOC: kVD / 5 for 12 V  
    float vmax, v90, v80, v70, v60, v50, v40, v30, v20, v10, vmin;

    static uint16_t VoltageValue; 
    static uint16_t ChargeCurrentValue; 
    static uint16_t DischargeCurrentValue; 
    static uint16_t TemperatureValue; 

    static float moment_voltage; 
    static float moment_current; 
    static float moment_power; 
    static uint8_t moment_SOC; 

    static float voltage; 
    static float current; 
    static float power; 
    static uint8_t SOC; 
    static float temperature; 

    static float voltages[1000 / SENSOR_UPDATE_PERIOD]; 
    static float currents[1000 / SENSOR_UPDATE_PERIOD]; 
    static float powers[1000 / SENSOR_UPDATE_PERIOD]; 
    static float temperatures[1000 / SENSOR_UPDATE_PERIOD]; 
    static uint8_t socs[1000 / SENSOR_UPDATE_PERIOD]; 

    static float minute_currents[60]; 
    static float average_current; 

    static uint8_t counter; 
    static uint8_t minute_current_counter;

    static uint8_t state_sec;
    static uint8_t state_min;
    static uint16_t state_hour;

    void setupAnalogInputs();
    uint8_t getSOC(float v);
    void tick();
public:
    static uint8_t STATE;
    SensorsHandler();
    ~SensorsHandler();

    void ReadSensors();
    void GetSecondAverage();
    void GetMinuteAverageCurrent();
    void UpdateVoltageMap();

    uint8_t GetSOC();
    float GetVoltage();
    float GetCurrent();
    float GetPower();
    float GetCapacity();

    uint8_t GetNomCapacity();
    uint8_t GetNomVoltage();
    void SetNomVoltage(uint8_t v);
    void SetNomCapacity(uint8_t c);
    float GetAverageCurrent();
    void SetVoltageQuot(uint8_t k);

    uint8_t GetMomentSOC();
    float GetMomentVoltage();
    float GetMomentCurrent();
    float GetMomentPower();

    uint16_t GetHours();
    uint8_t GetMinutes();
    uint8_t GetSeconds();
};

#endif