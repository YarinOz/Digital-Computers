#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;

char counter1 = 0;
char index2 = 1;
int flag1 = 0;
int* flag1ptr = &flag1;

void main(void){
  
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  
  while(1){
	switch(state){

	  case state0:
	  	print2LEDs(0);
		enterLPM(lpm_mode); 
		break;

	  case state1:  // enable pb0 interrupt to switch to -- or ++
		disable_interrupts();
		delay(LEDs_SHOW_RATE);	// delay of 62.5 [ms]
		counter1 = bin_counter(flag1ptr, counter1); // count up or down and save the last state
		enable_interrupts();
		break;
		 
	  case state2:
		disable_interrupts();
		delay(LEDs_SHOW_RATE);		// delay of 62.5 [ms]
                index2 = Bedilugim(index2);
		enable_interrupts();
		break;

	  case state3:
		delay(LEDs_SHOW_RATE);		// delay of 62.5 [ms]
		PWM();
		break;
		
	  case state4: // lab case RT
	  	disable_interrupts();
		delay(LEDs_SHOW_RATE);		// delay of 62.5 [ms]
		enable_interrupts();			
		break;
	}
  }
}
  
  
  
  
  
  