# Lab 4 : UART Communication with MSP430G2553

## Introduction

In this lab, we will be developing a system based on asynchronous serial communication using the Universal Asynchronous Receiver/Transmitter (UART) module. The goal is to establish communication between a PC and the MSP430G2553 microcontroller on a personal development kit.

### Overview
The communication setup will consist of configuring both the PC and the microcontroller to facilitate data transfer. The physical connection will be made through a USB interface, which also serves for debugging and application development through the Code Composer Studio (CCS). The USB connection will utilize an internal chip that converts USB serial communication into JTAG protocol for debugging and RS-232 protocol for asynchronous serial communication with the microcontroller via UART.

### Microcontroller Side
On the microcontroller side, the UART module will manage communication through interrupt-driven transmission and reception. You will write a program that handles both sending and receiving data solely through interrupts. This approach ensures efficient data processing without blocking the main execution flow.

### PC Side
On the PC side, an application will be developed in a high-level language of your choice (e.g., Python, C#, MATLAB, C++, etc.). The provided example codes demonstrate a blocking communication model where the application listens to the RX input buffer until it's empty and sends data through the TX output buffer until transmission is complete. Each key press on the keyboard will send a corresponding character to the microcontroller.

## Code Writing

The software architecture on the microcontroller side must be based on a Simple Finite State Machine (FSM) that executes code segments corresponding to system states when an interrupt request is received from the PC through the UART communication channel. The system code should be modular to facilitate portability across MSP430 families by only swapping the BSP layer.

1. The microcontroller starts in sleep mode.
2. Required hardware connections:
   - RGB LED connections: R - P2.2, G - P2.1, B - P2.0
   - Buzzer connected to P2.4
   - Potentiometer connected to P1.3
   - LCD screen connections: D7-D4 to P1.7-P1.4, control lines to P2.7, P2.6, P2.5
3. Communication requirements between the microcontroller and the PC:
   - When a number corresponding to a menu line is pressed on the keyboard, the appropriate action is executed on the microcontroller.
   
### Menu Definition
- Each line in the menu represents a state in the system, with the default value of delay (X) being 500 ms.

1. Blink RGB LED, color by color with a delay of X [ms].
2. Counting up on the LCD screen with a delay of X [ms].
3. Circular tone series via Buzzer with a delay of X [ms].
4. Get delay time X [ms]:
5. Potentiometer 3-digit value [V] displayed on the LCD.
6. Clear LCD screen.
7. Show menu.
8. Sleep.

**Menu Explanation:**
- Each menu item corresponds to specific functionality within the system, enhancing interactivity and demonstrating the use of UART communication effectively.


