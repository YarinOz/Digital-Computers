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

  // Keypad Setup
  KeypadPortSel &= ~0xFF;
  KeypadPortDIR = 0x0F; //10.0-10.3 output, 10.4-10.7 input
  KeypadPortOUT = 0x00; // CLR output
  // Keypad IRQ Setup
  KeypadIRQPortSel &= ~0x02;
  KeypadIRQPortDir &= ~0x02;             // P2.1 input
  KeypadIRQIntEdgeSel |= 0x02;         // pull-up mode  P2.1 - '1'
  KeypadIRQIntEn |= 0x02;               // P2.1 - '1'
  KeypadIRQIntPend &= ~0x02;            // clear pending interrupts P2.1

  // PushButtons Setup
  PBsArrPortSel &= ~0x0F;           //
  PBsArrPortOut &= ~0x0F;            // Set P1Out to '0'
  PBsArrPortDir &= ~0x0F;            // P1.0-2 - Input ('0')
  PBsArrPortDir |= 0x00;             // P1.3 - Output ('1')
  PBsArrIntEdgeSel |= 0x03;          // pull-up mode   P1.0-P1.1 - '1'
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode  P1.2 - '0'
  PBsArrIntEn |= 0x0F;               // P1.0-3 - '1'
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts P1.0-P1.3 all P1

  _BIS_SR(GIE);                     // enable interrupts globally
}

//------------------------------------------------------------------------------------- 
//            Timer 1sec configuration - For state2
//-------------------------------------------------------------------------------------
void TIMER0_A0_config(void){
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    TACCR0 = 0xFFFF;
    TA0CTL = TASSEL_2 + MC_3 + ID_3;  //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8
    __bis_SR_register(GIE);       //interrupt

} 
//-------------------------------------------------------------------------------------
//               Timer B
//-------------------------------------------------------------------------------------
void TIMERB_config(void){
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    TB0CCR2 = 0xFFFF;  // Assuming SMCLK frequency is 1 MHz, 25000 cycles = 25ms period
    TB0CTL = TBSSEL_2 + MC_3 + ID_3; //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8
    __bis_SR_register(GIE);       //interrupt
}
//-------------------------------------------------------------------------------------
//            DMA configuration 
//-------------------------------------------------------------------------------------
void DMA_config(void){
    DMA0CTL = DMAEN + DMASRCINCR_3 + DMASWDW + DMAIE + DMADT_1; //block-transfer, source inc, word-word trans, Interupt enable
    DMA1CTL = DMAEN + DMASRCINCR_3 + DMASWDW + DMAIE + DMADT_1; //block-transfer, source inc, word-word trans, Interupt enable
}

void DMA_config_RT(void){ // For Main Lab

}

