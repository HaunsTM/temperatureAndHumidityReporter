#include <Arduino.h>
#include "HTTPWebServer.h"
#include "TemperatureAndHumidityData.h"
#include "TextMessageGenerator.h"

#include "webpages/axios_js.h"
#include "webpages/info_html.h"
#include "webpages/knockout_js.h"
#include "webpages/paho_js.h"
#include "webpages/simple_css.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

HTTPWebServer::HTTPWebServer(ESP8266WebServer& server, TextMessageGenerator& tMG, TemperatureAndHumidityData& currentMeterData, String mqttBrokerURL, int mqttPort, String mqttUsername, String mqttPassword, String mqttPublishTopicTemperatureC, String mqttPublishTopicHumidityPercent)
    :   _server(server),
        _tMG(tMG),
        _currentMeterData(currentMeterData)
{    
    _mqttBrokerURL = mqttBrokerURL;
    _mqttPort = mqttPort;
    _mqttUsername = mqttUsername;
    _mqttPassword = mqttPassword;
    
    _mqttPublishTopicTemperatureC = mqttPublishTopicTemperatureC;
    _mqttPublishTopicHumidityPercent = mqttPublishTopicHumidityPercent;

    _initialized = false;
}

HTTPWebServer::~HTTPWebServer() {
}


bool HTTPWebServer::isConnectedToWifi() {
    bool isConnected = WiFi.status() == WL_CONNECTED;
    return isConnected;
}

void HTTPWebServer::initialize() {
    if (!_initialized) {
        // https://techtutorialsx.com/2017/04/09/esp8266-connecting-to-mqtt-broker/

        setUpRouteHandlers();
        _server.begin();

        _initialized = true;
    }
}

String HTTPWebServer::randomMQTTClientId() {
    String randomMQTTClientId = "Chicken House Hatch - HTTPWebserver MQTT clientId: ";
    randomMQTTClientId += String(random(0xffff), HEX);

    return randomMQTTClientId;
};

void HTTPWebServer::setUpRouteHandlers() {

    _server.on("/", [this]() { routeGetInfo(); });

    _server.on("/info", [this]() { routeGetInfo(); });
    
    _server.on("/javascriptAxios_js", [this]() { routeGetJavascriptAxiosJs(); });
    _server.on("/javascriptKnockout_js", [this]() { routeGetJavascriptKnockoutJs(); });
    _server.on("/javascriptPaho_js", [this]() { routeGetJavascriptPahoJs(); });
    _server.on("/constJavascriptParameters_js", [this]() { routeGetConstJavascriptParameters(); });

    _server.on("/styleSimple_css", [this]() { routeGetStyleSimpleCss(); });

    _server.on("/sensorData", [this]() { routeGetSensorData(); });

    _server.onNotFound([this]() { routeGetNotFound(); });
}

String HTTPWebServer::baseMQTTTopic() {
    return "<code>iot/temperature_and_humidity/</code>";
}

void HTTPWebServer::routeGetInfo() {    
    const unsigned long SIZE_WITHOUT_TERMINATING_NULL_CHARACTER = sizeof(INFO_HTML) - 1;
    _server.send_P(200, "text/html", INFO_HTML, SIZE_WITHOUT_TERMINATING_NULL_CHARACTER);
}

void HTTPWebServer::routeGetJavascriptAxiosJs() {
    const unsigned long SIZE_WITHOUT_TERMINATING_NULL_CHARACTER = sizeof(AXIOS_JS) - 1;
    _server.send_P(200, "text/javascript", AXIOS_JS, SIZE_WITHOUT_TERMINATING_NULL_CHARACTER);
}

void HTTPWebServer::routeGetJavascriptKnockoutJs() {
    const unsigned long SIZE_WITHOUT_TERMINATING_NULL_CHARACTER = sizeof(KNOCKOUT_JS) - 1;
    _server.send_P(200, "text/javascript", KNOCKOUT_JS, SIZE_WITHOUT_TERMINATING_NULL_CHARACTER);
}

void HTTPWebServer::routeGetJavascriptPahoJs() {
    const unsigned long SIZE_WITHOUT_TERMINATING_NULL_CHARACTER = sizeof(PAHO_JS) - 1;
    _server.send_P(200, "text/javascript", PAHO_JS, SIZE_WITHOUT_TERMINATING_NULL_CHARACTER);
}

void HTTPWebServer::routeGetConstJavascriptParameters() {
    
    char wifiChannel[6];
    itoa(WiFi.channel(), wifiChannel, 10);

    const String constJavascriptParameters = 
        String("const constJavascriptParameters = {") +
            String("'device' : {") +
                    String("'firmwareVersion': '") + _tMG.firmwareVersion() + String("',") +
                    String("'macAddress': '") + WiFi.macAddress() + String("',") +
                    String("'serialMonitorBaud': '") + _tMG.serialMonitorBaud() + String("',") +
            String("},") +

            String("'mqtt' : {") +
                String("'publishTopics': {") +
                    String("'temperatureC': '") + _mqttPublishTopicTemperatureC + String("',") +
                    String("'humidityPercent': '") + _mqttPublishTopicHumidityPercent + String("',") +
                String("},") +
                String("'hostname': '") + _mqttBrokerURL + String("',") +
                String("'clientId': '") + randomMQTTClientId() + String("',") +
                String("'connectionOptions': {") +
                    String("'userName': '") + _mqttUsername + String("',") +
                    String("'password': '") + _mqttPassword + String("',") +
                    String("'keepAliveInterval': 60,") +
                String("},") +
                String("'port':") + String(_mqttPort) +
            String("},") +
            
            String("'wifi' : {") +
                    String("'channel': '") + wifiChannel + String("',") +
                    String("'localIP': '") + WiFi.localIP().toString() + String("',") +
                    String("'SSID': '") + WiFi.SSID() + String("',") +
            String("}") +
        String("};");
    _server.send_P(200, "text/javascript", constJavascriptParameters.c_str());
}


void HTTPWebServer::routeGetStyleSimpleCss() {
    const unsigned long SIZE_WITHOUT_TERMINATING_NULL_CHARACTER = sizeof(SIMPLE_CSS) - 1;
    _server.send_P(200, "text/css", SIMPLE_CSS, SIZE_WITHOUT_TERMINATING_NULL_CHARACTER);
}

void HTTPWebServer::routeGetSensorData() {

    String sensorData = 
        String("{") +
            String("\"temperatureC\":") + String(_currentMeterData.temperatureC)  + String(",") +
            String("\"humidityPercent\":") + String(_currentMeterData.humidityPercent)   +
        String("}");
    _server.send(200, "application/json", sensorData.c_str());
}

void HTTPWebServer::routeGetNotFound() {
    _server.send(404, "text/plain", "Not found");
}

void HTTPWebServer::handleClient() {
    _server.handleClient();
}