#include  "../header/api.h"     // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"

// Global Variables
unsigned char SWstate;
char strMerge[102];
char LEDarray[9] = {128, 64, 32, 16, 8, 4, 23, 13, 40};

//-------------------------------------------------------------
//             Idiom Recorder
//-------------------------------------------------------------
void IdiomRecorder(char *idiom_recorder){
    lcd_clear();
    lcd_home();
    lcd_puts("Enter Idiom: ");
    lcd_goto(0x40);
    // hal func to get the idiom from keypad and put in idiom_recorder and display on LCD


}
//-------------------------------------------------------------
//             Merge
//-------------------------------------------------------------
void Merge(){
    char merge1[51], merge2[51];
    lcd_clear();
    lcd_home();
    lcd_puts("Merge: ");
    lcd_goto(0x40);
    // hal func to get 2 numbers
    // maybe need the DMA to do the transfer of the data_matrix to the merge1 and merge2
    // take 2 numbers and add to merge1=data_matrix[number1] and merge2=data_matrix[number2]
    // print strMerge to LCD
}
//-------------------------------------------------------------
//             DMALEDS
//-------------------------------------------------------------
void DMALEDS(){
    lcd_clear();
    // print to leds the LEDarray periodically from left to right with 0.5sec delay using TimerB trigger

}
//-------------------------------------------------------------
//              StartTimer For StopWatch
//-------------------------------------------------------------
void startTimerA0(){
    TACCR0 = 0xFFFF;  // Timer Cycles - max
    TA0CTL = TASSEL_2 + MC_3 + ID_3;  //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8
    // ACLK doesn't work on our msp, so we have to use smclk and divide the freq to get to 1 sec.
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

