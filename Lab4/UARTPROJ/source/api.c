#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"


int colors[] = {0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111};
unsigned int count_up = 0;
char count_up_str[5];
unsigned int* count_up_address = &count_up;
unsigned int count_down = 65535;
char count_down_str[5];
unsigned int* count_down_address = &count_down;
const unsigned int resolution = 1024;
const float v_ref = 3.41;
float pot_meas;
char afterDigit_str[4];
char beforeDigit_str[1];

//-------------------------------------------------------------
//                1. Blink RGB
//-------------------------------------------------------------
void blinkRGB(){
    lcd_clear();
    unsigned int i = 0;
    while(state==state1){
        print2RGB(colors[i]);
        timer_call_counter();
        i = (i + 1) % 9;
    }
}
//-------------------------------------------------------------
//                2. Count up to 65535
//------------------------------------------------------------
void count_up_LCD(){
    while(state==state2){
        lcd_clear();
        lcd_home();
        lcd_puts("Count Up: ");
        lcd_new_line;
        int2str(count_up_str, *count_up_address);
        lcd_puts(count_up_str);
        timer_call_counter();

        *count_up_address = (*count_up_address + 1) % 65536;
    }
}
//-------------------------------------------------------------
//                3. Count Down from 65535 to 0
//------------------------------------------------------------
void count_down_LCD(){
    while(state==state3){
        lcd_clear();
        lcd_home();
        lcd_puts("Count Down: ");
        lcd_new_line;
        int2str(count_down_str, *count_down_address);
        lcd_puts(count_down_str);
        timer_call_counter();
        *count_down_address = (*count_down_address - 1);
        if (*count_down_address == 0) *count_down_address = 65535;
    }
}
//-------------------------------------------------------------
//                4. Change Delay Time [ms]
//------------------------------------------------------------
void change_delay_time(){
    delay_time = atoi(string1);  // Get delay time from user
    state = state8;
}
//-------------------------------------------------------------
//                5. Measure Pot voltage [v]
//------------------------------------------------------------
void measure_pot(){
    ADC_config();
    ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
    ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt

    pot_meas = ( v_ref * ADC10MEM ) / resolution;

    int ipart = (int)pot_meas;

    float fpart = pot_meas - (float)ipart;

    int afterdigit = (int) (fpart * 1000);

    int2str(beforeDigit_str, ipart);

    int2str(afterDigit_str, afterdigit);

    lcd_clear();
    lcd_home();
    lcd_puts("Pot Measurement:");
    lcd_new_line;
    if (ipart == 0) lcd_puts("0");
    else lcd_puts(beforeDigit_str);
    lcd_puts(".");
    lcd_puts(afterDigit_str);
    lcd_puts(" [v]");
    timer_call_counter();

}

//-------------------------------------------------------------
//                6. Clear LCD and init counters
//------------------------------------------------------------
void clear_counters(){
    disable_interrupts();
    lcd_clear();
    lcd_home();
    count_up = 0;
    count_down = 65535;
    enable_interrupts();
    state = state8;
}


