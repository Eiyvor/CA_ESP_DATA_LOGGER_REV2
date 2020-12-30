#include<ESPmDNS.h>
#include<WiFi.h>
#include<WebServer.h>
#include "RelayControls.h"

 WebServer relay_server(80);
const uint8_t relay_1 = 33;
const uint8_t relay_2 = 25;
const uint8_t relay_3 = 26;
const uint8_t relay_4 = 27;

bool relay_1_status = LOW;
bool relay_2_status = LOW;
bool relay_3_status = LOW;
bool relay_4_status = LOW;

 void relay_pin_init(){

  pinMode(relay_1,OUTPUT);
  pinMode(relay_2,OUTPUT);
  pinMode(relay_3,OUTPUT);
  pinMode(relay_4,OUTPUT);
  
 }

void relay_server_handle(void){

 

  relay_server.on("/",handle_OnConnect);
  relay_server.on("/relay1On",handle_relay1on);
  relay_server.on("/relay1Off",handle_relay1off);
  relay_server.on("/relay2On",handle_relay2on);
  relay_server.on("/relay2Off",handle_relay2off);
  relay_server.on("/relay3On",handle_relay3on);
  relay_server.on("/relay3Off",handle_relay3off);
  relay_server.on("/relay4On",handle_relay4on);
  relay_server.on("/relay4Off",handle_relay4off);
  relay_server.onNotFound(handle_NotFound);

  relay_server.begin();
  Serial.println("HTTP server started");
  
}

void relay_mdns_init(){
  if(!MDNS.begin("cybene")) {
     Serial.println("Error starting mDNS");
     return;
  }
   Serial.println(WiFi.localIP());
}
void relay_server_loop(){
  
  relay_server.handleClient();
  if(relay_1_status){
    digitalWrite(relay_1,HIGH);
    Serial.println("relay 1 ON");
  }
  else
  { digitalWrite(relay_1,LOW);}
  
if(relay_2_status){
    digitalWrite(relay_2,HIGH);
    Serial.println("relay 2 ON");
  }
  else
  { digitalWrite(relay_2,LOW);}
  
  if(relay_3_status){
    digitalWrite(relay_3,HIGH);
    Serial.println("relay 3 ON");
  }
  else
  { digitalWrite(relay_3,LOW);}
  
  if(relay_4_status){
    digitalWrite(relay_4,HIGH);
    Serial.println("relay 4 ON");
  }
  else
  { digitalWrite(relay_4,LOW);}
}
void handle_OnConnect(){
  
  relay_1_status = LOW;
  relay_2_status = LOW;
  relay_3_status = LOW;
  relay_4_status = LOW;

  Serial.println("relay1 status: OFF |relay2 status: OFF |relay3 status: OFF |relay4 status: OFF");
  relay_server.send(200,"text/html",SendHTML(relay_1_status,relay_2_status,relay_3_status,relay_4_status));
}

void handle_relay1on(){
  relay_1_status = HIGH;
  Serial.println("relay1 status: ON");
  relay_server.send(200,"text/html",SendHTML(true,relay_2_status,relay_3_status,relay_4_status));
}

void handle_relay1off(){
  relay_1_status = LOW;
  Serial.println("relay1 status: OFF");
  relay_server.send(200,"text/html",SendHTML(false,relay_2_status,relay_3_status,relay_4_status));
}

void handle_relay2on(){
  relay_2_status = HIGH;
  Serial.println("relay2 status: ON");
  relay_server.send(200,"text/html",SendHTML(relay_1_status,true,relay_3_status,relay_4_status));
}

void handle_relay2off(){
  relay_2_status = LOW;
  Serial.println("relay2 status: OFF");
  relay_server.send(200,"text/html",SendHTML(relay_1_status,false,relay_3_status,relay_4_status));
}

void handle_relay3on(){
  relay_3_status = HIGH;
  Serial.println("relay3 status: ON");
  relay_server.send(200,"text/html",SendHTML(relay_1_status,relay_2_status,true,relay_4_status));
}

void handle_relay3off(){
  relay_3_status = LOW;
  Serial.println("relay3 status: OFF");
  relay_server.send(200,"text/html",SendHTML(relay_1_status,relay_2_status,false,relay_4_status));
}

void handle_relay4on(){
  relay_4_status = HIGH;
  Serial.println("relay4 status: ON");
  relay_server.send(200,"text/html",SendHTML(relay_1_status,relay_2_status,relay_3_status,true));
}

void handle_relay4off(){
  relay_4_status = LOW;
  Serial.println("relay4 status: OFF");
  relay_server.send(200,"text/html",SendHTML(relay_1_status,relay_2_status,relay_3_status,false));
}
void handle_NotFound(){
  relay_server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t relay1stat,uint8_t relay2stat,uint8_t relay3stat,uint8_t relay4stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>RELAY Controller</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #a569bd;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #a569bd;}\n";
  ptr +=".button-on:active {background-color: #bb8fce;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Cybene Automations</h1>\n";
    ptr +="<h3>Relay Controller</h3>\n";
  
   if(relay1stat)
  {ptr +="<p>Relay1 Status: ON</p><a class=\"button button-off\" href=\"/relay1Off\">OFF</a>\n";}
  else
  {ptr +="<p>Relay1 Status: OFF</p><a class=\"button button-on\" href=\"/relay1On\">ON</a>\n";}

  if(relay2stat)
  {ptr +="<p>Relay2 Status: ON</p><a class=\"button button-off\" href=\"/relay2Off\">OFF</a>\n";}
  else
  {ptr +="<p>Relay2 Status: OFF</p><a class=\"button button-on\" href=\"/relay2On\">ON</a>\n";}

 if(relay3stat)
  {ptr +="<p>Relay3 Status: ON</p><a class=\"button button-off\" href=\"/relay3Off\">OFF</a>\n";}
  else
  {ptr +="<p>Relay3 Status: OFF</p><a class=\"button button-on\" href=\"/relay3On\">ON</a>\n";}

  if(relay4stat)
  {ptr +="<p>Relay4 Status: ON</p><a class=\"button button-off\" href=\"/relay4Off\">OFF</a>\n";}
  else
  {ptr +="<p>Relay4 Status: OFF</p><a class=\"button button-on\" href=\"/relay4On\">ON</a>\n";}
 
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
