#include  "../header/api.h"     // private library - API layer
#include  "../header/app.h"     // private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){
  
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  lcd_init();
  lcd_clear();

  while(1){
    switch(state){
      case state0: //idle state
          lcd_home();
          lcd_clear();
          enterLPM(mode0);
          break;

      case state1: //PB0
        enable_interrupts();  // Enable to Cut the state
        FreqMeas();  // Frequency Measurements
        break;

      case state2: //PB1
        enable_interrupts();  // Enable to Cut the state
        lcd_clear();          // Clear LCD screen
        StopWatch();          // Perform stop watch
        break;

      case state3: ; //PB2
        enable_interrupts();  // Enable to Cut the state
        lcd_clear();          // Clear LCD screen
        GenTones();
        break;
                
      case state4: //PB3 RT
        enable_interrupts();  // Enable to Cut the state
        lcd_clear();          // Clear LCD screen
        Bedilugim2();
        break;
    }
  }
}
