#include  "../header/api.h"     // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"
#include <string.h>

int LEDarray[10] = {128, 64, 32, 16, 8, 4, 23, 13, 40, 0};
int *ledptr;
//-------------------------------------------------------------
//             Idiom Recorder
//-------------------------------------------------------------
void IdiomRecorder(){
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    StopAllTimers();
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
    char merge1[38], merge2[38];
    char strMerge[76];
    int len1=0, len2=0, sent1len=0, sent2len=0;
    char *ptr1, *ptr2, *ptr_merge, *print;
    char *end1, *end2;
    char *space_pos;
    int lcdptr = 0;
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    StopAllTimers();

    while(state==state2){
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
        DMACTL0 = DMA0TSEL_0;  // No trigger source (manual transfer)

        while (ptr1 < end1 || ptr2 < end2) {
            // Find the next word in merge1
            if (ptr1 < end1) {
                space_pos = strchr(ptr1, ' ');
                if (space_pos) {
                    len1 = space_pos - ptr1;
                } else { // space not found (last word in the sentence)
                    len1 = end1 - ptr1;
                }
                DMA0_STATE2(ptr1, len1, ptr_merge);  // Use DMA to transfer the word

                ptr1 += len1;
                ptr_merge += len1;

                if (*ptr1 == ' ') {
                    ptr1++;
                    *ptr_merge = ' ';
                    ptr_merge++;
                } else if (ptr1 < end1 || ptr2 < end2) {
                    *ptr_merge = ' ';
                    ptr_merge++;
                }
            }

            // Find the next word in merge2
            if (ptr2 < end2) {
                space_pos = strchr(ptr2, ' ');
                if (space_pos) {
                    len2 = space_pos - ptr2;
                } else {
                    len2 = end2 - ptr2;
                }
                DMA1_STATE2(ptr2, len2, ptr_merge);  // Use DMA to transfer the word

                ptr2 += len2;
                ptr_merge += len2;

                if (*ptr2 == ' ') {
                    ptr2++;
                    *ptr_merge = ' ';
                    ptr_merge++;
                } else if (ptr1 < end1 || ptr2 < end2) {
                    *ptr_merge = ' ';
                    ptr_merge++;
                }
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
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
    }
}

//-------------------------------------------------------------
//             DMALEDS
//-------------------------------------------------------------
void DMALEDS(){
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    // StopAllTimers();    
    ledptr = LEDarray;
    while(state==state3){
        DMA0_STATE3();
        startTimerB();            
        __bis_SR_register(LPM0_bits + GIE);  // Enter low power mode 0
    }
    LEDsArrPort &= ~0xFF;  // Turn off all LEDs
    TB0CCTL0 &= ~CCIE;  // Disable TimerB0 interrupt
    DMA0CTL &= ~DMAEN;  // Disable DMA0
    ledptr = LEDarray;  // Reset ledptr
    __bis_SR_register(GIE);  // Exit low power mode 0
}
//-------------------------------------------------------------
//             Mirror
//-------------------------------------------------------------
void Mirror(){
    int len, l;
    char strMirror[160];
    char *ptrOriginal, *ptr_mirror, *printmirror;
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    StopAllTimers();
    ptrOriginal = strBM + 156;
    ptr_mirror = strMirror;
    DMACTL0 = DMA0TSEL_0;  // No trigger source (manual transfer)
    while(state==state4){
        disable_interrupts();  // Disable GIE
        lcd_clear();
        lcd_home();
        len = strlen(strBM);  // Get the length of the string
        while(len >= 0){
            DMA0_STATE2(ptrOriginal, 1, ptr_mirror);
            len--; 
        }
        // printing function
        printmirror = strMirror;
        for(l=0;l<32;l++){
            lcd_data(*printmirror);
            printmirror++;
            if(l==15){
                lcd_new_line;
            }
        }
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
    }
}
//-------------------------------------------------------------
//             count string length
//-------------------------------------------------------------
int strlength(char *str){
    int u = 0;
    while(str[u] != '\0'){
        u++;
    }
    return u;
}
