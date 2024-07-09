#include  "../header/api.h"     // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"
#include <string.h>


void ClearAll(){
    disable_interrupts();
    lcd_clear();
    lcd_home();
    count_up = 0;
    count_down = 65535;
    enable_interrupts();
    state = state0;
}