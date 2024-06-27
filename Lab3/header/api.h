#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern char idiom_recorder[33];
extern unsigned int EndOfRecord;
extern unsigned int i;
extern unsigned int KB;
extern char data_matrix[][38];
extern const int LEDarray[9];

extern void startTimerA0();
extern void IdiomRecorder();
extern void Merge();
extern void DMALEDS();
extern int strlength(char *str);
// extern void RTLAB();

#endif
