/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../C/device.h"
#include "../C/app.h"

static volatile uint32_t deviceADC1 = 0;
static volatile uint32_t deviceADC2 = 0;
static volatile uint32_t deviceADC3 = 0;
static volatile uint32_t deviceADC4 = 0;
static volatile uint32_t deviceADC5 = 0;
static volatile uint32_t deviceADC6 = 0;
static volatile uint32_t deviceADC7 = 0;
static volatile uint32_t deviceADC8 = 0;
static volatile uint8_t deviceLEDState = 0;

bool logicSave(uint32_t logic[]) {
    return true;
}

bool logicDelete(int i) {
    return true;
}

void setValue(int port, void * val) {
    uint8_t i = 0;
    if (port >= _O1 && port <= _O8) {
        bool b = *(bool *) val;
        switch (port) {
                //TODO: invert bitmap so (_O2 -_O1 = 1)
            case _O1: i = 7;
                break;
            case _O2: i = 6;
                break;
            case _O3: i = 5;
                break;
            case _O4: i = 4;
                break;
            case _O5: i = 3;
                break;
            case _O6: i = 2;
                break;
            case _O7: i = 1;
                break;
            case _O8: i = 0;
                break;
        }
        if (b) deviceLEDState |= 1 << i;
        else deviceLEDState &= ~(1 << i);
        LEDSet(deviceLEDState);
    } else if (port >= _A1 && port <= _A8) {
        //TODO: cast void * to double
    } else if (port == _STR) {
        //TODO: case void * to uint8_t
    }
}

//TODO: add this functionality in setValue

void doADCCalibrate(int port, float val) {

}
//TODO: add this functionality in setValue

void doADCZero(int port) {

}

double getADC(int port) {
    return 0.0;
}

uint32_t getADCZero(int port) {
    return 0;
}

uint32_t getADCCal(int port) {
    return 0;
}

int32_t getValue(int what) {
    switch (what) {
        case _I1: return ButtonRead() & 1;
        case _I2: return (ButtonRead() >> 1) & 1;
        case _O1: return (deviceLEDState >> 7) & 1;
        case _O2: return (deviceLEDState >> 6) & 1;
        case _O3: return (deviceLEDState >> 5) & 1;
        case _O4: return (deviceLEDState >> 4) & 1;
        case _O5: return (deviceLEDState >> 3) & 1;
        case _O6: return (deviceLEDState >> 2) & 1;
        case _O7: return (deviceLEDState >> 1) & 1;
        case _O8: return (deviceLEDState) & 1;
        case _A1: return deviceADC1;
        case _A2: return deviceADC2;
        case _A3: return deviceADC3;
        case _A4: return deviceADC4;
        case _A5: return deviceADC5;
        case _A6: return deviceADC6;
        case _A7: return deviceADC7;
        case _A8: return deviceADC8;
    }
    return 0;
}

void deviceDoADC() {
    while (IFS0bits.AD1IF == 0);
    IFS0CLR = _IFS0_AD1IF_MASK;
    //  NAME   |   PIC      
    //  --------------------
    //  A1     |   RA3/AN6  
    //  A2     |   RA2/AN5    
    //  A3     |   RB3/AN11   
    //  A4     |   RB2/AN4    
    //  A5     |   RA1/AN1    
    //  A6     |   RA0/AN0    
    //  A7     |   RB13/AN8 
    //  A8     |   RB12/AN7 
    deviceADC1 = ADC1BUF0;
    deviceADC2 = ADC1BUF1;
    deviceADC3 = ADC1BUF2;
    deviceADC4 = ADC1BUF3;
    deviceADC5 = ADC1BUF4;
    deviceADC7 = ADC1BUF5;
    deviceADC7 = ADC1BUF6;
    deviceADC8 = ADC1BUF7;
}

void printOutput(const char *fmt, ...) {
    va_list args;
    int txLen;
    char txBuffer[BUFFER_SIZE];
    va_start(args, fmt);
    memset(txBuffer, 0, sizeof (txBuffer));
    txLen = vsnprintf(txBuffer, sizeof (txBuffer), fmt, args);
    va_end(args);
    EZBL_FIFOWrite(EZBL_STDOUT, txBuffer, txLen);
    EZBL_FIFOFlush(EZBL_STDOUT, 10);
}


/* *****************************************************************************
 End of File
 */
