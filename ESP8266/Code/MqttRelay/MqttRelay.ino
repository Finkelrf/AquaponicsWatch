#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

//#define DEBUG
#define VERSION "Version_0.1"

const char* ssid = "MY_SSID";
const char* password = "MY_PASSWORD";
const char* mqtt_server = "192.168.1.8";

#define CONFIGURATION_TOPIC "sensors/config"
#define MAX_STRING_LENGTH 50

WiFiClient espClient;
PubSubClient client(espClient);
char msg[MAX_STRING_LENGTH];
int i = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  #ifdef DEBUG
    Serial.println("Callback called");
  #endif
  Serial.print('\r');
  for (int i = 0; i < length; i++) {
    //forward messsage to arduino
    Serial.print((char)payload[i]);
  }
  Serial.print('\n');
}

void reconnect() {
  #ifdef DEBUG
    Serial.println("Reconnect called");
  #endif
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      client.publish("logs","ESP8266 reconnected.");
      client.subscribe(CONFIGURATION_TOPIC);
    } else {
      delay(5000);
    }
  }
}

void setupMqtt(){
  #ifdef DEBUG
    Serial.println("Setup mqtt called");
  #endif
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (!client.connected()) {
    reconnect();
  }
}

void setup_wifi() {
  // We start by connecting to a WiFi network
  #ifdef DEBUG
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  #endif
  WiFi.begin(ssid, password);

  int wifiStatus = WiFi.status();
  bool wifiConnected = false;
  while (!wifiConnected) {
   delay(2000);
   wifiStatus = WiFi.status();
    switch(wifiStatus){
      case WL_IDLE_STATUS:
        Serial.println("wifistatus|idle");
        break;
      case WL_NO_SSID_AVAIL:
        Serial.println("wifistatus|no ssid available");
        break;
      case WL_CONNECTED:
        wifiConnected = true;
        Serial.println("wifistatus|connected");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("wifistatus|failed");
        break;
      case WL_CONNECTION_LOST:
        Serial.println("wifistatus|connection lost");
        break;
      case WL_DISCONNECTED:
        Serial.println("wifistatus|disconnected");
        break;
    }
  }
  #ifdef DEBUG
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  #endif
  digitalWrite(2,LOW);
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  #ifdef DEBUG
    Serial.println(VERSION);
  #endif
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  setup_wifi();
  setupMqtt();
  
}

void msgParser(char *msg, char *topic, char *payload){
  #ifdef DEBUG
    //Serial.println("Msg parser called");
  #endif
// message format => my/topic|My payload
  int msgIndex = 0;
  int topicIndex = 0;
  int payloadIndex = 0;
  char separator = '|';
  while(msg[msgIndex]!=separator){
    topic[topicIndex++] = msg[msgIndex++];
  }
  topic[topicIndex] = '\0';
  msgIndex++;
  while(msgIndex<MAX_STRING_LENGTH){
    payload[payloadIndex++] = msg[msgIndex++];
  }
  payload[payloadIndex] = '\0';

}

void readSerial(){
  while (Serial.available() > 0) {
    char charRead = Serial.read();
    if(charRead == '\n'){
      #ifdef DEBUG
        Serial.println("End of command reached");
        Serial.println(msg);
      #endif
      i = 0;
      char topic[MAX_STRING_LENGTH];
      char payload[MAX_STRING_LENGTH];
      msgParser(msg,topic,payload);
      if (!client.connected()) {
        reconnect();
      }
      client.publish(topic, payload);
      memset(msg, 0, sizeof(msg));
      memset(topic, 0, sizeof(topic));
      memset(payload, 0, sizeof(payload));
    }else{
      msg[i] = charRead;
      i++;
    }
  }
}

void loop() {    
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  readSerial();
}
