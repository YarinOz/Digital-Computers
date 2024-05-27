#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;

int counter1 = 0;
int timer1 = 0;
bool flag1 = false; // false for first state, true for second state

void main(void){
  
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
//   zeroLEDs(); maybe not needed
  sysConfig();
  
  while(1){
	switch(state){
		 
	  case state1:  // enable pb0 interrupt to switch to -- or ++
		disable_interrupts();
		if timer1 != 20{
			LEDsArrPort = counter1 & 0xFF; // show counter1 on 8bit LEDs
			if flag1 { 
				counter1++ ;
			} else {
				counter1-- ;
			}
			timer1++ ;
			for (i=0; i<8; i++;){
				delay(LEDs_SHOW_RATE);	// delay of 0.5 [s]
			}	
		}
		enable_interrupts();
		break;
		 
	  case state2:
		disable_interrupts();
		incLEDs(-1);
		delay(LEDs_SHOW_RATE);		// delay of 62.5 [ms]
		enable_interrupts();
		break;

	  case state3:
		disable_interrupts();
		incLEDs(-1);
		delay(LEDs_SHOW_RATE);		// delay of 62.5 [ms]
		enable_interrupts();
		break;
		
	}
  }
}
  
  
  
  
  
  