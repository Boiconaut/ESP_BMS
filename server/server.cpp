#include <SD.h>
#include "server.h"

String ServerHandler::getAuthToken(){
    unsigned long token = random(998) + 1;
    return String(token);
}

void ServerHandler::handle_NotFound(AsyncWebServerRequest *request){
    //request->send(404, "text/html", SendNotFound());
}

void ServerHandler::handle_onAuth(AsyncWebServerRequest *request, String data){
    String token;
    String body = data;
  
    String correct = "{\"login\":\"user\",\"password\":\"bms\"}";
  
    if(body == correct){
      token = getAuthToken();
      valid_token = token;
      IsAuth = true;
    }
    else{
      token = "0";
      valid_token = "null";
    }
  
    String json_token = "{\"token\":\"";
    json_token += token;
    json_token += "\"}";
    
    request->send(200, "text/json", json_token); 
}

void ServerHandler::handle_Prognosis(AsyncWebServerRequest *request, String data, SensorsHandler *sensors){
    String body = data;
    int str_length = body.length() + 1;
    char char_array[str_length];
    body.toCharArray(char_array, str_length);
    
    float capacity = atof(char_array);
    sensors->SetNomCapacity((int)(capacity));
    float time_past = sensors->GetSeconds() + (sensors->GetMinutes() * 60) + (sensors->GetHours() * 3600);
    float time_all = (capacity / (0 - sensors->GetAverageCurrent())) * 3600;
    float time_fore = time_all - time_past;

    if(time_fore < 0){
    time_fore = 0;
    }
    float hours_fore = floor(time_fore / 3600);
    float minutes_fore = (time_fore - (hours_fore * 3600)) / 60;

    if(String(hours_fore) == "inf" || String(minutes_fore) == "nan"){
    minutes_fore = 0.00;
    hours_fore = 999999999.00;
    }

    String prognosis = "Приблизительное остаточное время работы: ";
    if(sensors->GetAverageCurrent() > 0 || time_past < 60){
    prognosis += "Для расчёта должно пройти не менее минуты с момента начала разряда";
    }
    else{
        prognosis += String(hours_fore).substring(0, String(hours_fore).length() - 3);
        prognosis += ":";
        if(minutes_fore < 10){
        prognosis += "0";
        }
        prognosis += String(minutes_fore).substring(0, String(minutes_fore).length() - 3);
    }

    request->send(200, "text/plain", prognosis);
}

void ServerHandler::handle_Voltage(AsyncWebServerRequest *request, String data, SensorsHandler *sensors){
    String body = data;
    int str_length = body.length() + 1;
    char char_array[str_length];
    body.toCharArray(char_array, str_length);
    
    float nominal_voltage = atof(char_array);
    sensors->SetNomVoltage((uint8_t)nominal_voltage);
    sensors->SetVoltageQuot((nominal_voltage / 60) * 14.65);
    sensors->UpdateVoltageMap();
    request->send(200, "text/plain", String(nominal_voltage));
}

void ServerHandler::assembleData(SensorsHandler *sensors, MotoHandler *moto, Clock *clock, ErrorHandler *error){
    json["voltage"] = String(sensors->GetVoltage());
    json["current"] = String(sensors->GetCurrent());
    json["power"] = String(sensors->GetPower());
    json["soc"] = String(sensors->GetSOC());

    String date_hour = String(clock->GetHours());
    String date_minute = clock->GetMinutes() < 10 ? "0" + String(clock->GetMinutes()) : String(clock->GetMinutes());
    String date_second = clock->GetSeconds() < 10 ? "0" + String(clock->GetSeconds()) : String(clock->GetSeconds());
    String datetime = date_hour + ":" + date_minute + ":" + date_second;
    json["datetime"] = datetime;

    String hour = String(moto->GetHours());
    String minute = moto->GetMinutes() < 10 ? "0" + String(moto->GetMinutes()) : String(moto->GetMinutes());
    String second = moto->GetSeconds() < 10 ? "0" + String(moto->GetSeconds()) : String(moto->GetSeconds());
    String moto = hour + ":" + minute + ":" + second;
    json["time"] = moto;

    String state_time = String(sensors->GetHours()) + ":" + String(sensors->GetMinutes()) 
                          + ":" + String(sensors->GetSeconds());
    json["state_time"] = state_time;

    String current_state;
    if(sensors->STATE == STATE_CHARGE) current_state = "В заряде:";
    else if (sensors->STATE == STATE_DISCHARGE) current_state = "В разряде:";
    else current_state = "Релаксация";
    json["state"] = current_state;

    json["temperature"] = clock->GetTemperature();
    json["capacity"] = String(sensors->GetCapacity()).substring(0, String(sensors->GetCapacity()).length() - 3);

    String error_state = error->ERROR_CODE ? "Ошибка" : "Ошибок нет";
    json["error_state"] = error_state;

    String error_code = "";
    if(bitRead(error->ERROR_CODE, ERROR_LOW_VOLTAGE)) { error_code += "<h3>Низкое напряжение</h3>"; }
    if(bitRead(error->ERROR_CODE, ERROR_OVERCURRENT)) { error_code += "<h3>Перегрузка по току</h3>"; }
    if(bitRead(error->ERROR_CODE, ERROR_NO_SD)) { error_code += "<h3>Отсутствует SD-карта</h3>"; }
    json["error"] = error_code;
}

void ServerHandler::GetCreentials(Clock *clock){
    File f = SD.open("/config.json", FILE_READ);
    if(f){
        DynamicJsonDocument doc(1024);
        String s = f.readString();
        deserializeJson(doc, s);
        JsonObject obj = doc.as<JsonObject>();
        String s1 = obj["ssid"];
        String s2 = obj["password"];
        net_ssid = s1.c_str();
        net_password = s2.c_str();
        f.close();
        
        Serial.print("\nString config: ");
        Serial.println(s);
        Serial.print("\nString ssid: ");
        Serial.println(s1);
        Serial.print("\nString password: ");
        Serial.println(s2);

        Serial.print("Connecting to ");
        Serial.print(net_ssid);
        Serial.print(" with password ");
        Serial.println(net_password);

        WiFi.begin(net_ssid, net_password);

        while (WiFi.status() != WL_CONNECTED) 
        {
        delay(1000);
        Serial.print(".");
        }
        Serial.println("");
        Serial.println("WiFi connected..!");
        Serial.print("Got IP: "); 
    
        timeClient.begin();
        timeClient.setTimeOffset(10800);
        timeClient.update();
        clock->SyncTime();
    }
}

void ServerHandler::SetupServer(){
    WiFi.config(ip, gateway, subnet);
  Serial.println(WiFi.localIP());
  
  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      String string_data = "";
      String dat = "";
      dat += (const char*)data;
      
      if(request->url() == "/auth"){
        string_data = dat.substring(0, dat.indexOf("}") + 1);
        handle_onAuth(request, string_data);
      }
      if(request->url() == "/getprognosis"){
        handle_Prognosis(request, dat);
      }
      if(request->url() == "/setvoltage"){
        handle_Voltage(request, dat);
      }
    });
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    isAuthenticated = false;
    request->send(200, "text/html", SendLoginForm());
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
      /*Serial.print("Token: ");
      Serial.print(token);
      Serial.print(" ");
      Serial.print("Valid token: ");
      Serial.print(valid_token);
      Serial.print("\n");
    
      if(token == valid_token){
        isAuthenticated = true;
      }*/
      
      if(isAuthenticated){
        request->send(200, "text/html", SendHTML());
      }
      else{
        request->send(200, "text/html", SendInvalidCredentials());
      }
  });

  server.on("/getdata", HTTP_GET, [](AsyncWebServerRequest *request){
      assembleData();
      String json_string; 
      serializeJson(json, json_string);
      request->send(200, "text/json", json_string);
      json.clear(); 
    });

    server.on("/downloadlog", HTTP_GET, [](AsyncWebServerRequest *request){
      File file = SD.open("/errors.txt");
      
      if(file){
        if(file.available()){
          AsyncWebServerResponse *response = request->beginResponse(SD, "/errors.txt");
          request->send(response);
          file.close();
        }
      }
      else{
        request->send(404, "text/plain", "File not found");
      }
    });

    server.on("/downloadlogfile", HTTP_GET, [](AsyncWebServerRequest *request){
      File file = SD.open("/datalog.txt");
      
      if(file){
        if(file.available()){
          AsyncWebServerResponse *response = request->beginResponse(SD, "/datalog.txt");
          request->send(response);
          file.close();
        }
      }
      else{
        request->send(404, "text/plain", "File not found");
      }
    });

    server.onNotFound(handle_NotFound);
    server.begin();
    
    Serial.println("HTTP server started");
    Serial.print("Mac address: ");
    Serial.print(WiFi.macAddress());
    Serial.print("\n");
}