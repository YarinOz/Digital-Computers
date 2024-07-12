#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
#include  <stdio.h>


enum FSMstate state;
enum SYSmode lpm_mode;


void main(void){
  
  state = state7;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();     // Configure GPIO, Stop Timers, Init LCD


  while(1){
	switch(state){
	case state0: //idle
	    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
	    break;
	case state1: // Blink RGB LED
	    RGBlink();
	    RGBArrPortOut = 0;
	    break;
	case state2: // Count up onto LCD
	    count_up_LCD();
	    break;

	case state3: ; // Tone Buzzer
	    CircBuzzer();
		break;
		
	case state4: // Change Delay Time in ms
	    XDelay();
		break;

    case state5: // Measure Potentiometer 3-digit value
        PotentiometerMeas();
        break;

    case state6: // Clear Counts and LCD
        clear_counters();
        break;

    case state7: // Print Menu to PC
        IE2 |= UCA0TXIE;                      // Enable USCI_A0 TX interrupt
        __bis_SR_register(LPM0_bits + GIE);   // Enter LPM0
        break;

    case state8:
        state=state0;
        break;

//    case state9: // RT
//		//RT function
//        break;
	}
  }
}

  
  
  
  
  
  
