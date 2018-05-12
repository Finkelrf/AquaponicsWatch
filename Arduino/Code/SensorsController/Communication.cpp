#include "Communication.h"

Communication::Communication(SoftwareSerial *ss){
	softSerial = ss;
	softSerial->begin(SOFT_SERIAL_BAUD);
};

Communication::sendMsg(String topic, String msg){
	softSerial->print(topic);
	softSerial->print(SEPARATION_CHAR);
	softSerial->print(msg);
	softSerial->print(END_OF_COMMAND_CHAR);

#ifdef DEBUG
	Serial.print(topic);
	Serial.print(SEPARATION_CHAR);
	Serial.print(msg);
	Serial.print(END_OF_COMMAND_CHAR);
#endif
};