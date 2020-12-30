#ifndef modbus_h
#define modbus_h 100

void enableTx(void);
void disableTx();
void modbus_init(void);
uint16_t read_R_phase_voltage(void);
uint16_t read_Y_phase_voltage(void);
uint16_t read_B_phase_voltage(void);
uint16_t read_Frequency(void);
uint16_t read_R_phase_PF(void);
uint16_t read_Y_phase_PF(void);
uint16_t read_B_phase_PF(void);

#endif
