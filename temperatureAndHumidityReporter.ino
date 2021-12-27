#include <Arduino.h>
#include "Blinker.h"
#include <DHT.h>
#include <ESP8266WiFi.h>
#include "Pins.h"
#include "SettingsData.h"
 
#define DHTPIN PIN_D4_GPIO2 //pin gpio 2 in sensor
#define DHTTYPE DHT22 // DHT 22 Change this if you have a DHT11
 
DHT dht(DHTPIN, DHTTYPE);
 
OnboardLED onboardLED;
Blinker blinker(onboardLED);
 
float t,h;
unsigned long previousMillis = 0; // will store last temp was read
const long interval = 2000; // interval at which to read sensor
 
void setup() {
    
    Serial.begin(SETTINGS_DATA_SERIAL_MONITOR_BAUD); // initialize serial monitor with 115200 baud
    delay(500);            // And wait for half a second.

    onboardLED.initialize();

    blinker.initialize();
    blinker.start();

    dht.begin();
}
 
void loop() {
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
    
}
 

