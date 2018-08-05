/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


void SPI_Init(char master){

  
    SPI1STATbits.SPIEN = 1; // Enable SPI module
    //din
     RPINR20bits.SDI1R0=0;
    //dout
     RPOR1bits.RP2R=7;
     SPI1CON1bits.MSTEN = master; // Master mode enabled
    
    if(master==1){
        //clock out
        RPOR1bits.RP3R=8;
              
    }
    else {
        //clk in nd cs
        RPINR20bits.SCK1R0=1;
        RPINR21bits.SS1R0=4;
    
    }
   
    
    //rp0 = din
    //rp1 =salve clk 
    //rp2 dout
    //rp3 =master clock
    
   TRISBbits.TRISB0=1;
   TRISBbits.TRISB2=0;
   if(master==1)TRISBbits.TRISB3=0;
   else TRISBbits.TRISB1=1;
    
IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
IEC0bits.SPI1IE = 0; // Disable the interrupt

// SPI1CON1 Register Settings
SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
SPI1CON1bits.MODE16 = 0; // Communication is word-wide (16 bits)
SPI1CON1bits.PPRE = 2; //Pre-scaler
SPI1CON1bits.SPRE = 8;//Pre-scaler 2
 // Input data is sampled at the middle of data output time
SPI1CON1bits.CKE = 0; // Serial output data changes on transition from
// Idle clock state to active clock state
SPI1CON1bits.CKP = 0; // Idle state for clock is a low level;
SPI1CON1bits.SMP = 0;
// active state is a high level

// Interrupt Controller Settings
IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
IEC0bits.SPI1IE = 1; // Enable the interrupt


} 

void SPI_Writ(char w){
  
  SPI1BUF=w; 
  while (SPI1STATbits.SPITBF); 
  return SPI1BUF;
  
}
char SPI_Read()        /* SPI read data function */
{
 
 
 while(!SPI1STATbits.SPIRBF);
 return SPI1BUF; 
    // read a byte
}


#endif	/* XC_HEADER_TEMPLATE_H */

