#include "Communication.h"

SoftwareSerial Communication::softSerial = SoftwareSerial (SOFT_SERIAL_RX_PIN,SOFT_SERIAL_TX_PIN);
uint8_t Communication::bufferCursor = 0;
char Communication::inputBuffer[255];

void Communication::begin(){
	Communication::softSerial.begin(SOFT_SERIAL_BAUD);
}

void Communication::sendMsg(String topic, String msg){
	Communication::softSerial.print(topic);
	Communication::softSerial.print(SEPARATION_CHAR);
	Communication::softSerial.print(msg);
	Communication::softSerial.print(END_OF_COMMAND_CHAR);

	Serial.print(topic);
	Serial.print(SEPARATION_CHAR);
	Serial.print(msg);
	Serial.print(END_OF_COMMAND_CHAR);
};

void Communication::checkForMsg(){
	while (Communication::softSerial.available()) {
		char input = (char)Communication::softSerial.read();
	    Communication::inputBuffer[Communication::bufferCursor++] = input;
	    Serial.print(input);
  }
}