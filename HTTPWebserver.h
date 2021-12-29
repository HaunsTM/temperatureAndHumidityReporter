#include <Arduino.h>
#include <ESP8266WebServer.h>

#include "TemperatureAndHumidityData.h"
#include "TextMessageGenerator.h"

#ifndef HTTP_WEB_SERVER_H
#define HTTP_WEB_SERVER_H

class HTTPWebServer {

private:
    int _webserverPort;

    bool _initialized;
    ESP8266WebServer& _server;
    TextMessageGenerator& _tMG;
    TemperatureAndHumidityData& _currentMeterData;

    String _mqttBrokerURL;
    int _mqttPort;
    String _mqttUsername;
    String _mqttPassword;

    String _mqttPublishTopicTemperatureC;
    String _mqttPublishTopicHumidityPercent;
    
    String baseMQTTTopic();

    String randomMQTTClientId();
    void setUpRouteHandlers();

    void routeGetJavascriptAxiosJs();
    void routeGetJavascriptKnockoutJs();
    void routeGetJavascriptPahoJs();
    void routeGetConstJavascriptParameters();
    
    void routeGetStyleSimpleCss();

    void routeGetInfo();
    void routeGetNotFound();
    
    void routeGetSensorData();
public:
    HTTPWebServer(ESP8266WebServer& server, TextMessageGenerator& tMG, TemperatureAndHumidityData& currentMeterData, String mqttBrokerURL, int mqttPort, String mqttUsername, String mqttPassword, String mqttPublishTopicTemperatureC, String mqttPublishTopicHumidityPercent);
    
    ~HTTPWebServer();

    void initialize();

    bool isConnectedToWifi();
    void handleClient();

};

#endif