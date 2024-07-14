#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"


int colors[] = {0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111};
unsigned int count_up = 0;
char count_up_str[5];
unsigned int* count_up_address = &count_up;
const unsigned int resolution = 1024;
const float v_ref = 3.40;
float pot_meas;
char afterDigit_str[4];
char beforeDigit_str[1];
int active = 0;

//-------------------------------------------------------------
//                1. Blink RGB
//-------------------------------------------------------------
void RGBlink(){
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
//                3. Create a periodic buzzer with changing frequencies
//------------------------------------------------------------
void CircBuzzer() {
    TIMERconfig();
    unsigned int tones[7] = {1001, 801, 667, 571, 500, 444, 400}; // Array of tone frequencies for the buzzer
    unsigned int buz_pos = 0; // Initialize buzzer position to the first tone
    TIMER1_A1_config(); // Enable TimerA1 in up mode to start the buzzer

    while (state == state3) { // Loop while the system is in state3
        TA1CCR0 = tones[buz_pos]; // Set the period of the timer to the current tone frequency
        TA1CCR2 = TA1CCR0 >> 1; // Set the duty cycle to 50% (TA1CCR2 is half of TA1CCR0)

        timer_call_counter(); // Call a delay function to wait for the duration specified by x

        if (buz_pos == 6) // If the buzzer position is at the last tone in the array
            buz_pos = 0; // Reset to the first tone
        else
            buz_pos++; // Move to the next tone
    }
    disable_TimerA1(); // Disable TimerA1 when exiting state3
}
//-------------------------------------------------------------
//                4. Change Delay Time [ms]
//------------------------------------------------------------
void XDelay(){
    delay_time = atoi(string1);  // Get delay time from user
    state = state0;
}
//-------------------------------------------------------------
//                5. Measure Pot voltage [v]
//------------------------------------------------------------
void PotentiometerMeas(){
    ADC_config();
    ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
    ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt

    pot_meas = ( v_ref * ADC10MEM ) / resolution; // vref*[0-1]

    int ipart = (int)pot_meas;

    float fpart = pot_meas - (float)ipart;

    int afterdigit = (int) (fpart * 1000);
    // example 2.86v-> ipart=2, fpart=0.86, afterdigit=860

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
    enable_interrupts();
    state = state0;
}
//-------------------------------------------------------------
//                 RT lab
//------------------------------------------------------------
void SendLove(){
    while(state==state9 && active!=0){
        // if in current state and active (PB1/PB2) follow with function
        printf("RT debug in function");
    }
    state=state0;
}
