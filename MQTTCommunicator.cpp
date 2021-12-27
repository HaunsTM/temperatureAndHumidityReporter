
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "MQTTCommunicator.h"
#include <PubSubClient.h>
#include "TextMessageGenerator.h"

MQTTCommunicator::MQTTCommunicator(PubSubClient& pubSubClient, TextMessageGenerator& tMG, String mqttBrokerURL, int mqttPort, String mqttUsername, String mqttPassword, String mqttPublishTopicActuatorAction, String mqttPublishTopicHatchLidarDistanceToObjectCm, String mqttPublishTopicHatchLidarStrengthOrQualityOfReturnSignal, String mqttPublishTopicHatchLidarTemperatureInternalOfLidarSensorChipCelsius)
    :   
        _pubSubClient(pubSubClient),
        _tMG(tMG)
{
    _mqttBrokerURL = mqttBrokerURL;
    _mqttPort = mqttPort;
    _mqttUsername = mqttUsername;
    _mqttPassword = mqttPassword;

    _mqttPublishTopicActuatorAction = mqttPublishTopicActuatorAction;
    _mqttPublishTopicHatchLidarDistanceToObjectCm = mqttPublishTopicHatchLidarDistanceToObjectCm;
    _mqttPublishTopicHatchLidarStrengthOrQualityOfReturnSignal = mqttPublishTopicHatchLidarStrengthOrQualityOfReturnSignal;
    _mqttPublishTopicHatchLidarTemperatureInternalOfLidarSensorChipCelsius = mqttPublishTopicHatchLidarTemperatureInternalOfLidarSensorChipCelsius;

    _initialized = false;
}

MQTTCommunicator::~MQTTCommunicator() {
}

bool MQTTCommunicator::isConnectedToWifi() {
    bool isConnected = WiFi.status() == WL_CONNECTED;
    return isConnected;
}

bool MQTTCommunicator::isConnectedToMQTTBroker() {
    bool isConnected = _pubSubClient.connected();
    return isConnected;
}

void MQTTCommunicator::initialize() {
    if (!_initialized) {
        // https://techtutorialsx.com/2017/04/09/esp8266-connecting-to-mqtt-broker/
        _pubSubClient.setServer(_mqttBrokerURL.c_str(), _mqttPort);
        // https://hobbytronics.com.pk/arduino-custom-library-and-pubsubclient-call-back/
        _pubSubClient.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->mqttReceived(topic, payload, length); });

        _initialized = true;
    }
}

void MQTTCommunicator::mqttReceived(char* topic, byte* payload, unsigned int length) {
    Serial.print(_tMG.messageArrived(topic, payload, length).c_str());
}

String MQTTCommunicator::randomMQTTClientId() {
    String randomMQTTClientId = "Chicken House Hatch - MQTTCommunicator MQTT clientId: ";
    randomMQTTClientId += String(random(0xffff), HEX);

    return randomMQTTClientId;
};

void MQTTCommunicator::connectToMQTTBroker() {
    // make sure we are not connected already
    if (!_pubSubClient.connected()) {
        // Create a random client ID
        String clientId = randomMQTTClientId();

        Serial.print(_tMG.connectingToMQTTServer(clientId.c_str()).c_str());

        // Attempt to connect
        if (_pubSubClient.connect(clientId.c_str(), _mqttUsername.c_str(), _mqttPassword.c_str())) {

            Serial.print(_tMG.mQTTServerConnectionEstablished(WiFi.localIP().toString()).c_str());

        }
        else {
            Serial.print(_tMG.mQTTServerConnectionFailed(_pubSubClient.state()).c_str());
        }
    }
};

/* Report functions */


