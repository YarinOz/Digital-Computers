#include  "../header/api.h"     // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"
#include <string.h>

// Global Variables
char strMerge[102];
char LEDarray[9] = {128, 64, 32, 16, 8, 4, 23, 13, 40};

//-------------------------------------------------------------
//             Idiom Recorder
//-------------------------------------------------------------
void IdiomRecorder(){
    int len,m;
    while(state==state1){
        disable_interrupts();  // Disable GIE
        // StopAllTimers();    // Stop All Timers(A,DMA)
        // Check If recording was finished by user or by reaching notes limit
        if(EndOfRecord || i == 32){  // 32 prints
            lcd_clear();
            lcd_home();
            lcd_puts("Finished Rec.");
            lcd_new_line;
            idiom_recorder[i] = '\0'; // End the string
            startTimerA0(); // Start Timer for StopWatch
            startTimerA0(); // Start Timer for StopWatch
            i = 0; // Reset i
            EndOfRecord = 0;
            state = state0;
            // uncomment to print the idiom_recorder
            // lcd_clear();
            // lcd_home();
            // len=strlen(idiom_recorder);
            // for (m=0;m<len;m++){
            //     lcd_data(idiom_recorder[m]);
            //     if (m==15){
            //         lcd_new_line;
            //     }
            // }
            // __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
        }
        enable_interrupts();  // Enable GIE
    }
    // if change state in middle of recording
    idiom_recorder[i] = '\0'; // End the string
    i = 0; // Reset i
    EndOfRecord = 0;
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}
//-------------------------------------------------------------
//             Merge
//-------------------------------------------------------------

void Merge() {
    char merge1[51], merge2[51];
    char temp1[51], temp2[51];
    int len1, len2, sent1len, sent2len;
    char *ptr1, *ptr2, *ptr_merge, *print;
    char *end1, *end2;
    char *space_pos;
    int lcdptr = 0;

    lcd_home();
    lcd_puts("First index: ");
    lcd_goto(0x40);
    delay(1500); // debounce
    delay(1500);
    delay(1500);
    delay(1500);
    __bis_SR_register(LPM0_bits + GIE);
    if(KB==16)KB=0;
    strcpy(merge1, data_matrix[KB]);
    lcd_clear();
    lcd_home();
    lcd_puts("Second index: ");
    lcd_goto(0x40);
    delay(1500); // debounce
    delay(1500);
    delay(1500);
    delay(1500);
    __bis_SR_register(LPM0_bits + GIE);
    if(KB==16)KB=0;
    strcpy(merge2, data_matrix[KB]);
    lcd_clear();
    lcd_home();

    ptr1 = merge1;
    ptr2 = merge2;
    ptr_merge = strMerge;
    sent1len = strlen(merge1);
    sent2len = strlen(merge2);
    end1 = merge1 + sent1len;
    end2 = merge2 + sent2len;
    strMerge[0] = '\0'; // init the string

    while (ptr1 < end1 || ptr2 < end2) {
        // Find the next word in merge1
        if (ptr1 < end1) {
            space_pos = strchr(ptr1, ' ');
            if (space_pos) {
                len1 = space_pos - ptr1 + 1;
            } else {
                len1 = end1 - ptr1;
                if (ptr1 + len1 < end1) {
                    len1 += 1; // Include space if it's not the last word
                }
            }

            // Use DMA to transfer the word
            DMA0SA = (unsigned int)ptr1;  // Source address for merge1
            DMA0DA = (unsigned int)ptr_merge;  // Destination address for strMerge
            DMA0SZ = len1;  // Block size
            DMA0CTL = DMAEN + DMASRCINCR_3 + DMADSTINCR_3 + DMADT_1;  // Enable DMA, source and destination increment, block transfer mode
            DMA0CTL |= DMAREQ;  // Manually trigger DMA transfer for merge1 word
            while (DMA0CTL & DMAEN);  // Wait for DMA transfer to complete

            ptr1 += len1;
            ptr_merge += len1;
        }

        // Find the next word in merge2
        if (ptr2 < end2) {
            space_pos = strchr(ptr2, ' ');
            if (space_pos) {
                len2 = space_pos - ptr2 + 1;
            } else {
                len2 = end2 - ptr2;
                if (ptr2 + len2 < end2) {
                    len2 += 1; // Include space if it's not the last word
                }
            }

            // Use DMA to transfer the word
            DMA1SA = (unsigned int)ptr2;  // Source address for merge2
            DMA1DA = (unsigned int)ptr_merge;  // Destination address for strMerge
            DMA1SZ = len2;  // Block size
            DMA1CTL = DMAEN + DMASRCINCR_3 + DMADSTINCR_3 + DMADT_1;  // Enable DMA, source and destination increment, block transfer mode
            DMA1CTL |= DMAREQ;  // Manually trigger DMA transfer for merge2 word
            while (DMA1CTL & DMAEN);  // Wait for DMA transfer to complete

            ptr2 += len2;
            ptr_merge += len2;

        }
    }

    // Remove trailing space
    if (ptr_merge > strMerge && *(ptr_merge - 1) == ' ') {
        *(ptr_merge - 1) = '\0';
    } else {
        *ptr_merge = '\0';
    }

    lcd_clear();
    lcd_home();
    print = strMerge;
    while(*print != '\0'){
        lcd_data(*print);
        print++;
        lcdptr++;
        if(lcdptr==16){
            lcd_new_line;
        }
        else if(lcdptr==32){
            lcd_home();
            lcdptr = 0;
            __bis_SR_register(LPM0_bits + GIE);
            lcd_clear();
        }
    }
    // lcd_puts(strMerge);
    __bis_SR_register(LPM0_bits + GIE);
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

