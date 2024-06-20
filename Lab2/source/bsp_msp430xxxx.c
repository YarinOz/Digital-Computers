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
   
  // LCD configuration
  LCD_DATA_WRITE &= ~0xFF;
  LCD_DATA_DIR |= 0xF0;    // P1.4-P1.7 To Output('1')
  LCD_DATA_SEL &= ~0xF0;   // Bit clear P2.4-P2.7
  LCD_CTL_SEL  &= ~0xE0;   // Bit clear P2.5-P2.7
  
  // Generator Setup
  GenPortDir &=  ~BIT4;               // P2.4 Input Capture = '1'
  GenPortSel |=  BIT4;              // P2.4 Select = '1'

  // Buzzer Setup
  BuzzPortDir |= BIT2;             // P2.2 Output compare - '1'
  BuzzPortSel |= BIT2;             // P2.2 Select = '1'
  BuzzPortOut &= ~BIT2;             // P2.2 out = '0'

 // Switches Setup
  SWsArrPortDir  &= 0xFE;
  SWsArrPortSel  &= 0xFE;
  SWsArrIntEn  |= 0x01;
  SWsArrIntPend &= ~0xFF;           // clear pending interrupts P2.0-2

  // PushButtons Setup
  PBsArrPortSel &= ~0x07;           //
  PBsArrPortOut &= ~0x07;            // Set P1Out to '0'
  PBsArrPortDir &= ~0x07;            // P1.0-2 - Input ('0')
  PBsArrPortDir |= 0x08;             // P1.3 - Output ('1')
  PBsArrIntEdgeSel |= 0x03;          // pull-up mode   P1.0-P1.1 - '1'
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode  P1.2 - '0'
  PBsArrIntEn |= 0x07;               // P1.0-2 - '1'
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts P1.0-P1.3 all P1
  // PB3 RT
  PB3sArrPortSel &= ~BIT1;           //
  PB3sArrPortOut &= ~BIT1;            // Set P2Out to '0'
  PB3sArrPortDir &= ~BIT1;            // P2.1 - Input ('0')
  PB3sArrIntEdgeSel &= ~BIT1;         // pull-down mode  P2.1 - '0'
  PB3sArrIntEn |= BIT1;               // P1.0-2 - '1'
  PB3sArrIntPend &= ~BIT1;            // clear pending interrupts P2.1

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
void ADCconfig(void){
      ADC10CTL0 = ADC10SHT_2 + ADC10ON+ SREF_0 + ADC10IE;  // 16*ADCLK+ Turn on, set ref to Vcc and Gnd, and Enable Interrupt
      ADC10CTL1 = INCH_3 + ADC10SSEL_3;     // Input A3 and SMCLK, was |
      ADC10AE0 |= BIT3;                         // P1.3 ADC option select
}
