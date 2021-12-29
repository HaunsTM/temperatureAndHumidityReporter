#include <Arduino.h>
#include "Blinker.h"
#include <DHT.h>
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
 
DHT dht(DHTPIN, DHTTYPE);

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
    SETTINGS_DATA_MQTT_PUBLISHED_TOPIC_ACTUATOR_ACTION,
    SETTINGS_DATA_MQTT_PUBLISHED_TOPIC_TEMPERATURE_CELCIUS,
    SETTINGS_DATA_MQTT_PUBLISHED_TOPIC_HUMIDITY_PERCENT);

void reportData() {

    bool temperatureCHasChanged = currentMeterData.temperatureC != previousMeterData.temperatureC;
    bool humidityPercentHasChanged = currentMeterData.humidityPercent != previousMeterData.humidityPercent;

    if (temperatureCHasChanged || humidityPercentHasChanged) {

        mQTTC.reportTempAndHumidity(currentMeterData);

    }

};

void setup() {
    
    Serial.begin(SETTINGS_DATA_SERIAL_MONITOR_BAUD); // initialize serial monitor with 115200 baud
    delay(500);            // And wait for half a second.

    onboardLED.initialize();

    blinker.initialize();
    blinker.start();

    dht.begin();
}
 
void loop() {
    wifiManager.monitorWiFi();
    webserver.handleClient();
    // Wait a few seconds between measurements.
    delay(2000);

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
/*
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }
    */

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F(" Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("C "));
    Serial.print(f);
    Serial.print(F("F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("C "));
    Serial.print(hif);
    Serial.println(F("F"));


    if (wifiManager.isConnectedToWifi()) {
        if (!mQTTC.isConnectedToMQTTBroker()) {
            mQTTC.connectToMQTTBroker();
        }
        pubSubClient.loop();
    }
}
 

