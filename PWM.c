/*
 * MPLAB X IDE
 * Author: Nazim
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

#define FCY 40000000

void PWM_Init()
{
    TRISB = 0x00; // make sure PWM pins are set to be outputs
    PORTB = 0x00; // clear the outputs 
 
    PTCONbits.PTOPS = 1; // PWM timer post-scale
    PTCONbits.PTCKPS = 0; // PWM timer pre-scale
    PTCONbits.PTMOD = 2; // PWM operates in Up-down Mode continuously
 
    PTMR = 0; // PWM counter value, start at 0
 
    PTPER = 19999; // PWM Timebase period
 
    PWMCON1bits.PMOD3 = 0; // PWM in complimentary mode
    PWMCON1bits.PMOD2 = 0; // PWM in complimentary mode
    PWMCON1bits.PMOD1 = 0; // PWM in complimentary mode
    PWMCON1bits.PEN3H = 1; // PWM High pin is enabled
    PWMCON1bits.PEN2H = 1; // PWM High pin is enabled
    PWMCON1bits.PEN1H = 1; // PWM High pin is enabled
    PWMCON1bits.PEN3L = 1; // PWM Low pin enabled (direction control later?)
    PWMCON1bits.PEN2L = 1; // PWM Low pin enabled (direction control later?)
    PWMCON1bits.PEN1L = 1; // PWM Low pin enabled (direction control later?)
 
    DTCON1bits.DTAPS = 0;  //DeadTime pre-scaler
    DTCON1bits.DTA = 59;   //DeadTime value for 4 us. 

    PDC1 = 19999; // PWM#1 Duty Cycle register (11-bit)
    PDC2 = 19999; // PWM#2 Duty Cycle register (11-bit)
    PDC3 = 19999; // PWM#3 Duty Cycle register (11-bit)
 
    PTCONbits.PTEN = 1; // Enable PWM Timerbase!
}

void OSC_Init(){
	
    PLLFBD =  33;		        // M=45
    CLKDIVbits.PLLPOST = 0;		// N1=2
    CLKDIVbits.PLLPRE = 0;		// N2=2
    // OSCCONbits.OSWEN=1;
    while (OSCCONbits.COSC != 0b001);
    while(OSCCONbits.LOCK!=1);
} 
   
void main(void) {
    
   OSC_Init();
   PWM_Init();  
   
   while(1);       
}
