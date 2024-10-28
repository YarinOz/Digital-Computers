# Lab 2 : Timers, ADC, and DAC 

## Introduction

This lab introduces essential concepts and programming techniques for using timers, Analog-to-Digital Converters (ADC), and Digital-to-Analog Converters (DAC) within the MSP430 microcontroller. We will explore creating an interrupt-driven system based on Simple Finite State Machine (FSM) architecture, structured in layered abstraction to enhance portability across MSP430 families. The main tasks include configuring timers and ADC, handling user input via buttons, and displaying results on an LCD screen. The lab aims to build proficiency in creating embedded system applications that meet performance and portability requirements.

### Key Concepts
- **Timer_B**: Understand the difference between `Basic Timer1` and `TimerB`, including Capture and Compare modes.
- **ADC12**: Familiarize yourself with the clock sources for ADC12, sampling techniques, and operational modes.

## Practical Section - Writing Portable System Code

The practical section requires developing system code that can run both on the lab's evaluation kit and personal kit. The design approach utilizes a **Simple FSM** architecture to handle button-triggered interrupts, executing tasks like frequency counting, stopwatch timing, and tone generation.

### Hardware Connections
1. **Switches and Buttons**: Connect the SW0 switch to P2.0 and buttons PB0, PB1, PB2 to P1.0, P1.1, and P1.2, respectively.
2. **Analog Input**: Connect an analog input to P1.3 (input A3).
3. **LCD Screen**: Connect LCD data lines D7-D4 to P1.7-P1.4 and control lines to P2.7, P2.6, and P2.5.
4. **Signal Generator**: Attach the signal generator to P2.4 for input capture, and the PWM output on P2.2 connects to an oscilloscope and a buzzer.

### Software Architecture and Code Layers
1. **Finite State Machine (FSM)**: The FSM handles four main states, based on external interrupt requests from the button presses. Each state corresponds to a specific task:
    - **Idle State**: The system enters a low-power mode after RESET or upon completion of other states.
    - **State 1 (PB0)**: Implements a frequency counter using Input Capture on P2.4, displays the frequency on the LCD.
    - **State 2 (PB1)**: Starts/stops a stopwatch on the LCD, utilizing Timer A0 in Up/Down mode.
    - **State 3 (PB2)**: Generates tones via the PWM output on P2.2, with frequency controlled by a potentiometer input.

2. **HAL and API Layers**: 
   - **HAL (Hardware Abstraction Layer)**: Contains low-level drivers for the LCD, Timer, and ADC functionalities.
   - **API Layer**: Implements higher-level functions that interact with HAL functions, such as displaying strings on the LCD or setting the timer configurations.

3. **Driver Functions**: Specific drivers will be added to support each hardware feature:
   - **Input Capture** for frequency counting (state 1),
   - **Basic Timer** for stopwatch timing (state 2),
   - **Output Compare** for PWM tone generation (state 3).

