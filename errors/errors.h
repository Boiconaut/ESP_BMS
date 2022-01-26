#ifndef ERRORS_H
#define ERRORS_H

#include "logging.h"
#include "clock.h"
#include "sensors.h"

#define tonePin 17            
#define RedLED 10             
#define BlueLED 11   

#define ERROR_LOW_VOLTAGE 0   
#define ERROR_OVERCURRENT 1   
#define ERROR_NO_SD 2        

class ErrorHandler : Loggable{
private:
    static String error_string;
    void addMessage(String mes);
    void clear();
public:
    static uint16_t ERROR_CODE;
    ErrorHandler();
    ~ErrorHandler();
    void SetupErrors();
    void CheckErrors(Clock *clock, SensorsHandler *sensors);
    void LogError(Clock *clock);
};

#endif