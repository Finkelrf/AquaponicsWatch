#include "Common.h"

Air air;
Fishtank fishtank;
Feeder feeder;
Heater heater;
WaterLevel waterLevel;

void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  Serial.begin(115200);
  Serial.print("Version:");
  Serial.println(VERSION);

  air.begin();
  fishtank.begin();
  heater.begin();
  Communication::begin();

  while(!Communication::isWifiConnected()){
    Communication::checkForMsg();
  }

  digitalWrite(LED_BUILTIN,HIGH);
  
  Alarm::infiniteTimer(RAIN_SENSOR_INTERVAL,updateAirRainLevel,NULL, true, 1000);
  Alarm::infiniteTimer(AIR_HUMIDITY_INTERVAL,updateAirHumidity,NULL, true, 2000);
  Alarm::infiniteTimer(AIR_TEMPERATURE_INTERVAL, updateAirTemp, NULL, true, 3000);
  Alarm::infiniteTimer(FISHTANK_TEMP_INTERVAL,updateFishtankTemp,NULL, true, 4000);
  Alarm::infiniteTimer(FISHTANK_WATER_LEVEL_INTERVAL,updateWaterLvl,NULL, true, 5000);
  //Alarm::infiniteTimer(FEED_INTERVAL,feed,NULL, true, 5000);
  Alarm::infiniteTimer(HEAT_CONTROL_INTERVAL,runHeaterControl); 

}

void updateWaterLvl(){
#ifdef DEBUG
  Serial.println("Executando updateWaterLvl");
  Serial.println(waterLevel.read());
#endif
  if(Communication::isWifiConnected())
    Communication::sendMsg(FISHTANK_WATER_LEVEL_TOPIC,String(waterLevel.read(),DEC));
  else
    Alarm::oneTimeTimer(NO_WIFI_RETRY_INTERVAL,updateWaterLvl);
  
}

void runHeaterControl(void *){
  bool isHeaterOn = heater.heaterControl(fishtank.readTemp());
  if(isHeaterOn){
    Communication::sendMsg(HEATER_STATUS_TOPIC,"1");
  }else{
    Communication::sendMsg(HEATER_STATUS_TOPIC,"0");
  }
}

void feed(void *){
  feeder.feed();
  Communication::sendMsg(FEED_TOPIC,"1");
}

void updateFishtankTemp(void *){
#ifdef DEBUG
  Serial.println("Executando updateFishtankTemp");
#endif
  if(Communication::isWifiConnected())
    Communication::sendMsg(FISHTANK_TEMP_TOPIC,String(fishtank.readTemp(),DEC));
  else
    Alarm::oneTimeTimer(NO_WIFI_RETRY_INTERVAL,updateFishtankTemp);
}

void updateAirTemp(void *){
#ifdef DEBUG
  Serial.println("Executando updateAirTemp");
#endif
  if(Communication::isWifiConnected())
    Communication::sendMsg(AIR_TEMP_TOPIC, String(air.readTemp(),DEC));
  else
    Alarm::oneTimeTimer(NO_WIFI_RETRY_INTERVAL,updateAirTemp);
}

void updateAirHumidity(void *){
#ifdef DEBUG
  Serial.println("Executando updateAirHumidity");
#endif
  if(Communication::isWifiConnected())
    Communication::sendMsg(AIR_HUMIDITY_TOPIC, String(air.readHumidity(),DEC));
  else
    Alarm::oneTimeTimer(NO_WIFI_RETRY_INTERVAL,updateAirHumidity);
}

void updateAirRainLevel(void *){
#ifdef DEBUG
  Serial.println("Executando updateAirRainLevel");
#endif
  if(Communication::isWifiConnected())
    Communication::sendMsg(AIR_RAIN_TOPIC, String(air.readRainLevel(),DEC));
  else
    Alarm::oneTimeTimer(NO_WIFI_RETRY_INTERVAL,updateAirRainLevel);
}

void loop(){
  Alarm::checkTimers();
  Communication::checkForMsg();
}

/*
#include <SoftwareSerial.h>
#include <Ultrasonic.h>
 
#define DEBUG
#define VERSION "Version_0.1"
#define MAX_STRING_LENGTH 50
#define BEGIN_OF_COMMAND_CHAR '\r'
#define SEPARATION_CHAR '|'
#define END_OF_COMMAND_CHAR '\n'

#define NUMBER_OF_SENSORS 1

#define SENSOR_RAIN 0
#define SENSOR_RAIN_TOPIC "fishtank/rain"
#define SENSOR_RAIN_DEFAULT_INTERVAL 5000 //10 minutes
#define SENSOR_RAIN_ANALOG_INPUT A0
#define SENSOR_RAIN_POWER_PIN 6

#define SENSOR_DISTANCE 1
#define SENSOR_DISTANCE_TOPIC "fishtank/distance"
#define SENSOR_DISTANCE_DEFAULT_INTERVAL 10000 //10 seconds
#define SENSOR_DISTANCE_TRIGGER_PIN 6
#define SENSOR_DISTANCE_ECHO_PIN 5
#define DISTANCE_SENSOR_SAMPLES 100

char msg[MAX_STRING_LENGTH];
int i = 0;

SoftwareSerial SoftSerial(10, 11); //RX, TX
Ultrasonic ultrasonic(SENSOR_DISTANCE_TRIGGER_PIN, SENSOR_DISTANCE_ECHO_PIN);    

struct sensorStruct {
   unsigned long previousMillis;
   unsigned long interval;
   void (*execution)();
};

struct sensorStruct sensor[NUMBER_OF_SENSORS];

void initSensors(){
  //config rain sensor
  sensor[SENSOR_RAIN].previousMillis = millis();
  sensor[SENSOR_RAIN].interval = SENSOR_RAIN_DEFAULT_INTERVAL;
  sensor[SENSOR_RAIN].execution = sendRainValue;
  pinMode(SENSOR_RAIN_POWER_PIN,OUTPUT);
  digitalWrite(SENSOR_RAIN_POWER_PIN,LOW);

  //config distance sensor
  pinMode(SENSOR_DISTANCE_TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(SENSOR_DISTANCE_ECHO_PIN, INPUT); // Sets the echoPin as an Input
  sensor[SENSOR_DISTANCE].previousMillis = millis();
  sensor[SENSOR_DISTANCE].interval = SENSOR_DISTANCE_DEFAULT_INTERVAL;
  sensor[SENSOR_DISTANCE].execution = sendDistanceValue;
}

float sampleDistance(){
  float measures[DISTANCE_SENSOR_SAMPLES];
  for(int i=0;i<DISTANCE_SENSOR_SAMPLES;i++){
    long microsec = ultrasonic.timing();
    measures[i] = ultrasonic.convert(microsec, Ultrasonic::CM);
  }
  float total=0;
  for(int i=0;i<DISTANCE_SENSOR_SAMPLES;i++){
    total += measures[i];
  }
  total = total/DISTANCE_SENSOR_SAMPLES;
  return total;
}


void sendRainValue(){
  digitalWrite(SENSOR_RAIN_POWER_PIN,HIGH);
  delay(500);
  //SoftSerial.print(BEGIN_OF_COMMAND_CHAR);
  SoftSerial.print(SENSOR_RAIN_TOPIC);
  SoftSerial.print("|");
  SoftSerial.println(1024-analogRead(SENSOR_RAIN_ANALOG_INPUT));
  digitalWrite(SENSOR_RAIN_POWER_PIN,LOW);
}

void sendDistanceValue(){
  //SoftSerial.print(BEGIN_OF_COMMAND_CHAR);
  SoftSerial.print(SENSOR_DISTANCE_TOPIC);
  SoftSerial.print("|");
  float distance = sampleDistance();
  SoftSerial.println(distance);
  //#ifdef DEBUG
    Serial.print("Distance: ");
    Serial.println(distance);
  //#endif
}

void setup() {
  Serial.begin(9600);
  SoftSerial.begin(9600);
  Serial.println(VERSION);
  initSensors();  
}

void parseMsg(String msg){
  int commandEndIndex = msg.indexOf(SEPARATION_CHAR);
  int deviceEndIndex = msg.indexOf(SEPARATION_CHAR,commandEndIndex+1);
  int valueEndIndex = msg.indexOf(END_OF_COMMAND_CHAR,deviceEndIndex+1);
  String command = msg.substring(0,commandEndIndex);
  String device = msg.substring(commandEndIndex+1,deviceEndIndex);
  String value = msg.substring(deviceEndIndex+1,valueEndIndex);

  unsigned long *intervalPointer;
  if(command.equals("interval")){
    if(device.equals("rain")){
      intervalPointer = &sensor[SENSOR_RAIN].interval;
    }
    if(device.equals("distance")){
      intervalPointer = &sensor[SENSOR_DISTANCE].interval;
    }
    *intervalPointer = value.toInt();
  }
  #ifdef DEBUG
    Serial.print("command: ");
    Serial.println(command);
    Serial.print("device: ");
    Serial.println(device);
    Serial.print("value: ");
    Serial.println(value);
    Serial.print("Rain interval: ");
    Serial.println(sensor[SENSOR_RAIN].interval);
    Serial.print("Distance interval: ");
    Serial.println(sensor[SENSOR_DISTANCE].interval);
  #endif
}

void readSerial(){
  while (SoftSerial.available() > 0) {
    char charRead = SoftSerial.read();
    if(charRead == BEGIN_OF_COMMAND_CHAR){
      i = 0;
      memset(msg, 0, sizeof(msg));
    }else if(charRead == END_OF_COMMAND_CHAR){
      #ifdef DEBUG
        Serial.print("Msg received: ");
        Serial.println(msg);
      #endif
      parseMsg(String(msg));
      i = 0;
      memset(msg, 0, sizeof(msg));
    }else{
      msg[i] = charRead;
      i++;
    }
  }
}

void loop() {
  for(int i=0;i<NUMBER_OF_SENSORS;i++){
    unsigned long currentMillis = millis();
    if (currentMillis -  sensor[i].previousMillis >= sensor[i].interval) {
       sensor[i].previousMillis = currentMillis;
       sensor[i].execution();      
    }
  }

  //readSerial();
  if(SoftSerial.available()> 0){
    Serial.print((char)SoftSerial.read());
  }
}
*/