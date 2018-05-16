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

static const String AIR_TEMP_TOPIC = "aquaponics/airtemperature";
static const String AIR_HUMIDITY_TOPIC = "aquaponics/airhumidity";
static const String AIR_RAIN_TOPIC = "aquaponics/rain";
static const String FISHTANK_TEMP_TOPIC = "aquaponics/fishtanktemperature";
static const String FEED_TOPIC = "aquaponics/feed";

class Communication
{
	static SoftwareSerial softSerial;
	static char inputBuffer[];
	static uint8_t bufferCursor;
public:
	static void begin();
	static void sendMsg(String topic, String msg);
	static void checkForMsg();
};


#endif
