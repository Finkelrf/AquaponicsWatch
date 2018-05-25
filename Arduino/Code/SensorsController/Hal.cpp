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
	OneWire newOneWire(WATER_TEMP_SENSOR); 
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

Servo Feeder::myservo;

void Feeder::feed(){
	Feeder::myservo.attach(FEEDER_PIN); 
	Feeder::myservo.write(5);
	Alarm::oneTimeTimer(FEEDING_TIME, Feeder::stopFeed);
}

static void Feeder::stopFeed(){
	Feeder::myservo.detach();
}

void Heater::begin(){
	pinMode(HEATER_PIN,OUTPUT);
}

void Heater::turnOn(){
	digitalWrite(HEATER_PIN,HIGH);
}
void Heater::turnOff(){
	digitalWrite(HEATER_PIN,LOW);
}

// return true if heater is on;
bool Heater::heaterControl(float temperature){
	if(temperature>=HEATER_TEMPERATURE_THRESHOLD){
#ifdef DEBUG
		Serial.println("Turning heater off");
#endif
		Heater::turnOff();
		return false;
	}else{
#ifdef DEBUG
		Serial.println("Turning heater on");
#endif
		Heater::turnOn();
		return true;
	}
}

double WaterLevel::read(){
	int sensorValue = analogRead(WATER_LEVEL_PIN);
	// map it to degrees
	int degrees = map(sensorValue, WATER_LEVEL_CALIBRATION_MIN, WATER_LEVEL_CALIBRATION_MAX, 0, 90);
	double rad = double(degrees) * 1000 / 57296;
	double waterLvl = sin(rad)*WATER_LEVEL_MAX_DEPTH;
	return waterLvl;
}