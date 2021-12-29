#include <PubSubClient.h>
#include "TemperatureAndHumidityData.h"
#include "TextMessageGenerator.h"

#ifndef MQTT_COMMUNICATOR_H
#define MQTT_COMMUNICATOR_H

class MQTTCommunicator {

private:

    String _mqttBrokerURL;
    int _mqttPort;
    String _mqttUsername;
    String _mqttPassword;

    String _mqttPublishTopicTemperatureC;
    String _mqttPublishTopicHumidityPercent;

    bool _initialized;

    PubSubClient& _pubSubClient;
    TextMessageGenerator& _tMG;
    
    String randomMQTTClientId();

public:
    MQTTCommunicator(PubSubClient& pubSubClient, TextMessageGenerator& tMG, String mqttBrokerURL, int mqttPort, String mqttUsername, String mqttPassword, String mqttPublishTopicTemperatureC, String mqttPublishTopicHumidityPercent);

    ~MQTTCommunicator();

    void initialize();

    bool isConnectedToWifi();

    bool isConnectedToMQTTBroker();

    void mqttReceived(char* topic, byte* payload, unsigned int length);

    void connectToMQTTBroker();
    
    void reportTempAndHumidity(TemperatureAndHumidityData currentMeterData);

};

#endif