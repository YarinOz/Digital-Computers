#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#ifdef __MSP430FG4619__
#include "../header/bsp_msp430x4xx.h" //lab kit
#else
#include "../header/bsp_msp430x2xx.h" //pesronal kit
#endif
//-------------------------------------------------------------
//            Print SWs value onto LEDs
//-------------------------------------------------------------
void printSWs2LEDs(void){
	unsigned char ch;
	
	ch = readSWs();
	print2LEDs(ch);
}
//--------------------------------------------------------------------
//            Print array to LEDs array with rate of LEDs_SHOW_RATE
//--------------------------------------------------------------------            
void printArr2SWs(char Arr[], int size, unsigned int rate){
	unsigned int i;
	
	for(i=0; i<size; i++){
		print2LEDs(Arr[i]);
		delay(rate);
	}
}
//--------------------------------------------------------------------
// count up or down and save the last state
//--------------------------------------------------------------------
char bin_counter(int* flag, char last_count){
	int i;
	(*flag) = 1-(*flag);
	for (i=0; i<20; i++){
		if ((*flag)==1){
			last_count++;
		}
		else if ((*flag)==0){
			last_count--;
		}
        print2LEDs(last_count);
        delay(LEDs_SHOW_RATE);	// delay of 0.5 [s]
	}
	state = state0;
	return last_count;
}
//--------------------------------------------------------------------
// bedilugim, skip a single bit from right to left
//--------------------------------------------------------------------
char Bedilugim(char last_count){
    int left = 14;
    if (last_count&0x01){
        print2LEDs(last_count);
        left--;
        delay(LEDs_SHOW_RATE);
    }
	while(left){
          if (last_count & 0x80){ // if reached 256
              last_count=1;
              print2LEDs(last_count);
              left--;
              delay(LEDs_SHOW_RATE);
          }
          last_count = (last_count*2);
          print2LEDs(last_count);
          left--;
          delay(LEDs_SHOW_RATE);	// delay of 0.5 [s]
	}
	state = state0;
	return last_count;
}
//--------------------------------------------------------------------
// PWM function DC-75% freq-4KHz
//--------------------------------------------------------------------
void genPWM(void) {
    
    while (1) {
        if (state == state3) {
            // Output PWM signal on P2.7 // 3ms high, 1ms low
            PBsArrPortOut |= 0x80;
            delay(0x11);
            PBsArrPortOut &= ~0x80;
            delay(0x03);
        } else {
            // Stop PWM and reset state
            PBsArrPortOut &= ~0x80;
            // state = state0;
            break; // Exit the loop
        }
    }
}
//--------------------------------------------------------------------
// PWM function DC-0-100% freq-2KHz
//--------------------------------------------------------------------
void pwm(int Nfreq) {
    unsigned char SWstate;
    while (1) {
        SWstate=readSWs();          // read the switches state
        if (state == state4) {
            switch(SWstate){
                case 0x13:
                    PBsArrPortOut &= ~0x80;
                    break;
                case 0x23:
                    PBsArrPortOut |= 0x80;
                    delay(0x0B);
                    PBsArrPortOut &= ~0x80;
                    delay(0x22);
                    break;
                case 0x33:
                    PBsArrPortOut |= 0x80;
                    delay(0x18);
                    PBsArrPortOut &= ~0x80;
                    delay(0x15);
                    break;
                case 0x43:
                    PBsArrPortOut |= 0x80;
                    delay(0x28);
                    PBsArrPortOut &= ~0x80;
                    delay(0x08);
                    break;
                case 0x53:
                    PBsArrPortOut |= 0x80;
                    break;
                default:
                    PBsArrPortOut &= ~0x80;
                    break;
            }
        } else {
            // Stop PWM and reset state
            PBsArrPortOut &= ~0x80;
            // state = state0;
            break; // Exit the loop
        }
    }
}


