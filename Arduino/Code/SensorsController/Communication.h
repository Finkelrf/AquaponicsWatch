#ifndef COMMUNICATION_H	
#define COMMUNICATION_H

#include "common.h"

#define MAX_STRING_LENGTH 50
#define BEGIN_OF_COMMAND_CHAR '\r'
#define SEPARATION_CHAR '|'
#define END_OF_COMMAND_CHAR '\n'

#define SOFT_SERIAL_RX_PIN 10
#define SOFT_SERIAL_TX_PIN 11
#define SOFT_SERIAL_BAUD 9600

static const String AIR_TEMP_TOPIC = "aquaponics/air/temperature";
static const String AIR_HUMIDITY_TOPIC = "aquaponics/air/humidity";
static const String AIR_RAIN_TOPIC = "aquaponics/air/rain";

class Communication
{
	SoftwareSerial *softSerial;
public:
	Communication(SoftwareSerial *ss);
	sendMsg(String topic, String msg);
};


#endif
