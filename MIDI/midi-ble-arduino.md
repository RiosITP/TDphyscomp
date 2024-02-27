---
title: MIDI BLE With Arduino
layout: default
nav_order: 2
parent: MIDI
---

# TouchDesigner MIDI BLE Control Examples

## Sections

* [Description](#description)
* [Arduino Circuit](#arduino-circuit)
* [Arduino Serial Setup with IMU](#arduino-serial-w-imu)
* [TouchDesigner](#touchdesigner)
* [Selecting and Exporting](#selecting-and-exporting)
* [Resources](#resources)
* [Further Resources](#further-resources)

## Description
In this example we will use the Arduino Nano 33 IoT as a MIDI controller in TouchDesigner. The Arduino Nano 33 IoT comes with an onboard bluetooth module.  This Example does not explain the BLE protocol.  Tom Igoe has an excellent write up on BLE and MIDIBLE please have a look a his resources before continuing: https://tigoe.github.io/SoundExamples/midi-ble.html

## Materials and Libraries
* [Arduino Nano 33 IoT](https://store-usa.arduino.cc/products/arduino-nano-33-iot) (or other Wifi Enabled board, MKR 1010, etc)
* [Arduino IDE](https://www.arduino.cc/en/software)
* [ArduinoBLE Library](https://www.arduino.cc/reference/en/libraries/arduinoble/)
* [TouchDesigner (free version)](https://derivative.ca/)
* Breadboard, USB cable, Jumper wires

## Arduino Circuit:

## Hardware
https://tigoe.github.io/SoundExamples/midi-ble.html

### Arduino Circuit:
For this example I'm using a circut with 3 analog inputs.  This can be adjusted for any number of analog or digital inputs.


![Breadboard of Arduino Nano 33 IoT with Sensors connected to A0, A1, A2](./imgs/analogsensors.png?raw=true "Breadboard Sensors")


![Breadboard of Arduino Nano 33 IoT with Buttons connected to D2, D3, D4](./imgs/arduinoButtons.png?raw=true "Breadboard Switches")

## Arduino Code:
Because Arduino and Touchdesigner are communicating asynchronously, we want to set up a Call and Response system to ensure that Arduino only sends values at a rate that Touchdesigner can accommodate. We'll write code that tells arduino to print sensor values to Touchdesigner, but only if Touchdesigner has first sent a byte to arduino. We later tell touchdesigner to respond to Arduino, only if it has received values. In this way they will exchange information evenly without either ever sending before the other is ready to receive.


To begin, let's set up variables for our sensor values:

    int sensor1;
    int sensor2;
    int sensor3;

Next inside of your ```setup()``` make sure you use ```Serial.begin()```.  Remember the baudrate, this will be used later in your touchdesigner network.  I'll use 9600.  
    
    void setup(){
        Serial.begin(9600);
    }