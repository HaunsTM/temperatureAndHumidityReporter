#include <PubSubClient.h>
#include "TextMessageGenerator.h"

#ifndef MQTT_COMMUNICATOR_H
#define MQTT_COMMUNICATOR_H

class MQTTCommunicator {

private:

    String _mqttBrokerURL;
    int _mqttPort;
    String _mqttUsername;
    String _mqttPassword;

    String _mqttPublishTopicActuatorAction;
    String _mqttPublishTopicHatchLidarDistanceToObjectCm;
    String _mqttPublishTopicHatchLidarStrengthOrQualityOfReturnSignal;
    String _mqttPublishTopicHatchLidarTemperatureInternalOfLidarSensorChipCelsius;

    bool _initialized;

    PubSubClient& _pubSubClient;
    TextMessageGenerator& _tMG;
    
    String randomMQTTClientId();

public:
    MQTTCommunicator(PubSubClient& pubSubClient, TextMessageGenerator& tMG, String mqttBrokerURL, int mqttPort, String mqttUsername, String mqttPassword, String _mqttPublishTopicActuatorAction, String _mqttPublishTopicHatchLidarDistanceToObjectCm, String _mqttPublishTopicHatchLidarStrengthOrQualityOfReturnSignal, String _mqttPublishTopicHatchLidarTemperatureInternalOfLidarSensorChipCelsius);
    
    ~MQTTCommunicator();

    void initialize();

    bool isConnectedToWifi();

    bool isConnectedToMQTTBroker();

    void mqttReceived(char* topic, byte* payload, unsigned int length);

    void connectToMQTTBroker();

};

#endif