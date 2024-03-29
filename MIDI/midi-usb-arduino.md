---
title: MIDIUSB with Arduino
layout: home
nav_order: 1
parent: MIDI
---

# MIDIUSB with Arduino

This example uses the Arduino MIDIUSB library to send and receive MIDI Messages between your microcontroller and TouchDesigner

[Arduino MIDIUSB Library](https://www.arduino.cc/reference/en/libraries/midiusb/)

[Arduino MIDIUSB Write Example](https://github.com/arduino-libraries/MIDIUSB/blob/master/examples/MIDIUSB_write/MIDIUSB_write.ino)

## Hardware

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
