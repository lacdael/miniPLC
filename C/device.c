#include "device.h"
#include "app.h"

uint32_t LOGIC_STACK[MAX_COUNT_OF_APP_LOGIC][MAX_SIZE_OF_LOGIC] = {0};


/*
 *Device specific code, with functions called by app.c
 */


/**
 * 
 * @param port
 */
void setOutput(int port);

bool logicSave(uint32_t logic[]) {
    int i;
    for (i = 0; i < MAX_COUNT_OF_APP_LOGIC; i++) {
        if (LOGIC_STACK[i][0] == '\0' || LOGIC_STACK[i][0] == 0xFFFFFFFF) {
            int j;
            for (j = 0; j < MAX_SIZE_OF_LOGIC; j++) {
                LOGIC_STACK[i][j] = logic[j];
            }
            return true;
        }
    }
    return false;
}

bool logicDelete(int i) {
    int j;
    i--;
    if (i < 0 || i > MAX_COUNT_OF_APP_LOGIC) return false;
    for (j = 0; j < MAX_SIZE_OF_LOGIC; j++) {
        LOGIC_STACK[i][j] = 0;
    }
    return true;
}

int32_t getValue(int what){
    if (what >= _I1 && what <= _I2){
    
    } else if (what >= _A1 && what <= _A8){
    
    } else if (what >= _O1 && what _O8){
    
    }
    return 0;
} 

int getIO(int o) {
    switch (o) {
        case _O1: return outputs[0];
        case _O2: return outputs[1];
        case _O3: return outputs[2];
        case _O4: return outputs[3];
        case _O5: return outputs[4];
        case _O6: return outputs[5];
        case _O7: return outputs[6];
        case _O8: return outputs[7];
    }
}

void setIO(int o, int s) {
    switch (o) {
        case _O1: outputs[0] = s;
            break;
        case _O2: outputs[1] = s;
            break;
        case _O3: outputs[2] = s;
            break;
        case _O4: outputs[3] = s;
            break;
        case _O5: outputs[4] = s;
            break;
        case _O6: outputs[5] = s;
            break;
        case _O7: outputs[6] = s;
            break;
        case _O8: outputs[7] = s;
            break;
    }
}

float getADC(int port) {
        switch (port) {
        case _A1: return adcPorts[0];
        case _A2: return adcPorts[1];
        case _A3: return adcPorts[2];
        case _A4: return adcPorts[3];
        case _A5: return adcPorts[4];
        case _A6: return adcPorts[5];
        case _A7: return adcPorts[6];
        case _A8: return adcPorts[7];
    }
    //eturn 1.23;
}

void doADCCalibrate(int port, float val){
    float v = val / 100;
switch (port) {
        case _A1: adcPorts[0] = v;
            break;
        case _A2: adcPorts[1] = v;
            break;
        case _A3: adcPorts[2] = v;
            break;
        case _A4: adcPorts[3] = v;
            break;
        case _A5: adcPorts[4] = v;
            break;
        case _A6: adcPorts[5] = v;
            break;
        case _A7: adcPorts[6] = v;
            break;
        case _A8: adcPorts[7] = v;
            break;
    }
}

void doADCZero(int port){
switch (port) {
        case _A1: adcPorts[0] = 0.0;
            break;
        case _A2: adcPorts[1] = 0.0;
            break;
        case _A3: adcPorts[2] = 0.0;
            break;
        case _A4: adcPorts[3] = 0.0;
            break;
        case _A5: adcPorts[4] = 0.0;
            break;
        case _A6: adcPorts[5] = 0.0;
            break;
        case _A7: adcPorts[6] = 0.0;
            break;
        case _A8: adcPorts[7] = 0.0;
            break;
    }
}

int getADCZero(int port) {
    return 1234556;
}

int getADCCal(int port) {
    return 123456789;
}