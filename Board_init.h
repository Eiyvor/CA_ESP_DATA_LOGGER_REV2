#ifndef Board_init_h
#define Board_init_h

#include"energy_meter.h"
#include"battery.h"
#include"particle_sensor.h"
#include"Sensor.h"
void board_init(void){
  
  battery_init();
  battery_confiq(1);
  sensor_init();
  modbus_init();
  pms90003m_init();
 
  

}



#endif
