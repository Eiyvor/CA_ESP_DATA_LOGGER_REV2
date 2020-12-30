#ifndef brokercred_h
#define brokercred_h

#include<EEPROM.h>

String read_uuid(){
   String uuid_in_eeprom = "";
  for(int i=0;i<36;++i){
    uuid_in_eeprom += char(EEPROM.read(i+100));
  }
  return uuid_in_eeprom;
}

String read_userid(){
  String userid_in_eeprom = "";
  for(int i=0;i<15;++i){
    userid_in_eeprom += char(EEPROM.read(i+140));
  }
  return userid_in_eeprom;
}

String read_password(){
  String password_in_eeprom = "";
  for(int i=0;i<15;++i){
    password_in_eeprom += char(EEPROM.read(i+160));
  }
  return password_in_eeprom;
}



#endif
