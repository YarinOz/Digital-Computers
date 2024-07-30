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
	case state1: // Analog Joystick manual Stepper motor control
        IE2 |= UCA0RXIE;                      // Enable USCI_A0 RX interrupt
        __bis_SR_register(LPM0_bits + GIE);   // Enter LPM0
	    break;
	case state2: // Analog Joystick Paint
	    // Paint with Joystick in 3 modes (switched by analog click button):
		// 1. write, 2. erase, 3. neutral
	    break;

	case state3: ; // Stepper motor calibration
	    // calibrate stepper motor to 0 degrees
		break;
		
	case state4: // Change Delay Time in ms
	    change_delay_time();
		break;

    case state5: // Measure Potentiometer 3-digit value
        measure_pot();
        break;

    case state6: // Clear Counts and LCD
        clear_counters();
        break;

    case state7: // Print Menu to PC
        IE2 |= UCA0TXIE;                      // Enable USCI_A0 TX interrupt
        __bis_SR_register(LPM0_bits + GIE);   // Enter LPM0
        break;
		
	}
  }
}

  
  
  
  
  
  
