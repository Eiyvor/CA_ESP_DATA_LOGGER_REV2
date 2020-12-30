#ifndef AWS_WiFi_h
#define AWS_WiFi_h

#define DEVICE_NAME "Device_new"
#define AWS_IOT_ENDPOINT "aj9bkl2jv8zj-ats.iot.us-west-2.amazonaws.com"
#define AWS_IOT_TOPIC "topic_2"
#define AWS_MAX_RECONNECT_TRIES 50

void messageReceived(String &topic, String &payload);
void AWS_WiFi_CallBack();
void AWS_WiFi_SubTopic(String sub_tpoic);
void AWS_WiFi_loop(void);
void connectToAWS();

#endif
