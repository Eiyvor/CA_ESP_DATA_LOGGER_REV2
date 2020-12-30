#include "Arduino.h"
#include "gsm.h"
#include<TinyGsmClient.h>
#include<PubSubClient.h>
#include <Update.h>
#include<EEPROM.h>
#include<ArduinoJson.h>

#include "brokercred.h"
#include "energy_meter.h"
#include "Sensor.h"
#include "particle_sensor.h"

static void json_array(float humidity,float temperature,uint16_t pressure,uint16_t R_phase_v,uint16_t Y_phase_v,uint16_t B_phase_v,uint16_t frequency,uint16_t R_phase_pf,
                       uint16_t Y_phase_pf,uint16_t B_phase_pf,char sig_strength[20]);

const char apn[] = "";
const char gprsUser[] = "";
const char gprsPass[] = "";
const char* mqtt_server = "broker.cybene.in";
char jsonBuffer[512];
bool gprs_connected = false;

float _temperature = 0;
float _humidity = 0;
float _pressure = 0;
uint16_t R_volt = 0;
uint16_t B_volt = 0; 
uint16_t Y_volt = 0;
uint16_t _frequency = 0;
uint16_t R_pf = 0;
uint16_t B_pf = 0;
uint16_t Y_pf = 0;
char signalQuality[20];

float current_firmware_version = 1.0;
extern String name;
extern String gsm_mqtt_sub_topic;
TinyGsm modem(SerialAT);
TinyGsmClient gsm_client(modem,0);
//     TinyGsmClientSecure gsm_client(modem,0);     uncomment this line while using secure connection
PubSubClient gsm_mqtt(gsm_client);

void gsm_modem_init(void){

  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);

  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);
  
}

void gsm_setup_gprs(void){
  SerialAT.begin(9600,SERIAL_8N1,MODEM_RX,MODEM_TX);
    delay(6000);
    Serial.println("initalizing modem..");
    modem.restart();
    String modemInfo = modem.getModemInfo();
    Serial.print("modem info:");
    Serial.println(modemInfo);
    if(GSM_PIN && modem.getSimStatus()!= 3){
            modem.simUnlock(GSM_PIN);
     }
    Serial.print("Waiting for network...");
    if(!modem.waitForNetwork(240000l)){
            delay(10000);
            return;
    }
    Serial.println("Success");
    if(modem.isNetworkConnected()){
            Serial.println("Network connected");
     }
    Serial.print(F("Connecting to APN"));
    Serial.print(apn);
    if(!modem.gprsConnect(apn,gprsUser,gprsPass)){
            Serial.println("fail");
            delay(1000);
            return;
    }
    Serial.println("Success");
    if(modem.isGprsConnected()){
            Serial.println("GPRS connected");
            gprs_connected = true;
    }
}

void gsm_mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("gsm mqtt callback");
  Serial.println(topic);
  // if((String)topic == name+"/up"){
    if((String)topic == "both_directions"){
      Serial.println("received");
     DynamicJsonDocument jsonDoc(1024);
     DeserializationError error = deserializeJson(jsonDoc,payload);
     if(error) Serial.println("failed to deserialize");
     String update_url = jsonDoc["url"];
     String firmware_version = jsonDoc["version"];
     
    //  if(current_firmware_version > firmware_version.toFloat()){
          EEPROM.write(252,update_url.length());
          for(int i=0;i<update_url.length();i++){

                   EEPROM.write(i+300,update_url[i]);
                   
  }
         EEPROM.write(255,1); //set this flag to start ota update
         EEPROM.commit();     //always make sure to call this function after a read/write operation in EEPROM 
    //  }
  }
}
void gsm_mqtt_set_server_and_callback(void){
    Serial.print("Setting server:");
    Serial.println(mqtt_server);
    gsm_mqtt.setServer(mqtt_server,1883);
    gsm_mqtt.setCallback(gsm_mqtt_callback);
}
bool gsm_mqtt_client(void){

     char broker_id[15];
     char broker_password[15];
     
     
     
     read_userid().toCharArray(broker_id,15);
     read_password().toCharArray(broker_password,15);
     Serial.print("Connecting to:");
     Serial.println(mqtt_server);
    
     boolean status = gsm_mqtt.connect("Cybene_MQTT_Client",broker_id,broker_password);

     if(status == false){
      Serial.println("failed");
      EEPROM.write(250,0); // reset this flag if the client fails to connect to the broker
      EEPROM.commit();
      return false;
     }
     Serial.println("Success");
  return true;
}
void gsm_mqtt_loop(void){
  
  gsm_mqtt.loop();
  
}
bool check_gsm_mqtt_connected(void){
  bool stat;
  stat = gsm_mqtt.connected();
  return stat;
}
void gsm_send_json(void){

    StaticJsonDocument<1024> jsonDoc;
    JsonObject vibrationObj = jsonDoc.createNestedObject("Vibration");

    vibrationObj["acc.x"] = 0;
    vibrationObj["acc.y"] = 0;
    vibrationObj["acc.z"] = 0;

    jsonDoc["humidity"] = 0;
    jsonDoc["thing_name"] = "";
    jsonDoc["uuid"] = read_uuid();

    serializeJson(jsonDoc,jsonBuffer);
    gsm_mqtt.publish("topic name",jsonBuffer);

  
}

static void json_array(float humidity,float temperature,uint16_t pressure,uint16_t R_phase_v,uint16_t Y_phase_v,uint16_t B_phase_v,uint16_t frequency,uint16_t R_phase_pf,
                       uint16_t Y_phase_pf,uint16_t B_phase_pf,char sig_strength[20]){

           
                            StaticJsonDocument<1024> jsonDoc;

                            JsonObject SensorObj = jsonDoc.createNestedObject("sensor values");
                            
                            SensorObj["Humidity"] = humidity;
                            SensorObj["pressure"] = pressure ;
                            SensorObj["temperature"] = temperature;
                            SensorObj["Current"] = Current_Sensor();
                            SensorObj["CO"] = read_CO_sensor();
                            SensorObj["Gas"] = read_gas_sensor();
                            SensorObj["dust_pm_1.0"] = dust_sensor_pm_1_0(); 
                            SensorObj["dust_pm_2.5"] = dust_sensor_pm_2_5(); 
                            SensorObj["dust_pm_10"] = dust_sensor_pm_10();

                            jsonDoc["R_phase_vlotage"] = R_phase_pf;
                            jsonDoc["Y_phase_vlotage"] = Y_phase_v;
                            jsonDoc["B_phase_vlotage"] = B_phase_v;
                            jsonDoc["Frequency"] = frequency;
                            jsonDoc["R_phase_PF"] = R_phase_pf;
                            jsonDoc["Y_phase_PF"] = Y_phase_pf;
                            jsonDoc["B_phase_PF"] = B_phase_pf;

                            jsonDoc["signal strength"] = sig_strength;
                            jsonDoc["thing_name"] = "";
                            jsonDoc["uuid"] = read_uuid();

                            serializeJson(jsonDoc, jsonBuffer);
                            gsm_mqtt.publish("both_directions",jsonBuffer);
                            gsm_mqtt_subscribe(name);
                            }


void mqtt_send_to_broker(void){
       //     pms90003m_init();
        if(modem.getSignalQuality() > 15 && modem.getSignalQuality() <= 19){

                  strcpy(signalQuality,"Good");
          }
          else if(modem.getSignalQuality() > 20 && modem.getSignalQuality() <= 30){

                 strcpy(signalQuality,"Excellent");
           }
          else if(modem.getSignalQuality() > 10 && modem.getSignalQuality() <= 14){

                 strcpy(signalQuality,"OK");
           }
          else if(modem.getSignalQuality() <10){

                strcpy(signalQuality,"Marginal");
           }
              //   dust_sensor_show_data();  
               _humidity = 0;//read_humidity();
               _temperature =0;// read_temperature();
               _pressure = 5;


              R_volt =0;// read_R_phase_voltage();
              Y_volt =0;// read_Y_phase_voltage();
              B_volt =0;// read_B_phase_voltage();
                   
              _frequency =0;// read_Frequency();

              R_pf =0;// read_R_phase_PF();
              Y_pf =0;// read_Y_phase_PF();
              B_pf =0;// read_B_phase_PF();

              json_array(_humidity,_temperature,_pressure,R_volt,Y_volt,B_volt,_frequency,R_pf,Y_pf,B_pf,signalQuality);
              

}

void gsm_send_data_to_broker(void){
  
  mqtt_send_to_broker();
   
}
void startOtaUpdate(const String& ota_url)
{
  String protocol, host, url;
  int port;
  String temp_ = ota_url;
  
  if (!parseURL(temp_, protocol, host, port, url)) {
    DEBUG_FATAL(F("Cannot parse URL"));
  }

  DEBUG_PRINT(String("Connecting to ") + host + ":" + port);

  Client* client = NULL;
  if (protocol == "http") {
    client = new TinyGsmClient(modem,1);
    if (!client->connect(host.c_str(), port)) {
      DEBUG_FATAL(F("Client not connected"));
    }
  } else if (protocol == "https") {
    client = new TinyGsmClientSecure(modem,1);
    if (!client->connect(host.c_str(), port)) {
      DEBUG_FATAL(F("Client not connected"));
    }
  } else {
    DEBUG_FATAL(String("Unsupported protocol: ") + protocol);
  }
  
  DEBUG_PRINT(String("Requesting ") + url);

  client->print(String("GET ") + url + " HTTP/1.0\r\n"
               + "Host: " + host + "\r\n"
               + "Connection: keep-alive\r\n"
               + "\r\n");

  long timeout = millis();
  while (client->connected() && !client->available()) {
    if (millis() - timeout > 10000L) {
      DEBUG_FATAL("Response timeout");
    }
  }

  // Collect headers
  String md5;
  int contentLength = 0;

  while (client->available()) {
    String line = client->readStringUntil('\n');
    line.trim();
    SerialMon.println(line);    // Uncomment this to show response headers
    line.toLowerCase();
    if (line.startsWith("content-length:")) {
      contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
    } else if (line.startsWith("x-md5:")) {
      md5 = line.substring(line.lastIndexOf(':') + 1);
    } else if (line.length() == 0) {
      break;
    }
  }

  if (contentLength <= 0) {
    DEBUG_FATAL("Content-Length not defined");

  }

  bool canBegin = Update.begin(contentLength);
  if (!canBegin) {
    Update.printError(SerialMon);
    DEBUG_FATAL("OTA begin failed");
   
  }

  if (md5.length()) {
    DEBUG_PRINT(String("Expected MD5: ") + md5);
    if(!Update.setMD5(md5.c_str())) {
      DEBUG_FATAL("Cannot set MD5");
     
    }
  }

  DEBUG_PRINT("Flashing...");



  int written = 0;
  int progress = 0;
  uint8_t buff[256];
  while (client->connected() && written < contentLength) {
    timeout = millis();
    while (client->connected() && !client->available()) {
      delay(1);
      if (millis() - timeout > 60000L) {
        DEBUG_FATAL("Timeout");
        
      }
    }

    int len = client->read(buff, sizeof(buff));
    if (len <= 0) continue;

    Update.write(buff, len);
    written += len;

    int newProgress = (written*100)/contentLength;
    if (newProgress - progress >= 5 || newProgress == 100) {
      progress = newProgress;
      SerialMon.print(String("\r ") + progress + "%");
    }
  }
  SerialMon.println();

  if (written != contentLength) {
    Update.printError(SerialMon);
    DEBUG_FATAL(String("Write failed. Written ") + written + " / " + contentLength + " bytes");
    
    
  }

  if (!Update.end()) {
    Update.printError(SerialMon);
    DEBUG_FATAL(F("Update not ended"));

    
  }

  if (!Update.isFinished()) {
    DEBUG_FATAL(F("Update not finished"));

  }

  DEBUG_PRINT("=== Update successfully completed. Rebooting.");

  EEPROM.write(255,0);
  EEPROM.commit();
  delay(50);
  ESP.restart();
}

void printDeviceInfo()
{
  Serial.println();
  Serial.println("--------------------------");
  Serial.println(String("Build:    ") +  __DATE__ " " __TIME__);
#if defined(ESP8266)
  Serial.println(String("Flash:    ") + ESP.getFlashChipRealSize() / 1024 + "K");
  Serial.println(String("ESP core: ") + ESP.getCoreVersion());
  Serial.println(String("FW info:  ") + ESP.getSketchSize() + "/" + ESP.getFreeSketchSpace() + ", " + ESP.getSketchMD5());
#elif defined(ESP32)
  Serial.println(String("Flash:    ") + ESP.getFlashChipSize() / 1024 + "K");
  Serial.println(String("ESP sdk:  ") + ESP.getSdkVersion());
  Serial.println(String("Chip rev: ") + ESP.getChipRevision());
#endif
  Serial.println(String("Free mem: ") + ESP.getFreeHeap());
  Serial.println("--------------------------");
}

bool parseURL(String url, String& protocol, String& host, int& port, String& uri)
{
  int index = url.indexOf(':');
  if(index < 0) {
    return false;
  }

  protocol = url.substring(0, index);
  url.remove(0, (index + 3)); // remove protocol part

  index = url.indexOf('/');
  String server = url.substring(0, index);
  url.remove(0, index);       // remove server part

  index = server.indexOf(':');
  if(index >= 0) {
    host = server.substring(0, index);          // hostname
    port = server.substring(index + 1).toInt(); // port
  } else {
    host = server;
    if (protocol == "http") {
      port = 80;
    } else if (protocol == "https") {
      port = 443;
    }
  }

  if (url.length()) {
    uri = url;
  } else {
    uri = "/";
  }
  return true;
}

void gsm_mqtt_subscribe(String topic){
     
     char Sub_Topic[32];
     topic.toCharArray(Sub_Topic,30);
     bool status = gsm_mqtt.subscribe("both_directions");
     if(status){
      //  Serial.println("subscribtion successful");
     }
     else{
      //  Serial.println("sub failed");
     }
     
}