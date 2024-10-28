#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

// stepper motor and joystick functions
extern void Stepper_counter_clockwise();
extern void Stepper_clockwise();
extern void calibrate();
extern void ScriptFunc();
extern void ExecuteScript();
extern void JoyStick_Painter();
extern void JoyStickADC_Steppermotor();

extern void StepperUsingJoyStick(); 

// Macros for Script Mode
extern void inc_lcd(int);
extern void dec_lcd(int);
extern void rra_lcd(char);
// Analog stick values
extern int16_t Vrx;
extern int16_t Vry;
extern char step_str[4];
#endif







