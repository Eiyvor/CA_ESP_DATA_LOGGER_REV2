#define R_phase_voltage_reg 12       //Modbus register addresss of R phase voltage
#define Y_phase_voltage_reg 14       //Modbus register addresss of Y phase voltage
#define B_phase_voltage_reg 16       //Modbus register addresss of B phase voltage
#define frequency_reg       32       //Modbus register addresss of frequency
#define R_phase_PF_reg      40       //Modbus register addresss of R phase PF
#define Y_phase_PF_reg      42       //Modbus register addresss of Y phase PF
#define B_phase_PF_reg      44       //Modbus register addresss of B phase PF

#include<ModbusMaster.h>

uint16_t length= 0x0001;
uint8_t result;
uint8_t ret;








ModbusMaster node;

void enableTx(void){
//  delay(2);
  digitalWrite(32,HIGH);
}
void disableTx(){
//  delay(2);
  digitalWrite(32,LOW);
}

void modbus_init(void) {
              
              pinMode(32,OUTPUT);
              digitalWrite(32,LOW);
              node.preTransmission(enableTx);
              node.postTransmission(disableTx);
              node.begin(5,Serial);
}

uint16_t read_R_phase_voltage(void){
               
               static uint16_t R_phase_voltage = 0;
               uint8_t R_voltage_result = node.readInputRegisters(R_phase_voltage_reg,length);
               
               if(R_voltage_result == node.ku8MBSuccess){
                
                                      R_phase_voltage= node.getResponseBuffer(0);
                                      Serial.print("...R Phase Voltage=");
                                      R_phase_voltage /= 100;
                                      Serial.print((float)R_phase_voltage);
                                      Serial.println("V");
                                      }
               else{
                
                    // Serial.print("Failed to read R phase voltage...");
                    
                   }
               return R_phase_voltage;
}

uint16_t read_Y_phase_voltage(void){
  
               static uint16_t Y_phase_voltage = 0;
               uint8_t Y_voltage_result = node.readInputRegisters(Y_phase_voltage_reg,length);

               if(Y_voltage_result == node.ku8MBSuccess){

                                      Y_phase_voltage= node.getResponseBuffer(0);
                                      Serial.print("...Y Phase Voltage=");
                                      Y_phase_voltage /= 100;
                                      Serial.print((float)Y_phase_voltage);
                                      Serial.println("V");
                                      }
              else{
                
               //     Serial.print("Failed to read Y phase voltage...");
                   
                  }
               return Y_phase_voltage;
}

uint16_t read_B_phase_voltage(void){
  
              static uint16_t B_phase_voltage = 0;  
              uint8_t B_voltage_result = node.readInputRegisters(B_phase_voltage_reg,length);

              if(B_voltage_result == node.ku8MBSuccess){
    
                                     B_phase_voltage= node.getResponseBuffer(0);
                                     Serial.print("...B Phase Voltage=");
                                     B_phase_voltage /= 100;
                                     Serial.print((float)B_phase_voltage);
                                     Serial.println("V");
                                     }
              else{
                
               //     Serial.print("Failed to read B phase voltage...");
                   
                  }
               return B_phase_voltage;
}

uint16_t read_Frequency(void){
  
          static uint16_t frequency = 0;
          uint8_t frequency_result = node.readInputRegisters(frequency_reg,length);

          if(frequency_result == node.ku8MBSuccess){
                                 
                                 frequency = node.getResponseBuffer(0);
                                 Serial.print("...Frequency=");
                                 frequency /= 100;
                                 Serial.print((float)frequency);
                                 Serial.println("Hz");
                                 }
          else{ 
            
               // Serial.print("Failed to read Frequency...");
               
              }
           return frequency;
}

uint16_t read_R_phase_PF(void){
  
           static uint16_t R_phase_PF = 0;
            uint8_t R_phase_PF_result = node.readInputRegisters(R_phase_PF_reg,length);

            if(R_phase_PF_result == node.ku8MBSuccess){
    
                                    R_phase_PF = node.getResponseBuffer(0);
                                    Serial.print("...R Phase Power Factor=");
                                    R_phase_PF /= 100;
                                    Serial.println((float)R_phase_PF);
                                    }
            else{
              
               //   Serial.print("Failed to read R phase power factor...");
                 
                }
             return R_phase_PF;
}

uint16_t read_Y_phase_PF(void){
  
           static uint16_t Y_phase_PF = 0;
            uint8_t Y_phase_PF_result = node.readInputRegisters(Y_phase_PF_reg,length);

            if(Y_phase_PF_result == node.ku8MBSuccess){
    
                                    Y_phase_PF = node.getResponseBuffer(0);
                                    Serial.print("...Y Phase Power Factor=");
                                    Y_phase_PF /= 100;
                                    Serial.println((float)Y_phase_PF);
                                    }
            else{
                 
               //   Serial.print("Failed to read Y phase power factor...");
  
                }
             return Y_phase_PF;
}

uint16_t read_B_phase_PF(void){
  
           static uint16_t B_phase_PF = 0;
            uint8_t B_phase_PF_result = node.readInputRegisters(B_phase_PF_reg,length);

            if(B_phase_PF_result == node.ku8MBSuccess){
                                    
                                    B_phase_PF = node.getResponseBuffer(0);
                                    Serial.print("...B Phase Power Factor=");
                                    B_phase_PF /= 100;
                                    Serial.println((float)B_phase_PF);
                                    }
            else{
    
               //   Serial.print("Failed to read B phase power factor...");
                 
                }
             return B_phase_PF;
}
