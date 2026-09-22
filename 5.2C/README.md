# SIT210 Task 5.2C - Change the Light Intensity Using GUI

## Overview

This project extends the Smart Home Lights GUI developed in Task 5.1P. The system uses a Raspberry Pi, Python, Tkinter and GPIO control to operate three room lights.

The main improvement in this task is the addition of **PWM (Pulse Width Modulation)** to control the brightness of the Living Room LED using a GUI slider.

## Objective

The objective of this task is to:

- Control room lights using a graphical user interface.
- Use Raspberry Pi GPIO pins to control LEDs.
- Use PWM to change the brightness of the Living Room LED.
- Allow the user to select different rooms from the GUI.
- Provide a simple and user-friendly interface.

## Hardware Requirements

- Raspberry Pi
- Yellow LED
- Red LED
- Green LED
- Resistors
- Breadboard
- Jumper wires

## GPIO Connections

| Room | LED | GPIO Pin | Control |
|---|---|---:|---|
| Living Room | Yellow | GPIO 18 | PWM brightness |
| Bathroom | Red | GPIO 27 | ON/OFF |
| Closet | Green | GPIO 22 | ON/OFF |

## Software Requirements

- Raspberry Pi OS
- Python 3
- Tkinter
- RPi.GPIO
- SSH/PuTTY for remote access

