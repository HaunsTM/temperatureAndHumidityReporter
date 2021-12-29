#include <Arduino.h>
#include "ArduinoJson-v6.17.3.h"
#include "WifiManager.h"
#include "TextMessageGenerator.h"

#include <ESP8266WiFiMulti.h>

WifiManager::WifiManager(TextMessageGenerator& tMG, String credentialsJSONString)
    :   
        _tMG(tMG)
{

    _credentialsJSONString = credentialsJSONString;

    _initialized = false;
}

WifiManager::~WifiManager() {
}


bool WifiManager::isConnectedToWifi() {
    bool isConnected = WiFi.status() == WL_CONNECTED;
    return isConnected;
}

DynamicJsonDocument WifiManager::getCredentials() {
    const size_t capacity = JSON_ARRAY_SIZE(4) + 4 * JSON_OBJECT_SIZE(2) + 200;
    DynamicJsonDocument doc(capacity);

    DeserializationError error = deserializeJson(doc, _credentialsJSONString);

    if (error) {
        Serial.print(_tMG.wifiCredentialsJSONParsingError(error.c_str()).c_str());
    }

    return doc;
}

void WifiManager::initialize() {
    if (!_initialized) {

        auto credentials = getCredentials();

        for (JsonObject elem : credentials.as<JsonArray>()) {

            const char* SSID = elem["SSID"];
            const char* password = elem["password"];
           
            _wifiMulti.addAP(SSID, password);
        }

        WiFi.mode(WIFI_STA);
        WiFi.disconnect(true);

        _initialized = true;
    }
}

void WifiManager::monitorWiFi() {
    // maintains wifi connection
    if (_wifiMulti.run() != WL_CONNECTED)
    {
        if (_connectioWasAlive == true)
        {
            _connectioWasAlive = false;

            Serial.print(_tMG.lookingForWifiConnection().c_str());
        }
    }
    else if (_connectioWasAlive == false)
    {
        _connectioWasAlive = true;

        Serial.print(_tMG.wifiConnectionEstablished(WiFi.SSID(), WiFi.localIP().toString()).c_str());
    }

}