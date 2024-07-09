#include  "../header/api.h"     // private library - API layer
#include  "../header/app.h"     // private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;

unsigned int EndOfRecord = 0;
unsigned int i = 0;
unsigned int KB;

void main(void){
  
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();          // configure system

  while(1){
    switch(state){
      case state0: //idle state
        IE2 |= UCA0RXIE;                     // Enable USCI_A0 RX interrupt
	      __bis_SR_register(LPM0_bits + GIE);  // Enter LPM0 w/ interrupt
        break;

      case state1: // RGB blink
        RGBlink();          //
        RGBArrPortOut &= ~0x07; // Turn off all LEDs
        break;

      case state2: // LCD count
        lcd_clear();          // Clear LCD screen
        lcd_home();
        LCDcount();          //
        break;

      case state3: ; // Circular tone buzzer
        CircBuzzer();         
        break;
                
      case state4: // Get Delay
        GetDelay();          //
        break;

      case state5: // Potentiometer 3-Digits to LCD
        Pot2LCD();          //
        break;

      case state6: ; // Clear LCD screen
        ClearAll();          //
        break;
                
      case state7: // Show Menu
        ShowMenu();          //
        break;

      case state8: // Sleep Mode
        state = state0;
        break;

    }
  }
}
