#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern unsigned int Xdelay;   // global variable

extern void startTimerA0();
extern void startTimerB();
extern void StopAllTimers();
extern void RGBlink();
extern void LCDcount();
extern void CircBuzzer();
extern void GetDelay();
extern void Pot2LCD();
extern void ClearAll();

// extern void RTLAB();

#endif
