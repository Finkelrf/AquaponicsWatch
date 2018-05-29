#ifndef HAL_H
#define HAL_H

#include "Common.h"

#define DHT_PIN 2
#define DHT_TYPE DHT22 

#define RAIN_POWER_PIN 6
#define RAIN_SENSOR_PIN A0

#define WATER_TEMP_SENSOR 7

#define FEEDER_PIN 9
#define FEEDING_TIME 5000

#define HEATER_PIN 5
#define HEATER_TEMPERATURE_THRESHOLD 18.0f

#define WATER_LEVEL_PIN A1
#define WATER_LEVEL_MAX_DEPTH 22 //cm
#define WATER_LEVEL_CALIBRATION_MIN 200
#define WATER_LEVEL_CALIBRATION_MAX 580
#define WATER_LEVEL_FISHTANK_HEIGHT 50.5

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

class Feeder
{
	static Servo myservo;
public:
	void feed();
	static void stopFeed();
};

class Heater
{
public:
	void begin();
	void turnOn();
	void turnOff();
	bool heaterControl(float temperature);
};

class WaterLevel
{
public:
	double read();
};

#endif