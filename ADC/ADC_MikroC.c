
unsigned adcRes;

void main() {
   
  //this program turns on an LED (Output) when the ADC input (Potentiometer) is over 500
 
  TRISA= 1;
  
  TRISB.B0=0;
  ADC1_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF);

  while (1) {
  
    adcRes = ADC1_Get_Sample(1);

    if(adcRes>500)LATB.B0=1;
    else LATB.B0=0;
    Delay_ms(50);
  }
}