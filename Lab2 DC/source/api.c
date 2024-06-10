#include  "../header/api.h"     // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"

// Global Variables
unsigned int REdge1, REdge2;
float previous_freq = 0.0;
unsigned char SWstate;
unsigned int savedMinutes = 0;
unsigned int savedSeconds = 0;
#define ADC_NUMBER_CAPTURES 100
unsigned int adcCaptureValues[ADC_NUMBER_CAPTURES];
unsigned int adcCapturePointer;



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
    TA1CTL |= TASSEL_2 + MC_2 + TACLR; // start Timer

    while (state == state1) {
        disable_interrupts();
        strFreq[6] = '\0'; // Reset strFreq
        REdge2 = REdge1 = 0;
        TA1CCTL2 |= CCIE; // enable interrupt
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0

        if (REdge1 == 0 && REdge2 == 0) // first time
            continue;

        error = 0.99450; // error value (need to calculate)
        N_SMCLK = (REdge2 - REdge1) * error;
        freq = SMCLK_FREQ / N_SMCLK; // Calculate Frequency
        real_freq = (unsigned int)freq; // int casting

        if (previous_freq == 0 || (fabs((freq - previous_freq) / previous_freq) > 0.03)) {
            // If frequency changed by more than 0.03%
            previous_freq = freq;

            sprintf(strFreq, "%d", real_freq);
            // Wipe frequency template
            lcd_home();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_puts(blank);
            // Write Frequency
            lcd_home();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_cursor_right();
            lcd_puts(strFreq);
        }

        cursor_off;
        DelayMs(500);
        enable_interrupts();
    }

    TA1CTL = MC_0; // Stop Timer
}

//-------------------------------------------------------------
//                         StopWatch
//-------------------------------------------------------------
void StopWatch(){
      char const * initial ="00:00";
      char current[6] = {'\0'};
      if(savedMinutes != 0 || savedSeconds != 0){ // If saved time is not 0
          // Load the saved time
          minutes = savedMinutes;
          seconds = savedSeconds;
          // Print the saved time
          lcd_home();
          lcd_puts(current);
      }
      else{
          // Print initial time
          lcd_home();
          lcd_puts(initial);
      }
      __bis_SR_register(LPM0_bits + GIE); // Enter LPM0
      while(1){
          if(SWstate == 0x01){
              // Start Timer
              startTimerA0();
              // add 1 second
              seconds++;
              if (seconds >= 60) {
                  seconds = 0;
                  minutes++;
              }
              sprintf(current, "%d:%d", minutes, seconds);
              // Print the time
              lcd_home();
              lcd_puts(current);
              // Save the current time
              savedMinutes = minutes;
              savedSeconds = seconds;
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
//              Tone Generator
//-------------------------------------------------------------
void GenTones(){
    TA1CTL = TASSEL_2 + MC_1;                  // SMCLK, upmode
    while(state == state3){
        ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
        ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt

        unsigned int adc_conv = ADC10MEM;
        float coeff = 1.956;  // coeff = 2000 / 1023;
        float f_out = coeff * adc_conv + 1000;  // Choose Linear Mapping

        float SMCLK_FREQ = 1048576; // SMCLK freq 2^20
        unsigned int period_to_pwm = SMCLK_FREQ/f_out;

        TA1CCR0 = period_to_pwm;
        TA1CCR1 = (int) period_to_pwm/2;
    }
    TA1CTL = MC_0 ; // Stop Timer
}
//-------------------------------------------------------------
//              Signal Shape
//-------------------------------------------------------------
// void Signal_shape(){
//         while(state == state4){
//             ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
//             __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
//             ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt
//             char * strLCD;
//             if(adcCapturePointer < ADC_NUMBER_CAPTURES) adcCaptureValues[adcCapturePointer++] = ADC10MEM;
//             else {
//                 adcCapturePointer = 0;
//                 unsigned int i;
//                 unsigned int low_val_counter;
//                 unsigned int high_val_counter;
//                 unsigned int tri_counter;
//                 low_val_counter = 0;
//                 high_val_counter = 0;
//                 tri_counter = 0;
//                 for (i=1;i<ADC_NUMBER_CAPTURES-1;i++){
//                     //pwm
//                     if(adcCaptureValues[i] <= 5) low_val_counter ++;
//                     else if (adcCaptureValues[i] >= 800) high_val_counter ++;
//                     // tri
//                     if(abs(2*adcCaptureValues[i]-adcCaptureValues[i-1]-adcCaptureValues[i+1])<3) tri_counter++;


//                 }
//                 if (low_val_counter > 5 && high_val_counter > 5) strLCD = "pwm";
//                 else if (tri_counter > 16) strLCD = "tri";
//                 else strLCD = "sin";
//                 DelayMs(500);
//                 write_signal_shape_tmp_LCD();
//                 lcd_puts(strLCD);
//             }

//         }
// }
