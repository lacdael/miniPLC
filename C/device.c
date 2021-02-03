#include "../C/device.h"
#include "../C/app.h"
#include <stdio.h>
#include <sys/time.h>

uint32_t LOGIC_STACK[MAX_COUNT_OF_APP_LOGIC][MAX_SIZE_OF_LOGIC];

uint32_t _TIMEOUT_VAL = 3000;

/*
 *Device specific code, with functions called by app.c
 */

struct timeval now;

bool logicExists(const uint32_t * l) {
    return false;
}
uint32_t j = 0;

const uint32_t * getLogic(int i) {
    return &j;
}

void setUartTimeStamp() {
    return;
}

void updateTimeStamp() {
    return;
}

uint32_t getMillisencondElapsed(TimeStamp t) {
    return (uint32_t) 5000;
}

TimeStamp getTimeStampNow(void) {


}


int8_t _OUTPUTS[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int8_t _INPUTS[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setValue(int port, void * val) {
    switch (port) {
        case _O1: _OUTPUTS[0] = (*(int8_t*) val);
            break;
        case _O2: _OUTPUTS[1] = (int8_t) (*(int32_t*) val);
            break;
        case _O3: _OUTPUTS[2] = (int8_t) (*(int32_t*) val);
            break;
        case _O4: _OUTPUTS[3] = (int8_t) (*(int32_t*) val);
            break;
        case _O5: _OUTPUTS[4] = (int8_t) (*(int32_t*) val);
            break;
        case _O6: _OUTPUTS[5] = (int8_t) (*(int32_t*) val);
            break;
        case _O7: _OUTPUTS[6] = (int8_t) (*(int32_t*) val);
            break;
        case _O8: _OUTPUTS[7] = (int8_t) (*(int32_t*) val);
            break;
        case _TIMEOUT: _TIMEOUT_VAL = (*(int32_t*) val);
            break;
    }
    return;
}

int32_t getValue(int what) {
    switch (what) {
        case _O1: return _OUTPUTS[0];
        case _O2: return _OUTPUTS[1];
        case _O3: return _OUTPUTS[2];
        case _O4: return _OUTPUTS[3];
        case _O5: return _OUTPUTS[4];
        case _O6: return _OUTPUTS[5];
        case _O7: return _OUTPUTS[6];
        case _O8: return _OUTPUTS[7];
        case _TIMEOUT: return _TIMEOUT_VAL;
    }

    return 1;
}


float _ADC[8] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};

void doADCCalibrate(int port, float val) {
    switch (port) {
        case _A1: _ADC[0] = val;
            break;
        case _A2: _ADC[1] = val;
            break;
        case _A3: _ADC[2] = val;
            break;
        case _A4: _ADC[3] = val;
            break;
        case _A5: _ADC[4] = val;
            break;
        case _A6: _ADC[5] = val;
            break;
        case _A7: _ADC[6] = val;
            break;
        case _A8: _ADC[7] = val;
            break;
    }

    return;
}

void doADCZero(int port) {
    return;
}

float getADC(int port) {
    switch (port) {
        case _A1: return _ADC[0];
            break;
        case _A2: return _ADC[1];
            break;
        case _A3: return _ADC[2];
            break;
        case _A4: return _ADC[3];
            break;
        case _A5: return _ADC[4];
            break;
        case _A6: return _ADC[5];
            break;
        case _A7: return _ADC[6];
            break;
        case _A8: return _ADC[7];
            break;
    }
    return 123.45;
}

void printChar(char c) {
    putchar(c);
}

void printOut(char *str) {
    printf("%s", str);
}

void deviceRecieveData(void) {

};

void deviceDoADC(void) {

};

bool isInitialised() {
    return true;
}

void loadValues(void) {
    return;
}

uint32_t ADCZERO[8] = {1, 12, 12, 1, 1234, 1234, 1234, 1234};

uint32_t getADCZero(int port) {
    return ADCZERO[port];
}

uint32_t ADCCAL[8] = {1234, 1234, 1234, 1234, 1234, 1234, 1234, 1234};

uint32_t getADCCal(int port) {
    return ADCCAL[port];
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