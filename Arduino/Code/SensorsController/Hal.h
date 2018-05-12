#ifndef HAL_H
#define HAL_H

#include "Common.h"

#define DHT_PIN 8
#define DHT_TYPE DHT22 

#define RAIN_POWER_PIN 6
#define RAIN_SENSOR_PIN A0

#define WATER_TEMP_SENSOR 7

class Air
{
	DHT *dht;
public:
	void begin();
	float readTemp();
	float readHumidity();
	int readRainLevel();
};

class FishTank
{
	
public:
	void begin();
	float readTemp();
};

#endif