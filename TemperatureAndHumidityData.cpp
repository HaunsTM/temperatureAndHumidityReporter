#include "TemperatureAndHumidityData.h"

TemperatureAndHumidityData::TemperatureAndHumidityData(float temperatureC, float humidityPercent): 
    _temperatureC(temperatureC), _humidityPercent(humidityPercent) {};

float TemperatureAndHumidityData::getTemperatureC()
{
    return _temperatureC;
}

void TemperatureAndHumidityData::setTemperatureC(float temperatureC)
{
    _temperatureC = temperatureC;
}

float TemperatureAndHumidityData::getHumidityPercent()
{
    return _humidityPercent;
}

void TemperatureAndHumidityData::setHumidityPercent(float humidityPercent)
{
    _humidityPercent = humidityPercent;
}

String TemperatureAndHumidityData::toJSON()
{
    const String message =
        String("{") +
            String("temperatureC : ") + String(_temperatureC, 2) +
            String(", humidityPercent : ") + String(_humidityPercent, 2) +
        String("}");
    return message;
}