#include "Credentials.h"
#include "AWS_WiFi.h"
#include "gsm.h"
#include "RelayControls.h"
#include "Board_init.h"
#include "particle_sensor.h"

credentials cred;

String ap_mode_ssid_ = "Cybene";
char ap_mode_ssid[35];
char* ap_mode_password = "Cybene@123";
String macID="";
String name = "CONF/SN-";
bool connected_to_internet = 0;
String gsm_mqtt_sub_topic = "";
void setup() {

Serial.begin(9600); // initalize serial communication 
board_init(); 
gsm_modem_init(); // initialize gsm modem
gsm_setup_gprs(); // sets up gprs
macID = WiFi.macAddress();// this function reads the mac address of the device
ap_mode_ssid_+= macID;
ap_mode_ssid_.toCharArray(ap_mode_ssid,35);
name+=macID;  
Serial.println(name);
cred.EEPROM_Config();// this function reads the data stored in EEPROM

if(cred.credentials_get()){ //checks if the device is connected to wifi

  connected_to_internet = 1; // set this flag is the device connects to a wifi router
}
else {
  cred.setupAP(ap_mode_ssid,ap_mode_password); // turns on AP mode if wifi isn't connected
  connected_to_internet = 0;
}

if(connected_to_internet){

connectToAWS();// connect to AWS via wifi
AWS_WiFi_SubTopic(name);// name= CONF/SN-macid
AWS_WiFi_CallBack();// setup a callback function to get triggered when a message arrive to the subscribed topic
relay_mdns_init();// initialize mDNS server for relay control
relay_server_handle();
gsm_mqtt_sub_topic = name+"/up";
gsm_mqtt_set_server_and_callback(); // callback function for cybene broker
}
}   
String read_url(){
  String url_in_eeprom = "";
  for(int i=0;i<int(EEPROM.read(252));++i){
    url_in_eeprom += char(EEPROM.read(i+300));
  }
  return url_in_eeprom;
}
void loop() {
   if(connected_to_internet){

                       AWS_WiFi_loop(); // need to call this function to run AWS APIs in loop
    
          }
         if(!check_gsm_mqtt_connected() && int(EEPROM.read(250)==1)){ 
           // this flag is set in messageReceived function in AWS_WiFi to indicate successful reception of broker credentials

                      gsm_mqtt_client();
                      Serial.println("loop");
                      Serial.println(int(EEPROM.read(255)));
                     
    
          }
          cred.server_loops();// need to call this to run Websocket server in repeat
          gsm_send_data_to_broker();// sends data to cybene broker via gsm
          relay_server_loop(); 
          gsm_mqtt_loop();
          /*check if the flag is set to start ota update.
          This flag is set in gsm_mqtt_callback function in gsm.cpp*/
          while(int(EEPROM.read(255) == 1)){ 
        
            Serial.println("ota loop");
            gsm_setup_gprs();
            Serial.println(int(EEPROM.read(255)));
            startOtaUpdate(read_url());// starts ota update
          }

  }
