#include  "../header/api.h"     // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"
#include <string.h>

int colors[] = {0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111};
unsigned int count_up = 0;
char count_up_str[5];
unsigned int* count_up_address = &count_up;
const unsigned int resolution = 1024;
const float v_ref = 3.40;  // Reference voltage for ADC (need to be measured)
float potentiometer;
char leftSTR[4];
char rightSTR[1];

//-------------------------------------------------------------
//                1. Blink RGB
//-------------------------------------------------------------
void RGBlink(){
    lcd_clear();
    unsigned int i = 0;
    while(state==state1){
        CH2RGB(colors[i]);
        XmsDelay();
        i = (i + 1) % 9;
    }
}
//-------------------------------------------------------------
//                2. Count up to 65535
//------------------------------------------------------------
void LCDcount(){
    while(state==state2){
        lcd_clear();
        lcd_home();
        lcd_puts("Count Up: ");
        lcd_new_line;
        int2str(count_up_str, *count_up_address);
        lcd_puts(count_up_str);
        XmsDelay();
        *count_up_address = (*count_up_address + 1) % 65536;
    }
}
//-------------------------------------------------------------
//                3. Count Down from 65535 to 0
//------------------------------------------------------------
void CircBuzzer(){
    while(state==state3){
        lcd_clear();
        lcd_home();
        // buzzer function 
        // tone the buzzer using {1, 1.25, 1.5, 1.75, 2, 2.25, 2.5} kHz periodically with Xdelay
        
    }
}
//-------------------------------------------------------------
//                4. Change Delay Time [ms]
//------------------------------------------------------------
void GetDelay(){
    Xdelay = atoi(DelayString);  // Get delay time from user
    state = state8;
}
//-------------------------------------------------------------
//                5. Measure Pot voltage [v]
//------------------------------------------------------------
void Pot2LCD(){
    ADC_config();
    ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
    ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt

    potentiometer = (v_ref*ADC10MEM)/resolution; // (Vref * ADC10MEM) / (2^10)

    int ipart = (int)potentiometer;  // int part
    float fpart = potentiometer - (float)ipart; // float part

    int right = (int) (fpart * 1000);

    int2str(leftSTR, ipart);
    int2str(rightSTR, right);

    lcd_clear();
    lcd_home();
    lcd_puts("Pot Measurement:");
    lcd_new_line;
    if (ipart == 0) lcd_puts("0");
    else lcd_puts(leftSTR);
    lcd_puts(".");
    lcd_puts(rightSTR);
    lcd_puts(" [v]");
    XmsDelay();
}
//-------------------------------------------------------------
//                6. Clear LCD and init counters
//------------------------------------------------------------
void ClearAll(){
    disable_interrupts();
    lcd_clear();
    lcd_home();
    count_up = 0;
    enable_interrupts();
    state = state8;
}
//-------------------------------------------------------------
