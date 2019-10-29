/*
 * Author: Nazim
 * Created on 18 juin 2018, 11:58
 */
 
#include <p33fj32mc204.h>
#include "xc.h"
#include "SPI.h"

_FOSCSEL(FNOSC_FRCPLL);			// Start with FRC will switch to Primary (XT, HS, EC) Oscillator with PLL
_FOSC(FCKSM_CSDCMD & POSCMD_NONE);	// Clock Switching disabled 
_FBS (BSS_NO_FLASH & BWRP_WRPROTECT_OFF);
_FWDT (FWDTEN_OFF);
_FGS (GSS_OFF & GCP_OFF & GWRP_OFF);
_FPOR (PWMPIN_ON & HPOL_ON & LPOL_ON & FPWRT_PWR128);
_FICD (ICS_PGD1 & JTAGEN_OFF);

#define ON_LED	LATAbits.LATA0
#define FCY 40000000

char master=0;
char data='z';

//SPI ISR, asynchronous read when data buffer is full
void __attribute__((__interrupt__,__auto_psv__)) _SPI1Interrupt(void)
{

	if (SPI1STATbits.SPIROV)
	{
		//----- OVERFLOW ERROR -----
		data = SPI1BUF;
		SPI1STATbits.SPIROV = 0;
		IFS0bits.SPI1IF = 0;		
		return;
	}
    while(!SPI1STATbits.SPIRBF);
	  data = SPI1BUF;
	  IFS0bits.SPI1IF = 0;			//Clear the Interrupt Flag
}

void OSC_Init(){

    PLLFBD =  33;		        // M=45
	  CLKDIVbits.PLLPOST = 0;		// N1=2
	  CLKDIVbits.PLLPRE = 0;		// N2=2
   // OSCCONbits.OSWEN=1;
	  while (OSCCONbits.COSC != 0b001);
    while(OSCCONbits.LOCK!=1);
}

void main() {
    
   OSC_Init();
   SPI_Init(master);
   TRISAbits.TRISA0=0;
       
    while(1)
    {
        // decomment to read synchronously   
        //if(IFS0bits.SPI1IF)data=SPI_Read(); //Polling
        //data='l';
        if(data==0x1E)ON_LED=1;
        else ON_LED=0;           
    }    
}
