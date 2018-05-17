#include "Communication.h"

SoftwareSerial Communication::softSerial = SoftwareSerial (SOFT_SERIAL_RX_PIN,SOFT_SERIAL_TX_PIN);
uint8_t Communication::beginCursor = 0;
uint8_t Communication::endCursor = 0;
char Communication::inputBuffer[255];
uint8_t Communication::messagesOnBuffer = 0;
bool Communication::wifiConnected = false;

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
		/*char charRead = (char)Communication::softSerial.read();
		Communication::inputBuffer[Communication::endCursor++] = charRead;

		if(charRead == END_OF_COMMAND_CHAR){
			Communication::messagesOnBuffer++;
		}*/
		char wifiStatus = (char)Communication::softSerial.read();
		if(wifiStatus == '3'){
#ifdef DEBUG
			Serial.println("Wifi Connected");
#endif
			Communication::wifiConnected = true;
		}else{
#ifdef DEBUG
			Serial.print("Wifi Not Connected: ");
			Serial.println(wifiStatus);
#endif
			Communication::wifiConnected = false;
		}
  	}
}

bool Communication::isWifiConnected(){
	return Communication::wifiConnected;
}


