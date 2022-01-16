#include <Arduino.h>
#include "Blinker.h"
#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include "HTTPWebServer.h"
#include "MQTTCommunicator.h"
#include "Pins.h"
#include <PubSubClient.h>
#include "SettingsData.h"
#include "TemperatureAndHumidityData.h"
#include "TextMessageGenerator.h"
#include "WifiManager.h"
 
#define DHTPIN PIN_D4_GPIO2 //pin gpio 2 in sensor
#define DHTTYPE DHT22 // DHT 22 Change
 
DHTesp dht;

TemperatureAndHumidityData currentMeterData;
TemperatureAndHumidityData previousMeterData;

OnboardLED onboardLED;
Blinker blinker(onboardLED);

TextMessageGenerator tMG(
    SETTINGS_DATA_FIRMWARE_VERSION, SETTINGS_DATA_SERIAL_MONITOR_BAUD,
    SETTINGS_DATA_MQTT_BROKER_URL, SETTINGS_DATA_MQTT_PORT_1883,
    SETTINGS_DATA_MQTT_USERNAME, SETTINGS_DATA_MQTT_PASSWORD);

WifiManager wifiManager( tMG, CREDENTIALS_JSON_STRING);

WiFiClient wifiClient;
PubSubClient pubSubClient(wifiClient);
ESP8266WebServer eSP8266WebServer(SETTINGS_DATA_WEB_SERVER_PORT);

HTTPWebServer webserver(
    eSP8266WebServer,
    tMG, currentMeterData,
    SETTINGS_DATA_MQTT_BROKER_URL,
    SETTINGS_DATA_MQTT_PORT_FOR_PAHO_1884,
    SETTINGS_DATA_MQTT_USERNAME, SETTINGS_DATA_MQTT_PASSWORD,
    SETTINGS_DATA_MQTT_PUBLISHED_TOPIC_TEMPERATURE_CELCIUS,
    SETTINGS_DATA_MQTT_PUBLISHED_TOPIC_HUMIDITY_PERCENT);

MQTTCommunicator mQTTC(
    pubSubClient, tMG,
    SETTINGS_DATA_MQTT_BROKER_URL, SETTINGS_DATA_MQTT_PORT_1883,
    SETTINGS_DATA_MQTT_USERNAME, SETTINGS_DATA_MQTT_PASSWORD,
    SETTINGS_DATA_MQTT_PUBLISHED_TOPIC_TEMPERATURE_CELCIUS,
    SETTINGS_DATA_MQTT_PUBLISHED_TOPIC_HUMIDITY_PERCENT);

enum class TemperatureAndHumidityReporterStates {
    STAND_BY,
    MEASURE,
    MEASURED,
    REPORT,
    REPORTED,
    WAIT_2_S
};

TemperatureAndHumidityReporterStates state;
unsigned long previousCollectedDataMillis, previousReportedDataMillis, previousWait2SMillis;

bool timeIsDueToCollectData() {
    
    unsigned long currentMillis = millis();
    previousCollectedDataMillis = previousCollectedDataMillis < currentMillis ? previousCollectedDataMillis : 0;
    bool timeIsDue = currentMillis - previousCollectedDataMillis > SETTINGS_SENSOR_DURATION_BETWEEN_EVERY_DATA_COLLECTION_MS;
    return timeIsDue;
}

bool timeIsDueToReportData() {
    
    unsigned long currentMillis = millis();
    previousReportedDataMillis = previousReportedDataMillis < currentMillis ? previousReportedDataMillis : 0;
    bool timeIsDue = currentMillis - previousReportedDataMillis > SETTINGS_DURATION_BETWEEN_EVERY_DATA_REPORT_MQTT_MS;
    return timeIsDue;
}


bool waitTime2SIsDue() {
    
    unsigned long timeToWait = 2 * 1000;
    
    unsigned long currentMillis = millis();
    previousWait2SMillis = previousWait2SMillis < currentMillis ? previousWait2SMillis : 0;
    bool timeIsDue = currentMillis - previousWait2SMillis > timeToWait;
    return timeIsDue;
}

void collectData() {
    currentMeterData.temperatureC = dht.getTemperature();
    currentMeterData.humidityPercent= dht.getHumidity();
}

void reportDataWithMQTT() {

    bool temperatureCHasChanged = currentMeterData.temperatureC != previousMeterData.temperatureC;
    bool humidityPercentHasChanged = currentMeterData.humidityPercent != previousMeterData.humidityPercent;

    if (temperatureCHasChanged || humidityPercentHasChanged) {

        mQTTC.reportTempAndHumidity(currentMeterData);
        currentMeterData = previousMeterData;
    }
};

void temperatureAndHumidityReporterStateMachine() {

    switch (state)
    {
        case TemperatureAndHumidityReporterStates::STAND_BY:
            
            wifiManager.isConnectedToWifi() ? blinker.setBlinkerSequence(Blinker::BlinkerSequence::STAND_BY) : blinker.setBlinkerSequence(Blinker::BlinkerSequence::STAND_BY_NON_WIFI);
            
            if(timeIsDueToCollectData()) {
                    state = TemperatureAndHumidityReporterStates::MEASURE;    

            } else if (timeIsDueToReportData()) {
                    state = TemperatureAndHumidityReporterStates::REPORT;
            }
            break;
        
        case TemperatureAndHumidityReporterStates::REPORT:
            reportDataWithMQTT();
            state = TemperatureAndHumidityReporterStates::REPORTED;
            break;

        case TemperatureAndHumidityReporterStates::REPORTED:
            previousReportedDataMillis = millis();
            state = TemperatureAndHumidityReporterStates::STAND_BY;
            break;

        case TemperatureAndHumidityReporterStates::MEASURE:
            collectData();
            previousCollectedDataMillis = millis();
            state = TemperatureAndHumidityReporterStates::MEASURED;
            break;
        
        case TemperatureAndHumidityReporterStates::MEASURED:
            state = TemperatureAndHumidityReporterStates::WAIT_2_S;
            previousWait2SMillis = millis();
            break;

        case TemperatureAndHumidityReporterStates::WAIT_2_S:

            if (waitTime2SIsDue()) {
                state = TemperatureAndHumidityReporterStates::STAND_BY;
            }
            break;
    }
}

void setup() {
    
    Serial.begin(SETTINGS_DATA_SERIAL_MONITOR_BAUD); // initialize serial monitor with 115200 baud
    delay(500);            // And wait for half a second.

    onboardLED.initialize();

    previousCollectedDataMillis = 0;
    previousWait2SMillis = 0;

    blinker.initialize();
    blinker.start();

    dht.setup(D2, DHTesp::DHT22);
    delay(500);            // And wait for half a second.

    state = TemperatureAndHumidityReporterStates::STAND_BY;
    
    wifiManager.initialize();
    webserver.initialize();
    mQTTC.initialize();
}
 
void loop() {
    wifiManager.monitorWiFi();
    webserver.handleClient();

    temperatureAndHumidityReporterStateMachine();

    blinker.handleBlinker();

    if (wifiManager.isConnectedToWifi()) {
        if (!mQTTC.isConnectedToMQTTBroker()) {
            mQTTC.connectToMQTTBroker();
        }
        pubSubClient.loop();
    }
}
 
