#include <string>

#ifndef TEXT_MESSAGE_GENERATOR_H
#define TEXT_MESSAGE_GENERATOR_H

class TextMessageGenerator
{
	private:
		bool _initialized;

		String _firmwareVersion;
		int _serialMonitorBaud;

		String _credentialsJSONString;
		String _mqttBrokerURL;
		int _mqttPort;
		String _mqttUsername;
		String _mqttPassword;

		const String HEADER_UNDERLINE = String("====================================\n");

	public:
		TextMessageGenerator(String firmwareVersion, int serialMonitorBaud, String mqttBrokerURL, int mqttPort, String mqttUsername, String mqttPassword);

		void initialize();


		String firmwareVersion();
		String serialMonitorBaud();

		String credentialsJSONString();
		String mqttBrokerURL();
		String mqttPort();
		String mqttUsername();
		String mqttPassword();


		String deviceInfo();

		String lookingForWifiConnection();
		String wifiConnectionEstablished(String SSID, String assignedIP);
		String wifiCredentialsJSONParsingError(String error);

		String connectingToMQTTServer(String clientId);
		String mQTTServerConnectionEstablished(String assignedIP);
		String mQTTServerConnectionFailed(int state);
		String messageArrived(char* topic, byte* payload, unsigned int length);

		String hatchWereManuallyPositioned(String direction);
		String hatchArrivedToItsDestination(String position);


};

#endif