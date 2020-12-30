#include "PMS.h"
#include"gsm.h"
#include<HardwareSerial.h>

PMS pms(Serial2);
PMS::DATA data;

String dust_pm_1_0;
String dust_pm_2_5;
String dust_pm_10_;

void pms90003m_init(void){

// swSerial.begin(9600, SWSERIAL_8N1, 4, 15, false, 256);
Serial2.begin(9600,SERIAL_8N1,4,15); //rx=4,tx=15
   

}

String dust_sensor_pm_1_0(void){
                if(pms.read(data)){
                Serial.println("PM1.0 Dust Concentration");
                Serial.println("PM1.0 :"+String(data.PM_AE_UG_1_0) + "(ug/m3)");
                dust_pm_1_0 = String(data.PM_AE_UG_1_0)+"(ug/m3)";
                
                
}
return dust_pm_1_0;

}
String dust_sensor_pm_2_5(void){
                if(pms.read(data)){
                Serial.println("PM2.5 Dust Concentration");
                Serial.println("PM2.5 :"+String(data.PM_AE_UG_2_5) + "(ug/m3)");
                dust_pm_2_5 = String(data.PM_AE_UG_2_5)+"(ug/m3)";
                
                
}
return dust_pm_2_5;

}
String dust_sensor_pm_10(void){
                if(pms.read(data)){
                Serial.println("PM10 Dust Concentration");
                Serial.println("PM10 :"+String(data.PM_AE_UG_10_0) + "(ug/m3)");
                dust_pm_10_ = String(data.PM_AE_UG_10_0)+"(ug/m3)";
                
                
}
return dust_pm_10_;

}