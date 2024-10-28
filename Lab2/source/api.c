#include  "../header/api.h"     // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"

// Global Variables
unsigned int REdge1, REdge2;
unsigned int savedMinutes = 0;
unsigned int savedSeconds = 0;
unsigned char SWstate;

// Last displayed frequency
unsigned int last_displayed_freq = 0;


//-------------------------------------------------------------
//      Print B to LCD skipping modulus31
//-------------------------------------------------------------
void Bedilugim2(){
    int pointer = 0;
    lcd_home();
    while(state==state4){
        if (pointer==16){
            lcd_new_line;
        }
        else if(pointer==32){
            lcd_home();
            pointer = 0;
        }
        lcd_puts("B");
        startTimerA0RT();
        startTimerA0RT();
        lcd_cursor_left();
        lcd_puts(" ");
        cursor_off;
        pointer++;
    }
}
//-------------------------------------------------------------
//      Frequency Measurement fin=SMCLK/#(SM cycles between 2 Rising edges)
//-------------------------------------------------------------
void FreqMeas(){
    WDTCTL = WDTPW + WDTHOLD;
    float N_SMCLK, freq, error = 0, SMCLK_FREQ = 1048576; // 2^20
    const char* blank = "     \0";
    unsigned int real_freq;
    char strFreq[6] = {'\0'};

    freq_template_LCD(); // Write template of Frequency
    last_displayed_freq = 0 ;
    TA1CTL |= TASSEL_2 + MC_2 + TACLR; // start Timer

    while (state == state1) {
        disable_interrupts();
        strFreq[6] = '\0'; // Reset strFreq
        REdge2 = REdge1 = 0;
        TA1CCTL2 |= CCIE; // enable interrupt
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0

        if (REdge1 == 0 && REdge2 == 0) // first time
            continue;

        error = 0.99200; // error value (need to calculate)
        N_SMCLK = (REdge2 - REdge1) * error;
        freq = SMCLK_FREQ / N_SMCLK; // Calculate Frequency
        real_freq = (unsigned int)freq; // int casting

        if (last_displayed_freq == 0) {
            // First time, always print
            sprintf(strFreq, "%d", real_freq);
            lcd_home();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_puts(blank);
            lcd_home();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_puts(strFreq);
            last_displayed_freq = real_freq;
        } else {
            // Calculate the percentage difference
            float percentage_diff = ((float)abs((int)(real_freq - last_displayed_freq)) / last_displayed_freq) * 100;
            if (((real_freq <= 5000 && percentage_diff > 0.46) ||
                 (real_freq > 5000 && real_freq <= 10000 && percentage_diff > 0.9) ||
                 (real_freq > 17000 && real_freq <= 20000 && percentage_diff > 8) ||
                 (real_freq > 10000 && real_freq <= 17000 && percentage_diff > 2.3))) {
                sprintf(strFreq, "%d", real_freq);
                lcd_home();
                lcd_cursor_right();
                lcd_cursor_right();
                lcd_cursor_right();
                lcd_cursor_right();
                lcd_puts(blank);
                lcd_home();
                lcd_cursor_right();
                lcd_cursor_right();
                lcd_cursor_right();
                lcd_cursor_right();
                lcd_puts(strFreq);
                last_displayed_freq = real_freq;
            }
        }

        cursor_off;
        DelayMs(500);
        enable_interrupts();
    }
    TA1CTL = MC_0+TACLR; // Stop Timer
}

//-------------------------------------------------------------
//                         StopWatch
//-------------------------------------------------------------
void StopWatch(){
      char const * initial ="00:00";
      char currentmin[3] = {'\0'};
      char currentsec[3] = {'\0'};
      int minutes=0, seconds=0;
      WDTCTL = WDTPW + WDTHOLD;

      if(savedMinutes != 0 || savedSeconds != 0){ // If saved time is not 0
          // Load the saved time
          minutes = savedMinutes;
          seconds = savedSeconds;
          sprintf(currentmin, "%d", minutes);
          sprintf(currentsec, "%d", seconds);
          lcd_home();
          if (minutes < 10){
              lcd_puts("0");
          }
          lcd_puts(currentmin);
          lcd_puts(":");
          if (seconds < 10){
              lcd_puts("0");
          }
          lcd_puts(currentsec);
      }
      else{
          // Print initial time
          lcd_home();
          lcd_puts(initial);
      }
      // if the switch is up and the state changes
      SWstate = readSWs();  
      if (SWstate != 0x00){
          __bis_SR_register(LPM0_bits + GIE);
      }
      // after init CPU goes to sleep, then wakes up if the switch is pressed
      while(1){
          if (state == state2){
              SWstate = readSWs();
              if(SWstate == 0x00){
                  // Start Timer
                  startTimerA0();
                  startTimerA0();
                  // add 1 second
                  seconds++;
                  if (seconds >= 60) {
                      seconds = 0;
                      minutes++;
                  }
                  sprintf(currentmin, "%d", minutes);
                  sprintf(currentsec, "%d", seconds);
                  // Print the time
                  lcd_home();
                  if (minutes < 10){
                      lcd_puts("0");
                  }
                  lcd_puts(currentmin);
                  lcd_puts(":");
                  if (seconds < 10){
                      lcd_puts("0");
                  }
                  lcd_puts(currentsec);
                  // Save the current time
                  savedMinutes = minutes;
                  savedSeconds = seconds;

                  cursor_off;
          }else{
              __bis_SR_register(LPM0_bits + GIE);
          }
        }else{
            TA0CTL = MC_0;
            break;
        }
    }
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
//-------------------------------------------------------------
//              StartTimer For Bedilugim2
//-------------------------------------------------------------
void startTimerA0RT(){
    TACCR0 = 0xE666;  // Timer Cycles - max
    TA0CTL = TASSEL_2 + MC_3 + ID_3;  //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}
//-------------------------------------------------------------
//   Tone Generator     buzzer_potentiometer[0-3.3V] -> [1-2.5KHz]
//-------------------------------------------------------------
void GenTones(){
    TA1CTL = TASSEL_2 + MC_1;                  // SMCLK, upmode
    while(state == state3){
        ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
        ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt

        unsigned int Nadc = ADC10MEM;  // 0-1023 , 1024 voltage levels
        float coeff = 1.465;  // coeff = 1500 / 1023; 
        float f_out = coeff * Nadc + 1000;  // Choose Linear Mapping

        float SMCLK_FREQ = 1048576; // SMCLK freq 2^20
        unsigned int period_to_pwm = SMCLK_FREQ/f_out;

        TA1CCR0 = period_to_pwm;            // PWM Period
        TA1CCR1 = (int) period_to_pwm/2;    // 50% Duty Cycle
    }
    TA1CTL = MC_0 ; // Stop Timer
}
