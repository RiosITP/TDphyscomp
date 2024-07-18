---
title: Serial from TD one way controlling an LED
layout: default
nav_order: 3
parent: Serial Communication
---

# Serial to TD: One-way Communication Controlling an LED

## Sections:
- [Intro](#introduction) 
- [Arduino Circuit](#arduino-circuit)
- [Arduino Code](#arduino-code)
- [TouchDesigner Network](#the-touchdesigner-network)
    - [Make a button to Control Port](#make-the-button-control-your-port-opening)
    - [Send a Byte Using a Script](#send-a-byte-using-a-script)
    - [Serial DAT: Receiving Data](#serial-dat-receive-data)
    - [Serial Callback: Sending More data](#serial-callback-send-more-data-when-a-message-is-received)
- [Control Arduino Output](#control-arduino-physical-outputs-with-td)
    - [Add an LED to the Circuit](#add-an-led-to-the-arduino-circuit)
    - [Update Arduino Code](#update-the-arduino-code)
    - [Add a Control Slider](#add-a-control-slider)
- [Related Resources](#related-resources)

## Introduction

When working with Arduino and TouchDesigner you may want TouchDesigner to control the behavior of an output.  In this case we will start with changing the brightness of an LED.

We'll build an LED circuit, and write code that tells Arduino: 
- to use Serial Communication to receive one byte of information 
- use that byte to control the brightness of our LED

We'll create a TouchDesigner Network that tells TD to: 
- to use a slider or a number generating CHOP to generate a value
- Use Serial Communication to send that value as one byte of information to Arduino

At the end our TD controller (slider or CHOP) should be controlling the brightness of our LED

This example uses the the free version of TouchDesigner 2023.11880 and the [Arduino Nano 33 IoT](https://store.arduino.cc/products/arduino-nano-33-iot).
[See Arduino Pinout here](https://docs.arduino.cc/resources/pinouts/ABX00027-full-pinout.pdf).      
[See more info here](https://docs.arduino.cc/hardware/nano-33-iot/)

## Arduino Circuit
 The circut uses the following components:
 - Arduino Nano 33 IoT
 - Solderless breadboard
 - 220 Ohm resistor
 - LED
 - Hookup wire / jumper wire

To begin, set up the breadboard so that you connect the Arduino Digital Pin 2 to one end of a 220 ohm resistor. Connect the other end of the resistor to the Anode (the longer lead) of the LED.  Connect the Cathode (the shorter lead) of the LED to GND. See reference image below.

![Breadboard of Arduino Nano 33 IoT with LED connected to an 220 ohm resistor connected to D2](../imgs/LEDPWM.png?raw=true "Breadboard Switches")

Note: this example will use PWM.  Make sure that the resistor-LED series is connected to digital pin 2 or any other PWM pin if you are modifying this example.  PWM pins are labelled with a tilde symbol *~* in the [Arduino Pinout Diagram](https://docs.arduino.cc/resources/pinouts/ABX00027-full-pinout.pdf).  

## Arduino Code:
Once the circuit is set up lets write code to receive data via serial communication and use that serial data to change the brightness of the LED.

First set up global variables for our LED's brightness value and LED pin.  The brightness value can be any number between 0 and 255 to start, the ledPin should be which ever pin you are connected to (Ddigital pin 2 in this example):

    int brightness = 255;
    int ledPin = 2;

Next inside of your ```setup()``` Use ```Serial.begin()``` to start communication at a baud rate of 9600.  Remember the baud rate, it will be used later in your touchdesigner network.  

Set your led pin (digital pin 2) as an output using ```pinMode()```
    
    void setup(){
        Serial.begin(9600);
        pinMode(ledPin, OUTPUT);
    }
    
Inside of your ```loop()``` use the ```Serial.available()``` function to read incoming data. If there is data coming across the serial port the ```Serial.available()``` function will respond with the number of data bytes available. If ```Serial.available()``` returns a value greater than `0` that means we have received data from TD.

This also means whatever you put inside of this ```if``` statement will only execute if the Arduino has received information from Touchdesigner.

Inside our if statement (meaning once we have data) read that data and store it as an integer variable called ```incoming```.  Next assign the ```brightness``` variable to be the same value as the ```incoming``` variable.

Outside of the if statement, lets use ```analogWrite()``` to change the brightness of the LED.  Since it is outside of the the ```if``` statement, it should default to maximum brightness (255) if we do not yet connect to TouchDesigner, and if communication is ever interrupted it will stay at its last received value
    
    void loop() {
        if (Serial.available() > 0) {
            int incoming = Serial.read();
            brightness = incoming;  
            // Serial.println(incoming);
        }
        analogWrite(ledPin, brightness); 
    }

Full Code below:

    int brightness = 255;
    int ledPin = 2;

    void setup(){
        Serial.begin(9600);
        pinMode(ledPin, OUTPUT);
    }

    void loop() {
        if (Serial.available() > 0) {
            int incoming = Serial.read();
            brightness = incoming;  
            // Serial.println(incoming);
        }
        analogWrite(ledPin, brightness); 
    }


## The TouchDesigner Network:
[Downloadable link here]()
<!-- ![Serial Communication Network](https://github.com/RiosITP/DILP2022/blob/main/In%20Class%20Examples/Serial/imgs/network.png?raw=true "Network") -->


### Make the button control your port opening:
Use a button to open and close the Serial port.  Create a ```button``` component (COMP), make sure the "Button Type" parameter is set to Toggle Up. Connect it to a ```null``` CHOP. 

![Serial Communication Network](../imgs/buttonNull.png?raw=true "Toggle Button to Null")
![button settings toggle](../imgs/buttonSettingsSerial.png?raw=true "button settings toggle")

Next Create a ```serial``` DAT.

Make the ```null``` CHOP active and export it to the "Active" parameter of the ```serial``` DAT. Then make your button component active and click it to see if the serial DAT toggles on and off. 

![Serial Communication Network](../imgs/serialButtonExports.gif?raw=true "Toggle Button to Null")

### Serial DAT: Receive data
Click on the `serial` DAT.  Set the parameters to read incoming information as such:

- `Row/Callback Format`: "One Per Message"
- `Port`: Portnames should match what you see in your Arduino IDE (e.g. COM4, usbmodem14201, etc). Will differ by computer and usb port
- `Baud Rate`: 9600 (Baud should match your Arduino Baud)
- `Data Bits`: 8 (Default)
- `Parity`: None (Default)
- `DTR`: Enable (Default)
- `RTS`: Disable (Default)

![Serial DAT Settings](../imgs/serialDATsettings.png?raw=true "Serial DAT Settings")

### Add a Control Slider



Add a `slider` COMP and connect it to a `select` CHOP

![Slider](../imgs/slider.png?raw=true "Slider to Select")

Make the slider active in order to interact with it, move the slider and look at the range of values it outputs

![Slider](../imgs/sliderSelect.gif?raw=true "Slider gif")

Also notice how the channel name is different from the other CHOP channesl.  Our other CHOPs generate channels named `chan1`,`chan2`, etc, but the slider has generated a channed with the name `v1`. 

To avoid any potential future conflicts you may want to rename the channel in the `select` tab of the `select` CHOP

![Select rename](../imgs/selectRename.gif?raw=true "Select rename")

Closer View:
![Select rename](../imgs/selectRenameClose.gif?raw=true "Select rename")

By default TouchDesigner uses normalized values (between -1.0 and 1.0) for nearly all operators.  This means if we want to use the slider to control the brightness of the LED we need to map the slider's initial range of 0-1 to 0-255 so that it can control the full range of brightness of the LED.

Connect the `select` CHOP to a `math` CHOP

Change the parameters of the `math` CHOP.  First navigate to the `OP` Tab, and look for the `Integer` dropdown.  Select `Round`



Navigate to the `Range` Tab and set the following fields:

 ```From Range:``` minimum to 0, maximum to 1.  This is the incoming range of the slider.

```To Range: 0 255``` minimum to 0, maximum to 255.  This is the outgoing mapped value that we want to send to Arduino.

Make the slider active and observe how the range changes:

![math](../imgs/intMathClose.gif?raw=true "Math Params")

![math](../imgs/intMath.gif?raw=true "Math")

Lastly connect the `math` CHOP to a `null` CHOP.  

![null](../imgs/serialSliderMathNull.png?raw=true "Connect to Null")

If all is set up correctly you should be able to use the slider to output a value between 0 and 255.

### Send a byte using a CHOP Execute script:

Create a ``` CHOP Execute``` DAT.  

The ```CHOP Execute``` DAT is designed to trigger specific functions whenever the target CHOP channel values have exhibited any of the parameter states or changes listed: 
- ```Off to On```
- ```While On```
- ```On to Off```
- ```While Off ```
- ```Value Change```

For this case we will only use ```Value Change```. We only want to communicate to Arduino when necessary, and for this interaction we only need to update our Arduino's LED if there is a change in value.

Make sure the ```Value Change``` parameter is enabled. 

Also make sure the `CHOPs` field refers back to the name of the null that your slider is connected to,  otherwise it won't know what component should be controlling the communication. In this example `null2` is the CHOP connected to the button.

![Serial Communication Network](../imgs/CHOPexecSerialOneWay.png?raw=true "ChopExecute Script")

Make the ```chopExec``` DAT Active to enable editing directly in the DAT. We want to write a python script that will execute whenever the value changes. Make sure to only edit the ```onValueChange``` function

![CHOPexec Settings](../imgs/CHOPexecEditorEmpty.png?raw=true)

The ```onValueChange``` function has four parameters built in (and documented at the top of the script in the comments) one of them is the numeric value of the changed sample: ```val```, this is our slider data. 

When the value changes lets use the ```serial1``` DAT to send the ```val``` to the arduino as a byte:

    def onValueChange(channel, sampleIndex, val, prev):
        op('serial1').sendBytes(val)
        return
	

![Serial Communication Network](../imgs/CHOPExecEdited.png?raw=true "ChopExecute Script")


If all set up and connected correctly you should be able to activate the button use it to start communicating with the Arduino, then active the slider and use the slider to change the LED's brightness.


![Serial Communication Network](../imgs/SerialLED.gif?raw=true "ChopExecute Script")

At this point you can try connecting other operators in place of the slider. Try `LFO` or `Noise` and play with different options and mappings to automate how the LED brightness changes.

## Related Resources
* [ITP Physical Computing Site](https://itp.nyu.edu/physcomp/)




[Just the Docs]: https://just-the-docs.github.io/just-the-docs/
[GitHub Pages]: https://docs.github.com/en/pages
[README]: https://github.com/just-the-docs/just-the-docs-template/blob/main/README.md
[Jekyll]: https://jekyllrb.com
[GitHub Pages / Actions workflow]: https://github.blog/changelog/2022-07-27-github-pages-custom-github-actions-workflows-beta/
[use this template]: https://github.com/just-the-docs/just-the-docs-template/generate
