#ifndef TEMPERATURE_AND_HUMIDITY_DATA_H
#define TEMPERATURE_AND_HUMIDITY_DATA_H

#include <Arduino.h>
#include <stdlib.h>
#include <string>

class TemperatureAndHumidityData
{
	private:
		float _temperatureC;
        float _humidityPercent;

	public:
		TemperatureAndHumidityData(float temperatureC = 0, float humidityPercent = 0);

		float getTemperatureC();
        void setTemperatureC(float temperatureC);

		float getHumidityPercent();
        void setHumidityPercent(float humidityPercent);
		
        String toJSON();
};

#endif