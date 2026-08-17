# SIT210
# Task 1.1P – Switching ON Lights

## Description
In this task, we are going to use the 'Arduino Nano 33 IoT' board for controlling two LEDs through a push button. On pressing the button, the LEDs will switch ON. The porch LED switches OFF after '30 seconds', and the hallway LED switches OFF after '60 seconds'.

## Components:
* Arduino Nano 33 IoT
* Push button
* 2 LEDs
* Resistors
* Breadboard and jumper wires

## Pin Connection:
* Button – D2
* Porch LED – D11
* Hallway LED – D12

## Code Explanation:
Here the program first senses the button press and then starts the timer by calling the function 'millis()' and turning ON both LEDs. The porch LED switches OFF after 30 seconds, and the hallway LED switches OFF after 60 seconds. Here the program is divided into different modules for taking inputs, creating time delay, and managing the LED output.
