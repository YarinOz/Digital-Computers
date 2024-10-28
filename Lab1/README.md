# Lab 1 : System Programming, GPIO, Interrupts, LPM

## Introduction
This lab focuses on the fundamentals of **system programming** using **GPIO**, **Interrupts**, and **Low Power Modes (LPM)** on MSP430 microcontrollers. It is part of a course in **System Programming** and aims to introduce students to the essential methods for interfacing and controlling hardware at a low level using C programming. 

## What the Code Does
This lab code implements a Finite State Machine (FSM) for handling tasks based on **interrupt-driven programming**. The code is structured with several layers to ensure modularity and hardware abstraction. The specific functionalities covered by the code are:

1. **GPIO Configuration**:
   - Initializes GPIO pins as inputs or outputs based on the requirements.
   - Manages port settings like `PxDIR` (data direction), `PxOUT` (output), `PxREN` (resistor enable), and `PxIES` (interrupt edge select).

2. **Interrupt Handling**:
   - Configures specific GPIO pins to trigger interrupts on state changes (e.g., button presses).
   - Defines interrupt service routines (ISRs) to respond to these events, allowing asynchronous handling of inputs.

3. **Low Power Modes**:
   - Configures the microcontroller to enter different low-power modes, conserving energy when idle or waiting for an event.
   - Exits low-power modes upon interrupt to handle necessary tasks, then re-enters low-power mode after completion.

4. **Finite State Machine (FSM)**:
   - Implements a simple or advanced FSM to manage the system's states and transitions based on user inputs or events.
   - FSM transitions are triggered by interrupt-driven events, ensuring efficient and responsive state management.

This lab code is structured into distinct layers for clarity and modularity:
- **BSP (Board Support Package)**: Manages hardware initialization and configuration for LEDs, buttons, and other peripherals.
- **HAL (Hardware Abstraction Layer)**: Provides low-level routines for accessing hardware registers.
- **API (Application Programming Interface)**: Exposes high-level functions for other code to interact with the hardware without directly accessing registers.
- **Application Layer**: Implements the FSM and main application logic.

---

The goal of this lab is to build a strong foundation in system-level programming by controlling microcontroller hardware with efficient and structured code.

