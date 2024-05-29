#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer

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
	(*flag) = 1-(*flag);
	for (int i=0; i<20; i++){
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
        if (last_count&0x01){
        print2LEDs(last_count);
        delay(LEDs_SHOW_RATE);
        }
	for (int i=0; i<13; i++){
          if (last_count & 0x80){ // if reached 256
              last_count=1;
              print2LEDs(last_count);
              delay(LEDs_SHOW_RATE);
          }
          last_count = (last_count*2);
          print2LEDs(last_count);
          delay(LEDs_SHOW_RATE);	// delay of 0.5 [s]
	}
	state = state0;
	return last_count;
}
//--------------------------------------------------------------------
// PWM function DC-75% freq-4KHz
//--------------------------------------------------------------------
// void genPWN(){
//  while(state==state3){
    
  

