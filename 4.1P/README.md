# Task 4.1P – Handling Interrupts

## Description:
This project implements an interrupt-based lighting system using an **Arduino Nano 33 IoT**. A PIR motion sensor detects movement, while a BH1750 light intensity sensor determines whether the environment is dark. When motion is detected in darkness, the two LEDs representing the porch and hallway lights are switched ON. A slider switch is provided as a manual backup option.
Hardware interrupts are used for both the PIR sensor and slider switch so that the Arduino can respond to these events without continuously polling their states.

## Components:
* Arduino Nano 33 IoT
* PIR Motion Sensor
* BH1750 Light Intensity Sensor
* Slider Switch
* 2 × LEDs
* 2 × Resistors
* Breadboard
* Jumper Wires

## Pin Connections:
| Component          | Arduino Nano 33 IoT |
| ------------------ | ------------------- |
| PIR OUT            | D2                  |
| Slider Switch      | D3                  |
| Porch LED (LED1)   | D5                  |
| Hallway LED (LED2) | D6                  |
| BH1750 SDA         | SDA                 |
| BH1750 SCL         | SCL                 |
| BH1750 VCC         | 3.3V                |
| BH1750 GND         | GND                 |

The slider switch is configured using the internal pull-up resistor.

## Configuration:
The PIR sensor is connected to **D2** and configured to generate an interrupt on a **RISING** edge.
The slider switch is connected to **D3** and configured to generate an interrupt on a **FALLING** edge using `INPUT_PULLUP`.

The BH1750 communicates with the Arduino through **I²C** using SDA and SCL. The system considers the environment dark when the measured light intensity is below **50 lux**.

The two LEDs are connected to D5 and D6 and are configured as output pins.

## Code Explanation:
The code uses hardware interrupts to detect motion from the PIR sensor and activation of the slider switch. The BH1750 continuously measures the ambient light intensity, and when motion is detected in a dark environment, both LEDs are turned ON automatically. The slider switch acts as a manual backup and can also activate both LEDs. The Serial Monitor displays the light intensity and relevant messages for each event.
