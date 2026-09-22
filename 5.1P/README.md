# SIT210 Task 5.1P – Making a Graphical User Interface

## Overview

This project implements a simple graphical user interface (GUI) using Python and Tkinter on a Raspberry Pi.

The system allows the user to control three LEDs representing different rooms in a house:

- Living Room
- Bathroom
- Closet

When a room is selected from the GUI, its corresponding LED is turned ON while the other two LEDs are turned OFF.

---

## Hardware Used

| Component | Quantity | Purpose |
|---|---:|---|
| Raspberry Pi | 1 | Main controller |
| Yellow LED | 1 | Living Room light |
| Red LED | 1 | Bathroom light |
| Green LED | 1 | Closet light |
| Resistors | 3 | Limit LED current |
| Breadboard | 1 | Circuit implementation |
| Jumper wires | As required | Circuit connections |

---

## GPIO Configuration

| Room | LED Colour | GPIO Pin |
|---|---|---:|
| Living Room | Yellow | GPIO 17 |
| Bathroom | Red | GPIO 27 |
| Closet | Green | GPIO 22 |

The GPIO pins are configured as output pins so that the Raspberry Pi can control the LEDs.

---

## Software Used

- Raspberry Pi OS
- Python
- Tkinter
- RPi.GPIO
- GitHub

---

## GUI Features

The GUI provides the following controls:

1. **Living Room** – turns ON the Living Room LED.
2. **Bathroom** – turns ON the Bathroom LED.
3. **Closet** – turns ON the Closet LED.
4. **Exit** – closes the GUI and performs GPIO cleanup.

Only one LED is ON at a time.

For example:

```text
Living Room selected
        ↓
Living Room LED → ON
Bathroom LED    → OFF
Closet LED      → OFF
