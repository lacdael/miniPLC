#include <stdint.h>
//#include <stdio.h>

#include "../C/app.h"
#include "../C/device.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/memory/flash.h"

#define TMR1_PERIOD 125
#define TMR1_PERIOD_HZ (1000 / TMR1_PERIOD)
#define FLASH_INITIALIZED 0
#define FLASH_NAME_MSB 1
#define FLASH_NAME_LSB 2
#define FLASH_TIMEOUT 3
#define FLASH_ADC1_ZERO 4
#define FLASH_ADC1_CAL 5
#define FLASH_ADC2_ZERO 6
#define FLASH_ADC2_CAL 7
#define FLASH_ADC3_ZERO 8
#define FLASH_ADC3_CAL 9
#define FLASH_ADC4_ZERO 10
#define FLASH_ADC4_CAL 11
#define FLASH_ADC5_ZERO 12
#define FLASH_ADC5_CAL 13
#define FLASH_ADC6_ZERO 14
#define FLASH_ADC6_CAL 15
#define FLASH_ADC7_ZERO 16
#define FLASH_ADC7_CAL 17
#define FLASH_ADC8_ZERO 18
#define FLASH_ADC8_CAL 19
#define FLASH_LOGIC1 100
#define FLASH_LOGIC2 132
#define FLASH_LOGIC3 200
#define FLASH_LOGIC4 232
#define FLASH_LOGIC5 300
#define FLASH_LOGIC6 332
#define FLASH_LOGIC7 400
#define FLASH_LOGIC8 432


#define MY_BUFFER_SIZE 128
#define RECIEVE_TIMEOUT (TMR1_PERIOD*2)

uint8_t numBytes = 0;
char recieveBuffer[MY_BUFFER_SIZE];
TimeStamp commTimeStamp;

TimeStamp timeStamp = {
    .countMillisends = 0,
    .countSeconds = 0,
};

DeviceData deviceData = {
    .b0 = 7.199e-31,
    .b1 = 1.44e-30,
    .b2 = 7.199e-31,
    .a1 = -2,
    .a2 = 1,
    .adc1_x =
    {0},
    .adc1_y =
    {0},
};

#define DATA_STORAGE_SIZE FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS / 4

static const uint32_t __attribute((space(prog), aligned(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS))) flashData[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS / 4] = {0x0, //0: initialized
    0x0100, //1: version XX.YY
    0x572D4254, //2: Name MSB 'W-BT'
    0x0, //3: Name LSB
    3000, //4: timeout
    0x0, //5: adc1 zero
    0x0, //6: adc1 cal
    0x0, //7: adc2 zero
    0x0, //8: adc2 cal
    0x0, //9: adc3 zero
    0x0, //10: adc3 cal
    0x0, //11: adc4 zero
    0x0, //12: adc4 cal
    0x0, //13: adc5 zero
    0x0, //14: adc5 cal
    0x0, //15: adc6 zero
    0x0, //16: adc6 cal
    0x0, //17: adc7 zero
    0x0, //18: adc7 cal
    0x0, //19: adc8 zero
    0x0, //20: adc8 cal
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//*START:USER LOGIC STORAGE*//
static const uint32_t __attribute((space(prog), aligned(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS))) flashLogic1_2[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS / 4];
static const uint32_t __attribute((space(prog), aligned(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS))) flashLogic3_4[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS / 4];
static const uint32_t __attribute((space(prog), aligned(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS))) flashLogic5_6[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS / 4];
static const uint32_t __attribute((space(prog), aligned(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS))) flashLogic7_8[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS / 4];
//*END:USER LOGIC STORAGE*//

uint32_t flashBuffer[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS / 4];

TimeStamp timeStamp;

static void _flashPageWrite(uint32_t);
int32_t _adcReading(double, uint32_t, uint32_t);
uint32_t _doADCAverage(int, uint8_t);

bool isInitialised() {
    return flashData[FLASH_INITIALIZED] == 1;
}

static void _FlashError() {
    //TODO: REPORT ERROR
    while (1) {
    }
}

static void _flashMiscompareError() {
    //TODO: REPORT ERROR
    while (1) {
    }
}

static void _flashSetValue(int what, void * value) {
    uint8_t i;
    int size = FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS / 4;
    uint32_t * pointer;
    switch (what) {
        case FLASH_NAME_MSB:
        case FLASH_NAME_LSB:
        case FLASH_INITIALIZED:
        case FLASH_TIMEOUT:
        case FLASH_ADC1_ZERO:
        case FLASH_ADC1_CAL:
        case FLASH_ADC2_ZERO:
        case FLASH_ADC2_CAL:
        case FLASH_ADC3_ZERO:
        case FLASH_ADC3_CAL:
        case FLASH_ADC4_ZERO:
        case FLASH_ADC4_CAL:
        case FLASH_ADC5_ZERO:
        case FLASH_ADC5_CAL:
        case FLASH_ADC6_ZERO:
        case FLASH_ADC6_CAL:
        case FLASH_ADC7_ZERO:
        case FLASH_ADC7_CAL:
        case FLASH_ADC8_ZERO:
        case FLASH_ADC8_CAL: pointer = (uint32_t *) flashData;
            break;
        case FLASH_LOGIC1:
        case FLASH_LOGIC2: pointer = (uint32_t *) flashLogic1_2;
            break;
        case FLASH_LOGIC3:
        case FLASH_LOGIC4: pointer = (uint32_t *) flashLogic3_4;
            break;
        case FLASH_LOGIC5:
        case FLASH_LOGIC6: pointer = (uint32_t *) flashLogic5_6;
            break;
        case FLASH_LOGIC7:
        case FLASH_LOGIC8: pointer = (uint32_t *) flashLogic7_8;
            break;
    }
    for (i = 0; i < size; i++) flashBuffer[i] = pointer[i];
    switch (what) {
        case FLASH_NAME_MSB:
        case FLASH_NAME_LSB:
        {
        }
            break;
        case FLASH_INITIALIZED:
        case FLASH_TIMEOUT:
        case FLASH_ADC1_ZERO:
        case FLASH_ADC1_CAL:
        case FLASH_ADC2_ZERO:
        case FLASH_ADC2_CAL:
        case FLASH_ADC3_ZERO:
        case FLASH_ADC3_CAL:
        case FLASH_ADC4_ZERO:
        case FLASH_ADC4_CAL:
        case FLASH_ADC5_ZERO:
        case FLASH_ADC5_CAL:
        case FLASH_ADC6_ZERO:
        case FLASH_ADC6_CAL:
        case FLASH_ADC7_ZERO:
        case FLASH_ADC7_CAL:
        case FLASH_ADC8_ZERO:
        case FLASH_ADC8_CAL:
        {
            flashBuffer[what] = *(uint32_t *) value;
        }
            break;
        case FLASH_LOGIC1:
        case FLASH_LOGIC2:
        case FLASH_LOGIC3:
        case FLASH_LOGIC4:
        case FLASH_LOGIC5:
        case FLASH_LOGIC6:
        case FLASH_LOGIC7:
        case FLASH_LOGIC8:
        {
            if (value) for (i = what % 100; i < MAX_SIZE_OF_LOGIC; i++) flashBuffer[i] = ((uint32_t *) value)[i];
            else for (i = what % 100; i < MAX_SIZE_OF_LOGIC; i++) flashBuffer[i] = 0;
        }
            break;
    }
    _flashPageWrite((uint32_t) pointer);
}

static void _flashPageWrite(uint32_t flash_storage_address) {
    uint32_t flashOffset, readData;
    uint16_t result;
    uint32_t write_data[4] = {0};
    uint32_t read_data[4] = {0};

    // ------------------------------------------
    // Fill a page of memory with data.  
    // ------------------------------------------

    // Program Valid Key for NVM Commands
    FLASH_Unlock(FLASH_UNLOCK_KEY);


    // Erase the page of flash at this address
    result = FLASH_ErasePage(flash_storage_address);
    if (result == false) {
        _FlashError();
    }

    // Program flash with a data pattern.  For the data pattern we will use the index 
    // into the flash as the data.
    uint8_t j = 0;
    for (flashOffset = 0; flashOffset < FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS; flashOffset += 8) {
        if (j < DATA_STORAGE_SIZE) {
            result = FLASH_WriteDoubleWord(flash_storage_address + flashOffset, flashBuffer[j++], flashBuffer[j++]);
            if (result == false) {
                _FlashError();
            }
        }
    }

    // Clear Key for NVM Commands so accidental call to flash routines will not corrupt flash
    FLASH_Lock();


    // Verify the flash data is correct.  If it's not branch to error loop.
    // The data in the flash is the offset into the flash page.
    j = 0;
    for (flashOffset = 0; flashOffset < FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS; flashOffset += 4) {

        readData = FLASH_ReadWord(flash_storage_address + flashOffset);

        if (readData != flashBuffer[j++]) {
            _flashMiscompareError();
        }
    }

}

void loadValues(void) {



}

TimeStamp getTimeStampNow(void) {
    return timeStamp;
}

void updateTimeStamp() {
    uint32_t tmp = CORETIMER_CountGet();
    timeStamp.countSeconds = tmp / TMR1_PERIOD_HZ; //interrupt @ 125ms
    timeStamp.countMillisends = (tmp % TMR1_PERIOD_HZ) * TMR1_PERIOD;
}

uint32_t getMillisencondElapsed(TimeStamp then) {
    return (timeStamp.countSeconds - then.countSeconds) + (timeStamp.countMillisends - then.countMillisends);
}

void clearBuffer() {
    int i;
    for (i = 0; i < MY_BUFFER_SIZE; i++) {
        recieveBuffer[i] = 0;
    }
    numBytes = 0;
}

void deviceRecieveData(void) {
    while (UART1_IsRxReady() && numBytes < MY_BUFFER_SIZE) {
        if (numBytes == 0) {
            recieveBuffer[numBytes++] = UART1_Read();
            //numBytes += UART1_ReadBuffer(recieveBuffer + numBytes, MY_BUFFER_SIZE - numBytes);
            if (numBytes > 0) commTimeStamp = getTimeStampNow();
        } else if (numBytes > 0) {
            if (getMillisencondElapsed(commTimeStamp) > RECIEVE_TIMEOUT) {
                parseInput(recieveBuffer, numBytes);
                clearBuffer();
            } else {
                recieveBuffer[numBytes++] = UART1_Read();
                if (numBytes >= MY_BUFFER_SIZE) {
                    parseInput(recieveBuffer, numBytes);
                    clearBuffer();
                }
                //numBytes += UART1_ReadBuffer(recieveBuffer + numBytes, MY_BUFFER_SIZE - numBytes);
            }
        }
    }
}

void deviceDoADC() {
    
    uint16_t adc[12];
    
    ADC1_ConversionResultBufferGet(adc);
    
//    IFS0CLR = _IFS0_AD1IF_MASK;
  //  while (IFS0bits.AD1IF == 0);
 //   return;
    //ADC1
  //  deviceData.adc1_x[2] = deviceData.adc1_x[1];
  //  deviceData.adc1_x[1] = deviceData.adc1_x[0];
  //  deviceData.adc1_x[0] = ADC1BUF6;
  //  deviceData.adc1_y[0] = ADC1BUF6;
  //  deviceData.adc1_y[2] = deviceData.adc1_y[1];
  //  deviceData.adc1_y[1] = deviceData.adc1_y[0];
  //  deviceData.adc1_y[0] = deviceData.b0 * deviceData.adc1_x[0]
  //          + deviceData.b1 * deviceData.adc1_x[1] + deviceData.b2 * deviceData.adc1_x[3]
  //          - deviceData.a1 * deviceData.adc1_y[1] - deviceData.a2 * deviceData.adc1_y[2];
    //ADC2
  //  deviceData.adc2_x[2] = deviceData.adc2_x[1];
  //  deviceData.adc2_x[1] = deviceData.adc2_x[0];
  //  deviceData.adc2_x[0] = ADC1BUF5;
  //  deviceData.adc2_y[2] = deviceData.adc2_y[1];
  //  deviceData.adc2_y[1] = deviceData.adc2_y[0];
  //  deviceData.adc2_y[0] = deviceData.b0 * deviceData.adc2_x[0]
  //          + deviceData.b1 * deviceData.adc2_x[1] + deviceData.b2 * deviceData.adc2_x[3]
  //          - deviceData.a1 * deviceData.adc2_y[1] - deviceData.a2 * deviceData.adc2_y[2];
    //ADC3
  //  deviceData.adc3_x[2] = deviceData.adc3_x[1];
  //  deviceData.adc3_x[1] = deviceData.adc3_x[0];
    
  //  deviceData.adc3_x[0] = ADC1BUF11;
// deviceData.adc3_y[0] = ADC1BUF11;
    //   deviceData.adc3_y[2] = deviceData.adc3_y[1];
 //   deviceData.adc3_y[1] = deviceData.adc3_y[0];
  //  deviceData.adc3_y[0] = deviceData.b0 * deviceData.adc3_x[0]
  //          + deviceData.b1 * deviceData.adc3_x[1] + deviceData.b2 * deviceData.adc3_x[3]
  //          - deviceData.a1 * deviceData.adc3_y[1] - deviceData.a2 * deviceData.adc3_y[2];
    //ADC4
 //   deviceData.adc4_x[2] = deviceData.adc4_x[1];
 ///   deviceData.adc4_x[1] = deviceData.adc4_x[0];
  //  deviceData.adc4_x[0] = ADC1BUF4;
  //  deviceData.adc4_y[0] = ADC1BUF4;
    //deviceData.adc4_y[2] = deviceData.adc4_y[1];
   // deviceData.adc4_y[1] = deviceData.adc4_y[0];
   // deviceData.adc4_y[0] = deviceData.b0 * deviceData.adc4_x[0]
   //         + deviceData.b1 * deviceData.adc4_x[1] + deviceData.b2 * deviceData.adc4_x[3]
    //        - deviceData.a1 * deviceData.adc4_y[1] - deviceData.a2 * deviceData.adc4_y[2];
    //ADC5
//    deviceData.adc5_x[2] = deviceData.adc5_x[1];
//    deviceData.adc5_x[1] = deviceData.adc5_x[0];
   // deviceData.adc5_x[0] = ADC1BUF1;
  //  deviceData.adc5_y[0] = ADC1BUF1;
//    deviceData.adc5_y[2] = deviceData.adc3_y[1];
//    deviceData.adc5_y[1] = deviceData.adc3_y[0];
//    deviceData.adc5_y[0] = deviceData.b0 * deviceData.adc5_x[0]
//            + deviceData.b1 * deviceData.adc5_x[1] + deviceData.b2 * deviceData.adc5_x[3]
//            - deviceData.a1 * deviceData.adc5_y[1] - deviceData.a2 * deviceData.adc5_y[2];
    //ADC6
 //   deviceData.adc6_x[2] = deviceData.adc6_x[1];
 //   deviceData.adc6_x[1] = deviceData.adc6_x[0];
 //   deviceData.adc6_x[0] = ADC1BUF0;
//deviceData.adc6_y[0] = ADC1BUF0;
    //    deviceData.adc6_y[2] = deviceData.adc6_y[1];
//    deviceData.adc6_y[1] = deviceData.adc6_y[0];
//    deviceData.adc6_y[0] = deviceData.b0 * deviceData.adc6_x[0]
 //           + deviceData.b1 * deviceData.adc6_x[1] + deviceData.b2 * deviceData.adc6_x[3]
 //           - deviceData.a1 * deviceData.adc6_y[1] - deviceData.a2 * deviceData.adc6_y[2];
    //ADC7
 //   deviceData.adc7_x[2] = deviceData.adc7_x[1];
//    deviceData.adc7_x[1] = deviceData.adc7_x[0];
  //  deviceData.adc7_x[0] = ADC1BUF3;
 //deviceData.adc7_y[0] = ADC1BUF3;
    //   deviceData.adc7_y[2] = deviceData.adc7_y[1];
 //   deviceData.adc7_y[1] = deviceData.adc7_y[0];
 //   deviceData.adc7_y[0] = deviceData.b0 * deviceData.adc7_x[0]
 //           + deviceData.b1 * deviceData.adc7_x[1] + deviceData.b2 * deviceData.adc7_x[3]
 //           - deviceData.a1 * deviceData.adc7_y[1] - deviceData.a2 * deviceData.adc7_y[2];
    //ADC8
   // deviceData.adc8_x[2] = deviceData.adc8_x[1];
 //   deviceData.adc8_x[1] = deviceData.adc8_x[0];
   // deviceData.adc8_x[0] = ADC1BUF8;
 //   deviceData.adc8_y[0] = ADC1BUF8;
 //   deviceData.adc8_y[2] = deviceData.adc8_y[1];
 //   deviceData.adc8_y[1] = deviceData.adc8_y[0];
 //   deviceData.adc8_y[0] = deviceData.b0 * deviceData.adc8_x[0]
  //          + deviceData.b1 * deviceData.adc8_x[1] + deviceData.b2 * deviceData.adc8_x[3]
   //         - deviceData.a1 * deviceData.adc8_y[1] - deviceData.a2 * deviceData.adc8_y[2];
}

int32_t getValue(int what) {
    switch (what) {
        case _I1: return I1_GetValue();
        case _I2: return I2_GetValue();
        case _O1: return O1_GetValue();
        case _O2: return O2_GetValue();
        case _O3: return O3_GetValue();
        case _O4: return O4_GetValue();
        case _O5: return O5_GetValue();
        case _O6: return O6_GetValue();
        case _O7: return O7_GetValue();
        case _O8: return O8_GetValue();
        case _A1: return _adcReading(deviceData.adc1_y[0], flashData[FLASH_ADC1_ZERO], flashData[FLASH_ADC1_CAL]);
        case _A2: return _adcReading(deviceData.adc2_y[0], flashData[FLASH_ADC2_ZERO], flashData[FLASH_ADC2_CAL]);
        case _A3: return _adcReading(deviceData.adc3_y[0], flashData[FLASH_ADC3_ZERO], flashData[FLASH_ADC3_CAL]);
        case _A4: return _adcReading(deviceData.adc4_y[0], flashData[FLASH_ADC4_ZERO], flashData[FLASH_ADC4_CAL]);
        case _A5: return _adcReading(deviceData.adc5_y[0], flashData[FLASH_ADC5_ZERO], flashData[FLASH_ADC5_CAL]);
        case _A6: return _adcReading(deviceData.adc6_y[0], flashData[FLASH_ADC6_ZERO], flashData[FLASH_ADC6_CAL]);
        case _A7: return _adcReading(deviceData.adc7_y[0], flashData[FLASH_ADC7_ZERO], flashData[FLASH_ADC7_CAL]);
        case _A8: return _adcReading(deviceData.adc8_y[0], flashData[FLASH_ADC8_ZERO], flashData[FLASH_ADC8_CAL]);
        default: return 0;
    }
}

int32_t _adcReading(double adc, uint32_t zero, uint32_t cal) {
    //incr 0.01 //4096
    double factor = cal / 0.01;
    return;
    (int32_t) adc / factor * 100;
}

void setValue(int port, void * val) {
    if (port >= _O1 && port <= _O8) {
        bool outputState = *(bool *) val;
        switch (port) {
            case _O1:
            {
                if (outputState) O1_SetHigh();
                else O1_SetLow();
            }
                break;
            case _O2:
            {
                if (outputState) O2_SetHigh();
                else O2_SetLow();
            }
                break;
            case _O3:
            {
                if (outputState) O3_SetHigh();
                else O3_SetLow();
            }
                break;
            case _O4:
            {
                if (outputState) O4_SetHigh();
                else O4_SetLow();
            }
                break;
            case _O5:
            {
                if (outputState) O5_SetHigh();
                else O5_SetLow();
            }
                break;
            case _O6:
            {
                if (outputState) O6_SetHigh();
                else O6_SetLow();
            }
                break;
            case _O7:
            {
                if (outputState) O7_SetHigh();
                else O7_SetLow();
            }
                break;
            case _O8:
            {
                if (outputState) O8_SetHigh();
                else O8_SetLow();
            }
                break;
        }
    } else if (port >= _A1 && port <= _A8) {
        double value = *(double *) val;
        if (value == 0) doADCZero(port);
        else if (value > 0) doADCCalibrate(port, value);
    }
}

uint32_t _doADCAverage(int port, uint8_t count) {
    uint8_t i;
    uint32_t reading;
    for (i = 0; i < count; i++) {
        deviceDoADC();
        reading += getADC(port);
    }
    return reading / count;
}

void doADCZero(int port) {
    uint32_t newZeroValue = _doADCAverage(port, 10);
    switch (port) {
        case _A1: _flashSetValue(FLASH_ADC1_ZERO, &newZeroValue);
            break;
        case _A2: _flashSetValue(FLASH_ADC2_ZERO, &newZeroValue);
            break;
        case _A3: _flashSetValue(FLASH_ADC3_ZERO, &newZeroValue);
            break;
        case _A4: _flashSetValue(FLASH_ADC4_ZERO, &newZeroValue);
            break;
        case _A5: _flashSetValue(FLASH_ADC5_ZERO, &newZeroValue);
            break;
        case _A6: _flashSetValue(FLASH_ADC6_ZERO, &newZeroValue);
            break;
        case _A7: _flashSetValue(FLASH_ADC7_ZERO, &newZeroValue);
            break;
        case _A8: _flashSetValue(FLASH_ADC8_ZERO, &newZeroValue);
            break;
        default: return;
    }
}

void doADCCalibrate(int port, float value) {
    uint32_t newCalValue = _doADCAverage(port, 10);
    switch (port) {
        case _A1: _flashSetValue(FLASH_ADC1_CAL, &newCalValue);
            break;
        case _A2: _flashSetValue(FLASH_ADC1_CAL, &newCalValue);
            break;
        case _A3: _flashSetValue(FLASH_ADC1_CAL, &newCalValue);
            break;
        case _A4: _flashSetValue(FLASH_ADC1_CAL, &newCalValue);
            break;
        case _A5: _flashSetValue(FLASH_ADC1_CAL, &newCalValue);
            break;
        case _A6: _flashSetValue(FLASH_ADC1_CAL, &newCalValue);
            break;
        case _A7: _flashSetValue(FLASH_ADC1_CAL, &newCalValue);
            break;
        case _A8: _flashSetValue(FLASH_ADC1_CAL, &newCalValue);
            break;
        default: return;
    }
}

void printOut(char *str) {
    uint8_t numBytes = 0;
    char txBuffer[BUFFER_SIZE];
    
    uint16_t i = 0;
    uint16_t size = strlen ( str);
    for (i = 0; i < size; i++) txBuffer[i] = str[i];

    while (size > numBytes) {

        while (!UART1_IsTxReady());

        UART1_Write(txBuffer[numBytes++]);

    }
}

float getADC(int port) {
    return 0.0;
    switch (port) {
        case _A1: return deviceData.adc1_y[0];
        case _A2: return deviceData.adc2_y[0];
        case _A3: return deviceData.adc3_y[0];
        case _A4: return deviceData.adc4_y[0];
        case _A5: return deviceData.adc5_y[0];
        case _A6: return deviceData.adc6_y[0];
        case _A7: return deviceData.adc7_y[0];
        case _A8: return deviceData.adc8_y[0];
        default: return 0;
    }
}

uint32_t getADCZero(int port) {
    switch (port) {
        case _A1: return flashData[FLASH_ADC1_ZERO];
        case _A2: return flashData[FLASH_ADC2_ZERO];
        case _A3: return flashData[FLASH_ADC3_ZERO];
        case _A4: return flashData[FLASH_ADC4_ZERO];
        case _A5: return flashData[FLASH_ADC5_ZERO];
        case _A6: return flashData[FLASH_ADC6_ZERO];
        case _A7: return flashData[FLASH_ADC7_ZERO];
        case _A8: return flashData[FLASH_ADC8_ZERO];
        default: return 0;
    }
}

uint32_t getADCCal(int port) {
    switch (port) {
        case _A1: return flashData[FLASH_ADC1_CAL];
        case _A2: return flashData[FLASH_ADC2_CAL];
        case _A3: return flashData[FLASH_ADC3_CAL];
        case _A4: return flashData[FLASH_ADC4_CAL];
        case _A5: return flashData[FLASH_ADC5_CAL];
        case _A6: return flashData[FLASH_ADC6_CAL];
        case _A7: return flashData[FLASH_ADC7_CAL];
        case _A8: return flashData[FLASH_ADC8_CAL];
        default: return 0;
    }
}

uint32_t tmp = 1;

const uint32_t * getLogic(int which) {
    switch (which) {
        case 1: return &flashLogic1_2[0];
        case 2: return &flashLogic1_2[32];
        case 3: return &flashLogic3_4[0];
        case 4: return &flashLogic3_4[32];
        case 5: return &flashLogic5_6[0];
        case 6: return &flashLogic5_6[32];
        case 7: return &flashLogic7_8[0];
        case 8: return &flashLogic7_8[32];
    }
}

bool logicExists(const uint32_t * logic) {
    if (logic[0] >= _I1 && logic[0] <= _STR) return true;
    else return false;
}

bool logicSave(uint32_t * logic) {
    if (logicExists(logic)) {
        uint8_t i;
        for (i = 0; i < MAX_COUNT_OF_APP_LOGIC; i++) {
            const uint32_t * l = getLogic(i + 1);
            if (logicExists(l)) continue;
            else {
                switch (i) {
                    case 0:_flashSetValue(FLASH_LOGIC1, logic);
                        break;
                    case 1:_flashSetValue(FLASH_LOGIC2, logic);
                        break;
                    case 2:_flashSetValue(FLASH_LOGIC3, logic);
                        break;
                    case 3:_flashSetValue(FLASH_LOGIC4, logic);
                        break;
                    case 4:_flashSetValue(FLASH_LOGIC5, logic);
                        break;
                    case 5:_flashSetValue(FLASH_LOGIC6, logic);
                        break;
                    case 6:_flashSetValue(FLASH_LOGIC7, logic);
                        break;
                    case 7:_flashSetValue(FLASH_LOGIC8, logic);
                        break;
defualt:
                        return false;
                }
            }
        }
    } else return false;
    return false;
}

bool logicDelete(int index) {
    uint8_t i;
    const uint32_t * logic = getLogic(index);
    if (!logicExists(logic)) return false;
    else {
        switch (i) {
            case 1:_flashSetValue(FLASH_LOGIC1, NULL);
                break;
            case 2:_flashSetValue(FLASH_LOGIC2, NULL);
                break;
            case 3:_flashSetValue(FLASH_LOGIC3, NULL);
                break;
            case 4:_flashSetValue(FLASH_LOGIC4, NULL);
                break;
            case 5:_flashSetValue(FLASH_LOGIC5, NULL);
                break;
            case 6:_flashSetValue(FLASH_LOGIC6, NULL);
                break;
            case 7:_flashSetValue(FLASH_LOGIC7, NULL);
                break;
            case 8:_flashSetValue(FLASH_LOGIC8, NULL);
                break;
defualt:
                return false;
        }
        return true;
    }
    return false;
}

uint16_t getTimeout() {
    return flashData[FLASH_TIMEOUT];
}

bool setTimeout(uint32_t timeInMiliseconds) {
    if (timeInMiliseconds < 1000 || timeInMiliseconds > 30000) {
        return false;
    }
    _flashSetValue(FLASH_TIMEOUT, &timeInMiliseconds);
    return true;
}
