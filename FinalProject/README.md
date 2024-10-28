Overview:
This project involves controlling various hardware components, such as stepper motors, LCDs, and joysticks, through GPIO and UART interfaces. Below is a detailed description of each source file and the functions within them.

Source Files:
api.c

Purpose: This file contains functions that provide a higher-level API for controlling the stepper motor, joystick, and script execution. It handles the interactions with the hardware through lower-level functions defined in other modules.
Functions:
calibrate(): Calibrates the stepper motor and sends calibration data.
Stepper_clockwise(): Rotates the stepper motor clockwise.
Stepper_counter_clockwise(): Rotates the stepper motor counter-clockwise.
JoyStickADC_Steppermotor(): Controls the stepper motor using joystick input.
StepperUsingJoyStick(): Manages joystick input to rotate the stepper motor.
JoyStick_Painter(): Uses joystick input to control a painter function.
ScriptFunc(): Executes a predefined script of commands stored in flash memory.
ExecuteScript(): Processes and executes individual script commands.
inc_lcd(int X), dec_lcd(int X), rra_lcd(char X): LCD control functions for displaying and rotating numbers.

bsp.c

Purpose: This file configures the Board Support Package (BSP), including GPIO setup, timers, UART, and ADC configurations.
Functions:
GPIOconfig(): Configures the GPIO ports for various peripherals.
StopAllTimers(): Halts all running timers.
TIMER_A0_config(unsigned int counter): Configures Timer A with a specified count.
UART_init(): Initializes the UART for serial communication.
ADCconfig(): Configures the ADC for reading joystick values.

flash.c

Purpose: This file provides functions for writing to the flash memory, which is used for storing scripts and other data.
Functions:
write_to_mem(): Writes data to flash memory from a specified location.

halGPIO.c

Purpose: This file implements hardware abstraction layer (HAL) functions for interacting with the GPIO, timers, and other low-level components.
Functions:
sysConfig(): Configures the system by initializing GPIO, ADC, timers, LCD, and UART.
send_finish_to_PC(), send_degree_to_PC(): Sends status or data to the PC via UART.
int2str(char *str, unsigned int num): Converts an integer to a string.
hex2int(char *hex): Converts a hexadecimal string to an integer.
print_deg_to_lcd(float phi): Displays the angle on the LCD.
timer_call_counter(): Controls timing for certain operations.
START_TIMERA0(unsigned int counter): Starts Timer A0 with a specified count.
atan2_fp(int16_t y_fp, int16_t x_fp): Calculates the arctangent of y/x using fixed-point arithmetic.
Various interrupt service routines (ISRs) for handling timer, ADC, UART, and joystick events.
lcd_cmd(unsigned char c), lcd_data(unsigned char c), lcd_puts(const char * s), lcd_init(), lcd_strobe(): LCD control functions for sending commands, displaying data, and initializing the LCD.

main.c

Purpose: This is the main entry point of the program, which initializes the system and contains the main control loop to manage different operational states.
Functions:
main(): The main function initializes system state and enters a loop that handles different states, such as manual control, painting, calibration, and script execution based on user input and sensor feedback.