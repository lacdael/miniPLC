#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "device.h"
#include "app.h"

#include "../W-BT.X/mcc_generated_files/uart1.h"

#define TIME_APP_TASK 500
#define APP_STATE_INIT 0
#define APP_STATE_SETUP 1
#define APP_STATE_WORK 2

int appState = APP_STATE_INIT;
bool debug = true;
bool stream = false;
bool slave = true;
int pnt = 0;
char STR_VAL[] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
int taskCount = 0;

char _deviceName[8] = {'\0'};

TimeStamp appTimeStamp;
TimeStamp appTimeoutTimeStamp;

void app_task(void) {

    updateTimeStamp();

    //deviceRecieveData();
    //printState(false);
    
   //return;
    
    switch (appState) {
        case APP_STATE_INIT:
        {
            UART1_Write('i');
            appTimeStamp = getTimeStampNow();
            appTimeoutTimeStamp = getTimeStampNow();

            loadValues();
            if (isInitialised()) appState = APP_STATE_WORK;
            else appState = APP_STATE_SETUP;
        }
            break;
        case APP_STATE_SETUP:
        {
            UART1_Write('s');
            appState = APP_STATE_WORK;
        }
            break;
        case APP_STATE_WORK:
        {
           // UART1_Write('w');
            deviceRecieveData();

            if (getMillisencondElapsed(appTimeStamp) < TIME_APP_TASK) return;
            appTimeStamp = getTimeStampNow();

            deviceDoADC();

           // if (getDoStream()) 
                printState(false);
            
            if (getMillisencondElapsed(appTimeoutTimeStamp) < getValue(_TIMEOUT)) {
                clearSTR();
                appTimeoutTimeStamp = getTimeStampNow();
            }
            //handleLogic();
        }
            break;
    }
}

void logInfo(char *str) {
    if (debug) {
        int pnt = 0;
        memset(txBuffer, '\0', sizeof (txBuffer));
        pnt = bufferAppendString(txBuffer, pnt, "info: ");
        pnt = bufferAppendString(txBuffer, pnt, str);
        txBuffer[pnt++] = '\n';
        txBuffer[pnt++] = '\r';
        printOut(txBuffer);
    }
}

bool getDoStream(void) {
    return stream;
}

void setDoStream(bool b) {
    stream = b;
}

bool getIsSlave(void) {
    return slave;
}

void setIsSave(bool b) {
    slave = b;
}

void error(int what) {
    int pnt = 0;
    memset(txBuffer, '\0', sizeof (txBuffer));
    pnt = bufferAppendString(txBuffer, pnt, "{\"err\":\"");
    switch (what) {
        case ERR_FORMAT: pnt = bufferAppendString(txBuffer, pnt, ERR_MSG_FORMAT);
            break;
        case ERR_VALUE: pnt = bufferAppendString(txBuffer, pnt, ERR_MSG_VALUE);
            break;
        case ERR_LOGIC: pnt = bufferAppendString(txBuffer, pnt, ERR_MSG_LOGIC);
            break;
        case ERR_FIRST_DELETE_LOGIC: pnt = bufferAppendString(txBuffer, pnt, ERR_MSG_FIRST_DELETE_LOGIC);
            break;
        default: pnt = bufferAppendString(txBuffer, pnt, ERR_GENERAL);
            break;
    }
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = '}';
    printOut(txBuffer);
}

void printHelp(void) {
    int i = 0;
    memset(txBuffer, '\0', sizeof (txBuffer));
    i = bufferAppendString(txBuffer, i, (char*) SET);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_SET);
    i = bufferAppendString(txBuffer, i, (char*) NAME);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_NAME);
    i = bufferAppendString(txBuffer, i, (char*) TIMEOUT);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_TIMEOUT);
    i = bufferAppendString(txBuffer, i, (char*) LOGIC);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_LOGIC);
    i = bufferAppendString(txBuffer, i, (char*) DELETE);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_DELETE);
    i = bufferAppendString(txBuffer, i, (char*) ZERO);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_ZERO);
    i = bufferAppendString(txBuffer, i, (char*) GET);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_GET);
    i = bufferAppendString(txBuffer, i, (char*) SLAVE);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_SLAVE);
    i = bufferAppendString(txBuffer, i, (char*) MASTER);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_MASTER);
    i = bufferAppendString(txBuffer, i, (char*) STREAM);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_STREAM);
    i = bufferAppendString(txBuffer, i, (char*) HELP);
    txBuffer[i++] = '\t';
    txBuffer[i++] = ':';
    i = bufferAppendString(txBuffer, i, (char*) HELP_HELP);
    i = bufferAppendString(txBuffer, i, (char*) HELP_LOGIC_EG);
    printOut(txBuffer);
}

/*EXAMPLES:
 * "A1 > 1234 -> O1 = 1";
 * "GET"; X
 * "NAME = TEST"; X
 * "LOGIC"; X
 * "DELETE 1"; X
 * "ZERO A1"; X
 * "STREAM"; X
 * "SET A1 = 1234"; X
 * "SET O1 = 1"; X
 */
void handleLogic(uint32_t logic[MAX_COUNT_OF_APP_LOGIC][MAX_SIZE_OF_LOGIC]) {
    //*PARSE STRING STATES*//

    bool orNotAnd = false;
    bool evaluate = true;

#define HANDLE_LOGIC_LOGIC_SECTION 1
#define HANDLE_LOGIC_THEN_EVALUATE 2
#define HANDLE_LOGIC_ELSE_EVALUATE 3
    int handleLogicState = HANDLE_LOGIC_LOGIC_SECTION;

    int i, j;
    for (i = 0; i < MAX_COUNT_OF_APP_LOGIC; i++) {
        if (logic[i][0] != '\0' && logic[i][0] != 0xFFFFFFFF) {
            bool logicElementState = false;
            for (j = 0; j < MAX_SIZE_OF_LOGIC; j++) {

                switch (handleLogicState) {
                    case HANDLE_LOGIC_LOGIC_SECTION:
                    {
                        if (logic[i][j] >= _I1 && logic[i][j] <= _O8) {
                            logicElementState = evaluteLogicElement(getValue(logic[i][j]),
                                    logic[i][j + 1], logic[i][j + 2], 0);
                            if (i + 2 < MAX_SIZE_OF_LOGIC) i += 2;
                            evaluate = orNotAnd ?
                                    evaluate || logicElementState :
                                    evaluate && logicElementState;
                        } else if (logic[i][j] == _STR) {
                            logicElementState = evaluteLogicElement(getValue(logic[i][j]),
                                    logic[i][j + 1], logic[i][j + 2], logic[i][j + 3]);
                            if (i + 3 < MAX_SIZE_OF_LOGIC)i += 3;
                            evaluate = orNotAnd ?
                                    evaluate || logicElementState :
                                    evaluate && logicElementState;
                        } else if (logic[i][j] == _AND) {
                            orNotAnd = false;
                        } else if (logic[i][j] == _OR) {
                            orNotAnd = true;
                        } else if (logic[i][j] == _THEN) {
                            handleLogicState = HANDLE_LOGIC_THEN_EVALUATE;
                        }
                    }
                        break;
                    case HANDLE_LOGIC_THEN_EVALUATE:
                    {
                        if (logic[i][j] >= _I1 && logic[i][j] <= _O8) {

                            if (evaluate) setLogicElement(getValue(logic[i][j]),
                                    logic[i][j + 1], logic[i][j + 2], 0);
                            i += 2;
                        } else if (logic[i][j] == _STR) {
                            if (evaluate) setLogicElement(getValue(logic[i][j]),
                                    logic[i][j + 1], logic[i][j + 2], logic[i][j + 3]);
                            i += 3;
                        } else if (logic[i][j] == _ELSE) {
                            handleLogicState = HANDLE_LOGIC_ELSE_EVALUATE;
                        }
                    }
                        break;
                    case HANDLE_LOGIC_ELSE_EVALUATE:
                    {
                        if (logic[i][j] >= _I1 && logic[i][j] <= _O8) {
                            if (!evaluate) setLogicElement(getValue(logic[i][j]),
                                    logic[i][j + 1], logic[i][j + 2], 0);
                            i += 2;
                        } else if (logic[i][j] == _STR) {
                            if (!evaluate) setLogicElement(getValue(logic[i][j]),
                                    logic[i][j + 1], logic[i][j + 2], logic[i][j + 3]);
                            i += 3;
                        }
                    }

                }
            }
        }
    }
}

bool evaluteLogicElement(int what, int turnary, uint32_t val1, uint32_t val2) {
    if (what >= _I1 && what <= _O8) {
        switch (turnary) {
            case _EQ: return val1 == getValue(what);
            case _NE: return val1 != getValue(what);
        }
    } else if (what >= _A1 && what <= _A8) {
        switch (turnary) {
            case _EQ: return getValue(what) == val1;
            case _NE: return getValue(what) != val1;
            case _GT: return getValue(what) > val1;
            case _LT: return getValue(what) < val1;
        }
    } else if (what == _STR) {

    }
    return false;
}

void setLogicElement(int what, int turnary, uint32_t val1, uint32_t val2) {
    if (turnary == _EQ) {

        if (what >= _O1 && what <= _O8) {

        } else if (what == _STR) {

        }
    }
}

void clearSTR(void) {
    uint8_t i;
    uint8_t size = sizeof (STR_VAL);
    for (i = 0; i < size; i++) STR_VAL[i] = '\0';
}

bool isSpace(char c) {
    return (c != '\0' && (c <= ' ' || c > '~'));
}

bool isNext(const char *a, const char *b) {
    uint8_t i = 0;
    while (b[i] != '\0' && b[i] >= ' ') {
        //int d = tolower((unsigned char) *a) - tolower((unsigned char) *b);
        if (toupper(a[i]) != toupper(b[i])) return false;
        i++;
    }
    return true;
}

int * isNumber(char *str) {
    int * p;
    static int i = 0;
    int point = 0;
    i = 0;
    while (isdigit(*str) || *str == '-' || *str == '.') {
        if (*str == '-') {
            if (i != 0) break;
        } else if (isdigit(*str)) {

        } else if (*str == '.' && point < 2) {
            point++;
        } else {
            i = 0;
            break;
        }
        str++;
        i++;
    }
    p = &i;
    return p;
}

void printState(bool verbose) {
    static char _value[] = "value";
    static char _cal[] = "cal";
    static char _zero[] = "zero";

    int16_t pnt = 0;
    memset(txBuffer, '\0', sizeof (txBuffer));
    txBuffer[pnt++] = '{';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) O1);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_O1));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) O2);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_O2));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) O3);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_O3));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) O4);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_O4));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) O5);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_O5));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) O6);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_O6));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) O7);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_O7));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) O8);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_O8));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) A1);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    if (verbose) {
        txBuffer[pnt++] = '{';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _value);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A1));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _cal);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCCal(_A1));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _zero);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCZero(_A1));
    } else pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A1));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) A2);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    if (verbose) {
        txBuffer[pnt++] = '{';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _value);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A2));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _cal);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCCal(_A2));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _zero);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCZero(_A2));
    } else pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A2));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) A3);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    if (verbose) {
        txBuffer[pnt++] = '{';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _value);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A3));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _cal);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCCal(_A3));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _zero);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCZero(_A3));
    } else pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A3));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) A4);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    if (verbose) {
        txBuffer[pnt++] = '{';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _value);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A4));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _cal);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCCal(_A4));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _zero);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCZero(_A4));
    } else pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A4));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) A5);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    if (verbose) {
        txBuffer[pnt++] = '{';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _value);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A5));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _cal);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCCal(_A5));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _zero);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCZero(_A5));
    } else pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A5));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) A6);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    if (verbose) {
        txBuffer[pnt++] = '{';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _value);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A6));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _cal);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCCal(_A6));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _zero);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCZero(_A6));
    } else pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A6));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) A7);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    if (verbose) {
        txBuffer[pnt++] = '{';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _value);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A7));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _cal);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCCal(_A7));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _zero);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCZero(_A7));
    } else pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A7));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) A8);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    if (verbose) {
        txBuffer[pnt++] = '{';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _value);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A8));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _cal);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCCal(_A8));
        txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, _zero);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendInt(txBuffer, pnt, getADCZero(_A8));
    } else pnt = bufferAppendFloat(txBuffer, pnt, getADC(_A8));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) I1);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_I1));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) I2);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    pnt = bufferAppendInt(txBuffer, pnt, getValue(_I2));
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) STR);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, STR_VAL);
    txBuffer[pnt++] = '"';
    if (verbose) {
       txBuffer[pnt++] = ',';
        txBuffer[pnt++] = '"';
        pnt = bufferAppendString(txBuffer, pnt, (char *) TIMEOUT);
        txBuffer[pnt++] = '"';
        txBuffer[pnt++] = ':';
        pnt = bufferAppendFloat(txBuffer, pnt, (getValue(_TIMEOUT) / 1000.0)); //as seconds
    }
    txBuffer[pnt++] = ',';
    txBuffer[pnt++] = '"';
    pnt = bufferAppendString(txBuffer, pnt, (char *) SLAVE);
    txBuffer[pnt++] = '"';
    txBuffer[pnt++] = ':';
    if (slave) {
        pnt = bufferAppendString(txBuffer, pnt, "true");
    } else {
        pnt = bufferAppendString(txBuffer, pnt, "false");
    }
    txBuffer[pnt++] = '}';
    printOut(txBuffer);
    return;
}

int bufferAppendInt(char *buf, int16_t pnt, int n) {
    int i, sign;
    char s[8];
    if ((sign = n) < 0) /* record sign */
        n = -n; /* make n positive */
    i = 0;
    do { /* generate digits in reverse order */
        s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0); /* delete it */
    if (sign < 0)
        s[i] = '-';
    else i--;

    while (i >= 0) {
        buf[pnt++] = s[i--];
    }
    return pnt;
}

int bufferAppendFloat(char *buf, int16_t pnt, float n) {
    int32_t n100 = (int32_t) (n * 100.0 + 0.5);
    int32_t n100remainder = n100 % 100;
    int32_t nInt = n100 / 100;
    pnt = bufferAppendInt(buf, pnt, nInt);
    buf[pnt++] = '.';
    pnt = bufferAppendInt(buf, pnt, n100remainder);
    return pnt;
}

int bufferAppendString(char *buf, int16_t pnt, char *s) {
    int len = strlen(s);
    int i = 0;
    while (i < len) {
        buf[pnt++] = s[i++];
    }
    return pnt;
}

void printLogic(void) {
    char json[BUFFER_SIZE];
    int i;
    char * ptr = &json[0];
    int pnt = 0;
    memset(txBuffer, '\0', sizeof (txBuffer));
    bool first = true;
    txBuffer[pnt++] = '[';
    for (i = 0; i < MAX_COUNT_OF_APP_LOGIC; i++) {
        if (first) {
            first = false;
        } else {
            txBuffer[pnt++] = ',';
        }
        txBuffer[pnt++] = '"';
        const uint32_t * logic = getLogic(i + 1);
        if (logicExists(logic)) {
            int state = _CON_IDENTIFIER;
            pnt = bufferAppendString(txBuffer, pnt, (char *) IF);
            for (i = 0; i < MAX_SIZE_OF_LOGIC; i++) {
                switch (state) {
                    case _CON_IDENTIFIER:
                        state = _CON_TURNARY;
                    case _RES_IDENTIFIER_OR_END:
                        if (logic[i] == 0) break;
                        if (state == _RES_IDENTIFIER_OR_END) state = _RES_IDENTIFIER;
                    case _RES_IDENTIFIER:
                    {
                        if (state == _RES_IDENTIFIER) state = _RES_EQUALS;
                        switch (logic[i]) {
                            case _O1: pnt = bufferAppendString(txBuffer, pnt, (char *) O1);
                                break;
                            case _O2: pnt = bufferAppendString(txBuffer, pnt, (char *) O2);
                                break;
                            case _O3: pnt = bufferAppendString(txBuffer, pnt, (char *) O3);
                                break;
                            case _O4: pnt = bufferAppendString(txBuffer, pnt, (char *) O4);
                                break;
                            case _O5: pnt = bufferAppendString(txBuffer, pnt, (char *) O5);
                                break;
                            case _O6: pnt = bufferAppendString(txBuffer, pnt, (char *) O6);
                                break;
                            case _O7: pnt = bufferAppendString(txBuffer, pnt, (char *) O7);
                                break;
                            case _O8: pnt = bufferAppendString(txBuffer, pnt, (char *) O8);
                                break;
                            case _A1: pnt = bufferAppendString(txBuffer, pnt, (char *) A1);
                                break;
                            case _A2: pnt = bufferAppendString(txBuffer, pnt, (char *) A2);
                                break;
                            case _A3: pnt = bufferAppendString(txBuffer, pnt, (char *) A3);
                                break;
                            case _A4: pnt = bufferAppendString(txBuffer, pnt, (char *) A4);
                                break;
                            case _A5: pnt = bufferAppendString(txBuffer, pnt, (char *) A5);
                                break;
                            case _A6: pnt = bufferAppendString(txBuffer, pnt, (char *) A6);
                                break;
                            case _A7: pnt = bufferAppendString(txBuffer, pnt, (char *) A7);
                                break;
                            case _A8: pnt = bufferAppendString(txBuffer, pnt, (char *) A8);
                                break;
                            case _I1: pnt = bufferAppendString(txBuffer, pnt, (char *) I1);
                                break;
                            case _I2: pnt = bufferAppendString(txBuffer, pnt, (char *) I2);
                                break;
                            case _STR: pnt = bufferAppendString(txBuffer, pnt, (char *) STR);
                                break;
                        }
                        break;
                    }
                    case _RES_EQUALS:
                        if (logic[i - 1] == _STR) state = _RES_STRING_OR_NONE;
                        else state = _RES_NUMBER;
                    case _CON_TURNARY:
                    {
                        switch (logic[i]) {
                            case _EQ: pnt = bufferAppendString(txBuffer, pnt, (char *) EQ);
                                break;
                            case _LT: pnt = bufferAppendString(txBuffer, pnt, (char *) LT);
                                break;
                            case _GT: pnt = bufferAppendString(txBuffer, pnt, (char *) GT);
                                break;
                            case _NE: pnt = bufferAppendString(txBuffer, pnt, (char *) NE);
                                break;
                        }
                        if (state == _CON_TURNARY && logic[i - 1] == _STR) state = _CON_STRING_OR_NONE;
                        else if (state == _CON_TURNARY) state = _CON_NUMBER;
                        break;
                    }
                    case _CON_STRING_OR_NONE:
                        state = _CON_LOGIC_OR_IMPLICATION;
                    case _RES_STRING_OR_NONE:
                        if (state == _RES_STRING_OR_NONE) state = _RES_IDENTIFIER_OR_ELSE_OR_END;
                        uint32_t tmp1 = logic[i++];
                        uint32_t tmp2 = logic[i];
                        char tmp[9] = {'\0'};
                        tmp[0] = (tmp1 >> 24)&0xff;
                        tmp[1] = (tmp1 >> 16)&0xff;
                        tmp[2] = (tmp1 >> 8)&0xff;
                        tmp[3] = (tmp1)&0xff;
                        tmp[4] = (tmp2 >> 24)&0xff;
                        tmp[5] = (tmp2 >> 16)&0xff;
                        tmp[6] = (tmp2 >> 8)&0xff;
                        tmp[7] = (tmp2)&0xff;
                        pnt = bufferAppendString(txBuffer, pnt, tmp);
                        break;
                    case _RES_NUMBER:
                    {
                        pnt = bufferAppendInt(txBuffer, pnt, logic[i] / 100);
                        state = _RES_IDENTIFIER_OR_ELSE_OR_END;
                        break;
                    }
                    case _CON_NUMBER:
                    {
                        if (logic[i - 2] >= _A1 && logic[i - 2] >= _A8) pnt = bufferAppendFloat(txBuffer, pnt, logic[i] / 100.0);
                        else pnt = bufferAppendInt(txBuffer, pnt, logic[i]);
                        state = _CON_LOGIC_OR_IMPLICATION;
                        break;
                    }
                    case _RES_IDENTIFIER_OR_ELSE_OR_END:
                        if (logic[i] == 0) break;
                    case _CON_LOGIC_OR_IMPLICATION:
                    {
                        switch (logic[i]) {
                            case _AND: pnt = bufferAppendString(txBuffer, pnt, (char *) AND);
                                break;
                            case _OR: pnt = bufferAppendString(txBuffer, pnt, (char *) OR);
                                break;
                            case _THEN: pnt = bufferAppendString(txBuffer, pnt, (char *) THEN);
                                break;
                            case _ELSE: pnt = bufferAppendString(txBuffer, pnt, (char *) ELSE);
                                break;
                        }
                        break;
                    }
                }
            }
        }
        txBuffer[pnt++] = '"';
    }
    txBuffer[pnt++] = ']';
    printOut(txBuffer);
}

char* boolAsStr(bool b) {
    if (b) return "true";
    else return "false";
}

void response(int cmd, int key, int val) {
    switch (cmd) {
        case _ZERO:
        case _GET:
        case _TIMEOUT:
        case _SLAVE:
        case _MASTER:
        case _SET: printState(true);
            break;
        case _LOGIC:
        case _DELETE: printLogic();
            break;
        case _HELP: printHelp();
            break;
        case _NAME:
        {          
            char tmp[] = {
                (char) (key >> 24)&0xff, (char) (key >> 16)&0xff, (char) (key >> 8)&0xff, (char) key & 0xff,
                (char) (val >> 24)&0xff, (char) (val >> 16)&0xff, (char) (val >> 8)&0xff, (char) val & 0xff, '\0'
            };
//            logInfo("NAME IS =");
//            logInfo(tmp);
            break;
        }
    }
}

bool isRecieveValid(int pnt, int type) {
    bool ok = true;
    bool hasElse = false;
    static const int _ANALOGUE = 111;
    static const int _IDENTIFIER = 112;
    static const int _EQUALS = 114;
    static const int _STRING = 115;
    static const int _NUM = 116;
    int state = -1;
    int next = _CON_IDENTIFIER;
    int i = 0;
    while (i <= pnt && ok && next != _END) {
        if (next == _NUM) {
            if (STACK[1] >= _O1 && STACK[1] <= _O8 && i >= 3 && STACK[0] == _SET) {
                if (STACK[i] == 0 || STACK[i] == 100) {
                    next = _END;
                } else {
                    ok = false;
                }
            } else if (STACK[1] >= _A1 && STACK[1] <= _A8 && i >= 3 && STACK[0] == _SET) {
                //TODO: upper and lower limits of the the analogue. Remember factor of 100.
                next = _END;
            } else if (STACK[0] == _DELETE) {
                if (STACK[i] != 0 && STACK[i] / 100 > 0 && STACK[i] / 100 <= MAX_COUNT_OF_APP_LOGIC) {
                    next = _END;
                    break;
                } else {
                    ok = false;
                }
            } else if (STACK[0] == _TIMEOUT) {
                next = _END;
            } else {
                ok = true;
            }
        } else if (next == _STRING || next == _CON_STRING_OR_NONE || next == _RES_STRING_OR_NONE) {
            uint32_t l = STACK[i];
            uint8_t c = (l >> 24)&0xff;
            if (c > ' ' && c <= '~') {
                if (STACK[0] == _NAME) {
                    logInfo("validate NAME");
                    next = _END;
                } else if (next == _CON_STRING_OR_NONE) {
                    i++;
                    state = _CON_STRING_OR_NONE;
                    next = _CON_LOGIC_OR_IMPLICATION;
                } else if (next == _RES_STRING_OR_NONE) {
                    i++;
                    state = _RES_STRING_OR_NONE;
                    next = _RES_IDENTIFIER_OR_ELSE_OR_END;
                }
            } else if (next == _CON_STRING_OR_NONE) {
                i++;
                state = _CON_STRING_OR_NONE;
                next = _CON_LOGIC_OR_IMPLICATION;
            } else if (next == _RES_STRING_OR_NONE) {
                i++;
                state = _RES_STRING_OR_NONE;
                next = _RES_IDENTIFIER_OR_ELSE_OR_END;
            }
        } else if (next == _CON_NUMBER) {
            if (STACK[i - 2] >= _I1 && STACK[i - 2] <= _O8) {
                if (STACK[i] != 0 && STACK[i] != 100) {
                    ok = false;
                    next = _END;
                } else {
                    next = _CON_LOGIC_OR_IMPLICATION;
                    state = _CON_NUMBER;
                }
            } else if (STACK[i - 2] >= _A1 && STACK[i - 2] <= _A8) {
                state = _CON_NUMBER;
                next = _CON_LOGIC_OR_IMPLICATION;
            }
        } else if (next == _RES_NUMBER) {
            if (STACK[i] == 0 || STACK[i] == 100) {
                if (i < MAX_SIZE_OF_LOGIC - 1) {
                    if (STACK[i + 1] == 0) {
                        next = _END;
                        break;
                    } else next = _RES_IDENTIFIER_OR_ELSE_OR_END; //_RES_IDENTIFIER_OR_END;
                } else {
                    next = _END;
                    break;
                }
                state = _RES_NUMBER;
            } else {
                ok = false;
            }
        } else if (next == _CON_TURNARY && state == _CON_IDENTIFIER) { //TURNARY CON
            if (STACK[i - 1] >= _I1 && STACK[i - 1] <= _STR) {
                state = _CON_TURNARY;
                if (STACK[i - 1] == _STR) {
                    next = _CON_STRING_OR_NONE;
                } else {
                    next = _CON_NUMBER;
                }
            }
        } else if (next == _CON_LOGIC_OR_IMPLICATION) { //LOGIC
            state = _CON_LOGIC_OR_IMPLICATION;
            if (STACK[i] == _AND) {
                next = _CON_IDENTIFIER;
            } else if (STACK[i] == _OR) {
                next = _CON_IDENTIFIER;
            } else if (STACK[i] == _THEN) {
                next = _RES_IDENTIFIER;
            } else {
                ok = false;
                break;
            }
        } else if (STACK[i] >= _I1 && STACK[i] <= _STR) { // INPUT | OUTPUT | ANALOGUE PORT | COMMUNICATION
            if (i != 0 && next != -1) {
                if (next == _ANALOGUE && STACK[i] >= _A1 && STACK[i] <= _A8 && STACK[0] == _ZERO) { // ZERO ANALOGUE
                    next = _END;
                } else if (next == _IDENTIFIER && (STACK[i] >= _O1 && STACK[i] <= _A8) && STACK[0] == _SET) { // SET OUT | ADC
                    next = _EQUALS;
                } else if (next == _RES_IDENTIFIER) {
                    next = _RES_EQUALS;
                    state = _RES_IDENTIFIER;
                } else if (next == _RES_IDENTIFIER_OR_ELSE_OR_END) {
                    next = _RES_EQUALS;
                    state = _RES_IDENTIFIER_OR_ELSE_OR_END;
                } else if (next == _CON_IDENTIFIER || (state == _CON_IDENTIFIER && next == -1)) {
                    next = _CON_TURNARY;
                    state = _CON_IDENTIFIER;
                } else {
                    ok = false;
                    break;
                }
            } else if (next == _CON_IDENTIFIER) {
                next = _CON_TURNARY;
                state = _CON_IDENTIFIER;
            } else {
            }
        } else if (next == _EQUALS) {
            if (STACK[0] == _SET && i == 2) {
                next = _NUM;
            } else if (STACK[0] == _NAME && i == 1) {
                next = _STRING;
            } else if (STACK[0] == _TIMEOUT && i == 1) {
                next = _NUM;
            } else {
                ok = false;
            }
        } else if (STACK[i] >= _EQ && STACK[i] <= _LT) { //TURNARY
            if (next == _RES_EQUALS) {
                if (STACK[i - 1] == _STR) {
                    state = _RES_EQUALS;
                    next = _RES_STRING_OR_NONE;
                } else if (STACK[i - 1] >= _O1 && STACK[i - 1] <= _O8) {
                    next = _RES_NUMBER;
                    state = _RES_EQUALS;
                } else {
                    ok = false;
                }
            } else {
                ok = false;
            }
        } else if (STACK[i] >= _AND && STACK[i] <= _ELSE && i > 0) { //LOGIC
            if (next == _CON_LOGIC_OR_IMPLICATION) {
                state = _CON_LOGIC_OR_IMPLICATION;
                next = _CON_TURNARY;
            } else if (next == _RES_IDENTIFIER_OR_ELSE_OR_END) {
                state = _RES_IDENTIFIER_OR_ELSE_OR_END;
                next = _RES_IDENTIFIER;
                hasElse = true;
            }
        } else if (STACK[i] == _GET && i == 0) { //COMMAND: GET
            next = _END;
        } else if (STACK[i] == _SET && i == 0) { //COMMAND: SET
            next = _IDENTIFIER;
        } else if (STACK[i] == _NAME && i == 0) { //COMMAND: NAME
            next = _EQUALS;
        } else if (STACK[i] == _TIMEOUT && i == 0) { //COMMAND: TIMEOUT
            next = _EQUALS;
        } else if (STACK[i] == _STREAM && i == 0) { //COMMAND: STREAM
            next = _END;
        } else if (STACK[i] == _SLAVE && i == 0) { //COMMAND: SLAVE
            next = _END;
        } else if (STACK[i] == _MASTER && i == 0) { //COMMAND: SLAVE
            next = _END;
        } else if (STACK[i] == _LOGIC && i == 0) { //COMMAND: LOGIC
            next = _END;
        } else if (STACK[i] == _HELP && i == 0) { //COMMAND: LOGIC
            next = _END;
        } else if (STACK[0] == _DELETE && i == 0) { //COMMAND: DELETE
            next = _NUM;
        } else if (STACK[i] == _ZERO && i == 0) { //COMMAND: ZERO
            next = _ANALOGUE;
        } else if (STACK[i] == _IF && i == 0) { //START OF LOGIC
            next = _CON_IDENTIFIER;
        } else if (next == _RES_IDENTIFIER_OR_ELSE_OR_END) {
            next = _END;
        } else {
            ok = false;
            //  PRINT_OUT("ElSE OK = false; %d",STACK[i]);
        }
        i++;
    }
    //printf("%d  %d   %d    %d",type, ok, next, hasElse);
    if (type == _END) return ((ok || next == _END) || (!hasElse && next == _RES_IDENTIFIER_OR_ELSE_OR_END) || (hasElse && next == _RES_IDENTIFIER_OR_END));
    else return ok;

    return false;
}


void setName(uint32_t val1, uint32_t val2){
   _deviceName[0] = (char) (val1 >> 24)&0xff;
   _deviceName[1] = (char) (val1 >> 16)&0xff;
   _deviceName[2] = (char) (val1 >> 8)&0xff;
   _deviceName[3] = (char) val1 & 0xff;
   _deviceName[4] = (char) (val2 >> 24)&0xff;
   _deviceName[5] = (char) (val2 >> 16)&0xff;
   _deviceName[6] = (char) (val2 >> 8)&0xff;
   _deviceName[7] = (char) val2 & 0xff;
   logInfo("NAME IS =");
   logInfo(_deviceName);
   //TODO: logic to change BT NAME
   //appState = APP_STATE_SETUP;
}

void handleStack(int pnt) {
    //*PARSE STRING STATES*//
    static const int _ANALOGUE = 111;
    static const int _IDENTIFIER = 112;
    static const int _STRING = 115;
    static const int _NUM = 116;
    // static const int _END = 117;
    int next = -1;
    //*PARSE STRING STATES*//
    int i;
    for (i = 0; i <= pnt; i++) {
        if (next == _STRING) {
            if (STACK[0] == _NAME) {
                //response(_NAME, STACK[i], STACK[i + 1]);
                setName(STACK[i],STACK[i + 1]);
            }
        } else if (next == _NUM) {
            if (STACK[0] == _SET && STACK[1] >= _A1 && STACK[1] <= _A8) {
                float tmp = (STACK[i] / 100.0);
                doADCCalibrate(STACK[1], tmp);
                response(_SET, STACK[1], STACK[i]);
                break;
            } else if (STACK[0] == _SET && STACK[1] >= _O1 && STACK[1] <= _O8) {
                uint8_t v = (uint8_t) STACK[i] / 100;
                setValue(STACK[1], &v);
                response(_SET, STACK[1], STACK[i]);
                break;
            } else if (STACK[0] == _DELETE) {
                if (logicDelete(STACK[i] / 100)) response(_DELETE, 0, 0);
                else error(-1);
                break;
            } else if (STACK[0] == _TIMEOUT) {
                //TODO: if (assert());
                setValue(_TIMEOUT, &STACK[i]);
                response(_TIMEOUT, 0, 0);
                //error(ERR_VALUE);
                break;
            }
        } else if (i == 0) {
            if (STACK[0] == _GET) {
                if (stream) stream = false;
                response(_GET, 0, 0);
            } else if (STACK[0] == _SET) {
                next = _IDENTIFIER;
            } else if (STACK[0] == _LOGIC) {
                response(_LOGIC, 0, 0);
            } else if (STACK[0] == _STREAM) {
                if (stream) stream = false;
                else stream = true;
            } else if (STACK[0] == _SLAVE) {
                setIsSave(true);
                response(_SLAVE, 0, 0);
            } else if (STACK[0] == _MASTER) {
                setIsSave(false);
                response(_MASTER, 0, 0);
            } else if (STACK[0] == _HELP) {
                response(_HELP, 0, 0);
            } else if (STACK[0] == _NAME) {
                next = _EQ;
            } else if (STACK[0] == _TIMEOUT) {
                next = _EQ;
            } else if (STACK[0] == _DELETE) {
                next = _NUM;
            } else if (STACK[0] == _ZERO) {
                next = _ANALOGUE;
            } else if (STACK[0] == _IF) { // I/O PORT
                i++;
                if (STACK[i] >= _I1 && STACK[i] <= _STR);
                else {
                    error(ERR_LOGIC);
                    break;
                }
                int n = _CON_TURNARY;
                bool logicOK = false;
                while (i <= pnt) {
                    if (n == _CON_NUMBER || n == _RES_NUMBER) {
                        i++;
                        if (n == _CON_NUMBER) n = _CON_LOGIC_OR_IMPLICATION;
                        else if (n == _RES_NUMBER) {
                            logicOK = true;
                            n = _RES_IDENTIFIER_OR_ELSE_OR_END;
                        }
                    } else if (n == _CON_STRING_OR_NONE || n == _RES_STRING_OR_NONE) {
                        i += 2;
                        if (n == _CON_STRING_OR_NONE) n = _CON_LOGIC_OR_IMPLICATION;
                        else if (n == _RES_STRING_OR_NONE) {
                            logicOK = true;
                            n = _RES_IDENTIFIER_OR_ELSE_OR_END;
                        }
                    } else if (STACK[i] >= _EQ && STACK[i] <= _LT) { //TURNARY
                        if (n == _CON_TURNARY && STACK[i - 1] == _STR) n = _CON_STRING_OR_NONE;
                        else if (n == _CON_TURNARY) n = _CON_NUMBER;
                        else if (n == _RES_EQUALS && STACK[i - 1] == _STR) n = _RES_STRING_OR_NONE;
                        else if (n == _RES_EQUALS) n = _RES_NUMBER;
                        i++;
                    } else if (STACK[i] >= _AND && STACK[i] <= _ELSE) { //LOGIC
                        if (n == _CON_LOGIC_OR_IMPLICATION && STACK[i] == _THEN) n = _RES_IDENTIFIER;
                        else if (n == _RES_IDENTIFIER_OR_ELSE_OR_END && STACK[i] == _ELSE) n = _RES_IDENTIFIER;
                        else n = _CON_IDENTIFIER;
                        i++;
                    } else if (STACK[i] >= _I1 && STACK[i] <= _STR) {
                        if (n == _CON_IDENTIFIER) n = _CON_TURNARY;
                        else if (n == _RES_IDENTIFIER) n = _RES_EQUALS;
                        i++;
                    } else {
                        i++;
                    }
                }
                if (logicOK) {
                    if (logicSave(&STACK[1])) response(_LOGIC, 0, 0);
                    else error(ERR_FIRST_DELETE_LOGIC);
                } else error(ERR_LOGIC);
                break;
            }
        } else if (next == _ANALOGUE && STACK[0] == _ZERO) {
            doADCZero(STACK[i]);
            response(_ZERO, 0, 0);
        } else if (next == _IDENTIFIER && STACK[0] == _SET) {
            next = _EQ;
        } else if (next == _EQ) {
            if (STACK[0] == _SET) next = _NUM;
            else if (STACK[0] == _NAME) next = _STRING;
            else if (STACK[0] == _TIMEOUT) next = _NUM;
        }
    }
}

void parseInput(char * TEST, int size) {
    bool justAString = false;
    int i = 0;
    while (i < size && TEST[i] != '\0') {
        //  justAString = false;
        while (isSpace(TEST[i])) i++;
        if (isNext(&TEST[i], GET)) {
            i += 3;
            STACK[pnt] = _GET;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], NAME)) {
            i += 4;
            STACK[pnt] = _NAME;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], SET)) {
            i += 3;
            STACK[pnt] = _SET;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], LOGIC)) {
            i += 5;
            STACK[pnt] = _LOGIC;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], DELETE)) {
            i += 6;
            STACK[pnt] = _DELETE;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], TIMEOUT)) {
            i += 7;
            STACK[pnt] = _TIMEOUT;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], ZERO)) {
            i += 4;
            STACK[pnt] = _ZERO;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], STREAM)) {
            i += 6;
            STACK[pnt] = _STREAM;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], SLAVE)) {
            i += 4;
            STACK[pnt] = _SLAVE;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], MASTER)) {
            i += 6;
            STACK[pnt] = _MASTER;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], HELP)) {
            i += 4;
            STACK[pnt] = _HELP;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], NONE)) {
            i += 4;
            STACK[pnt++] = 0;
            STACK[pnt] = 0;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A1)) {
            i += 2;
            STACK[pnt] = _A1;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A2)) {
            i += 2;
            STACK[pnt] = _A2;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A3)) {
            i += 2;
            STACK[pnt] = _A3;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A4)) {
            i += 2;
            STACK[pnt] = _A4;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A5)) {
            i += 2;
            STACK[pnt] = _A5;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A6)) {
            i += 2;
            STACK[pnt] = _A6;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A7)) {
            i += 2;
            STACK[pnt] = _A7;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A8)) {
            i += 2;
            STACK[pnt] = _A8;
            if (isRecieveValid(pnt, 0)) pnt++;
        } else if (isNext(&TEST[i], O1)) {
            i += 2;
            STACK[pnt] = _O1;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O2)) {
            i += 2;
            STACK[pnt] = _O2;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O3)) {
            i += 2;
            STACK[pnt] = _O3;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O4)) {
            i += 2;
            STACK[pnt] = _O4;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O5)) {
            i += 2;
            STACK[pnt] = _O5;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O6)) {
            i += 2;
            STACK[pnt] = _O6;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O7)) {
            i += 2;
            STACK[pnt] = _O7;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O8)) {
            i += 2;
            STACK[pnt] = _O8;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], I1)) {
            i += 2;
            STACK[pnt] = _I1;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], I2)) {
            i += 2;
            STACK[pnt] = _I2;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], STR)) {
            i += 3;
            STACK[pnt] = _STR;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], AND)) {
            i += 3;
            STACK[pnt] = _AND;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], OR)) {
            i += 2;
            STACK[pnt] = _OR;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], EQ)) {
            i += 1;
            STACK[pnt] = _EQ;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], NE)) {
            i += 2;
            STACK[pnt] = _NE;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], IF)) {
            i += 2;
            STACK[pnt] = _IF;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], THEN)) {
            i += 4;
            STACK[pnt] = _THEN;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], ELSE)) {
            i += 4;
            STACK[pnt] = _ELSE;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], GT)) {
            i += 1;
            STACK[pnt] = _GT;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], LT)) {
            i += 1;
            STACK[pnt] = _LT;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if ((numberCharLen = isNumber(&TEST[i])) && *numberCharLen > 0) {
            //float num;
            //sscanf(&TEST[i], "%f", &num);


            int j = 0;
            uint8_t sign = 1;
            uint8_t point = 0;
            uint16_t div = 1;
            float num = 0;

            while (j < *numberCharLen) {
                if (TEST[i + j] == '-') {
                    if (j != 0) break;
                    sign = -1;
                } else if (isdigit(TEST[i + j])) {
                    num = num * 10;
                    num += TEST[i + j] - '0';
                    if (point == 1) div = div * 10;
                } else if (TEST[i + j] == '.' && point < 2) {
                    point++;
                } else {
                    break;
                }
                j++;
            }

            num = num / div;
            //num = num*sign;
            //TODO: -ve numbers unisghned converstion somewhere

            i += *numberCharLen;

            int32_t tmp = (int32_t) (num * 100.0);
            tmp = tmp * sign;
            STACK[pnt] = tmp;


            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else {
            int j = 0;
            int k = i;
            char tmp[] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
            while (j < 8 && k < size && TEST[k] > ' ' && TEST[k] <= '~') {
                tmp[j++] = TEST[k++];
            }
            if (i == 0) {
                STR_VAL[0] = tmp[0];
                STR_VAL[1] = tmp[1];
                STR_VAL[2] = tmp[2];
                STR_VAL[3] = tmp[3];
                STR_VAL[4] = tmp[4];
                STR_VAL[5] = tmp[5];
                STR_VAL[6] = tmp[6];
                STR_VAL[7] = tmp[7];
                justAString = true;
                break;
            }

            i += j;
            STACK[pnt] = (tmp[0]&0xff) << 24 | (tmp[1]&0xff) << 16 | (tmp[2]&0xff) << 8 | (tmp[3]&0xff);
            STACK[++pnt] = (tmp[4]&0xff) << 24 | (tmp[5]&0xff) << 16 | (tmp[6]&0xff) << 8 | (tmp[7]&0xff);
            if (isRecieveValid(pnt, 0)) {
                pnt++;
            } else break;
        }
        i++;
    }
    if (isRecieveValid(pnt, _END)) handleStack(pnt);
    else if (!justAString) {
        if (STACK[0] == _IF) error(ERR_LOGIC);
        else if (STACK[0] == _SET && pnt == 3) error(ERR_VALUE);
        else if (STACK[0] == _DELETE && pnt == 1) error(ERR_VALUE);
        else error(ERR_FORMAT);
    }
}



