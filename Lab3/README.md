# Lab 3: Direct Memory Access (DMA) with MSP430

## Lab Topics
In this lab experiment, we will explore the concept of Direct Memory Access (DMA). The DMA module allows blocks of data to be transferred between peripheral hardware and memory without involving the CPU. This is particularly beneficial for transferring large amounts of data, as it minimizes CPU overhead, saving time and energy.

The MSP430x4xx family microcontroller includes a DMA module with three independent channels, though only one can be active at a time. Example code demonstrating DMA usage can be found in the course materials.

## Practical Part: System Code Implementation
### Introduction
In this experiment, we will implement an embedded system based on the DMA hardware accelerator to perform manipulation and processing of data structures. The processing of data structures requires numerous load and store operations, which are inefficient for the CPU to handle due to time complexity and energy consumption. 

We will learn how to correctly integrate CPU and DMA to optimize system response time and energy efficiency in a low-power MCU.

### Hardware Connections
1. Buttons PB2 - PB0 are connected to the MSP430 GPIO pins P1.2 - P1.0, respectively.
2. An LCD is connected with data lines D7-D4 to pins P1.7-P1.4 (operating in 4-bit mode) and control lines to P2.7, P2.6, and P2.5.
3. The Keypad is connected to P10, with its interrupt pin connected to P2.1.

### Software Architecture
1. The software architecture should be based on a Simple FSM (Finite State Machine) that performs one of four actions upon receiving an external interrupt from a button press.
2. The system code must be layered to ensure portability across MSP430 families by only modifying the BSP (Board Support Package) layer.
3. Before coding, draw a detailed FSM diagram illustrating the software architecture and attach it to the report.
4. Avoid using polling for delays except for debounce in the button interrupt service routine.

### Driver Layer Support
1. The LCD driver functions should reside in the HAL (Hardware Abstraction Layer), while a string writing function based on them should be in the API layer.
2. The Keypad driver functions should also be in the HAL, with a string input function in the API.
3. For State 2, create a driver for DMA-based data transfer from one memory address range to another.
4. For State 3, create a driver for DMA-based data transfer from memory to a peripheral component.

### Static Data Structures
Define a static two-dimensional character array in the main layer as follows:
```c
char data_matrix[M][N] = {
    "An apple a day keeps the doctor away",
    "Climb on the bandwagon",
    "Dot the i's and cross the t's",
    "He who pays the piper calls the tune",
    "The pen is mightier than the sword",
    "The pot calling the kettle black",
    "Shed crocodile tears",
    "Close but no cigar",
    "Cut from the same cloth",
    "Strike while the iron’s hot"
};
