#ifndef APP_DEVICE_H_INCLUDED
#define APP_DEVICE_H_INCLUDED

#define MAX_SIZE_OF_LOGIC 128
#define MAX_COUNT_OF_APP_LOGIC 12
#define MAX_TX_BUUFFER (MAX_SIZE_OF_LOGIC*MAX_COUNT_OF_APP_LOGIC + 3*MAX_SIZE_OF_LOGIC + 2)        

#include <stdint.h>
#include <stdbool.h>

int32_t LOGIC_STACK[MAX_COUNT_OF_APP_LOGIC][MAX_SIZE_OF_LOGIC];

char outputs[8];
int32_t adcPorts[8];

void setOutput(int port);

bool logicSave(uint32_t logic[]);

bool logicDelete(int i);
    
int getIO(int o);
void setIO(int port,int state);
void doADCCalibrate(int port, float val);
void doADCZero(int port);
double getADC(int port);
int getADCZero(int port);
int getADCCal(int port);
int32_t getValue(int what);

#endif /* DEVICE_H */
