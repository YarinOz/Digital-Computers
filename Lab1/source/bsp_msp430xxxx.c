#ifdef __MSP430FG4619__
#include "../header/bsp_msp430x4xx.h" //lab kit
#else
#include "../header/bsp_msp430x2xx.h" //pesronal kit
#endif
//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  // LEDs 8-bit Array Port configuration
  LEDsArrPortSel &= ~0xFF;            // GPIO capability
  LEDsArrPortDir |= 0xFF;             // output dir
  LEDsArrPort = 0x00;				  // clear all LEDs
  
  // PushButtons Setup
  PBsArrPortOut &= ~0x80;          // 2.7 output , 2.0-2.6 input
  PBsArrPortSel &= ~0xFF;
  PBsArrPortDir = 0x80;            // 2.7 output , 2.0-2.6 input PB-SW
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0xFC;       // pull-down mode
  PBsArrIntEn |= 0x0F;
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts 
  
  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
	
	//write here timers congiguration code
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
	
	//write here ADC congiguration code
}              

           
             

 
             
             
            
  

