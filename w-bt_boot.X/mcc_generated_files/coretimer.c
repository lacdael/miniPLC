
/**
  CORETIMER Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
   coretimer.c

  @Summary
    This is the generated driver implementation file for the CORETIMER driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides implementations for driver APIs for CORETIMER.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
        Device            :  PIC32MM0064GPL028
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB             :  MPLAB X 3.60
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "coretimer.h"

/**
  Section: Core Timer Module APIs
*/

void CORETIMER_Initialize()
{
   // Set the count value
   _CP0_SET_COUNT(0x0); 
   // Set the compare value
   _CP0_SET_COMPARE(0xFFFFFF); 
    // Enable the interrupt
   IEC0bits.CTIE = 1;

}

void CORETIMER_EnableInterrupt()
{
    IEC0bits.CTIE = 1;
}

void CORETIMER_DisableInterrupt()
{
    IEC0bits.CTIE = 0;
}

uint32_t CORETIMER_CountGet()
{
   return _CP0_GET_COUNT();
}

void __attribute__ ((vector(_CORE_TIMER_VECTOR), interrupt(IPL1SOFT))) _CORE_TIMER_ISR(void)
{
   uint32_t static compare = 0xFFFFFF;

   // Update the compare value
   compare = compare + 0xFFFFFF;

   _CP0_SET_COMPARE(compare);

   IFS0CLR= 1 << _IFS0_CTIF_POSITION;
   // Add your custom code here

}

/**
 End of File
*/
