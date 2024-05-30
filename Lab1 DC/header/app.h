#ifndef _app_H_
#define _app_H_

#define PWM_PERIOD (250 - 1) // (1 / 4kHz) / (1 / 1MHz) - 1 assuming SMCLK at 1 MHz
#define PWM_DUTY_CYCLE 187   // 75% of PWM_PERIOD (187 / 250)

enum FSMstate{state0,state1,state2,state3,state4}; // global variable
enum SYSmode{mode0,mode1,mode2,mode3,mode4}; // global variable


#endif







