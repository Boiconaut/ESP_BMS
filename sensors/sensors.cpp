#include "sensors.h"

SensorsHandler::SensorsHandler(){
    kVSOC = kVSOC_INIT;
    UpdateVoltageMap();

    _capacity = 0.00;
    STATE = 0;
    STATE_BEFORE = 0;
    NOM_VOLTAGE = 12;
    NOM_CAPACITY = 6;
    counter = 0;
    minute_current_counter = 0;

    state_sec = 0;
    state_min = 0;
    state_hour = 0;

    ReadSensors();
}

void SensorsHandler::setupAnalogInputs(){
    VoltageValue = analogRead(VoltPin);
    ChargeCurrentValue = analogRead(ChargeCurrPin);                                  
    DischargeCurrentValue = analogRead(DischargeCurrPin); 

    if (ChargeCurrentValue > DischargeCurrentValue) STATE = STATE_CHARGE;
    else if (ChargeCurrentValue < DischargeCurrentValue) STATE = STATE_DISCHARGE;
    else STATE = STATE_REST;

    moment_voltage = VoltageValue * (3.3 / 4095.0) * kVD;   
    moment_current = STATE == STATE_CHARGE ? (ChargeCurrentValue * (3.3 / 4095.0) * kI)
                        : (DischargeCurrentValue * (3.3 / 4095.0) * kI);     
}

uint8_t SensorsHandler::getSOC(float v){
    if(v >= v90 && v <= vmax){
    return 90 + (v - v90)/(vmax - v90) * 10;
    }
    else if(v >= v80 && v <= v90){
        return 80 + (v - v80)/(v90 - v80) * 10;
    }
    else if(v >= v70 && v <= v80){
        return 70 + (v - v70)/(v80 - v70) * 10;
    }
    else if(v >= v60 && v <= v70){
        return 60 + (v - v60)/(v70 - v60) * 10;
    }
    else if(v >= v50 && v <= v60){
        return 50 + (v - v50)/(v60 - v50) * 10;
    }
    else if(v >= v40 && v <= v50){
        return 40 + (v - v40)/(v50 - v40) * 10;
    }
    else if(v >= v30 && v <= v40){
        return 30 + (v - v30)/(v40 - v30) * 10;
    }
    else if(v >= v20 && v <= v30){
        return 20 + (v - v20)/(v30 - v20) * 10;
    }
    else if(v >= v10 && v <= v20){
        return 10 + (v - v10)/(v20 - v10) * 10;
    }
    else if(v >= vmin && v <= v10){
        return (v - vmin)/(v10 - vmin) * 10;
    }
    else if(v >= vmax){
        return 100;
    }
    else{
        return 0;
    }
}

void SensorsHandler::tick(){
     state_sec++;
    if(state_sec > 59){
        state_min++;
        state_sec = 0;
    }
    if(state_min > 59){
        state_hour++;
        state_min = 0;
    }

    if(STATE == STATE_DISCHARGE){
        _capacity += -1 * (current / 3.60);
    }
    else if(STATE == STATE_CHARGE){
        _capacity += current / 3.60;
    }
    else {
        _capacity = 0.0;
    }
    
    if(STATE != STATE_BEFORE){
        state_hour = 0;
        state_min = 0;
        state_sec = 0;
    }
    STATE_BEFORE = STATE;
}

void SensorsHandler::ReadSensors(){
    setupAnalogInputs();
    
    if(STATE == STATE_DISCHARGE){
      moment_current = 0 - moment_current;
    }
    moment_power = moment_voltage * moment_current;
    moment_SOC = getSOC(moment_voltage);

    voltages[counter] = moment_voltage;
    currents[counter] = moment_current;
    powers[counter] = moment_power;
    socs[counter] = moment_SOC;

    counter++;
}

void SensorsHandler::GetSecondAverage(){
    float sum_voltage = 0;
    float sum_current = 0;
    float sum_power = 0;
    int sum_soc = 0;

    for(int i = 0; i < counter; i++){
      sum_voltage += voltages[i];
      sum_current += currents[i];
      sum_power += powers[i];
      sum_soc += socs[i];
    }

    voltage = sum_voltage / counter;
    current = sum_current / counter;
    power = sum_power / counter;
    SOC = sum_soc / counter;

    counter = 0;
    tick();
    
    minute_currents[minute_current_counter] = current;
    minute_current_counter++;
}

void SensorsHandler::GetMinuteAverageCurrent(){
    float sum = 0;
    for(int i = 0; i < minute_current_counter; i++){
        sum += minute_currents[i];
    }

    average_current = sum / minute_current_counter;
    minute_current_counter = 0;
}

void SensorsHandler::UpdateVoltageMap(){
    vmax = VMAX * kVSOC;      
    v90 = V90 * kVSOC;     
    v80 = V80 * kVSOC;
    v70 = V70 * kVSOC;
    v60 = V60 * kVSOC;
    v50 = V50 * kVSOC;
    v40 = V40 * kVSOC;
    v30 = V30 * kVSOC;
    v20 = V20 * kVSOC;
    v10 = V10 * kVSOC;
    vmin = VMIN * kVSOC;
}

uint8_t SensorsHandler::GetSOC(){
    return SOC;
}

float SensorsHandler::GetVoltage(){
    return voltage;
}

float SensorsHandler::GetCurrent(){
    return current;
}

float SensorsHandler::GetPower(){
    return power;
}

float SensorsHandler::GetCapacity(){
    return _capacity;
}

uint8_t SensorsHandler::GetMomentSOC(){
    return moment_SOC;
}

float SensorsHandler::GetMomentVoltage(){
    return moment_voltage;
}

float SensorsHandler::GetMomentCurrent(){
    return moment_current;
}

float SensorsHandler::GetMomentPower(){
    return moment_power;
}

uint16_t SensorsHandler::GetHours(){
    return state_hour;
}

uint8_t SensorsHandler::GetMinutes(){
    return state_min;
}

uint8_t SensorsHandler::GetSeconds(){
    return state_sec;
}

uint8_t SensorsHandler::GetNomVoltage(){
    return NOM_VOLTAGE;
}

uint8_t SensorsHandler::GetNomCapacity(){
    return NOM_CAPACITY;
}

void SensorsHandler::SetNomCapacity(uint8_t c){
    NOM_CAPACITY = c;
}

void SensorsHandler::SetNomVoltage(uint8_t v){
    NOM_VOLTAGE = c;
}

float SensorsHandler::GetAverageCurrent(){
    return average_current;
}

void SensorsHandler::SetVoltageQuot(uint8_t k){
    kVSOC = k;
}