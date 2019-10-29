/*
* Nazim BL
* MikroC PRO For ARM
* Config : 72 Mhz  (8 Mhz Quartz, PLLx9)
* don't forget to add UART library from library manager
*/

void main() {

   UART1_Init_Advanced(19200,_UART_8BIT_NOPARITY,_UART_ONE_STOPBIT,_UART_LOW_SPEED);
   Delay_ms(100);     
	 UART_Write_Text("Start");
	 UART_Write(10);
	 UART_Write(13);
	 
	 //set Port A output, high
	 TRISA = 0;
	 LATA = 1;
   
   while(1){
   
   UART_Write('Z');
	 UART_Write(10);
	 UART_Write(13);
   }
}   

