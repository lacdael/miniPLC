/* ************************************************************************** */

#include <xc.h>
#include "flash_routines.h"
#include "ezbl_integration/ezbl.h"
#include <stdint.h>
#include <signal.h>

unsigned int NVOPM_NO_OPERATION = 0x4000;
unsigned int NVMOP_PAGE_ERASE = 0x4004;
unsigned int NVMOP_WRITE_ROW = 0x4003;
unsigned int NVMOP_WRITE_DOUBLE_WORD = 0x4002;
unsigned int NVMOP_WRITE_WORD = 0x4001;

unsigned int NVM_Unlock(unsigned int nvmop){
    /*Suspend all interrupts*/
//    INTERRUPT_GlobalDisable();
    __builtin_disable_interrupts();
    //ENABLE FLASH OPPERATIONS & SET OPPERATION
    NVMCON = nvmop;
    /*UNLOCK SEQUENCE*/
    NVMKEY = NVMKEY1;
    NVMKEY = NVMKEY2;
    /*Start the operation using the set register*/
    NVMCONSET = 0x8000;
    /*Wait for operation to complete*/
    while (NVMCON & 0x8000);
    /*restore all interrupts*/
//    INTERRUPT_GlobalEnable();
    __builtin_enable_interrupts();
    //LOCK FLASH
    NVMCONCLR = 0x0004000;
    /*Return WRERR and LVDERR Error Status Bits*/
    return (NVMCON & 0x3000);
}

unsigned int NVM_WriteWord2Flash(volatile unsigned int* address, unsigned int data0, unsigned int data1){
    unsigned int res;
    NVMDATA0 = data0;
    NVMDATA1 = data1;
    /*Convert virtual address to physical address*/
    //MUST BE PHYSICAL ADDRESS
    NVMADDR =  ((unsigned int)address) & 0x1FFFFFFF;
    /*Unlock and perform write*/
    //SEE DOC
    res = NVM_Unlock(NVMOP_WRITE_DOUBLE_WORD);
    return res;
}

unsigned int NVM_WriteRow2Flash(volatile unsigned int* address, void* data){
    unsigned int res;
    /*Convert virtual address to physical address*/
    //MUST BE PHYSICAL ADDRESS
    NVMADDR =  ((unsigned int)address) & 0x1FFFFFFF;
    /*Unlock and perform write*/
    //Set NVMSRCADDR to the SRAM data buffer address
    NVMSRCADDR = (unsigned int) data & 0x1FFFFFFF;    
    //SEE DOC
    res = NVM_Unlock(NVMOP_WRITE_ROW);
    return res;
}

unsigned int NVM_Erase(volatile unsigned int * address){
    unsigned int res;
    //MUST BE PHYSICAL ADDRESS
    NVMADDR = ((unsigned int)address) & 0x1FFFFFFF;
    /*Unlock and perform write*/
    //SEE DOC
    res = NVM_Unlock(NVMOP_PAGE_ERASE);
    return res;
}