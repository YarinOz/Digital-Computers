#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer


extern void RGBlink();
extern void count_up_LCD();
extern void count_down_LCD();
extern void clear_counters();
extern void PotentiometerMeas();
extern void XDelay();
extern void CircBuzzer();
extern void int2str(char *str, unsigned int num);


#endif







