#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#define FCY 40000000

#include <xc.h>  

int i=0;
void PPS();

void UartInit(unsigned int baud){
   
    AD1PCFGL = 0xFFFF;
    //AD1CON1bits.ADON = 0;
    //RX,TX
    TRISBbits.TRISB0=1;
    TRISBbits.TRISB2=0;
    
    PPS();//Peripheral Pin Select
    
    U1MODEbits.STSEL = 0; // 1 Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U1MODEbits.BRGH = 0; // Low Speed mode
    U1MODEbits.UEN = 0;
    U1STAbits.ADDEN = 0;
    U1BRG =((FCY/baud)/16)-1 ; // BAUD Rate Setting for 9600
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
    U1STAbits.UTXISEL1 = 0;
    U1STAbits.URXISEL = 0;
    IEC0bits.U1TXIE = 1; // Enable UART TX Interrupt
    IEC0bits.U1RXIE = 1;
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
    
    //wait at least 104 usec (1/9600) before sending first char
    for(i = 0; i < 4160; i++)Nop();
}

void UART_Write(char send){

while(U1STAbits.UTXBF== 1);
U1TXREG = send;
}

char UART_Read(){
return (char)U1RXREG;
}

void PPS(){
RPINR18bits.U1RXR = 0; // rx to RP0
//RPINR18bits.U1CTSR = 1;// Assign U1CTS To Pin RP1
RPOR1bits.RP2R = 3;// Assign U1Tx To Pin RP2
//RPOR1bits.RP3R = 4;// Assign U1RTS To Pin RP3
}

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef	__cplusplus
}
#endif /* __cplusplus */
#endif	/* XC_HEADER_TEMPLATE_H */

