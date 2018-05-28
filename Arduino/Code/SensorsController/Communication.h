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

static const char AIR_TEMP_TOPIC[] = "aquaponics/airtemperature";
static const char AIR_HUMIDITY_TOPIC[] = "aquaponics/airhumidity";
static const char AIR_RAIN_TOPIC[] = "aquaponics/rain";
static const char FISHTANK_TEMP_TOPIC[] = "aquaponics/fishtanktemperature";
static const char FEED_TOPIC[] = "aquaponics/feed";
static const char FISHTANK_WATER_LEVEL_TOPIC[] = "aquaponics/fishtankwaterlevel";
static const char HEATER_STATUS_TOPIC[] = "aquaponics/fishtankheaterstatus";

class Communication
{
	static SoftwareSerial softSerial;
	static char inputBuffer[];
	static uint8_t beginCursor;
	static uint8_t endCursor;
	static uint8_t messagesOnBuffer;
	static bool wifiConnected;

public:
	static void begin();
	static void sendMsg(char *topic, char *msg);
	static void checkForMsg();
	static bool isWifiConnected();
};


#endif
