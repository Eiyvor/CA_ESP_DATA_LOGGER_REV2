                                     CA_ESP_DATA_LOGGER_FIRMWARE_REVB


Features:

1) Runs a local webserver in access point mode for credential uploading
2) Runs another local webserver in station mode to control onboard relays
3) Connects to an MQTT broker at secured port 8883 and sends sensor data to a specific topic 
4) Capable of performing OTA firmware update using GSM 
5) Subscirbed to MQTT topic CONF/SN-device_mac_id to receive device configuration settings


Function explaination:

                                          AWS_WiFi.cpp

void messageReceived(String &topic, String &payload);

This function is a callback function which is invoked when data is published to the topic CONF/SN-device_mac_id


void AWS_WiFi_CallBack();

the messageReceived() function is called inside this function 

void AWS_WiFi_SubTopic(String sub_topic);

This function is used to subscribe to a mqtt topic

void connectToAWS()

This function connects the device to AWS via WiFi

void AWS_WiFi_loop()

This is just a loop, nothing is done inside, but needs to be called in the main function for things to work properly.

                                    RelayControls.cpp

void relay_server_handle();

This function handles the local server like a callback function

void relay_mdns_init();

This function initializes mdns(multicast DNS), which gives the device a domain name

void relay_server_loop();

This is just a loop, nothing is done inside, but needs to be called in the main function for things to work properly.

void handle_Onconnect();

This function is called when a client is connected to the server(the device)


void handle_relay1on(); void handle_relay2on(); void handle_relay3on(); void handle_relay4on();

These functions are called when a client request to turn respective relays.

void handle_NotFound();

This function is called when the client request for a page that doesn't exist.





