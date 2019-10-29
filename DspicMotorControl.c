/*
 * File: MPLAB X IDE
 * Author: Nazim BL
 * Created on 1 juillet 2018, 10:10
 */

#include "xc.h"

_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);

#define FCY 40000000
#define LED	LATBbits.LATB10

void SetupOSC();
void Buck_Boost();
void ADC_Setup();
void Timer_Setup();
void myDelay();

int ADCValue=0;
char temp;
float d=1;
int p1=100,p2=100;
void PWM_update(int pwm1,int pwm2);

int main(void) {
    
SetupOSC();
ADC_Setup();
PWM_Init();
//LED
TRISBbits.TRISB10=0;
LED=0;

while (1) 
{
    Buck_Boost();
    //PWM_update(800,1600); test 
}  
    return 0;
}

void Buck_Boost(){
 //read adc input
 AD1CON1bits.SAMP = 1;
 while (!AD1CON1bits.DONE);// conversion done?
 ADCValue = ADC1BUF0;
 if(ADCValue>2500)LED=1;
 else LED=0;
 
 d=(float)ADCValue/4095;
 //d=0.65;
 d=d*2;
    if(d>=1){
        d=d-1;
        if(d*800<600)PWM_update((int)(d*800),800);
        else PWM_update(600,800);
   
    }else PWM_update(0,(int)(d*800));    
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
    PTPER = 799; // PWM Timebase period
    
    PWMCON1bits.PMOD2 = 0; // PWM in complimentary mode
    PWMCON1bits.PMOD1 = 0; // PWM in complimentary mode
    PWMCON1bits.PEN2H = 1; // PWM High pin is enabled
    PWMCON1bits.PEN1H = 1; // PWM High pin is enabled
	
    DTCON1bits.DTAPS = 0;  //DeadTime pre-scaler = Tcy
    DTCON1bits.DTA = 0;   //DeadTime value for 4 us. 

    PDC1 = p1; // PWM#1 Duty Cycle register (11-bit)
    PDC2 = p2; // PWM#2 Duty Cycle register (11-bit)

    PTCONbits.PTEN = 1; // Enable PWM Timerbase!
}

void PWM_update(int pwm1,int pwm2){
PTCONbits.PTEN=0;
PDC1 = pwm1;
PDC2 = pwm2;
PTCONbits.PTEN=1;
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
 //10 bit //AD1CON1 = 0x00E0; 
 //12 bit
 AD1CON1 = 0x04E0;  
 AD1CHS0= 0x0000; // Connect RA0/AN0 as CH0 input ..
 AD1CSSL = 0;
 AD1CON3 = 0x1F02; // Tsampling = 2 Tad
 AD1CON2 = 0;
 AD1CON1bits.ADON = 1; // turn ADC ON 
}

