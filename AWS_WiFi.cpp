#include<MQTT.h>
#include<MQTTClient.h>
#include<WiFiClientSecure.h>
#include "AmazonSecrets.h"
#include <EEPROM.h>
#include<ArduinoJson.h>
#include "AWS_WiFi.h"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(512);

void messageReceived(String &topic, String &payload){
  Serial.println("ïncoming: "+topic+"-"+payload);
  client.publish(topic+"/STATUS","SUCCESS");
  DynamicJsonDocument jsonDoc(1024);
  DeserializationError error = deserializeJson(jsonDoc,payload);
  String uuid = jsonDoc["uuid"];
  String userID = jsonDoc["userid"];
  String password = jsonDoc["password"];
  EEPROM.write(250,1); //set this flag after successfully receiving the broker credentials
  for(int i=0;i<100;i++){
    EEPROM.write(i+100,0);
  }

  for(int i=0;i<36;i++){
    EEPROM.write(i+100,uuid[i]);
  }

  for(int i=0;i<15;i++){
    EEPROM.write(i+140,userID[i]);
  }

  for(int i=0;i<15;i++){
    EEPROM.write(i+160,password[i]);
  }
  EEPROM.commit();
  delay(500);
  ESP.restart();
}
void AWS_WiFi_CallBack(){
  
 client.onMessage(messageReceived); 
 
}

void AWS_WiFi_SubTopic(String sub_topic){

  client.subscribe(sub_topic);
  
}

void AWS_WiFi_loop(void){

  client.loop();
  
}
void connectToAWS(){
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.begin(AWS_IOT_ENDPOINT,8883,net);

  int retries = 0;
  Serial.print("Connecting to AWS IOT");

  while(!client.connect(DEVICE_NAME) && retries < AWS_MAX_RECONNECT_TRIES);
  Serial.print(".");
  delay(100);
  retries++;

  if(!client.connected()){
    Serial.println("Timeout");
    return;
  }
  Serial.println("Connected");
}
