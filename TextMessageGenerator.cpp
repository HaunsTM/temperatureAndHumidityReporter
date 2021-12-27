#include <Arduino.h>
#include <stdlib.h>
#include <string>
#include "TextMessageGenerator.h"

TextMessageGenerator::TextMessageGenerator(String firmwareVersion, int serialMonitorBaud, String mqttBrokerURL, int mqttPort, String mqttUsername, String mqttPassword) {

    _firmwareVersion = firmwareVersion;
    _serialMonitorBaud = serialMonitorBaud;
    _mqttBrokerURL = mqttBrokerURL;
    _mqttPort = mqttPort;
    _mqttUsername = mqttUsername;
    _mqttPassword = mqttPassword;
};

void TextMessageGenerator::initialize() {
};

String TextMessageGenerator::deviceInfo() {
    const String message =
        String("Chicken hatch PLC\n") +
        HEADER_UNDERLINE +
        String("Firmware version ") + _firmwareVersion + String("\n") +
        String("\n\n\n");
    return message;
};

String TextMessageGenerator::lookingForWifiConnection() {
	const String message =
        String("Looking for wifi...\n") +
        HEADER_UNDERLINE +
        String("\n\n\n");
	return message;
};

String TextMessageGenerator::wifiConnectionEstablished(String SSID, String assignedIP) {
    const String message =
        String("Established connection to wifi\n") +
        HEADER_UNDERLINE +
        String("Wifi SSID: ") + SSID + String("\n") +
        String("Assigned IP: ") + assignedIP + String("\n") +
        String("GUI controls: //") + assignedIP + String("\n") +
        String("Available endpoints list: //") + assignedIP + String("/info")  +
        String("\n\n\n");
    return message;
};

String TextMessageGenerator::wifiCredentialsJSONParsingError(String error) {
    const String message =
        String("Error when parsing wifi credentials JSON\n") +
        HEADER_UNDERLINE +
        String("Error: ") + error + String("\n\n\n");
    return message;
};


String TextMessageGenerator::connectingToMQTTServer(String clientId) {
    
    const String message =
        String("Connecting to MQTT server...\n") +
        HEADER_UNDERLINE +
        String("MQTT broker URL: ") + _mqttBrokerURL + String("\n") +
        String("MQTT port: ") + mqttPort() + String("\n") +
        String("Client Id: ") + clientId + String("\n") +
        String("Credentials to be used:\n") +
        String("Username: ") + _mqttUsername + String("\n") +
        String("Password: ") + _mqttPassword + String("\n");
    return message;
};

String TextMessageGenerator::mQTTServerConnectionEstablished(String assignedIP) {
    const String message =
        String("MQTT server connection established!\n") +
        HEADER_UNDERLINE +
        String("Published topics list: //") + assignedIP + String("/info") +
        String("\n\n\n");
    return message;
};

String TextMessageGenerator::mQTTServerConnectionFailed(int state) {

    String stateText = "";
    // Possible values for client.state()
    switch (state)
    {
        case -4:
            stateText = "MQTT_CONNECTION_TIMEOUT";
            break;
        case -3:
            stateText = "MQTT_CONNECTION_LOST";
            break;
        case -2:
            stateText = "MQTT_CONNECT_FAILED";
            break;
        case -1:
            stateText = "MQTT_DISCONNECTED";
            break;
        case 0:
            stateText = "MQTT_CONNECTED";
            break;
        case 1:
            stateText = "MQTT_CONNECT_BAD_PROTOCOL";
            break;
        case 2:
            stateText = "MQTT_CONNECT_BAD_CLIENT_ID";
            break;
        case 3:
            stateText = "MQTT_CONNECT_UNAVAILABLE";
            break;
        case 4:
            stateText = "MQTT_CONNECT_BAD_CREDENTIALS";
            break;
        case 5:
            stateText = "MQTT_CONNECT_UNAUTHORIZED";
            break;
    }

    const String message =
        String("MQTT server connection failed!\n") +
        HEADER_UNDERLINE +
        String("State: ") + stateText + String("\n") +
        String("\n\n\n");
    return message;
};

String TextMessageGenerator::messageArrived(char* topic, byte* payload, unsigned int length) {
    String messageContent = "";
    for (int i = 0; i < length; i++) {
        messageContent += (char)payload[i];
    }

    const String message =
        String("Message received\n") +
        HEADER_UNDERLINE +
        String("Topic: ") + topic + String("\n") +
        String("Message: ") + messageContent + String("\n") +
        String("\n\n\n");
    return message;
};


String TextMessageGenerator::hatchWereManuallyPositioned(String direction) {
    const String message =
        String("Hatch were manually positioned\n") +
        HEADER_UNDERLINE +
        String("\n\n\n");
    return message;
};

String TextMessageGenerator::hatchArrivedToItsDestination(String position) {
    const String message =
        String("Hatch arrived to its destination\n") +
        HEADER_UNDERLINE +
        String("Position: ") + position + String("\n\n\n");
    return message;
};

String TextMessageGenerator::firmwareVersion() {
    return _firmwareVersion;
};

String TextMessageGenerator::serialMonitorBaud() {
    char serialMonitorBaud[6];
    itoa(_serialMonitorBaud, serialMonitorBaud, 10);

    return serialMonitorBaud;
};

String TextMessageGenerator::credentialsJSONString() {
    return _credentialsJSONString;
};

String TextMessageGenerator::mqttBrokerURL() {
    return _mqttBrokerURL;
};

String TextMessageGenerator::mqttPort() {
    char mqttPort[6];
    itoa(_mqttPort, mqttPort, 10);
    return mqttPort;
};

String TextMessageGenerator::mqttUsername() {
    return _mqttUsername;
};

String TextMessageGenerator::mqttPassword() {
    return _mqttPassword;
};
