#include "xc.h"
#include "MyUart.h"

_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);

#define FCY 40000000

void SetupOSC();
void myDelay();

int main(void) {
    
SetupOSC();
UartInit(1200);

while (1) // repeat continuously
{
    
    UART_Write('1');
    UART_Write('2');
    UART_Write('3');
    
    //Receive buffer has data; at least one more character can be read
    if(U1STAbits.URXDA==1){
     ReceivedChar=(char)U1RXREG;
     UART_Write(ReceivedChar);
     ReceivedChar=(char)U1RXREG;
     UART_Write(ReceivedChar);
     ReceivedChar=(char)U1RXREG;
     UART_Write(ReceivedChar);
     UART_Write('\r');
     UART_Write('\n');
    }
    
   myDelay();
  
}
  
    return 0;
}

//generate 500 ms delay
void myDelay(){
    
T3CONbits.TON = 0; // Disable Timer
T3CONbits.TCS = 0; // Select internal instruction cycle clock
T3CONbits.TGATE = 0; // Disable Gated Timer mode
T3CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
TMR3 = 0x00; // Clear timer register
PR3 = 65000; // Load the period value
//IPC0bits.T3IP = 0x01; // Set Timer1 Interrupt Priority Level
IFS0bits.T3IF = 0; // Clear Timer1 Interrupt Flag
IEC0bits.T3IE = 1; // Enable Timer1 interrupt
T3CONbits.TON = 1;

while(IFS0bits.T3IF == 0); 
IFS0bits.T3IF = 0; 
    
}
