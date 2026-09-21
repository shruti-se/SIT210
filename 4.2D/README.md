# SIT210 Task 4.2D – Calling a Function from the Web

## Project Overview:

This project implements a web-based remote lighting control system using an Arduino Nano 33 IoT. The system allows a user to control three physical LEDs representing the Living Room, Bathroom, and Closet through a web interface.

The web application sends the selected room and ON/OFF state to a Node.js and Express backend. The backend communicates with Blynk Cloud, which sends the command to the Arduino Nano 33 IoT. The Arduino then uses the `controlLight()` function to control the corresponding LED.

## System Architecture:

```text
User
  ↓
Web Interface (HTML/CSS)
  ↓
JavaScript
  ↓
Node.js + Express Backend
  ↓
Blynk Cloud
  ↓
Arduino Nano 33 IoT
  ↓
controlLight()
  ↓
Physical LEDs
```
## Hardware Requirements:
Arduino Nano 33 IoT
Breadboard
3 LEDs
3 × 220–330 Ω resistors
Jumper wires
USB cable

## LED Connections: 
| Room | LED | Arduino Pin | Blynk Pin |
|------|-----|-------------|-----------|
| Living Room | Yellow | D2 | V0 |
| Bathroom | Red | D3 | V1 |
| Closet | Green | D4 | V2 |
