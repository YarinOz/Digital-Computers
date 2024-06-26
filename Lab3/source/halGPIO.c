#include  "../header/halGPIO.h"     // private library - HAL layer

// Global Variables
extern unsigned int preKB=0;
extern unsigned int flag=0;

//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
    GPIOconfig();
    TIMER0_A0_config();
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
//    write frequency template to LCD
//******************************************************************
void freq_template_LCD(){
   lcd_clear();
   lcd_home();
    const char SquareWaveFreq[] = "fin=";
    const char Hz[] = "Hz";
     lcd_puts(SquareWaveFreq);
     lcd_cursor_right();
     lcd_cursor_right();
     lcd_cursor_right();
     lcd_cursor_right();
     lcd_cursor_right();
     lcd_puts(Hz);
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
//            DMA ISR
//*********************************************************************
#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR (void){
    StopAllTimers();
}
//*********************************************************************
//            Port1 Interrupt Service Routine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){
   
    delay(debounceVal);
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
//-------------------------------------------------------------
//             print char to LCD
//-------------------------------------------------------------
void printChar(){
    if ((flag==0) && (KB != preKB)){  
        i++;
        if (KB<10 && KB!=0){
            lcd_data(0x30 + KB);
        }
        else if(KB==0){
            lcd_data('0');
        }
        else if (KB==15){
            lcd_data('A');
        }
        else if (KB==14){
            lcd_data('B');
        }
        else if (KB==13){
            lcd_data('F');
        }
        else if(KB==12){
            lcd_data('E');
        }
        else if(KB==11){
            lcd_data('D');
        }
        else if(KB==10){
            lcd_data('C');
        }
    }
    else if(flag==1 && (KB != preKB)){
        i++;
        if (KB==0){
            lcd_data('U');
        }
        else if (KB=1){
            lcd_data('G');
        }
        else if (KB=2){
            lcd_data('H');
        }
        else if (KB=3){
            lcd_data('I');
        }
        else if (KB=4){
            lcd_data('K');
        }
        else if (KB=5){
            lcd_data('L');
        }
        else if (KB=6){
            lcd_data('M');
        }
        else if (KB=7){
            lcd_data('O');
        }
        else if (KB=8){
            lcd_data('P');
        }
        else if (KB=9){
            lcd_data('Q');
        }
        else if (KB==15){
            lcd_data('S');
        }
        else if (KB==14){
            lcd_data('W');
        }
        else if (KB==13){
            lcd_data('Y');
        }
        else if(KB==12){
            lcd_data('R');
        }
        else if(KB==11){
            lcd_data('N');
        }
        else if(KB==10){
            lcd_data('J');
        }
    }
    else if(flag==2){
        i++;
        if (KB==0){
            lcd_data('V');
        }
        else if (KB==15){
            lcd_data('T');
        }
        else if (KB==14){
            lcd_data('X');
        }
        else if (KB==13){
            lcd_data('Z');
        }
    }
}
//*********************************************************************
//            SWitches Vector Interrupt Service Routine
//*********************************************************************
 #pragma vector=PORT2_VECTOR
 __interrupt void keypadIRQ (void)
 {
    delay(debounceVal);
//---------------------------------------------------------------------
//            keypad ISR
//---------------------------------------------------------------------
    if(KeypadIRQIntPend & 0x02){    // if keypad has been pressed find value
//        flag++;
//        startTimerA0();
        preKB = KB;
        KB = 75;
        KeypadPortOUT = 0x0E;
        if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 13;
        else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 14;
        else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 0;
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

        if (preKB == KB && flag==0){
            flag = 1;
        }
        else if(preKB == KB && flag==1){
            flag = 2;
        }
        else{
            flag = 0;
        }
        if (i==17){
            lcd_new_line;
        }
        printChar();
        delay(15000);   // For keypad debounce
        KeypadPortOUT &= ~0x0F;  // Reset Row1-4
        KeypadIRQIntPend &= ~BIT1; // Reset Flag
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
