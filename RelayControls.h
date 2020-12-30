#ifndef RelayControls_h
#define RelayControls_h

void relay_server_handle(void);
void relay_mdns_init();
void relay_server_loop();
void handle_OnConnect();
void handle_relay1on();
void handle_relay1off();
void handle_relay2on();
void handle_relay2off();
void handle_relay3on();
void handle_relay3off();
void handle_relay4on();
void handle_relay4off();
void handle_NotFound();
String SendHTML(uint8_t relay1stat,uint8_t relay2stat,uint8_t relay3stat,uint8_t relay4stat);

#endif
