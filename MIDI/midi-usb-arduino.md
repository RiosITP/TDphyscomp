---
title: MIDIUSB with Arduino
layout: home
nav_order: 1
parent: MIDI
---

# MIDIUSB with Arduino

## Sections
[Description](#description)  
[Materials and Libraries](#materials-and-libraries)  
[Arduino Circuit](#arduino-circuit)
[Arduino Code](#arduino-code)
[TouchDesigner Setup](#touchdesigner-setup)
[TouchDesigner Network](#touchdesigner-network)
[Related Resources](#related-resources)


## Description

In this example we will make our own MIDI controller to control visuals in TouchDesigner. This example uses an Arduino microcontroller with digital and analog sensors (buttons and potentiometer) as well as the Arduino MIDIUSB library and its example code, to send note on, note off, and control change messages. 

## Materials and Libraries

* [Arduino Nano 33 IoT](https://store-usa.arduino.cc/products/arduino-nano-33-iot) (or other Wifi Enabled board, MKR 1010, etc)
* [Arduino IDE](https://www.arduino.cc/en/software)
* [TouchDesigner (free version)](https://derivative.ca/)
* Breadboard, USB cable, Jumper wires
* [Arduino MIDIUSB Library](https://www.arduino.cc/reference/en/libraries/midiusb/)  
* [Arduino MIDIUSB Write Example](https://github.com/arduino-libraries/MIDIUSB/blob/master/examples/MIDIUSB_write/MIDIUSB_write.ino)



## Arduino Circuit:
For this example I'm using a circut with 3 buttons (digital inputs).

![Breadboard of Arduino Nano 33 IoT with Buttons connected to D2, D3, D4](../imgs/arduinoButtons.png?raw=true "Breadboard Switches")

## Arduino Code:

### Dependencies:

Start the sketch by including the [MIDIUSB Library](https://www.arduino.cc/reference/en/libraries/midiusb/).  From the menu go to "Sketch" -> "Include Library" -> "MIDIUSB"

![Include MIDIUSB Library](../imgs/midilibraryselect.png)

Next open the [MIDIUSB Write Example](https://github.com/arduino-libraries/MIDIUSB/blob/master/examples/MIDIUSB_write/MIDIUSB_write.ino) 

![Include MIDIUSB Library](../imgs/midiWriteExampleSelect.png)

Copy the following functions into your sketch.  These functions will allow you to write `note on`, `note off` and `control change` messages without having to look up and write out the hexidecimal values of each program and channel. Instead you can now use this functions and pass your MIDI arguments (channel, pitch, velocity, CC number, controller value) as decimal bytes ranging from 0 - 127.  Also note that this library zero indexes the channel numbers.  Many DAWs and MIDI devices will allow you to send messages over 1 of 16 channels ranging from 1-16, however arduino values will be shifted to a range of 0-15.
    
    void noteOn(byte channel, byte pitch, byte velocity) {
        midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
        MidiUSB.sendMIDI(noteOn);
    }

    void noteOff(byte channel, byte pitch, byte velocity) {
        midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
        MidiUSB.sendMIDI(noteOff);
    }

    void controlChange(byte channel, byte control, byte value) {
        midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
        MidiUSB.sendMIDI(event);
    }
### State Change:

Next declare some variables to track each button's state change. 

    int pButton1;
    int pButton2;
    int pButton3;

Next inside of your ```setup()``` make sure you use ```Serial.begin()```.  Remember the baudrate, this will be used later in your touchdesigner network.  I'll use 9600.  Set the registers of your connected pins as `INPUT_PULLUP`, this activates the internal pull up resistors allowing for a circuit with no external resistors on the breadboard.  
    
    void setup(){
        Serial.begin(9600);
        pinMode(2, INPUT_PULLUP);
        pinMode(3, INPUT_PULLUP);
        pinMode(4, INPUT_PULLUP);
        pButton1 = digitalRead(2);
        pButton2 = digitalRead(3);
        pButton3 = digitalRead(4);
    }

Next inside of your ```loop()``` store button readings for each button in a variable.  

    int button1 = digitalRead(2);
    int button2 = digitalRead(3);
    int button3 = digitalRead(4);

Then for each button, detect a state change, comparing current state to previous state.  When a change is detected send a `note on` if the button is pressed, send a `note off` when the button is released.

Since we are wired using the Arduino's internal pullup-resistors a reading of 1 means the button is not pressed, a reading of 0 means the button is pressed down. 

Make sure you update your previous button variables (`pButton`) to reflect the detected changes.

  if (pButton1 != button1) {

    if (button1 == 0) {
      noteOn(0, 48, 100);
      MidiUSB.flush();
    }
 
    if (button1 == 1) {
      // turn off that note if you let go of the button
      noteOff(0, 48, 100);
      MidiUSB.flush();
    }

    // update button status to avoide continuously sending MIDI notes
    pButton1 = button1;
  }

Use the above strategy for each button.

[See full example code here]()


## TouchDesigner Setup

## TouchDesigner Network


## Related Resources
* [Matthew Ragan MIDI Tutorials](https://www.youtube.com/watch?v=XLeghJmFBh0&ab_channel=MatthewRagan)
* [MIDI Notes Chart](https://newt.phys.unsw.edu.au/jw/notes.html)
* [MIDI CC Number Chart](https://anotherproducer.com/online-tools-for-musicians/midi-cc-list/)
* [ITP Physical Computing Site](https://itp.nyu.edu/physcomp/)

