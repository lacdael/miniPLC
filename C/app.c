#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "device.h"
#include "app.h"


bool debug = true;
bool stream = false;
bool slave = true;
int pnt = 0;
char STR[] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

//put this define in device.h
#define THREE_SECONDS 3
int taskCount = 0;

void app_task(void){
    if(getDoStream()) get();
    //Clear buffers
    //memset(STACK,0,sizeof(STACK));
    if (taskCount++ > THREE_SECONDS){
        clearSTR();
        taskCount = 0;
    }
    //If data in buffer
        //parse input
    //THE LOGIC needs to be an extern defined device.c
    //handleLogic(LOGIC);
}

void logInfo(char *str) {
    if (debug) {
        printf("\033[0;35m"); //Set the text to the color red
        printf("%s\r\n", str);
        printf("\033[0m");
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
    printf("{\"err\":\"%s\"}",
            (
            what == ERR_FORMAT ? ERR_MSG_FORMAT :
            what == ERR_VALUE ? ERR_MSG_VALUE :
            what == ERR_LOGIC ? ERR_MSG_LOGIC :
            what == ERR_FIRST_DELETE_LOGIC ? ERR_MSG_FIRST_DELETE_LOGIC : ERR_GENERAL
            )
            );
}

void printHelp(void) {
    printf("SET\t: Set a port's state, or, do a calibration. `SET [O|A][1-8] = [1|0]|<number>` e.g. SET O1 = 1 e.g. SET A1 = 20.0\n");
    printf("NAME\t: Set the bluetooth name. `NAME = <name>` e.g. NAME = myDevice\n");
    printf("TIMEOUT\t: Set the timeout for clearing a string. `TIMEOUT = <seconds>` e.g. TIMEOUT = 5\n");
    printf("LOGIC\t: Get the device's logic as a json array. `LOGIC`.\n");
    printf("DELETE\t: Delete logic from the device. `DELETE <index>` e.g. DELETE 1.\n");
    printf("ZERO\t: Zero an analogue port. `ZERO A[1-8]` e.g. ZERO A1.\n");
    printf("GET\t: Get the verbose state of the device as a json object. `GET`.\n");
    printf("SLAVE\t: Stop the evaluation of logic (default state). `SLAVE`.\n");
    printf("MASTER\t: Start the evaluation of logic. `MASTER`.\n");
    printf("STREAM\t: Toggle on/off a streamed output of the device's state as a json object. `STREAM`.\n");
    printf("HELP\t: This message.\n");
    printf("\nThe logic: IF [[O[1-8]|A[1-8]|STR] [=|!=|>|<] [<number>|<string>|NONE] [AND|OR]] THEN [O[1-8] | STR] = [[1|0]|<string>|NONE] [ ELSE [O[1-8] | STR] = [[1|0]|<string>|NONE]]\n");
    printf("\n\te.g. IF A1 > 5 AND I1 != 1 OR STR = password THEN O1 = 1 O2 = 0 ELSE O1 = 0 O2 = 0\n");
    printf("\n\te.g. IF A2 < 1.2 AND A3 < 3 THEN STR = password\n");
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
    logInfo("\t\t\t@handleLogic()");
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
                            if (i +2 < MAX_SIZE_OF_LOGIC) i += 2;
                            evaluate = orNotAnd ?
                                    evaluate || logicElementState :
                                    evaluate && logicElementState;
                        } else if (logic[i][j] == _STR) {
                            logicElementState = evaluteLogicElement(getValue(logic[i][j]),
                                    logic[i][j + 1], logic[i][j + 2], logic[i][j + 3]);
                            if (i +3 < MAX_SIZE_OF_LOGIC)i += 3;
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
                    } break;
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
    if (what >= _I1 && what <= _O8){
        switch(turnary){
            case _EQ: return val1 == getValue(what);
            case _NE: return val1 != getValue(what);
         }        
    } else if (what >= _A1 && what <= _A8){
        switch(turnary){
            case _EQ: return getValue(what) == val1;
            case _NE: return getValue(what) != val1;
            case _GT: return getValue(what) > val1;
            case _LT: return getValue(what) < val1;
         }
    } else if (what == _STR){
    
    }
    return false;
}

void setLogicElement(int what, int turnary, uint32_t val1, uint32_t val2) {
    if (turnary == _EQ){
            
    if (what >= _O1 && what <= _O8){
            
    } else if (what == _STR){
    
    }
    }
}

void clearSTR(void) {
    memset(STR, '\0', sizeof (STR));
}

bool isSpace(char c) {
    return (c != '\0' && (c <= ' ' || c > '~'));
}

bool isNext(const char *a, const char *b) {
    if (strncasecmp(a, b, strlen(b)) == 0) return 1;
    return 0;
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

void get(void) {
    printf("{\"O1\":%s,\"O2\":%s,\"O3\":%s,\"O4\":%s,\"O5\":%s,"
            "\"O6\":%s,\"O7\":%s,\"O8\":%s,\"A1\":%.2f,\"A2\":%.2f,"
            "\"A3\":%.2f,\"A4\":%.2f,\"A5\":%.2f,\"A6\":%.2f,\"A7\":%.2f,"
            "\"A8\":%.2f,\"I1\":%s,\"I2\":%s,\"STR\":\"%s\",\"SLAVE\",%s}",
            boolAsStr(getIO(_O1)),
            boolAsStr(getIO(_O2)),
            boolAsStr(getIO(_O3)),
            boolAsStr(getIO(_O4)),
            boolAsStr(getIO(_O5)),
            boolAsStr(getIO(_O6)),
            boolAsStr(getIO(_O7)),
            boolAsStr(getIO(_O8)),
            getADC(_A1),
            getADC(_A2),
            getADC(_A3),
            getADC(_A4),
            getADC(_A5),
            getADC(_A6),
            getADC(_A7),
            getADC(_A8),
            boolAsStr(getIO(_I1)),
            boolAsStr(getIO(_I2)),
            STR,
            boolAsStr(getIsSlave()));
}

void getVerbose(void) {
    printf("{\"O1\":%s,\"O2\":%s,\"O3\":%s,\"O4\":%s,"
            "\"O5\":%s,\"O6\":%s,\"O7\":%s,\"O8\":%s,"
            "\"A1\":{\"value\":%.2f,\"cal\":%d,\"zero\":%d},"
            "\"A2\":{\"value\":%.2f,\"cal\":%d,\"zero\":%d},"
            "\"A3\":{\"value\":%.2f,\"cal\":%d,\"zero\":%d},"
            "\"A4\":{\"value\":%.2f,\"cal\":%d,\"zero\":%d},"
            "\"A5\":{\"value\":%.2f,\"cal\":%d,\"zero\":%d},"
            "\"A6\":{\"value\":%.2f,\"cal\":%d,\"zero\":%d},"
            "\"A7\":{\"value\":%.2f,\"cal\":%d,\"zero\":%d},"
            "\"A8\":{\"value\":%.2f,\"cal\":%d,\"zero\":%d},"
            "\"I1\":%s,\"I2\":%s,\"STR\":\"%s\",\"SLAVE\",%s}",
            boolAsStr(getIO(_O1)),
            boolAsStr(getIO(_O2)),
            boolAsStr(getIO(_O3)),
            boolAsStr(getIO(_O4)),
            boolAsStr(getIO(_O5)),
            boolAsStr(getIO(_O6)),
            boolAsStr(getIO(_O7)),
            boolAsStr(getIO(_O8)),
            getADC(_A1), getADCCal(_A1), getADCZero(_A1),
            getADC(_A2), getADCCal(_A2), getADCZero(_A2),
            getADC(_A3), getADCCal(_A3), getADCZero(_A3),
            getADC(_A4), getADCCal(_A4), getADCZero(_A4),
            getADC(_A5), getADCCal(_A5), getADCZero(_A5),
            getADC(_A6), getADCCal(_A6), getADCZero(_A6),
            getADC(_A7), getADCCal(_A7), getADCZero(_A7),
            getADC(_A8), getADCCal(_A8), getADCZero(_A8),
            boolAsStr(getIO(_I1)),
            boolAsStr(getIO(_I2)),
            STR,
            boolAsStr(getIsSlave()));
}

void getLogic(void) {
    char json[MAX_TX_BUUFFER];
    int i;
    char *pnt = json;
    bool first = true;
    for (i = 0; i < MAX_COUNT_OF_APP_LOGIC; i++) {
        if (first) {
            pnt += sprintf(pnt, "[\"");
            first = false;
        } else pnt += sprintf(pnt, ",\"");
        if (LOGIC_STACK[i][0] != '\0' && LOGIC_STACK[i][0] != 0xFFFFFFFF) pnt = logicToString(pnt, LOGIC_STACK[i]);
        pnt += sprintf(pnt, "\"");
    }
    printf("%s]", json);
}

char* boolAsStr(bool b) {
    if (b) return "true";
    else return "false";
}

void response(int cmd, int key, int val) {
    switch (cmd) {
        case _LOGIC: getLogic();
            break;
        case _ZERO:
        case _GET: getVerbose();
            break;
        case _SLAVE:
        case _MASTER:
        case _SET: get();
            break;
        case _DELETE: getLogic();
            break;
        case _HELP: printHelp();
            break;
        case _NAME:
        {
            char tmp[] = {
                (char) (key >> 24)&0xff, (char) (key >> 16)&0xff, (char) (key >> 8)&0xff, (char) key & 0xff,
                (char) (val >> 24)&0xff, (char) (val >> 16)&0xff, (char) (val >> 8)&0xff, (char) val & 0xff, '\0'
            };
            logInfo(tmp);
            break;
        }
    }
}

char * logicToString(char *tx, int32_t logic[]) {
    logInfo("\t\t\t\t#logicToString()");
    int state = _CON_IDENTIFIER;
    char *pnt = tx;
    int i;
    pnt += sprintf(pnt, "%s ", IF);
    for (i = 0; i < MAX_SIZE_OF_LOGIC; i++) {
        switch (state) {
            case _CON_IDENTIFIER:
                state = _CON_TURNARY;
            case _RES_IDENTIFIER_OR_END:
                if (logic[i] == 0) break;
                if (state == _RES_IDENTIFIER_OR_END) state = _RES_IDENTIFIER;
            case _RES_IDENTIFIER:
            {
                logInfo("\t\t\t\t@IDENTIFIER");

                if (state == _RES_IDENTIFIER) state = _RES_EQUALS;
                switch (logic[i]) {
                    case _O1: pnt += sprintf(pnt, "%s ", O1);
                        break;
                    case _O2: pnt += sprintf(pnt, "%s ", O2);
                        break;
                    case _O3: pnt += sprintf(pnt, "%s ", O3);
                        break;
                    case _O4: pnt += sprintf(pnt, "%s ", O4);
                        break;
                    case _O5: pnt += sprintf(pnt, "%s ", O5);
                        break;
                    case _O6: pnt += sprintf(pnt, "%s ", O6);
                        break;
                    case _O7: pnt += sprintf(pnt, "%s ", O7);
                        break;
                    case _O8: pnt += sprintf(pnt, "%s ", O8);
                        break;
                    case _A1: pnt += sprintf(pnt, "%s ", A1);
                        break;
                    case _A2: pnt += sprintf(pnt, "%s ", A2);
                        break;
                    case _A3: pnt += sprintf(pnt, "%s ", A3);
                        break;
                    case _A4: pnt += sprintf(pnt, "%s ", A4);
                        break;
                    case _A5: pnt += sprintf(pnt, "%s ", A5);
                        break;
                    case _A6: pnt += sprintf(pnt, "%s ", A6);
                        break;
                    case _A7: pnt += sprintf(pnt, "%s ", A7);
                        break;
                    case _A8: pnt += sprintf(pnt, "%s ", A8);
                        break;
                    case _I1: pnt += sprintf(pnt, "%s ", I1);
                        break;
                    case _I2: pnt += sprintf(pnt, "%s ", I2);
                        break;
                    case _STR: pnt += sprintf(pnt, "%s ", COM);
                        break;
                }
                break;
            }
            case _RES_EQUALS:
                if (logic[i - 1] == _STR) state = _RES_STRING_OR_NONE;
                else state = _RES_NUMBER;
            case _CON_TURNARY:
            {
                logInfo("\t\t\t\t@TURNARY");
                switch (logic[i]) {
                    case _EQ: pnt += sprintf(pnt, "%s ", EQ);
                        break;
                    case _LT: pnt += sprintf(pnt, "%s ", LT);
                        break;
                    case _GT: pnt += sprintf(pnt, "%s ", GT);
                        break;
                    case _NE: pnt += sprintf(pnt, "%s ", NE);
                        break;
                }
                if (state == _CON_TURNARY && logic[i - 1] == _STR) state = _CON_STRING_OR_NONE;
                else if (state == _CON_TURNARY) state = _CON_NUMBER;
                break;
            }
            case _CON_STRING_OR_NONE:
                state = _CON_LOGIC_OR_IMPLICATION;
            case _RES_STRING_OR_NONE:
                logInfo("\t\t\t\t@STRING");
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
                pnt += sprintf(pnt, "%s ", (tmp[0] == '\0' ? "NONE" : tmp));
                break;
            case _RES_NUMBER:
            {
                pnt += sprintf(pnt, "%d ", logic[i] / 100);
                state = _RES_IDENTIFIER_OR_ELSE_OR_END;
                break;
            }
            case _CON_NUMBER:
            {
                logInfo("\t\t\t\t@NUMBER");
                if (logic[i - 2] >= _A1 && logic[i - 2] >= _A8) pnt += sprintf(pnt, "%0.2f ", logic[i] / 100.0);
                else pnt += sprintf(pnt, "%d ", logic[i] / 100);
                state = _CON_LOGIC_OR_IMPLICATION;
                break;
            }
            case _RES_IDENTIFIER_OR_ELSE_OR_END:
                if (logic[i] == 0) break;
            case _CON_LOGIC_OR_IMPLICATION:
            {
                logInfo("\t\t\t\t@LOGIC");
                switch (logic[i]) {
                    case _AND:
                        state = _CON_IDENTIFIER;
                        pnt += sprintf(pnt, "%s ", AND);
                        break;
                    case _OR:
                        state = _CON_IDENTIFIER;
                        pnt += sprintf(pnt, "%s ", OR);
                        break;
                    case _THEN:
                        state = _RES_IDENTIFIER;
                        pnt += sprintf(pnt, "%s ", THEN);
                        break;
                    case _ELSE:
                        state = _RES_IDENTIFIER;
                        pnt += sprintf(pnt, "%s ", ELSE);
                        break;
                }
                break;
            }
        }
    }
    return pnt;
}

bool isRecieveValid(int pnt, int type) {
    logInfo("\t#isRecieveValid()");
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
            logInfo("\t@NUMBER");
            if (STACK[1] >= _O1 && STACK[1] <= _O8 && i >= 3 && STACK[0] == _SET) {
                if (STACK[i] == 0 || STACK[i] == 100) {
                    next = _END;
                } else {
                    ok = false;
                }
            } else if (STACK[1] >= _A1 && STACK[1] <= _A8 && i >= 3 && STACK[0] == _SET) {
                //TODO: upper and lower limits of the the analogue. Remember factor of 100.
                if (true) {
                    next = _END;
                } else ok = false;
            } else if (STACK[0] == _DELETE) {
                if (STACK[i] != 0 && STACK[i] / 100 > 0 && STACK[i] / 100 <= MAX_COUNT_OF_APP_LOGIC) {
                    next = _END;
                    break;
                } else {
                    ok = false;
                }
            } else {
                ok = true;
            }
        } else if (next == _STRING || next == _CON_STRING_OR_NONE || next == _RES_STRING_OR_NONE) {
            logInfo("\t@STRING");
            uint32_t l = STACK[i];
            uint8_t c = (l >> 24)&0xff;
            if (c > ' ' && c <= '~') {
                if (STACK[0] == _NAME) {
                    next = _END;
                } else if (next == _CON_STRING_OR_NONE) {
                    i++;
                    logInfo("\t_CON_STRING_OR_NONE, next _CON_LOGIC_OR_IMPLICATION");
                    state = _CON_STRING_OR_NONE;
                    next = _CON_LOGIC_OR_IMPLICATION;
                } else if (next == _RES_STRING_OR_NONE) {
                    i++;
                    logInfo("\t_RES_STRING_OR_NONE, next _RES_IDENTIFIER_OR_ELSE_OR_END");
                    state = _RES_STRING_OR_NONE;
                    next = _RES_IDENTIFIER_OR_ELSE_OR_END;
                }
            } else if (next == _CON_STRING_OR_NONE) {
                i++;
                logInfo("\t_CON_STRING_OR_NONE, next _CON_LOGIC_OR_IMPLICATION");
                state = _CON_STRING_OR_NONE;
                next = _CON_LOGIC_OR_IMPLICATION;
            } else if (next == _RES_STRING_OR_NONE) {
                i++;
                logInfo("\t_RES_STRING_OR_NONE, next _RES_IDENTIFIER_OR_ELSE_OR_END");
                state = _RES_STRING_OR_NONE;
                next = _RES_IDENTIFIER_OR_ELSE_OR_END;
            }
        } else if (next == _CON_NUMBER) {
            logInfo("\t@CONDITION NUMBER");
            if (STACK[i - 2] >= _I1 && STACK[i - 2] <= _O8) {
                if (STACK[i] != 0 && STACK[i] != 100) {
                    ok = false;
                    next = _END;
                } else {
                    logInfo("\t_CON_NUMBER , next: _CON_LOGIC_OR_IMPLICATION");
                    next = _CON_LOGIC_OR_IMPLICATION;
                    state = _CON_NUMBER;
                }
            } else if (STACK[i - 2] >= _A1 && STACK[i - 2] <= _A8) {
                logInfo("\t_CON_NUMBER , next: _CON_LOGIC_OR_IMPLICATION");
                state = _CON_NUMBER;
                next = _CON_LOGIC_OR_IMPLICATION;
            }
        } else if (next == _RES_NUMBER) {
            logInfo("\t@RESULT NUMBER");
            logInfo("\t_RES_NUMBER , next: _RES_IDENTIFIER_OR_ELSE_OR_END");
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
            logInfo("\t@CONDITION TURNARY");
            if (STACK[i - 1] >= _I1 && STACK[i - 1] <= _STR) {
                state = _CON_TURNARY;
                if (STACK[i - 1] == _STR) {
                    logInfo("\t_CON_TURNARY , next: _CON_STRING_OR_NONE");
                    next = _CON_STRING_OR_NONE;
                } else {
                    logInfo("\t_CON_TURNARY , next: _CON_NUMBER");
                    next = _CON_NUMBER;
                }
            }
        } else if (next == _CON_LOGIC_OR_IMPLICATION) { //LOGIC
            logInfo("\t@CONDITION LOGIC OR IMPLICATION");
            state = _CON_LOGIC_OR_IMPLICATION;
            if (STACK[i] == _AND) {
                logInfo("\t_CON_LOGIC_OR_IMPLICATION , next: _CON_IDENTIER");
                next = _CON_IDENTIFIER;
            } else if (STACK[i] == _OR) {
                logInfo("\t_CON_LOGIC_OR_IMPLICATION , next: _CON_IDENTIER");
                next = _CON_IDENTIFIER;
            } else if (STACK[i] == _THEN) {
                logInfo("\t_CON_LOGIC_OR_IMPLICATION , next: _RES_IDENTIER");
                next = _RES_IDENTIFIER;
            } else {
                ok = false;
                break;
            }
        } else if (STACK[i] >= _I1 && STACK[i] <= _STR) { // INPUT | OUTPUT | ANALOGUE PORT | COMMUNICATION
            logInfo("\t@PORT");
            if (i != 0 && next != -1) {
                if (next == _ANALOGUE && STACK[i] >= _A1 && STACK[i] <= _A8 && STACK[0] == _ZERO) { // ZERO ANALOGUE
                    next = _END;
                } else if (next == _IDENTIFIER && (STACK[i] >= _O1 && STACK[i] <= _A8) && STACK[0] == _SET) { // SET OUT | ADC
                    ok = true;
                    next = _EQUALS;
                } else if (next == _RES_IDENTIFIER) {
                    logInfo("\t_RES_IDENTIFIER, next: _RES_EQUALS");
                    next = _RES_EQUALS;
                    state = _RES_IDENTIFIER;
                } else if (next == _RES_IDENTIFIER_OR_ELSE_OR_END) {
                    logInfo("\t_RES_IDENTIFIER_OR_ELSE_OR_END, next: _RES_EQUALS");
                    next = _RES_EQUALS;
                    state = _RES_IDENTIFIER_OR_ELSE_OR_END;
                } else if (next == _CON_IDENTIFIER || (state == _CON_IDENTIFIER && next == -1)) {
                    logInfo("\t_CON_IDENTIFIER , next: _CON_TURNARY");
                    next = _CON_TURNARY;
                    state = _CON_IDENTIFIER;
                } else {
                    ok = false;
                    break;
                }
            } else if (next == _CON_IDENTIFIER) {
                logInfo("\t_CON_IDENTIFIER , next: _CON_TURNARY");
                next = _CON_TURNARY;
                state = _CON_IDENTIFIER;
            } else {
                logInfo("\tELSE");
            }
        } else if (next == _EQUALS) {
            logInfo("\t@EQUALS");
            if (STACK[0] == _SET && i == 2) {
                next = _NUM;
            } else if (STACK[0] == _NAME && i == 1) {
                next = _STRING;
            } else {
                ok = false;
            }
        } else if (STACK[i] >= _EQ && STACK[i] <= _LT) { //TURNARY
            logInfo("\t@TURNARY");
            if (next == _RES_EQUALS) {
                if (STACK[i - 1] == _STR) {
                    logInfo("\t_RES_EQUALS, next: _RES_STRING_OR_NONE");
                    state = _RES_EQUALS;
                    next = _RES_STRING_OR_NONE;
                } else if (STACK[i - 1] >= _O1 && STACK[i - 1] <= _O8) {
                    logInfo("\t_RES_EQUALS, next: _RES_NUMBER");
                    next = _RES_NUMBER;
                    state = _RES_EQUALS;
                } else {
                    ok = false;
                }
            } else {
                ok = false;
            }
        } else if (STACK[i] >= _AND && STACK[i] <= _ELSE && i > 0) { //LOGIC
            logInfo("\t@LOGIC");
            if (next == _CON_LOGIC_OR_IMPLICATION) {
                state = _CON_LOGIC_OR_IMPLICATION;
                next = _CON_TURNARY;
                logInfo("\t_CON_LOGIC_OR_IMPLICATION, next: _CON_TURNARY");
            } else if (next == _RES_IDENTIFIER_OR_ELSE_OR_END) {
                state = _RES_IDENTIFIER_OR_ELSE_OR_END;
                next = _RES_IDENTIFIER;
                hasElse = true;
                logInfo("\t_CON_LOGIC_OR_IMPLICATION, next: _RES_IDENTIFIER");
            }
        } else if (STACK[i] == _GET && i == 0) { //COMMAND: GET
            logInfo("\t@GET");
            next = _END;
        } else if (STACK[i] == _SET && i == 0) { //COMMAND: SET
            logInfo("\t@SET");
            next = _IDENTIFIER;
        } else if (STACK[i] == _NAME && i == 0) { //COMMAND: NAME
            logInfo("\t@NAME");
            next = _EQUALS;
        } else if (STACK[i] == _STREAM && i == 0) { //COMMAND: STREAM
            logInfo("\t@STREAM");
            next = _END;
        } else if (STACK[i] == _SLAVE && i == 0) { //COMMAND: SLAVE
            logInfo("\t@SLAVE");
            next = _END;
        } else if (STACK[i] == _MASTER && i == 0) { //COMMAND: SLAVE
            logInfo("\t@MASTER");
            next = _END;
        } else if (STACK[i] == _LOGIC && i == 0) { //COMMAND: LOGIC
            logInfo("\t@LOGIC");
            next = _END;
        } else if (STACK[i] == _HELP && i == 0) { //COMMAND: LOGIC
            logInfo("\t@HELP");
            next = _END;
        } else if (STACK[0] == _DELETE && i == 0) { //COMMAND: DELETE
            logInfo("\t@DELETE");
            next = _NUM;
        } else if (STACK[i] == _ZERO && i == 0) { //COMMAND: ZERO
            logInfo("\t@ZERO");
            next = _ANALOGUE;
        } else if (STACK[i] == _IF && i == 0) { //START OF LOGIC
            logInfo("\t@IF");
            next = _CON_IDENTIFIER;
        } else if (next == _RES_IDENTIFIER_OR_ELSE_OR_END) {
            next = _END;
        } else {
            ok = false;
            //   printf("ElSE OK = false; %d",STACK[i]);
        }
        i++;
    }
    //printf("%d  %d   %d    %d",type, ok, next, hasElse);
    if (type == _END) return ((ok || next == _END) || (!hasElse && next == _RES_IDENTIFIER_OR_ELSE_OR_END) || (hasElse && next == _RES_IDENTIFIER_OR_END));
    else return ok;
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
    logInfo("\t\t#handleStack()");
    int i;
    for (i = 0; i <= pnt; i++) {
        if (next == _STRING) {
            logInfo("\t\t@STR");
            if (STACK[0] == _NAME) response(_NAME, STACK[i], STACK[i + 1]);
            break;
        } else if (next == _NUM) {
            if (STACK[0] == _SET && STACK[1] >= _A1 && STACK[1] <= _A8) {
                doADCCalibrate(STACK[1], STACK[i]);
                response(_SET, STACK[1], STACK[i]);
                break;
            } else if (STACK[0] == _SET && STACK[1] >= _O1 && STACK[1] <= _O8) {
                setIO(STACK[1], STACK[i] / 100);
                response(_SET, STACK[1], STACK[i]);
                break;
            } else if (STACK[0] == _DELETE) {
                if (logicDelete(STACK[i] / 100)) response(_DELETE, 0, 0);
                else error(-1);
                break;
            }
        } else if (STACK[i] == _GET && i == 0) {
            if (stream) stream = false;
            response(_GET, 0, 0);
            break;
        } else if (STACK[i] == _SET && i == 0) {
            logInfo("\t\t@SET");
            next = _IDENTIFIER;
        } else if (STACK[i] == _LOGIC && i == 0) {
            response(_LOGIC, 0, 0);
            break;
        } else if (STACK[i] == _STREAM && i == 0) {
            if (stream) stream = false;
            else stream = true;
            break;
        } else if (STACK[i] == _SLAVE && i == 0) {
            setIsSave(true);
            response(_SLAVE, 0, 0);
        } else if (STACK[i] == _MASTER && i == 0) {
            setIsSave(false);
            response(_MASTER, 0, 0);
        } else if (STACK[i] == _HELP && i == 0) {
            response(_HELP, 0, 0);
        } else if (STACK[i] == _NAME && i == 0) {
            next = _EQ;
        } else if (STACK[i] == _DELETE && i == 0) {
            next = _NUM;
        } else if (STACK[i] == _ZERO && i == 0) {
            next = _ANALOGUE;
        } else if (STACK[i] == _IF && i == 0) { // I/O PORT
            logInfo("\t\t@LOGIC");
            if (i == 0) {
                i++;
                if (STACK[i] >= _I1 && STACK[i] <= _STR)
                    logInfo("\t\t\t\t\t@IDENTIFIER");
                else {
                    error(ERR_LOGIC);
                    break;
                }
                int n = _CON_TURNARY;
                printf("pointer: %i next: %i\n", i, n);
                bool logicOK = false;
                while (i <= pnt) {
                    if (n == _CON_NUMBER || n == _RES_NUMBER) {
                        logInfo("\t\t\t\t\t@NUMBER");
                        i++;
                        if (n == _CON_NUMBER) n = _CON_LOGIC_OR_IMPLICATION;
                        else if (n == _RES_NUMBER) {
                            logicOK = true;
                            n = _RES_IDENTIFIER_OR_ELSE_OR_END;
                        }
                    } else if (n == _CON_STRING_OR_NONE || n == _RES_STRING_OR_NONE) {
                        logInfo("\t\t\t\t\t@STRING-");
                        i += 2;
                        if (n == _CON_STRING_OR_NONE) n = _CON_LOGIC_OR_IMPLICATION;
                        else if (n == _RES_STRING_OR_NONE) {
                            logicOK = true;
                            n = _RES_IDENTIFIER_OR_ELSE_OR_END;
                        }
                    } else if (STACK[i] >= _EQ && STACK[i] <= _LT) { //TURNARY
                        logInfo("\t\t\t\t\t@TURNARY");
                        if (n == _CON_TURNARY && STACK[i - 1] == _STR) n = _CON_STRING_OR_NONE;
                        else if (n == _CON_TURNARY) n = _CON_NUMBER;
                        else if (n == _RES_EQUALS && STACK[i - 1] == _STR) n = _RES_STRING_OR_NONE;
                        else if (n == _RES_EQUALS) n = _RES_NUMBER;
                        i++;
                    } else if (STACK[i] >= _AND && STACK[i] <= _ELSE) { //LOGIC
                        logInfo("\t\t\t\t\t@LOGIC");
                        if (n == _CON_LOGIC_OR_IMPLICATION && STACK[i] == _THEN) n = _RES_IDENTIFIER;
                        else if (n == _RES_IDENTIFIER_OR_ELSE_OR_END && STACK[i] == _ELSE) n = _RES_IDENTIFIER;
                        else n = _CON_IDENTIFIER;
                        i++;
                    } else if (STACK[i] >= _I1 && STACK[i] <= _STR) {
                        logInfo("\t\t\t\t\t@IDENTIFIER");
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
            if (i == 1 && next == _ANALOGUE && STACK[0] == _ZERO) {
                logInfo("\tDO ZERO");
                doADCZero(STACK[i]);
                next = _END;
                response(_ZERO, 0, 0);
            } else if (next == _IDENTIFIER && STACK[0] == _SET) {
                next = _EQ;
            }
        } else if (STACK[i] == _EQ && next == _EQ) {
            if (STACK[0] == _SET) next = _NUM;
            else if (STACK[0] == _NAME) next = _STRING;
        } else logInfo("handle stack");
    }
}

void parseInput(char * TEST, int size) {
    bool justAString = false;
    int i = 0;
    while (i < size && TEST[i] != '\0') {
        //  justAString = false;
        while (isSpace(TEST[i])) i++;
        if (debug) {
            printf("\033[0;36m"); //Set the text to the color red
            printf("\t\t...%s", &TEST[i]);
            printf("\033[0m");
        }
        if (isNext(&TEST[i], GET)) {
            logInfo("@GET");
            i += 3;
            STACK[pnt] = _GET;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], NAME)) {
            logInfo("@NAME");
            i += 4;
            STACK[pnt] = _NAME;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], SET)) {
            logInfo("@SET");
            i += 3;
            STACK[pnt] = _SET;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], LOGIC)) {
            logInfo("@LOGIC");
            i += 5;
            STACK[pnt] = _LOGIC;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], DELETE)) {
            logInfo("@DELETE");
            i += 6;
            STACK[pnt] = _DELETE;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], ZERO)) {
            logInfo("@ZERO");
            i += 4;
            STACK[pnt] = _ZERO;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], STREAM)) {
            logInfo("@STREAM");
            i += 6;
            STACK[pnt] = _STREAM;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], SLAVE)) {
            logInfo("@SLAVE");
            i += 4;
            STACK[pnt] = _SLAVE;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], MASTER)) {
            logInfo("@MASTER");
            i += 6;
            STACK[pnt] = _MASTER;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], HELP)) {
            logInfo("@HELP");
            i += 4;
            STACK[pnt] = _HELP;
            if (isRecieveValid(pnt, 0)) break;
            else break;
        } else if (isNext(&TEST[i], NONE)) {
            logInfo("@NONE");
            i += 4;
            STACK[pnt++] = 0;
            STACK[pnt] = 0;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A1)) {
            logInfo("@A1");
            i += 2;
            STACK[pnt] = _A1;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A2)) {
            logInfo("@A2");
            i += 2;
            STACK[pnt] = _A2;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A3)) {
            logInfo("@A3");
            i += 2;
            STACK[pnt] = _A3;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A4)) {
            logInfo("@A4");
            i += 2;
            STACK[pnt] = _A4;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A5)) {
            logInfo("@A5");
            i += 2;
            STACK[pnt] = _A5;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A6)) {
            logInfo("@A6");
            i += 2;
            STACK[pnt] = _A6;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A7)) {
            logInfo("@A7");
            i += 2;
            STACK[pnt] = _A7;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], A8)) {
            logInfo("@A8");
            i += 2;
            STACK[pnt] = _A8;
            if (isRecieveValid(pnt, 0)) pnt++;
        } else if (isNext(&TEST[i], O1)) {
            logInfo("@O1");
            i += 2;
            STACK[pnt] = _O1;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O2)) {
            logInfo("@O2");
            i += 2;
            STACK[pnt] = _O2;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O3)) {
            logInfo("@O3");
            i += 2;
            STACK[pnt] = _O3;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O4)) {
            logInfo("@O4");
            i += 2;
            STACK[pnt] = _O4;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O5)) {
            logInfo("@O5");
            i += 2;
            STACK[pnt] = _O5;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O6)) {
            logInfo("@O6");
            i += 2;
            STACK[pnt] = _O6;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O7)) {
            logInfo("@O7");
            i += 2;
            STACK[pnt] = _O7;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], O8)) {
            logInfo("@O8");
            i += 2;
            STACK[pnt] = _O8;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], I1)) {
            logInfo("@I1");
            i += 2;
            STACK[pnt] = _I1;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], I2)) {
            logInfo("@I2");
            i += 2;
            STACK[pnt] = _I2;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], COM)) {
            logInfo("@STR");
            i += 3;
            STACK[pnt] = _STR;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], AND)) {
            logInfo("@AND");
            i += 3;
            STACK[pnt] = _AND;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], OR)) {
            logInfo("@OR");
            i += 2;
            STACK[pnt] = _OR;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], EQ)) {
            logInfo("@EQ");
            i += 1;
            STACK[pnt] = _EQ;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], NE)) {
            logInfo("@NE");
            i += 2;
            STACK[pnt] = _NE;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], IF)) {
            logInfo("@IF");
            i += 2;
            STACK[pnt] = _IF;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], THEN)) {
            logInfo("@THEN");
            i += 4;
            STACK[pnt] = _THEN;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], ELSE)) {
            logInfo("@ELSE");
            i += 4;
            STACK[pnt] = _ELSE;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], GT)) {
            logInfo("@GT");
            i += 1;
            STACK[pnt] = _GT;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if (isNext(&TEST[i], LT)) {
            logInfo("@LT");
            i += 1;
            STACK[pnt] = _LT;
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else if ((numberCharLen = isNumber(&TEST[i])) && *numberCharLen > 0) {
            logInfo("@NUMBER");
            float num;
            sscanf(&TEST[i], "%f", &num);
            i += *numberCharLen;
            STACK[pnt] = (int32_t) (num * 100.0);
            if (isRecieveValid(pnt, 0)) pnt++;
            else break;
        } else {
            logInfo("@STRING");
            int j = 0;
            int k = i;
            char tmp[] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
            while (j < 8 && k < size && TEST[k] > ' ' && TEST[k] <= '~') {
                tmp[j++] = TEST[k++];
            }
            if (i == 0) {
                STR[0] = tmp[0];
                STR[1] = tmp[1];
                STR[2] = tmp[2];
                STR[3] = tmp[3];
                STR[4] = tmp[4];
                STR[5] = tmp[5];
                STR[6] = tmp[6];
                STR[7] = tmp[7];
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
    else {
        if (STACK[0] == _IF) error(ERR_LOGIC);
        else if (!justAString) {
            if (STACK[0] == _SET && pnt == 3) error(ERR_VALUE);
            else if (STACK[0] == _DELETE && pnt == 1) error(ERR_VALUE);
            else error(ERR_FORMAT);
        }
    }
}



