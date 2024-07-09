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
  
  // Buzzer Setup
  BuzzPortDir |= BIT4;             // P2.4 Output compare - '1'
  BuzzPortSel |= BIT4;             // P2.4 Select = '1'
  BuzzPortOut &= ~BIT4;             // P2.4 out = '0'

  // Potentiometer Setup
  PotPortSel &= ~BIT3;             // P1.3 Select = '0'
  PotPortDir &= ~BIT3;             // P1.3 Input = '0'
  PotPortOut &= ~BIT3;             // P1.3 out = '0'

  // RGP LEDs Setup
  RGBArrPortOut &= ~0x07;            // Clear P2.0-2
  RGBArrPortDir |= 0x07;             // Set P2.0-2 to output ('1')
  RGBArrPortSEL &= ~0x07;            // GPIO capability

  // PushButtons Setup
  // PBsArrPortSel &= ~0x0F;           //
  // PBsArrPortOut &= ~0x0F;            // Set P1Out to '0'
  // PBsArrPortDir &= ~0x0F;            // P1.0-2 - Input ('0')
  // PBsArrPortDir |= 0x00;             // P1.3 - Output ('1')
  // PBsArrIntEdgeSel |= 0x03;          // pull-up mode   P1.0-P1.1 - '1'
  // PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode  P1.2 - '0'
  // PBsArrIntEn |= 0x0F;               // P1.0-3 - '1'
  // PBsArrIntPend &= ~0xFF;            // clear pending interrupts P1.0-P1.3 all P1

  _BIS_SR(GIE);                     // enable interrupts globally
}

//-------------------------------------------------------------------------------------
//            Stop All Timers
//-------------------------------------------------------------------------------------
void StopAllTimers(void){
    TACTL = MC_0; // halt timer A

}
//-------------------------------------------------------------------------------------
//            ADC configuration
//-------------------------------------------------------------------------------------
void ADC_config(void){
      ADC10CTL0 = ADC10SHT_2 + ADC10ON+ SREF_0 + ADC10IE;  // 16*ADCLK+ Turn on, set ref to Vcc and Gnd, and Enable Interrupt
      ADC10CTL1 = INCH_3 + ADC10SSEL_3;     // Input A3 and SMCLK, was |
      ADC10AE0 |= BIT3;                         // P1.3 ADC option select
}

//-------------------------------------------------------------------------------------
//            Timer A 345msec configuration
//-------------------------------------------------------------------------------------
void TIMER_A0_config(unsigned int counter){
    TACCR0 = counter; // (2^20/8)*345m = 45219 -> 0xB0A3
    TACCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_3;  //  select: 2 - SMCLK ; control: 1 - Up ; divider: 3 - /8
}

//-------------------------------------------------------------------------------------
//                              UART init
//-------------------------------------------------------------------------------------
void UART_init(void){
    if (CALBC1_1MHZ==0xFF)                  // If calibration constant erased
      {
        while(1);                               // do not load, trap CPU!!
      }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;

    P2DIR = 0xFF;                             // All P2.x outputs
    P2OUT = 0;                                // All P2.x reset
    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1DIR |= RXLED + TXLED;
    P1OUT &= 0x00;

    UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
    UCA0BR0 = 104;                           //
    UCA0BR1 = 0x00;                           //
    UCA0MCTL = UCBRS0;               //
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

