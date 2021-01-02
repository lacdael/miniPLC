#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "device.h"
#include "app.h"


char TEST[ARRAY_SIZE];

int main(int argc, char** argv) {
	int x = 0;
	for(;;){
		if (stream)
                    printState(false);
		memset(STACK,0,sizeof(STACK));
		memset(TEST,'\0',sizeof(TEST));
		if (x++ > 3) {
			clearSTR();
			x = 0;
		}
		pnt = 0;
                printf("\033[1;32m"); //Set the text to the color red
        	printf("\r\n>>>Input a command string:\r\n");
                printf("\033[0m");
	if (*fgets(TEST,ARRAY_SIZE,stdin) != '\n') parseInput(TEST,ARRAY_SIZE);
		//int i;
               // for (i = 0; MAX_COUNT_OF_APP_LOGIC; i++) handleLogic(LOGIC_STACK[i]);
	}
    return (EXIT_SUCCESS);
}


