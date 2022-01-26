#ifndef SERVER_H
#define SERVER_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include "sensors.h"
#include "moto.h"
#include "clock.h"
#include "errors.h"

class ServerHandler{
private:
    const char *user_login;
    const char *user_password;
    const char *net_ssid;
    const char *net_password;

    static AsyncWebServer server(80);
    static WiFiUDP ntpUDP;               
    static NTPClient timeClient(ntpUDP);
    static DynamicJsonDocument json(2048); 

    String valid_token;
    static boolean IsAuth;

    String getAuthToken();
    void handle_NotFound(AsyncWebServerRequest *request);
    void handle_onAuth(AsyncWebServerRequest *request, String data);
    void handle_Prognosis(AsyncWebServerRequest *request, String data, SensorsHandler *sensors);
    void handle_Voltage(AsyncWebServerRequest *request, String data, SensorsHandler *sensors);
    void assembleData(SensorsHandler *sensors, MotoHandler *moto, Clock *clock, ErrorHandler *error);
public:
    ServerHandler();
    ~ServerHandler();
    void GetCredentials(Clock *clock);
    void SetupServer();
};

#endif