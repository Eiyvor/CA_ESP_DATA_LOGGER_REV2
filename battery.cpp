#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

#define I2C_SDA              21
#define I2C_SCL              22
#include<Wire.h>

void battery_init(void){

             Wire.begin(I2C_SDA,I2C_SCL); 

}
bool battery_confiq(int en){

             Wire.beginTransmission(IP5306_ADDR);

             Wire.write(IP5306_REG_SYS_CTL0);

            if (en == 1) {
                     Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
             } 
            else {
                      Wire.write(0x35); // 0x37 is default reg value
             }
             return Wire.endTransmission() == 0;
}
