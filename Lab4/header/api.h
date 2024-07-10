#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern unsigned int Xdelay;   // global variable

extern void startTimerA0();
extern void startTimerB();
extern void StopAllTimers();
extern void DMA0_STATE2(int *ptr1, int len1, int *ptr_merge);
extern void DMA1_STATE2(int *ptr2, int len2, int *ptr_merge);
extern void DMA0_STATE3();
extern void DMA0_STATE4(int *ptr1, int len1, int *ptr_merge);
// extern void RTLAB();

#endif
