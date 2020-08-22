/*
 * File:   pic32mm0064gpl036_explorer_16.c
 *
 * Created on 2016 November 28
 *
 * Initializes the device Configuration words, system clock frequency, UARTs, 
 * LEDs, buttons, NOW timer module on the PIC32 MIPS Core Timer, and exposes
 * some common APIs for reading buttons and manipulating LEDs.
 * 
 * This file is intended to be used on the PIC32MM0064GPL036 PIM for the
 * Explorer 16/32 (and Explorer 16) hardware, but is also the recommended
 * starting basis for any project implementing any of the PIC32MM0064GPL036
 * family devices.
 *
 * Reference (CTRL + Click on link):
 *     PIC32MM0064GPL036 Family data sheet
*      http://www.microchip.com/wwwproducts/productds/PIC32MM0064GPL036
 *
 *     PIC32MM0064GPL036 General Purpose PIM (Plug-in-Module)
 *     http://www.microchip.com/MA320020
 *     http://ww1.microchip.com/downloads/en/DeviceDoc/50002513a.pdf (Info Sheet)
 *
 *     Explorer 16/32 Development Board
 *     http://www.microchip.com/explorer1632
 *     http://ww1.microchip.com/downloads/en/DeviceDoc/Explorer_16_32_Schematics_R6_3.pdf (Schematic)
 *     http://ww1.microchip.com/downloads/en/DeviceDoc/Explorer_16_32_BillOfMaterials_R6_3.pdf (Bill of Materials)
 *     http://microchip.wikidot.com/boards:explorer1632 (User's Guide)
 */

/*******************************************************************************
  Copyright (C) 2018 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/

#include <xc.h>
#include <proc/p32mm0064gpl028.h>
#if defined(__PIC32MM) && (__PIC32_FEATURE_SET0 == 'G') && (__PIC32_FEATURE_SET1 == 'P') && (__PIC32_PRODUCT_GROUP == 'L')   // PIC32MM0064GPL036 Family devices

#include "../ezbl_integration/ezbl.h"


// PIC32MM0064GPL036 Configuration bit settings
// FDEVOPT
#pragma config SOSCHP = OFF              // Secondary Oscillator High Power mode bit (OFF = SOSC crystal oscillator, if enabled, drives crystal with minimum power; ON = SOSC crystal oscillator, if enabled, drives crystal with stronger bias for higher reliability in humid or electrically noisy environments)
// FICD
#pragma config JTAGEN = OFF             // JTAG Enable bit (OFF = No JTAG support, no TMS pull-up, cannot be enabled in software, must use "2-wire" ICSP on PGECx/PGEDx pair for program and debug; ON = JTAG Enabled on reset, weak pull-up on TMS/RB9, can be run-time disabled by setting CFGCON[JTAGEN] = 0 when not debugging)
#pragma config ICS = PGx1               // ICE/ICD Communication Channel Selection bits (Communicate on PGEC1/AN3/C1INC/C2INA/RP15/RB1 & PGED1/AN2/C1IND/C2INB/RP14/RB0)
// FPOR
#pragma config BOREN = BOR2             // Brown-out Reset mode bits (BOR3 = BOR enabled, SBOREN bit does nothing; BOR2 = BOR enabled, but not in sleep mode; BOR1 = BOR controlled by SBOREN; BOR0 = BOR disabled, SBOREN does nothing)
#pragma config RETVR = ON               // Retention Voltage Regulator Enable bit (ON = ~1.2V Retention regulator is enabled and controlled by RETEN bit during sleep; OFF = Sleep mode always uses higher power 1.8V standby regulator)
#pragma config LPBOREN = ON             // Downside Voltage Protection bit (ON = Low Power BOR used when main BOR is off; OFF = always use main BOR)
// FWDT
#pragma config SWDTPS = PS1024          // Sleep Mode Watchdog Timer Postscale Selection bits (1:1024)
#pragma config FWDTWINSZ = PS75_0       // Watchdog Timer Window Size bits (PS75_0 = window size is 75% with most freedom; PS50_0 = less freedom; PS37_5 = less freedom; PS25_0 = windows size is 25% with least freedom)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer mode bit (OFF = classic, non-window mode; ON = Windowed mode)
#pragma config RWDTPS = PS512           // Run Mode Watchdog Timer Postscale Selection bits (1:512 ~= 500 ms)
#pragma config RCLKSEL = LPRC           // Run Mode Watchdog Timer Clock Source Selection bits (Clock source is LPRC (same as for sleep mode))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT is disabled, use software to enable it)
// FOSCSEL
#pragma config FNOSC = PLL              // Oscillator Selection bits (FRCDIV; PLL; PRI; DCO; SOSC; LPRC)
#pragma config PLLSRC = FRC             // System PLL Input Clock Selection bit (FRC oscillator is selected as PLL reference input on device reset)
#pragma config SOSCEN = OFF              // Secondary Oscillator Enable bit (ON = Secondary crystal oscillator circuit enabled, RB4/RA4 function as SOSCI/SOSCO (no GPIO); OFF = Secondary oscillator disabled, GPIO and other functions available on RB4/RA4)
#pragma config IESO = ON                // Two Speed Startup Enable bit (OFF = Two speed startup is disabled; ON = Enabled, POR/BOR/Wake from sleep with FRC, then switch to FNOSC clock after it has warmed up)
#pragma config POSCMOD = OFF             // Primary Oscillator Selection bit (OFF = Primary oscillator disabled; EC = External digital clock input on OSC1; XT = Moderate cystal drive strength, up to at least 8.0MHz; HS = High Speed cystal)
#pragma config OSCIOFNC = OFF           // System Clock on CLKO Pin Enable bit (OFF = OSCO pin operates at GPIO; ON = OSCO pin operates as CLKO SYSCLK output)
#pragma config SOSCSEL = ON            // SOSC/CLKI External Clock Input bit (OFF = 32.768K crystal oscillator mode, ON = CLKI or GPIO mode)
#pragma config FCKSM = CSECME           // Clock Switching and Fail-Safe Clock Monitor Enable bits (Clock switching is enabled; Fail-safe clock monitor is enabled)
// FSEC
#pragma config CP = OFF                 // Code Protection Enable bit (Code protection is disabled)

// Device clock (SYSCLK, PBCLK/UPBCLK, all the same on PIC32MM devices)
#define FCY         24000000ul


//
//         (MCLR) - -|1     28|- - (3v3) AVdd
//     RP1/RA0 (A6) -|2     27|- - (GND) AVss
//     RP2/RA1 (A5) -|3     26|- (TX) RP10/RB15
//  RP14/RB0 (PGD1) -|4     25|- (TX) RP9/RB14
//  RP15/RB1 (PGC1) -|5     24|- (A7) RP13/RB13
//    RP16/RB2 (A4) -|6     23|- (A8) RP12/RB12
//         RB3 (A3) -|7     22|- (IN2) RP18/RB11
//      Vss (GND) - -|8     21|- (IN1) RP17/RB10
//     RP3/RA2 (A2) -|9     20|- - (10uF CAP to GND) Vcap
//     RP4/RA3 (A1) -|10    19|- (OUT8) RP19/RC9
//   RP5/RB4 (OUT1) -|11    18|- (OUT7) RP8/RB9
//   RP6/RA4 (OUT2) -|12    17|- (OUT6) RP7/RB8
//      Vdd (3v3) - -|13    16|- (OUT5) RP11/RB7
//       RB5 (OUT3) -|14    15|- (OUT4) RB6
//
//SET PIN DIRECTION (TRISx) DATA DIRECTION

//SET OUTPUT VALUE (LATx)

//ANALOGUE PORTS (ANSELx)

EZBL_FIFO *EZBL_COMBootIF __attribute__((persistent));  // Pointer to RX FIFO to check activity on for bootloading
const long EZBL_COMBaud = 115200;//-230400;                      // Communications baud rate: <= 0 means auto-baud with default value (for TX) set by the 2's complemented value; otherwise baud in bits/sec (ex: 460800)


/**
 * Initializes interrupt controller, NOW software timing module, UART and other 
 * I/O pins
 */
unsigned long InitializeBoard(void)
{
    __builtin_enable_interrupts();

    // Initialize NOW module (using PIC32 Core Timer without interrupts)
    NOW_Reset(CORETMR, FCY);    // Using 8.0MHz FRC with 3xPLL

    //  NAME   |   PIC
    //  ----------------
    //  OUT#1  |   RB4
    //  OUT#2  |   RA4    
    //  OUT#3  |   RB5
    //  OUT#4  |   RB6
    //  OUT#5  |   RB7
    //  OUT#6  |   RB8
    //  OUT#7  |   RB9
    //  OUT#8  |   RC9
    
    EZBL_DefineLEDMap(RB4, RA4, RB5, RB6, RB7, RB8, RB9, RC9);  // Use LEDSet()/LEDOn()/LEDOff()/LEDToggle() to access this bit mapping
    LATBCLR   = (1u<<4) | (1u<<5) | (1u<<6) | (1u<<7) | (1u<<8) | (1u<<9);
    TRISBCLR  = (1u<<4) | (1u<<5) | (1u<<6) | (1u<<7) | (1u<<8) | (1u<<9);
    ANSELBCLR = (1u<<4) | (1u<<5) | (1u<<6) | (1u<<7) | (1u<<8) | (1u<<9) | (1u<<10) | (1u<<11);;

    LATACLR   = (1u<<4);
    TRISACLR  = (1u<<4);
    ANSELACLR = (1u<<4);

    LATCCLR   = (1u<<9);
    TRISCCLR  = (1u<<9);
    ANSELCCLR = (1u<<9);
    
    //  NAME  |   PIC
    //  ----------------
    //  IN#1  |   RB10
    //  IN#2  |   RB11    
    
    EZBL_DefineButtonMap(RB10, RB11);
    // Use ButtonRead()/ButtonPeek()/ButtonsPushed/ButtonsReleased/ButtonsToggled, etc. to read this bit mapping
    //TRISBSET = (1u<<10) | (1u<<11);
    //ANSELBCLR = (1u<<10) | (1u<<11);

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
    
    TRISASET = (1u<<3) | (1u<<2) | (1u<<1) | (1u<<0);
    ANSELASET = (1u<<3) | (1u<<2) | (1u<<1) | (1u<<0);
    TRISBSET = (1u<<3) | (1u<<13) | (1u<<12) | (1u<<2) | (1u<<10) | (1u<<11);
    ANSELBSET = (1u<<3) | (1u<<13) | (1u<<12) | (1u<<2);
    
    
    
    
    //Add inputs to scan list:
    AD1CSS = (1u<<6) | (1u<<5) | (1u<<11) | (1u<<4) | (1u<<1) | (1u<<0) | (1u<<8) | (1u<<7);
    //Enable scan
    AD1CON2bits.CSCNA = 1;
    //Set conversion per interrupt
    AD1CON2bits.SMPI = 8 - 1;
    //Clock from peripheral TPB
    AD1CON3bits.ADRC = 0;
    //ADC clock TAD = 8; peripheral clock TPB
    AD1CON3bits.ADCS = 8;
    //Enable sampling after each conversion
    AD1CON1bits.ASAM = 1;
    //Set trigger from tmr
    AD1CON1bits.SSRC = 5;
    //Turn on ADC
    AD1CON1bits.ON = 1;
    //Start first sample
    AD1CON1bits.SAMP = 1;
    //RESET ADC int
    IFS0CLR = _IFS0_AD1IF_MASK;
    //set tmr period
    PR1 = 0x2000;
    //enable tmr
    T1CONbits.ON = 1;
    
    // Function     Explorer 16/32              PIC32MM0064GPL036 Device Pins
    // UART2        PIM#, Net                   PIC#, Pin Functions
    // U2TX (PIC out) 50, P50_TXB                 37, VREF-/AN1/RP2/OCM1F/RA1
    // U2RX (PIC in)  49, P49_RXB                 36, VREF+/AN0/RP1/OCM1E/INT3/RA0
    //ANSELACLR        = (1<<1) | (1<<0);
    //RPOUT(2)         = _RPOUT_U2TX; // RP2 = U2TX
    //RPINR9bits.U2RXR = 1;           // RP1 = U2RX
    //CNPUASET = _CNPUA_CNPUA0_MASK;  // Turn on weak pull up on U2RX so the signal stays idle if nobody is plugged in
    //if(EZBL_COMBaud <= 0)           // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    //{                               // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
     //   NOW_Wait(140u*NOW_ms);      // pin glitching could occur long after we have enabled our UART without this forced delay.
    //}
    //EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);   // Use EZBL_FIFORead*()/EZBL_FIFOPeek*()/EZBL_FIFOWrite*() and &UART2_RxFifo/&UART2_TxFifo to read/write. printf()/EZBL_printf()/EZBL_STDOUT/EZBL_STDIN will also map to this when useForStdio == 1.


//    // Initialize UART 1 for stdio and debugging (using air-wires to mikroBus B pins)
//    //      Fixed pins: TX = Explorer 16/32 P54_MISOB = PIC pin 31 = CDAC1/AN9/RP9/RTCC/U1TX/SDI1/C1OUT/INT1/RB14
//    //                  RX = Explorer 16/32 P78_PWMB  = PIC pin 32 =  AN10/REFCLKO/RP10/U1RX/SS1/FSYNC1/INT0/RB15
    CNPUBSET = 1<<15;
    ANSELBCLR = (1u<<15) | (1u<<14);
     if(EZBL_COMBaud <= 0)           // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                               // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);      // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(1, FCY, EZBL_COMBaud, 1);   // Use EZBL_FIFORead*()/EZBL_FIFOPeek*()/EZBL_FIFOWrite*() and &UART1_RxFifo/&UART1_TxFifo to read/write. printf()/EZBL_printf()/EZBL_STDOUT/EZBL_STDIN will also map to this when useForStdio == 1.
//    EZBL_ConsoleReset();
//    EZBL_printf("\n\n\nHello World!"
//            "\n  SYS_CLK = %lu"
//            "\n  U1BRG = 0x%04X"
//            "\n  U2BRG = 0x%04X"
//            "\n", NOW_Fcy, U1BRG, U2BRG);

    return FCY;
}

#endif  // defined(__PIC32MM) && (__PIC32_FEATURE_SET0 == 'G') && (__PIC32_FEATURE_SET1 == 'P') && (__PIC32_PRODUCT_GROUP == 'L')   // PIC32MM0064GPL036 Family devices
