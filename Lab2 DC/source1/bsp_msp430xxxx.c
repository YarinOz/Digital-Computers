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

  // LCD configuration
  LCD_DATA_WRITE &= ~0xFF;
  LCD_DATA_DIR |= 0xF0;    // P1.4-P1.7 To Output('1')
  LCD_DATA_SEL &= ~0xF0;   // Bit clear P2.4-P2.7
  LCD_CTL_SEL  &= ~0xE0;   // Bit clear P2.5-P2.7
   
  // LEDs 8-bit Array Port configuration
  LEDsArrPortSel &= ~0xFF;            // GPIO capability
  LEDsArrPortDir |= 0xFF;             // output dir
  LEDsArrPort = 0x00;				  // clear all LEDs
  
  // Switches Setup
//   SWsArrPortSel &= ~0xF0;
//   SWsArrPortDir &= ~0x70;
  
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
//-------------------------------------------------------------------------------------
//            Timer1 A2 configuration 
//-------------------------------------------------------------------------------------
void TIMER1_A2_config(void){
    TA1CCTL2 = CAP + CM_1 + CCIE + SCS + CCIS_0; // Timer1 configuration;
    // CM_1 - * Capture mode: 1 - pos. edge */
    // SCS - /* Capture synchronize */
}

//-----------------------------------------------------s--------------------------------
//            Timer1 A1 configuration
//-------------------------------------------------------------------------------------
void TIMER1_A1_config(void){
    TA1CCTL1 =  OUTMOD_7; // TA1CCR1 reset/set;
}

//------------------------------------------------------------------------------------- 
//            Timer 1sec configuration
//-------------------------------------------------------------------------------------
void TIMER0_A0_config(void){
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    TA0CCTL0 = CCIE;
    TACCR0 = 0xFFFF;
    TA0CTL = TASSEL_2 + MC_0 + ID_3;  //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
} 
//------------------------------------------------------------------------------------- 
//            ADC configuration
//-------------------------------------------------------------------------------------
void ADCconfig(void){
      ADC10CTL0 = ADC10SHT_2 + ADC10ON+ SREF_0 + ADC10IE;  // 16*ADCLK+ Turn on, set ref to Vcc and Gnd, and Enable Interrupt
      ADC10CTL1 = INCH_3 + ADC10SSEL_3;     // Input A3 and SMCLK, was |
      ADC10AE0 |= BIT3;                         // P1.3 ADC option select
}             

           
             

 
             
             
            
  

