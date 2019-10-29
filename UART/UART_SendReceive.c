/*
 * Author: Nazim
 * Created on 1 juillet 2018, 10:10
 */

#include "xc.h"
#include "UART.h"

_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);

#define FCY 40000000
#define LED	LATBbits.LATB10

void SetupOSC();
char ReceivedChar='x';
char received[4];

void main() {
    
SetupOSC();
UartInit(1200);
//LED
TRISBbits.TRISB10=0;
LED=0;

while (1) 
{
    
    // if(ReceivedChar=='1')LED=1;    //this was to test reception
    //else LED=0;
    //LED=~LED;
    
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
    UART_Write('1');
    UART_Write('2');
    UART_Write('3');
  
}

void SetupOSC(){

//FCY = 40 Mhz    
PLLFBD = 41;// M = 43
CLKDIVbits.PLLPOST = 0; // N1 = 2
CLKDIVbits.PLLPRE = 0; // N2 = 2

__builtin_write_OSCCONH(0x03);
__builtin_write_OSCCONL(0x01);
// Wait for clock switch to occur
while (OSCCONbits.COSC != 0b011);
while(OSCCONbits.LOCK !=1); // Wait for PLL to lock
}
}


