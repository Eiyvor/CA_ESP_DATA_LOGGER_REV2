#include "ClosedCube_HDC1080.h"
#include<Wire.h>
#include<math.h>
#include<driver/adc.h>
#include<EmonLib.h>

#define gas_sensor 34
#define CO_sensor 35
#define CT_sensor 36
#define gas_sensor_digitalIN 32
#define CO_sensor_digitalIN 31

float CO_sensor_value = 0;
float gas_sensor_value = 0;
float sensor_volt_CO = 0;
float sensor_volt_gas = 0;
float RS_gas = 0;
float RS_CO = 0;
float ratio_gas = 0;
float ratio_CO =0;
float R0_gas = 1.32;
float R0_CO = 0.51;
double m_CO = -0.8849;
double b_CO = 4.5743;
double CO_in_ppm = 0;
double m_gas = -0.3304;
double b_gas = 2.4264;
double gas_in_ppm = 0;

ClosedCube_HDC1080 hdc1080;
EnergyMonitor Emon;

void sensor_init(void){

            hdc1080.begin(0x40);
            pinMode(gas_sensor_digitalIN,INPUT);
            pinMode(CO_sensor_digitalIN,INPUT);
            analogReadResolution(12);
            Emon.current(CT_sensor,30);

}

float read_temperature(void){

            float temp = hdc1080.readTemperature();
            return temp;
}
float read_humidity(void){

            float humid = hdc1080.readHumidity();
            return humid;

}
float read_CO_sensor(void){

      CO_sensor_value = analogRead(34);
      sensor_volt_CO = ((float)CO_sensor_value/4095)*3.3;
      RS_CO = (10.0 - sensor_volt_CO)/sensor_volt_CO;

      ratio_CO = RS_CO/R0_CO;

     double power = (log(ratio_CO)-b_CO)/m_CO;

      CO_in_ppm = pow(10,power);
     
return CO_in_ppm; 
}

float read_gas_sensor(void){
      gas_sensor_value = analogRead(34);
      sensor_volt_gas = ((float)gas_sensor_value/4095)*3.3;
      RS_gas = (10.0 - sensor_volt_gas)/sensor_volt_gas;

      ratio_gas = RS_gas/R0_gas;

     double power = (log(ratio_gas)-b_gas)/m_gas;

      gas_in_ppm = pow(10,power);
     
return gas_in_ppm; 
      
 
}

double Current_Sensor(void){

      double Amps = Emon.calcIrms(1480);

return Amps;      
}