#include  "../header/halGPIO.h"     // private library - HAL layer
#define LCD_NEW_LINE   {lcd_cmd(0xC0);} // go to second line on the LCD
#define LCD_CLEAR_LAST {lcd_cmd(0x10);} // move cursor left to clear last character
// Global Variables
extern unsigned int preKB=0;
extern unsigned int flag=0;
extern unsigned int ledIndex = 0;

//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
    GPIOconfig();
    TIMER0_A0_config();
    TIMERB_config();    
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//              Read value of 4-bit SWs array
//--------------------------------------------------------------------
unsigned char readSWs(void){
    unsigned char ch;

    ch = SWsArrPort;
    ch &= SWmask;     // mask the least 4-bit
    return ch;
}
//--------------------------------------------------------------------
//              Set Byte to Port
//--------------------------------------------------------------------
void SetByteToPort(char ch){
    PBsArrPortOut |= ch;
} 
//--------------------------------------------------------------------
//              Clear Port Byte
//--------------------------------------------------------------------
void clrPortByte(char ch){
    PBsArrPortOut &= ~ch;
} 
//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  //
    volatile unsigned int i;

    for(i=t; i>0; i--);
}  
//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
    if (LPM_level == 0x00)
      _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
      _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
      _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
    else if(LPM_level == 0x03)
      _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
      _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}
//---------------------------------------------------------------------
//            LCD
//---------------------------------------------------------------------
//******************************************************************
// send a command to the LCD
//******************************************************************
void lcd_cmd(unsigned char c){

    LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

    if (LCD_MODE == FOURBIT_MODE)
    {
        LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
        LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
        lcd_strobe();
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
        lcd_strobe();
    }
    else
    {
        LCD_DATA_WRITE = c;
        lcd_strobe();
    }
}
//******************************************************************
// send data to the LCD
//******************************************************************
void lcd_data(unsigned char c){

    LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_RS(1);
    if (LCD_MODE == FOURBIT_MODE)
    {
            LCD_DATA_WRITE &= ~OUTPUT_DATA;
            LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
            lcd_strobe();
            LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
            LCD_DATA_WRITE &= ~OUTPUT_DATA;
            LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET;
            lcd_strobe();
    }
    else
    {
            LCD_DATA_WRITE = c;
            lcd_strobe();
    }

    LCD_RS(0);
}
//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s){

    while(*s)
        lcd_data(*s++);
}
//******************************************************************
// initialize the LCD
//******************************************************************
void lcd_init(){

    char init_value;

    if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
    else init_value = 0x3F;

    LCD_RS_DIR(OUTPUT_PIN);
    LCD_EN_DIR(OUTPUT_PIN);
    LCD_RW_DIR(OUTPUT_PIN);
    LCD_DATA_DIR |= OUTPUT_DATA;
    LCD_RS(0);
    LCD_EN(0);
    LCD_RW(0);

    DelayMs(15);
    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();
    DelayMs(5);
    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();
    DelayUs(200);
    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();

    if (LCD_MODE == FOURBIT_MODE){
        LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
        lcd_strobe();
        lcd_cmd(0x28); // Function Set
    }
    else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots

    lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
    lcd_cmd(0x1); //Display Clear
    lcd_cmd(0x6); //Entry Mode
    lcd_cmd(0x80); //Initialize DDRAM address to zero
}
//******************************************************************
// lcd strobe functions
//******************************************************************
void lcd_strobe(){
  LCD_EN(1);
  asm("NOP");
 // asm("NOP");
  LCD_EN(0);
}
//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){
    unsigned char i;
    for(i=cnt ; i>0 ; i--) asm("nop"); // tha command asm("nop") takes raphly 1usec
}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){
    unsigned char i;
    for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec
}
//*********************************************************************
//            TimerA0 Interrupt Service Routine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    LPM0_EXIT;
    TACTL = MC_0+TACLR;
}
//*********************************************************************
//            TimerB0 Interrupt Service Routine
//*********************************************************************
#pragma vector=TIMERB0_VECTOR
__interrupt void TimerB_ISR(void) {
    TB0CCTL0 &= ~CCIFG; // Clear the interrupt flag
    ledptr++;
    if(*ledptr == 0){
        ledptr = LEDarray;
    }
    LPM0_EXIT;
}
//*********************************************************************
//            DMA Interrupt Service Routine
//*********************************************************************
#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR(void)
{
  DMA0CTL &= ~DMAIFG;                       // Clear DMA0 interrupt flag
  __bic_SR_register_on_exit(LPM0_bits);     // Exit LPMx, interrupts enabled
}
//*********************************************************************
//            Port1 Interrupt Service Routine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){
   
    delay(debounceVal);
    delay(15000); // For keypad debounce
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
    if(PBsArrIntPend & PB0){
      state = state1;
      PBsArrIntPend &= ~PB0;
        }
        else if(PBsArrIntPend & PB1){
      state = state2;
      PBsArrIntPend &= ~PB1;
        }
    else if(PBsArrIntPend & PB2){
      state = state3;
      PBsArrIntPend &= ~PB2;
        }
    else if(PBsArrIntPend & PB3){
        state = state4;
        PBsArrIntPend &= ~PB3;
            }
//---------------------------------------------------------------------
//            Exit from a given LPM 
//---------------------------------------------------------------------
        switch(lpm_mode){
        case mode0:
         LPM0_EXIT; // must be called from ISR only
         break;
        case mode1:
         LPM1_EXIT; // must be called from ISR only
         break;
        case mode2:
         LPM2_EXIT; // must be called from ISR only
         break;
                case mode3:
         LPM3_EXIT; // must be called from ISR only
         break;
                case mode4:
         LPM4_EXIT; // must be called from ISR only
         break;
    }
}
//*********************************************************************
//            SWitches Vector Interrupt Service Routine
//*********************************************************************
 #pragma vector=PORT2_VECTOR
 __interrupt void keypadIRQ (void)
 {
      delay(debounceVal);
      delay(debounceVal);
      delay(debounceVal);
      delay(debounceVal);
      delay(debounceVal);
      delay(debounceVal);
      delay(debounceVal);
      delay(debounceVal);
      delay(debounceVal);
      if(KeypadIRQIntPend & 0x02) { // if keypad has been pressed find value
          preKB = KB;
          KB = 75;
          KeypadPortOUT = 0x0E;
          if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 13;
          else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 14;
          else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 16;
          else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 15;

          KeypadPortOUT = 0x0D;
          if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 12;
          else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 9;
          else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 8;
          else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 7;

          KeypadPortOUT = 0x0B;
          if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 11;
          else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 6;
          else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 5;
          else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 4;

          KeypadPortOUT = 0x07;
          if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 10;
          else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 3;
          else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 2;
          else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 1;

          if (preKB == KB) {
              flag++;
              if (flag > 2) flag = 0; // reset flag after 3 clicks
              LCD_CLEAR_LAST; // clear last character
          } else {
              flag = 0; // reset flag if a different key is pressed
          }

          if (i == 16) {
              LCD_NEW_LINE;
              //i=0;
          }
          printChar();
          i++;
          if(flag == 1 || flag == 2){
              i--;
          }
          delay(15000); // For keypad debounce
          delay(15000);
          KeypadPortOUT &= ~0x0F; // Reset Row1-4
          KeypadIRQIntPend &= ~BIT1; // Reset Flag
      }

      switch(lpm_mode) {
          case mode0: LPM0_EXIT; break;
          case mode1: LPM1_EXIT; break;
          case mode2: LPM2_EXIT; break;
          case mode3: LPM3_EXIT; break;
          case mode4: LPM4_EXIT; break;
      }
  }
//*********************************************************************
// print chat to LCD
//*********************************************************************
  void printChar() {
      if (KB < 10) {
          switch (KB) {
              case 1: lcd_data(flag == 0 ? '1' : (flag == 1 ? 'G' : '1')); 
                        idiom_recorder[i] = (flag == 0 ? '1' : (flag == 1 ? 'G' : '1'));
                        break;
              case 2: lcd_data(flag == 0 ? '2' : (flag == 1 ? 'H' : '2')); 
                        idiom_recorder[i] = (flag == 0 ? '2' : (flag == 1 ? 'H' : '2'));
                        break;
              case 3: lcd_data(flag == 0 ? '3' : (flag == 1 ? 'I' : '3')); 
                        idiom_recorder[i] = (flag == 0 ? '3' : (flag == 1 ? 'I' : '3'));
                        break;
              case 4: lcd_data(flag == 0 ? '4' : (flag == 1 ? 'K' : '4')); 
                        idiom_recorder[i] = (flag == 0 ? '4' : (flag == 1 ? 'K' : '4'));
                        break;
              case 5: lcd_data(flag == 0 ? '5' : (flag == 1 ? 'L' : '5')); 
                        idiom_recorder[i] = (flag == 0 ? '5' : (flag == 1 ? 'L' : '5'));
                        break;
              case 6: lcd_data(flag == 0 ? '6' : (flag == 1 ? 'M' : '6')); 
                        idiom_recorder[i] = (flag == 0 ? '6' : (flag == 1 ? 'M' : '6'));
                        break;
              case 7: lcd_data(flag == 0 ? '7' : (flag == 1 ? 'O' : '7')); 
                        idiom_recorder[i] = (flag == 0 ? '7' : (flag == 1 ? 'O' : '7'));
                        break;
              case 8: lcd_data(flag == 0 ? '8' : (flag == 1 ? 'P' : '8')); 
                        idiom_recorder[i] = (flag == 0 ? '8' : (flag == 1 ? 'P' : '8'));
                        break;
              case 9: lcd_data(flag == 0 ? '9' : (flag == 1 ? 'Q' : '9')); 
                        idiom_recorder[i] = (flag == 0 ? '9' : (flag == 1 ? 'Q' : '9'));
                        break;

          }
      } else {
          switch (KB) {
              case 16: lcd_data(flag == 0 ? '0' : (flag == 1 ? 'U' : 'V'));
                        idiom_recorder[i] = (flag == 0 ? '0' : (flag == 1 ? 'U' : 'V'));
                        break;
              case 15: lcd_data(flag == 0 ? 'A' : (flag == 1 ? 'S' : 'T')); 
                        idiom_recorder[i] = (flag == 0 ? 'A' : (flag == 1 ? 'S' : 'T'));
                        break;
              case 14: lcd_data(flag == 0 ? 'B' : (flag == 1 ? 'W' : 'X')); 
                        idiom_recorder[i] = (flag == 0 ? 'B' : (flag == 1 ? 'W' : 'X'));
                        break;
              case 13: lcd_data(flag == 0 ? 'F' : (flag == 1 ? 'Y' : 'Z')); 
                        idiom_recorder[i] = (flag == 0 ? 'F' : (flag == 1 ? 'Y' : 'Z'));
                        break;
              case 12: lcd_data(flag == 0 ? 'E' : 'R'); 
                        idiom_recorder[i] = (flag == 0 ? 'E' : 'R');
                        break;
              case 11: lcd_data(flag == 0 ? 'D' : 'N'); 
                        idiom_recorder[i] = (flag == 0 ? 'D' : 'N');
                        break;
              case 10: lcd_data(flag == 0 ? 'C' : 'J'); 
                        idiom_recorder[i] = (flag == 0 ? 'C' : 'J');
                        break;
          }
      }
  }
//*********************************************************************
//             Start TimerB
//*********************************************************************
void startTimerB(void){
    TB0CCTL0 = CCIE; // CCR0 interrupt enabled
    TB0CCR2 = 0xFFFF; // 
    TB0CTL = TBSSEL_2 + MC_2 + ID_3 + TBCLR; // ACLK, upmode, clear TAR
    __bis_SR_register(GIE); // Enter LPM0
}
//-------------------------------------------------------------
//              StartTimer For StopWatch
//-------------------------------------------------------------
void startTimerA0(){
    TA0CCTL0 = CCIE;  // CCR0 interrupt enabled
    TACCR0 = 0xFFFF;  // Timer Cycles - max
    TA0CTL = TASSEL_2 + MC_3 + ID_3;  //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8
    // ACLK doesn't work on our msp, so we have to use smclk and divide the freq to get to 1 sec.
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}
//-------------------------------------------------------------------------------------
//            Stop All Timers
//-------------------------------------------------------------------------------------
void StopAllTimers(void){
    TACTL = MC_0 + TACLR; // halt timer A
    TBCTL = MC_0 + TBCLR; // halt timer B
    DMA0CTL &= ~DMAEN; // Stop DMA0
    DMA1CTL &= ~DMAEN; // Stop DMA1
}
//-------------------------------------------------------------------------------------
//            DMA Configuration
//-------------------------------------------------------------------------------------
void DMA0_STATE2(int *ptr1, int len1, int *ptr_merge){
     // Use DMA to transfer the word
    DMA0SA = (void (*)())ptr1;  // Source address for merge1
    DMA0DA = (void (*)())ptr_merge;  // Destination address for strMerge
    DMA0SZ = len1;  // Block size
    DMA0CTL = DMAEN + DMASRCINCR_3 + DMADSTINCR_3 + DMADT_1 + DMASRCBYTE + DMADSTBYTE;  // Enable DMA, source and destination increment, block transfer mode
    DMA0CTL |= DMAREQ;  // Manually trigger DMA transfer for merge1 word
    while (DMA0CTL & DMAEN);  // Wait for DMA transfer to complete
}
void DMA1_STATE2(int *ptr2, int len2, int *ptr_merge){
    // Use DMA to transfer the word
    DMA1SA = (void (*)())ptr2;  // Source address for merge2
    DMA1DA = (void (*)())ptr_merge;  // Destination address for strMerge
    DMA1SZ = len2;  // Block size
    DMA1CTL = DMAEN + DMASRCINCR_3 + DMADSTINCR_3 + DMADT_1 + DMASRCBYTE + DMADSTBYTE;  // Enable DMA, source and destination increment, block transfer mode
    DMA1CTL |= DMAREQ;  // Manually trigger DMA transfer for merge2 word
    while (DMA1CTL & DMAEN);  // Wait for DMA transfer to complete
}
void DMA0_STATE3(){
    DMACTL0 = DMA0TSEL_2;  // TimerB0 trigger for DMA0
    DMA0SA = (void (*)())ledptr;  // Source address for LEDarray
    DMA0DA = (void (*)())&LEDsArrPort ;  // Destination address for LEDsArrPort
    DMA0SZ = 1;  // Block size
    DMA0CTL = DMAEN + DMASRCINCR_3 + DMADSTINCR_0 + DMADT_1 + DMASBDB + DMAIE;  // Enable DMA, source increment, block transfer mode
}
