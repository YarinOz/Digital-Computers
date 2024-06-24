#ifdef __MSP430FG4619__
#include "../header/bsp_msp430x4xx.h" //lab kit
#else
#include "../header/bsp_msp430x2xx.h" //pesronal kit
#endif
//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
  WDTCTL = WDTHOLD | WDTPW;     // Stop WDT
   
  // LEDs 8-bit Array Port configuration
  LEDsArrPortSel &= ~0xFF;            // GPIO capability
  LEDsArrPortDir |= 0xFF;             // output dir
  LEDsArrPort = 0x00;				  // clear all LEDs

  // LCD configuration
  LCD_DATA_WRITE &= ~0xFF;
  LCD_DATA_DIR |= 0xF0;    // P1.4-P1.7 To Output('1')
  LCD_DATA_SEL &= ~0xF0;   // Bit clear P2.4-P2.7
  LCD_CTL_SEL  &= ~0xE0;   // Bit clear P2.5-P2.7
  
  // Generator Setup
//   GenPortDir &=  ~BIT4;               // P2.4 Input Capture = '1'
//   GenPortSel |=  BIT4;              // P2.4 Select = '1'

  // Buzzer Setup
//   BuzzPortDir |= BIT2;             // P2.2 Output compare - '1'
//   BuzzPortSel |= BIT2;             // P2.2 Select = '1'
//   BuzzPortOut &= ~BIT2;             // P2.2 out = '0'

    // Keypad Setup
  KeypadPortSel &= ~0xFF;
  KeypadPortDIR = 0x0F; //10.0-10.3 output, 10.4-10.7 input
  KeypadPortOUT = 0x00; // CLR output

 // Switches Setup
//   SWsArrPortDir  &= 0xFE;
//   SWsArrPortSel  &= 0xFE;
//   SWsArrIntEn  |= 0x01;
//   SWsArrIntPend &= ~0xFF;           // clear pending interrupts P2.0-2

  // PushButtons Setup
  PBsArrPortSel &= ~0x07;           //
  PBsArrPortOut &= ~0x07;            // Set P1Out to '0'
  PBsArrPortDir &= ~0x07;            // P1.0-2 - Input ('0')
  PBsArrPortDir |= 0x08;             // P1.3 - Output ('1')
  PBsArrIntEdgeSel |= 0x03;          // pull-up mode   P1.0-P1.1 - '1'
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode  P1.2 - '0'
  PBsArrIntEn |= 0x07;               // P1.0-2 - '1'
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts P1.0-P1.3 all P1

  _BIS_SR(GIE);                     // enable interrupts globally
}
//-------------------------------------------------------------------------------------
//            Timer1 A2 configuration - For state1
//-------------------------------------------------------------------------------------
void TIMER1_A2_config(void){
    TA1CCTL2 = CAP + CM_1 + CCIE + SCS + CCIS_0; // Timer1 configuration;
    // CM_1 - * Capture mode: 1 - pos. edge */
    // SCS - /* Capture synchronize */
}
//-----------------------------------------------------s--------------------------------
//            Timer1 A1 configuration - For state3
//-------------------------------------------------------------------------------------
void TIMER1_A1_config(void){
    TA1CCTL1 =  OUTMOD_7; // TA1CCR1 reset/set;
}
//------------------------------------------------------------------------------------- 
//            Timer 1sec configuration - For state2
//-------------------------------------------------------------------------------------
void TIMER0_A0_config(void){
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    TA0CCTL0 = CCIE;
    TACCR0 = 0xFFFF;
    TA0CTL = TASSEL_2 + MC_0 + ID_3;  //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8
    __bis_SR_register(GIE);       //interrupt

} 
//------------------------------------------------------------------------------------- 
//            ADC configuration
//-------------------------------------------------------------------------------------
// void ADCconfig(void){
//       ADC10CTL0 = ADC10SHT_2 + ADC10ON+ SREF_0 + ADC10IE;  // 16*ADCLK+ Turn on, set ref to Vcc and Gnd, and Enable Interrupt
//       ADC10CTL1 = INCH_3 + ADC10SSEL_3;     // Input A3 and SMCLK, was |
//       ADC10AE0 |= BIT3;                         // P1.3 ADC option select
// }
//-------------------------------------------------------------------------------------
//            DMA configuration
//-------------------------------------------------------------------------------------
void DMA_config(void){
    DMA0DA = (int) &TBCCR0;
    DMA0CTL = DMAEN + DMASRCINCR_3 + DMASWDW + DMAIE; //repeated-single, source inc, word-word trans, Interupt enable
    DMACTL0 = DMA0TSEL_1; //TACCR2_IFG trigger
}

void DMA_config_RT(void){ // For Main Lab

}
//-------------------------------------------------------------------------------------
//            Stop All Timers
//-------------------------------------------------------------------------------------
void StopAllTimers(void){

    TACTL = MC_0; // halt timer A
    TBCTL = MC_0; // halt timer B
    TBCCTL1 = 0x00; // stop PWM
    DMA0CTL = 0; // Stop DMA0
}
