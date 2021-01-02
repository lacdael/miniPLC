/**
  ADC1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    adc1.c

  @Summary
    This is the generated driver implementation file for the ADC1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for ADC1.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.168.0
        Device            :  PIC32MM0064GPL028      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB 	          :  MPLAB X v5.40
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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
#include "adc1.h"

/**
  Section: File Specific Functions
*/

// ADC1 Default Interrupt Handler
static void (*ADC1_DefaultInterruptHandler)(void) = NULL;

/**
  Section: Driver Interface
*/

void ADC1_Initialize (void)
{
    // ASAM enabled; DONE disabled; FORM Integer 16-bit; SAMP disabled; SSRC Clearing sample bit ends sampling and starts conversion; SIDL disabled; MODE12 12-bit; ON enabled; 
    AD1CON1 = 0x800C;
    // CSCNA enabled; BUFM disabled; SMPI Generates interrupt after completion of every 6th sample/conversion operation; OFFCAL disabled; VCFG AVDD/AVSS; BUFREGEN disabled; 
    AD1CON2 = 0x414;
    // SAMC 9; EXTSAM disabled; ADRC PBCLK; ADCS 15; 
    AD1CON3 = 0x90F;
    // CH0SA AN0; CH0NA AVSS; 
    AD1CHS = 0x00;
    // CSS9 disabled; CSS8 disabled; CSS7 disabled; CSS6 disabled; CSS5 enabled; CSS4 disabled; CSS3 disabled; CSS2 disabled; CSS1 enabled; CSS0 enabled; CSS11 disabled; CSS10 disabled; CSS30 enabled; CSS29 enabled; CSS28 enabled; 
    AD1CSS = 0x70000023;
    // ASEN disabled; WM Legacy operation; ASINT No interrupt; CM Less Than mode; BGREQ disabled; LPEN Full power; 
    AD1CON5 = 0x00;
    // CHH2 disabled; CHH1 disabled; CHH0 disabled; CHH11 disabled; CHH10 disabled; CHH9 disabled; CHH8 disabled; CHH7 disabled; CHH6 disabled; CHH5 disabled; CHH4 disabled; CHH3 disabled; 
    AD1CHIT = 0x00;
    
    //Assign Default Callbacks
    ADC1_SetInterruptHandler(&ADC1_CallBack);
   
    // Enabling ADC1 interrupt.
    IEC0bits.AD1IE = 1;
}

void __attribute__ ((weak)) ADC1_CallBack(void)
{

}

void ADC1_SetInterruptHandler(void* handler)
{
    ADC1_DefaultInterruptHandler = handler;
}

void __attribute__ ( ( vector ( _ADC_VECTOR ), interrupt ( IPL1SOFT ), weak ) )ADC1_ISR (void)
{
    if(IFS0bits.AD1IF)
    {
        if(ADC1_DefaultInterruptHandler) 
        { 
            ADC1_DefaultInterruptHandler(); 
        }

        // clear the ADC interrupt flag
        IFS0CLR= 1 << _IFS0_AD1IF_POSITION;
    }
}

/*******************************************************************************

  !!! Deprecated Definitions and APIs !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/
void ADC1_Start(void)
{
   AD1CON1SET = (1 << _AD1CON1_SAMP_POSITION);
}

void ADC1_Stop(void)
{
   AD1CON1CLR = (1 << _AD1CON1_SAMP_POSITION);
}

uint16_t ADC1_ConversionResultBufferGet(uint16_t *buffer)
{
    int count;
    uint16_t *ADC16Ptr;
    uint16_t sampleCount = AD1CON2bits.SMPI;

    ADC16Ptr = (uint16_t *)&(ADC1BUF0);

    for(count=0;count<=sampleCount;count++)
    {
        buffer[count] = (uint16_t)*ADC16Ptr;
        ADC16Ptr++;
    }
    return count;
}


/**
  End of File
*/
