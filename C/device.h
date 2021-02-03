#ifndef APP_DEVICE_H_INCLUDED
#define APP_DEVICE_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

#define MAX_SIZE_OF_LOGIC 32
#define MAX_COUNT_OF_APP_LOGIC 8
#define BUFFER_SIZE 512        
#define THREE_SECONDS 3

#define TMR_PERIOD_MILLISECONDS 50


void printChar(char c);

typedef struct {
    uint8_t countMillisends;
    uint32_t countSeconds;
} TimeStamp;

typedef struct {
    double b0;
    double b1;
    double b2;
    double a1;
    double a2;
    double adc1_x[3];
    double adc1_y[3];
    double adc2_x[3];
    double adc2_y[3];
    double adc3_x[3];
    double adc3_y[3];
    double adc4_x[3];
    double adc4_y[3];
    double adc5_x[3];
    double adc5_y[3];
    double adc6_x[3];
    double adc6_y[3];
    double adc7_x[3];
    double adc7_y[3];
    double adc8_x[3];
    double adc8_y[3];
} DeviceData;


/*
 *Bessel Low Pass 2nd Order:
 * fs = 0.5
 * fc = 2
 * 
 * b0 = 7.199e-31
 * b1 = 1.44e-30
 * b2 = 7.199e-31
 * a0 = 1
 * a1 = -2
 * a2 = 1
 * 
 * y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
 */

bool logicExists(const uint32_t *);


const uint32_t * getLogic(int which);
void setUartTimeStamp();
void updateTimeStamp();
uint32_t getMillisencondElapsed(TimeStamp);
TimeStamp getTimeStampNow(void);
void deviceRecieveData(void);
void deviceDoADC(void);
void setValue(int, void *);
int32_t getValue(int what);
void loadValues(void);
void doADCCalibrate(int port, float val);
void doADCZero(int port);
float getADC(int port);
void printOut(char *str);
bool isInitialised();
uint32_t getADCZero(int port);
uint32_t getADCCal(int port);
bool logicSave(uint32_t logic[]);
bool logicDelete(int i);

#endif /* DEVICE_H */
