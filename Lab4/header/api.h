#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern char idiom_recorder[33];
extern unsigned int EndOfRecord;
extern unsigned int i;
extern unsigned int KB;
extern char data_matrix[][38];
extern char strBM[];
extern int LEDarray[10];
extern int *ledptr;

extern void startTimerA0();
extern void startTimerB();
extern void IdiomRecorder();
extern void Merge();
extern void DMALEDS();
extern void Mirror();
extern int strlength(char *str);
extern void StopAllTimers();
extern void DMA0_STATE2(int *ptr1, int len1, int *ptr_merge);
extern void DMA1_STATE2(int *ptr2, int len2, int *ptr_merge);
extern void DMA0_STATE3();
extern void DMA0_STATE4(int *ptr1, int len1, int *ptr_merge);
// extern void RTLAB();

#endif
