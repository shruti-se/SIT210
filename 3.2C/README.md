# Task 3.2C – MQTT

## Description:
In this task, we are going to use the 'Arduino Nano 33 IoT' board with an ultrasonic sensor and two LEDs to implement MQTT publish and subscribe communication. A wave detected by the ultrasonic sensor publishes a message to turn ON both LEDs, while a pat publishes a message to turn OFF both LEDs.

## Components:
* Arduino Nano 33 IoT
* Ultrasonic Sensor
* 2 LEDs
* Resistors
* Breadboard and jumper wires

## Pin Connection:
* Ultrasonic TRIG – D7
* Ultrasonic ECHO – D6
* Bathroom LED – D4
* Hallway LED – D5

## MQTT Configuration:
* Broker – broker.emqx.io
* Port – 1883
* Wave Topic – Shruti/ES/Wave
* Pat Topic – Shruti/ES/Pat

## Code Explanation:
Here the program connects the Arduino to Wi-Fi and the EMQX MQTT broker. The ultrasonic sensor measures the distance of the hand to detect a wave or pat. A wave publishes a message to the wave topic and a pat publishes a message to the pat topic. The Arduino subscribes to both topics and turns ON or OFF the two LEDs according to the received message. Messages from other students are ignored.
