#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
#include  <stdio.h>

// Final Procject in DCS lab by Yarin Oziel & Omer Kariti
// 2024 all rights reserved

enum FSMstate state;
enum Stepperstate stateStepp;
enum SYSmode lpm_mode;


void main(void){
  
  state = state0;  // start in idle state on RESET
  stateStepp = stateDefault;
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();     // Configure GPIO, Init ADC


  while(1){
	switch(state){

	case state0: //   StepperUsingJoyStick
	    IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
	    switch(stateStepp){
	    case stateAutoRotate: // start rotate
	        while(rotateIFG){START_TIMERA0(600); curr_counter++; Stepper_clockwise(); }
	        break;

        case stateStopRotate: // stop rotate
            break;

        case stateJSRotate: // rotate using joystick
            counter=513;   // value from calibration
            StepperUsingJoyStick();
            break;

        case stateDefault:
            __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ until Byte in RXed
            break;
	    }
	    break;

	case state1: // Paint with joystick
	    JoyStickIntEN |= BIT5;
	    while (state == state1){JoyStick_Painter();}
        JoyStickIntEN &= ~BIT5;
	    break;

	case state2: // Calibrate stepper motor
        IE2 |= UCA0RXIE;     // Enable USCI_A0 RX interrupt

        switch(stateStepp){ // Stepper motor calibration
        case stateDefault:
            JoyStickIntEN |= BIT5;
            __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ int until Byte RXed
            break;

        case stateAutoRotate: // start rotate
            counter = 0;
            while(rotateIFG) {START_TIMERA0(600);Stepper_clockwise(); counter++; }
            break;

        case stateStopRotate: // stop rotate, return counter and phi
            JoyStickIntEN &= ~BIT5;
            calibrate();
            break;
        }
	    break;

	case state3:  //Script mode
        IE2 |= UCA0RXIE;     // Enable USCI_A0 RX interrupt
	    while ( state == state3){
	        ScriptFunc();
	    }
		break;
		
	case state4: // optional

		break;
	}
  }
}
