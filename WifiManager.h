#include <string>
#include "ArduinoJson-v6.17.3.h"
#include "TextMessageGenerator.h"
#include <ESP8266WiFiMulti.h>

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

class WifiManager {

private:
    ESP8266WiFiMulti _wifiMulti;
    String _credentialsJSONString;
    boolean _connectioWasAlive = true;

    DynamicJsonDocument getCredentials();

    bool _initialized;
    TextMessageGenerator& _tMG;
public:
    WifiManager(TextMessageGenerator& tMG, String credentialsJSONString);
    
    ~WifiManager();

    void initialize();

    bool isConnectedToWifi();

    void monitorWiFi();
};

#endif