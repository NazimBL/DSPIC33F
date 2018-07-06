/*
 * File:   main.c
 * Author: DELL
 *
 * Created on 1 juillet 2018, 10:10
 */


#include "xc.h"
#include "MyUart.h"

_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);

#define FCY 40000000
#define LED	LATBbits.LATB10

char ReceivedChar='x';

int ADCValue=0;
void SetupOSC();
void ADC_Read();


int main(void) {
    
SetupOSC();
UartInit(9600);
//ADC_Setup();
//Timer_Setup();


//LED
TRISBbits.TRISB10=0;

LED=1;
U1TXREG = ReceivedChar;
while (1) // repeat continuously
{
 
  
}
  
    return 0;
}


void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
  
IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
ADC_Read(); 

}



void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{ 
    
    IFS0bits.U1TXIF = 0; // clear TX interrupt flag
    U1TXREG = ReceivedChar; // Transmit one character
    //LED=0;
    

}


void __attribute__((interrupt, auto_psv)) _U1RXInterrupt( void )               
{
   
    if(U1STAbits.FERR == 1)
{
 ReceivedChar=0;
}     
    
    if ( U1STAbits.OERR == 1 )
    {
        U1STAbits.OERR = 0;
    }
    ReceivedChar = U1RXREG;
    LED=0;
    IFS0bits.U1RXIF = 0;
}
void ADC_Read(){
 AD1CON1bits.SAMP = 1;
 while (!AD1CON1bits.DONE);// conversion done?
 ADCValue = ADC1BUF0;
 
 
 //ADCValue =ADChigh*256+ADClow;
 
 if(ADCValue>500)LED=1;
 else LED=0;
 
}

void Timer_Setup(){

T3CONbits.TON = 0; // Disable Timer
T3CONbits.TCS = 0; // Select internal instruction cycle clock
T3CONbits.TGATE = 0; // Disable Gated Timer mode
T3CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
TMR3 = 0x00; // Clear timer register
PR3 = 7999; // Load the period value
//IPC0bits.T3IP = 0x01; // Set Timer1 Interrupt Priority Level
IFS0bits.T3IF = 0; // Clear Timer1 Interrupt Flag
IEC0bits.T3IE = 1; // Enable Timer1 interrupt
T3CONbits.TON = 1;

}
void PWM_Init()
{
    TRISBbits.TRISB12=0;
    TRISBbits.TRISB14=0;
    // RB14 and Rb12
    PORTB = 0x00; // clear the outputs 
 
    PTCONbits.PTOPS = 1; // PWM timer post-scale
    PTCONbits.PTCKPS = 0; // PWM timer pre-scale
    PTCONbits.PTMOD = 2; // PWM operates in Up-down Mode continuously 
    //--> interupt each time wwe get to zero
 
    PTMR = 0; // PWM counter value, start at 0
    PTPER = 399; // PWM Timebase period
    
    PWMCON1bits.PMOD2 = 0; // PWM in complimentary mode
    PWMCON1bits.PMOD1 = 0; // PWM in complimentary mode
    PWMCON1bits.PEN2H = 1; // PWM High pin is enabled
    PWMCON1bits.PEN1H = 1; // PWM High pin is enabled
   
   
    DTCON1bits.DTAPS = 0;  //DeadTime pre-scaler = Tcy
    DTCON1bits.DTA = 0;   //DeadTime value for 4 us. 

    PDC1 = 399; // PWM#1 Duty Cycle register (11-bit)
    PDC2 = 199; // PWM#2 Duty Cycle register (11-bit)

    PTCONbits.PTEN = 1; // Enable PWM Timerbase!
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

//read each 5khz
void ADC_Setup(){

 TRISAbits.TRISA0=1;    
 AD1PCFGL = 0xFFFE;// all PORTA = Digital; RA0 = analog
 //10 bit
 AD1CON1 = 0x00E0; 
 //12 bit
 //AD1CON1 = 0x04E0; 
 
 AD1CHS0= 0x0000; // Connect RA0/AN0 as CH0 input ..
 
 AD1CSSL = 0;
 AD1CON3 = 0x1F02; // Tsampling = 2 Tad
 AD1CON2 = 0;
 AD1CON1bits.ADON = 1; // turn ADC ON
 
}

