#include "Hal.h"

void Air::begin(){
	pinMode(RAIN_POWER_PIN,OUTPUT);
	DHT newDht(DHT_PIN, DHT_TYPE);
	dht = &newDht;
	dht->begin();
};

float Air::readTemp(){
	return dht->readTemperature();
};

float Air::readHumidity(){
	return dht->readHumidity();
};

int Air::readRainLevel(){
	digitalWrite(RAIN_POWER_PIN,HIGH);
	delay(10);
	int rainLevel = analogRead(RAIN_SENSOR_PIN);
	digitalWrite(RAIN_POWER_PIN,LOW);
	return 1024-rainLevel;
};

