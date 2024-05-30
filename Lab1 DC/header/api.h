#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer


extern void printSWs2LEDs(void);
extern void printArr2SWs(char Arr[], int size, unsigned int rate);
extern char bin_counter(int* flag, char last_count);
extern char Bedilugim(char last_count);
extern void genPWM(void);

#endif







