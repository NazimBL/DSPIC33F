#include "xc.h"

_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);

//define operating frequency and LED pin on B10
#define FCY 40000000
#define LED	LATBbits.LATB10

void SetupOSC();
void myDelay();

void main(void) {
    
SetupOSC();
TRISBbits.TRISB10=0;

while (1) // repeat continuously
{    
    LED=~LED;
    myDelay();
}
}

void myDelay(){
    
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
while(IFS0bits.T3IF == 0); 
IFS0bits.T3IF = 0;     
}
