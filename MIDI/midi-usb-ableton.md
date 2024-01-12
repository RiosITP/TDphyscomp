---
title: Ableton Connect
layout: default
nav_order: 3
parent: MIDI
---

# TouchDesigner MIDI BLE Control Examples

- MIDI into TD using Arduino & MIDIUSB
- MIDI into TD using Phone App
- MIDI out to Ableton Direct
- MIDI out to Ableton Using TDAbleton Connect
- MIDI out to Garageband


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