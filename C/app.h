#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#define ARRAY_SIZE 128

#include <stdint.h>
#include <stdbool.h>
#include "device.h"

/*RESERVED WORDS*/
static const char SET[] = "SET";
static const char NAME[] = "NAME";
static const char TIMEOUT[] = "TIMEOUT";
static const char LOGIC[] = "LOGIC";
static const char DELETE[] = "DELETE";
static const char ZERO[] = "ZERO";
static const char GET[] = "GET";
static const char SLAVE[] = "SLAVE";
static const char MASTER[] = "MASTER";
static const char STREAM[] = "STREAM";
static const char AND[] = "AND";
static const char OR[] = "OR";
static const char IF[] = "IF";
static const char THEN[] = "THEN";
static const char ELSE[] = "ELSE";
static const char EQ[] = "=";
static const char NE[] = "!=";
static const char GT[] = ">";
static const char LT[] = "<";
static const char I1[] = "I1";
static const char I2[] = "I2";
static const char O1[] = "O1";
static const char O2[] = "O2";
static const char O3[] = "O3";
static const char O4[] = "O4";
static const char O5[] = "O5";
static const char O6[] = "O6";
static const char O7[] = "O7";
static const char O8[] = "O8";
static const char A1[] = "A1";
static const char A2[] = "A2";
static const char A3[] = "A3";
static const char A4[] = "A4";
static const char A5[] = "A5";
static const char A6[] = "A6";
static const char A7[] = "A7";
static const char A8[] = "A8";
static const char STR[] = "STR";
static const char NONE[] = "NONE";
static const char HELP[] = "HELP";
//*RESERVED WORDS*//

//*HELP*//
static const char HELP_SET[] = "Set a port's state, or, do a calibration. `SET [O|A][1-8] = [1|0]|<number>` e.g. SET O1 = 1 e.g. SET A1 = 20.0\n";
static const char HELP_NAME[] = "Set the bluetooth name. `NAME = <name>` e.g. NAME = myDevice\n";
static const char HELP_TIMEOUT[] = "Set the timeout for clearing a string. `TIMEOUT = <seconds>` e.g. TIMEOUT = 5\n";
static const char HELP_LOGIC[] = "Get the device's logic as a json array. `LOGIC`.\n";
static const char HELP_DELETE[] = "Delete logic from the device. `DELETE <index>` e.g. DELETE 1.\n";
static const char HELP_ZERO[] = "Zero an analogue port. `ZERO A[1-8]` e.g. ZERO A1.\n";
static const char HELP_GET[] = "Get the verbose state of the device as a json object. `GET`.\n";
static const char HELP_SLAVE[] = "Stop the evaluation of logic (default state). `SLAVE`.\n";
static const char HELP_MASTER[] = "Start the evaluation of logic. `MASTER`.\n";
static const char HELP_STREAM[] = "Toggle on/off a streamed output of the device's state as a json object. `STREAM`.\n";
static const char HELP_HELP[] = "This message.\n";
static const char HELP_LOGIC_EG[] = "\nIF [[O[1-8]|A[1-8]|STR] [=|!=|>|<] [<number>|<string>|NONE] [AND|OR]] THEN [O[1-8] | STR] = [[1|0]|<string>|NONE] [ ELSE [O[1-8] | STR] = [[1|0]|<string>|NONE]]\n\n\te.g. IF A1 > 5 AND I1 != 1 OR STR = password THEN O1 = 1 O2 = 0 ELSE O1 = 0 O2 = 0\n\te.g. IF A2 < 1.2 AND A3 < 3 THEN STR = password\n";
//*HELP*//

//*CODES*//
#define _IF 1
#define _AND 2
#define _OR 3
#define _THEN 4
#define _ELSE 5
#define _EQ 6
#define _NE 7
#define _GT 8
#define _LT 9
#define _I1 20
#define _I2 21
#define _O1 40
#define _O2 41
#define _O3 42
#define _O4 43
#define _O5 44
#define _O6 45
#define _O7 46
#define _O8 47
#define _A1 60
#define _A2 61
#define _A3 62
#define _A4 63
#define _A5 64
#define _A6 65
#define _A7 66
#define _A8 67
#define _STR 80
#define _NONE 81
#define _GET 82
#define _SET 83
#define _NAME 84
#define _LOGIC 85
#define _DELETE 86
#define _ZERO 87
#define _STREAM 90
#define _SLAVE 91
#define _MASTER 92
#define _HELP 93
#define _TIMEOUT 94

#define _CON_IDENTIFIER 100
#define _CON_TURNARY 101
#define _CON_NUMBER 102
#define _CON_STRING_OR_NONE 103
#define _CON_LOGIC_OR_IMPLICATION 104
#define _RES_IDENTIFIER 105
#define _RES_EQUALS 106
#define _RES_NUMBER 107
#define _RES_STRING_OR_NONE 108
#define _RES_IDENTIFIER_OR_ELSE_OR_END 109
#define _RES_IDENTIFIER_OR_END 110
#define _END 202

#define ERR_FORMAT 0    
#define ERR_MSG_FORMAT "Bad message format."
#define ERR_VALUE 1
#define ERR_MSG_VALUE "Bad value."
#define ERR_LOGIC 2
#define ERR_MSG_LOGIC "Bad logic syntax."
#define ERR_FIRST_DELETE_LOGIC 3
#define ERR_MSG_FIRST_DELETE_LOGIC "First delete an item of logic."
#define ERR_GENERAL "General Error."


int32_t STACK[ARRAY_SIZE];

char txBuffer[BUFFER_SIZE];

int bufferAppendInt(char *buf, int16_t pnt, int n);
int bufferAppendFloat(char *buf, int16_t pnt, float n);
int bufferAppendString(char *buf, int16_t pnt, char *s);





extern bool debug;
extern bool stream;
extern bool slave;

extern int pnt;

extern char STR_VAL[];

int * numberCharLen;

char* boolAsStr(bool b);
bool isNext(const char *a, const char *b);
int * isNumber(char *str);
int isNumeric(char *str);
bool isSpace(char c);
char* boolAsStr(bool b);



bool getIsStream(void);
void setIsStream(bool b);
bool getDoStream(void);
void setDoStream(bool b);
bool getIsSlave(void);
void setIsSlave(bool b);
void error(int what);

void app_task(void);
void logInfo(char *str);
void handleStack(int pnt);
void printState(bool b);
void printLogic(void);
void parseInput(char * TEST,int size);
void response(int cmd,int key,int val);
bool isRecieveValid(int pnt, int type);
void handleLogic();
void clearSTR(void);
void printHelp(void);
bool evaluteLogicElement(int what, int turnary, uint32_t val1, uint32_t val2);
void setLogicElement(int what, int turnary,uint32_t val1, uint32_t val2);
void setName(uint32_t val1, uint32_t val2);

#endif
