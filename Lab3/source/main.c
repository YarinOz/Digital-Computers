#include  "../header/api.h"     // private library - API layer
#include  "../header/app.h"     // private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;
char data_matrix[][51]={
  "An apple a day keeps the doctor away", "climb on the bandwagon",
  "Dot the i's and cross the t's",
  "He who pays the piper calls the tune", "The pen is mightier than the sword", "The pot calling the kettle black",
  "shed crocodile tears", "Close but no cigar",
  "Cut from the same cloth", "Strike while the iron’s hot"
};
char idiom_recorder[33]; // 32 + '\0'
unsigned int EndOfRecord = 0;
unsigned int i = 0;
unsigned int KB;

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
        IdiomRecorder();  // 
        break;

      case state2: //PB1
        lcd_clear();          // Clear LCD screen
        Merge();          // 
        break;

      case state3: ; //PB2
        enable_interrupts();  // Enable to Cut the state
        lcd_clear();          // Clear LCD screen
        DMALEDS();
        break;
                
      case state4: //PB3 RT
        enable_interrupts();  // Enable to Cut the state
        lcd_clear();          // Clear LCD screen
        // RTLAB();
        break;
    }
  }
}
