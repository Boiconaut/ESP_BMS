#ifndef LOGGING_H
#define LOGGING_H

#include "clock.h"
#include "sensors.h"

#define PATH_ERROR 1
#define PATH_DATA 0

class Loggable{
private:
    static String dataString;
public:
    Loggable();
    ~Loggable();
    void Log(String data, uint8_t path);
    void LogData(Clock *clock, SensorsHandler *sensors);
};

#endif