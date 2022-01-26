#include "error.h"

ErrorHandler::ErrorHandler(){
    error_string = "";
}

void ErrorHandler::addMessage(String mes){
    error_string += mes;
}

void ErrorHandler::clear(){
    error_string = "";
}

void ErrorHandler::SetupErrors(){
    pinMode(tonePin, OUTPUT);
    pinMode(RedLED, OUTPUT);
    pinMode(BlueLED, OUTPUT);
    ERROR_CODE = B00000000;
}

void ErrorHandler::ChechErrors(Clock *clock, SensorsHandler *sensors){
    error_string += String(clock->GetDay());
    error_string += "-";
    error_string += String(clock->GetMonth());
    error_string += "-";
    error_string += String(clock->GetYear());
    error_string += " ";
    error_string += String(clock->GetHours());
    error_string += ":";
    error_string += String(clock->GetMinutes());
    error_string += ":";
    error_string += String(clock->GetSeconds());
    error_string += ".";
    error_string += String(millisec);
    error_string += "  ";

    if(sensors->GetMomentVoltage() < v20) ERROR_CODE |= (1 << ERROR_LOW_VOLTAGE);
    if(sensors->GetMomentVoltage() >= v20) ERROR_CODE &= ~(1 << ERROR_LOW_VOLTAGE);
    if(sensors->GetMomentCurrent() > 200) ERROR_CODE |= (1 << ERROR_OVERCURRENT);
    if(sensors->GetMomentCurrent() <= 200) ERROR_CODE &= ~(1 << ERROR_OVERCURRENT);

    if(bitRead(ERROR_CODE, ERROR_LOW_VOLTAGE)){
        error_string += "Error: low voltage! U = ";
        error_string += String(moment_voltage);
        error_string += " V";
        error_string += " ";
        }
        if(bitRead(ERROR_CODE, ERROR_OVERCURRENT)){
        error_string += "Error: overcurrent! I = ";
        error_string += String(moment_current);
        error_string += " A";
        error_string += " ";
        }
        error_string += "\n";
}

void ErrorHandler::LogError(Clock *clock){
    Log(error_string, 1);
}