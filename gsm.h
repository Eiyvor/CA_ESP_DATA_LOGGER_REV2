#ifndef gsm_h
#define gsm_h

#define TINY_GSM_MODEM_SIM800
#define MODEM_RST         5
#define MODEM_PWKEY       4
#define MODEM_POWER_ON    23
#define MODEM_TX          27
#define MODEM_RX          26
#define GSM_PIN           ""
#define SerialAT          Serial1
#define SerialMon Serial

#define DEBUG_PRINT(...) { SerialMon.print(millis()); SerialMon.print(" - "); SerialMon.println(__VA_ARGS__); }
#define DEBUG_FATAL(...) { SerialMon.print(millis()); SerialMon.print(" - FATAL: "); SerialMon.println(__VA_ARGS__); delay(10000); ESP.restart(); }

void gsm_modem_init(void);
void gsm_setup_gprs(void);
void gsm_mqtt_set_server_and_callback(void);
bool gsm_mqtt_client(void);
void gsm_mqtt_loop(void);
void gsm_send_json(void);

void mqtt_send_to_broker();
void gsm_send_data_to_broker(void);                                                                   
bool check_gsm_mqtt_connected(void);
void startOtaUpdate(const String& ota_url);
void printDeviceInfo();
bool parseURL(String url, String& protocol, String& host, int& port, String& uri);
void gsm_mqtt_subscribe(String Sub_Topic);
#endif
