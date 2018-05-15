#include "Hal.h"

DHT Air::dht = DHT(DHT_PIN, DHT_TYPE);

void Air::begin(){
	pinMode(RAIN_POWER_PIN,OUTPUT);
	Air::dht.begin();
};

float Air::readTemp(){
	return Air::dht.readTemperature();
};

float Air::readHumidity(){
	return Air::dht.readHumidity();
};

int Air::readRainLevel(){
	digitalWrite(RAIN_POWER_PIN,HIGH);
	delay(10);
	int rainLevel = analogRead(RAIN_SENSOR_PIN);
	digitalWrite(RAIN_POWER_PIN,LOW);
	return 1024-rainLevel;
};

void Fishtank::begin(){
	OneWire newOneWire(WATER_TEMP_SENSOR); //
	oneWire = &newOneWire;
	DallasTemperature sensors(oneWire);
	tempSensor = &sensors;
	DeviceAddress sensor1;
	tempSensor->begin();
	tempSensor->getAddress(sensor1, 0); 
  	sensorAddress = &sensor1;
  	tempSensor->requestTemperatures();
}


float Fishtank::readTemp(){
  	tempSensor->requestTemperatures();
	return tempSensor->getTempC(*sensorAddress);
}
