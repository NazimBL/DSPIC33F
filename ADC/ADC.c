/*
 * Author: Nazim
 * MPLABX IDE 
 * Created on 18 juin 2018, 11:58
 */
#include <p33fj32mc204.h>
#include "xc.h"
#include <libpic30.h> 

_FOSCSEL(FNOSC_FRCPLL);			// Start with FRC will switch to Primary (XT, HS, EC) Oscillator with PLL
_FOSC(FCKSM_CSDCMD & POSCMD_NONE);	// Clock Switching disabled 
_FBS (BSS_NO_FLASH & BWRP_WRPROTECT_OFF);
_FWDT (FWDTEN_OFF);
_FGS (GSS_OFF & GCP_OFF & GWRP_OFF);
_FPOR (PWMPIN_ON & HPOL_ON & LPOL_ON & FPWRT_PWR128);
_FICD (ICS_PGD1 & JTAGEN_OFF);

#define ON_LED	LATAbits.LATA0
#define FCY 40000000

int ADCValue;
 
void ADC_Init(){

   TRISA = 0xFFFF;   //Set as Input Port
   ADPCFG = 0x0000;
   AD1CON1=0x0004;
   AD1CHS0=0x0001;
   AD1CSSL=0;
   AD1CON3=0x0002;
   AD1CON2=0;
   AD1CON1bits.ADON =1;
}

int readADC()
{                      
    AD1CON1bits.SAMP=0;
    while (!AD1CON1bits.DONE); // wait to complete the conversion
	  return ADCBUF0;                
}

void OSC_Init(){
	 
  PLLFBD =  33;		          // M=45
	CLKDIVbits.PLLPOST = 0;		// N1=2
	CLKDIVbits.PLLPRE = 0;		// N2=2
  // OSCCONbits.OSWEN=1;
	while (OSCCONbits.COSC != 0b001);
  while(OSCCONbits.LOCK!=1);
} 
   
int main(void) {
    
   OSC_Init();
   ADC_Init();
   TRISAbits.TRISA0=0;   
     
    while(1)
    {
        ADCValue=readADC();      
        if(ADCValue>500)ON_LED=1;
        else ON_LED=0;          
    }   
    return 0;    
}
