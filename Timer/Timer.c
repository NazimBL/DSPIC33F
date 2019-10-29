/*
 * MPLAB X IDE
 * Author: Nazim BL
 * Created on 1 juillet 2018, 10:10
 */

#include "xc.h"

_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);

#define FCY 40000000
#define LED	LATBbits.LATB10

void SetupOSC();
void Timer_Setup();

void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
  
IFS0bits.T3IF = 0; 
LED=!LED;
//ADC_Read(); to read adc input at a set frequency
}

void main(void) {
    
SetupOSC();
Timer_Setup();

//LED
TRISBbits.TRISB10=0;
LED=0;

while (1);
}

void Timer_Setup(){

T3CONbits.TON = 0; // Disable Timer
T3CONbits.TCS = 0; // Select internal instruction cycle clock
T3CONbits.TGATE = 0; // Disable Gated Timer mode
T3CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
TMR3 = 0x00; // Clear timer register
PR3 = 56000; // Load the period value
//IPC0bits.T3IP = 0x01; // Set Timer1 Interrupt Priority Level
IFS0bits.T3IF = 0; // Clear Timer1 Interrupt Flag
IEC0bits.T3IE = 1; // Enable Timer1 interrupt
T3CONbits.TON = 1;
}


