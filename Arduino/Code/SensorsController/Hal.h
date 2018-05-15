#ifndef HAL_H
#define HAL_H

#include "Common.h"

#define DHT_PIN 2
#define DHT_TYPE DHT22 

#define RAIN_POWER_PIN 6
#define RAIN_SENSOR_PIN A0

#define WATER_TEMP_SENSOR 7

class Air
{
	static DHT dht;
public:
	void begin();
	float readTemp();
	float readHumidity();
	int readRainLevel();
};

class Fishtank
{
	OneWire *oneWire;
	DallasTemperature *tempSensor;
	DeviceAddress *sensorAddress;
public:
	void begin();
	float readTemp();
	void findDevices();
};

#endif