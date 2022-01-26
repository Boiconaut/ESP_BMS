#include <SD.h>
#include "logging.h"
#include "errors.h"

Loggable::Loggable(){
    dataString = "";
}

void Loggable::LogData(Clock *clock, SensorsHandler *sensors){
    String dataString = "";
    
    dataString += String(clock->GetDay());
    dataString += "-";
    dataString += String(clock->GetMonth());
    dataString += "-";
    dataString += String(clock->GetYear());
    dataString += " ";
    dataString += String(clock->GetHours());
    dataString += ":";
    dataString += String(clock->GetMinutes());
    dataString += ":";
    dataString += String(clock->GetSeconds());
    dataString += "  ";
    
    dataString += "U = ";
    dataString += String(sensors->GetVoltage());
    dataString += ", I = ";
    dataString += String(sensors->GetCurrent());
    dataString += ", P = ";
    dataString += String(sensors->GetPower());
    dataString += ",  SOC = ";
    dataString += String(sensors->GetSOC());
    dataString += "%";
   
   Log(dataString, 0);
}

void Loggable::Log(String data, uint8_t path){
    File file;
    if(path == PATH_DATA) file = SD.open("/datalog.txt", FILE_APPEND);
    else if(path == PATH_ERROR) file = SD.open("/errors.txt", FILE_APPEND);
   
    if (file) {
      file.println(dataString);
      ERROR_CODE &= ~(1 << ERROR_NO_SD);
    }
    else{
      ERROR_CODE |= (1 << ERROR_NO_SD);
    }
    file.close();
}