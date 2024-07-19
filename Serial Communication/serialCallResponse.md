---
title: Two-way Communicaiton with Analog Sensors
layout: default
nav_order: 6
parent: Serial Communication
---

# Serial Call and Response w/ Analog Sensors

## Sections:
- [Introduction](#introduction) 
- [Arduino Circuit](#arduino-circuit)
- [Arduino Code](#arduino-code)
- [TouchDesigner Network](#the-touchdesigner-network)
    - [Make a button to Control Port](#make-the-button-control-your-port-opening)
    - [Send a Byte Using a Script](#send-a-byte-using-a-script)
    - [Serial DAT: Receiving Data](#serial-dat-receive-data)
    - [Serial Callback: Sending More data](#serial-callback-send-when-a-message-is-received)
- [Control Arduino Output](#control-arduino-physical-outputs-with-td)
    - [Add an LED to the Circuit](#add-an-led-to-the-arduino-circuit)
    - [Update Arduino Code](#update-the-arduino-code)
    - [Add a Control Slider](#add-a-control-slider)
- [Related Resources](#related-resources)

## Introduction

If we want Arduino and Touchdesigner exchange messages back and forth, one way to exchange information is with a Call and Response. Essentially we are taking turns exchanging information between Arduino and TD.  

We'll write code that tells the Arduino to print sensor values to Touchdesigner, but only if Touchdesigner has first sent a byte to Arduino. We later tell Touchdesigner to respond to Arduino, writing out bytes if it has received values.  In this way they will exchange information (bytes) evenly without either ever sending before the other is ready to receive. 

This example shows the necessary python scripts, DATs, and CHOPs to set up Serial Communication with an Arduino. This example uses the the free version of TouchDesigner 2022.31030 and the Arduino Nano 33 IoT.

## Arduino Circuit:
For this example I'm using a circut with the following components:
 - Arduino Nano 33 IoT
 - Solderless breadboard
 - Two potentiometers
 - One photoresistor
 - One 10 kilohm (10K Ω) Fixed Resistor


![Breadboard of Arduino Nano 33 IoT with Sensors connected to A0, A1, A2](../imgs/analogsensors.png?raw=true "Breadboard Sensors")


## Arduino Code:
If we want Arduino and Touchdesigner exchange messages back and forth. We'll write code that tells arduino to print sensor values to Touchdesigner, but only if Touchdesigner has first sent a byte to arduino. We later tell touchdesigner to respond to Arduino, only if it has received values. In this way they will exchange information evenly without either ever sending before the other is ready to receive.


To begin, let's set up variables for our sensor variables globally (before the setup or loop functions):

    int sensor1;
    int sensor2;
    int sensor3;

Next inside of your ```setup()``` Use ```Serial.begin()``` to start communication at a baud rate of 9600.  Remember the baud rate, it will be used later in your touchdesigner network.  
    
    void setup(){
        Serial.begin(9600);
    }
    
Inside of your ```loop()``` read your sensors using ```analogRead()```:
    
    void loop(){
        sensor1 = analogRead(A0);
        sensor2 = analogRead(A1);
        sensor3 = analogRead(A2);
    }

After reading your sensors (and while still inside the ```loop()```). Create and `if` statement that only executes if `Serial.available()` is greater than `0`.  

This means whatever you put inside of this statement will only execute if the Arduino has received information on the serial port (from Touchdesigner).

    void loop(){
        sensor1 = analogRead(A0);
        sensor2 = analogRead(A1);
        sensor3 = analogRead(A2);
    
        if (Serial.available() > 0) {
        
        }
    }

If Arduino receives information from TouchDesigner, read that information, remove it from the serial buffer by storing it with a variable `int incoming`.  Then print out the sensor values.  Format the communication by separating values with a delimiter (in this case a comma ",") and ending all communication with a newline character `'\n'`.  The newline will indicate to Touchdesigner that Arduino finished sending all the values for that turn.


    if (Serial.available() > 0) {
        int incoming = Serial.read();
        Serial.print(sensor1);
        Serial.print(",");
        Serial.print(sensor2);
        Serial.print(",");
        Serial.print(sensor3);
        Serial.print('\n');
    }

Full Code below:

    int sensor1;
    int sensor2;
    int sensor3;

    void setup() {
        Serial.begin(9600);
    }

    void loop() {
        sensor1 = analogRead(A0);
        sensor2 = analogRead(A1);
        sensor3 = analogRead(A2);

        if (Serial.available() > 0) {
            int incoming = Serial.read();
            Serial.print(sensor1);
            Serial.print(",");
            Serial.print(sensor2);
            Serial.print(",");
            Serial.print(sensor3);
            Serial.print('\n');
        }
    }


## The TouchDesigner Network:
<!-- ![Serial Communication Network](https://github.com/RiosITP/DILP2022/blob/main/In%20Class%20Examples/Serial/imgs/network.png?raw=true "Network") -->

### Make the button control your port opening:
Use a button to open and close the Serial port.  Create a ```button``` component (COMP), make sure the "Button Type" parameter is set to Toggle Up. Connect it to a ```null``` CHOP. 

![Serial Communication Network](../imgs/buttonNull.png?raw=true "Toggle Button to Null")

Make the ```null``` active and export it to the "Active" parameter of the ```serial``` DAT. Then make your button component active and click it to see if the serial DAT toggles on and off. 

![Serial Communication Network](../imgs/serialButtonExports.gif?raw=true "Toggle Button to Null")


### Send a byte using a script:

Create a ``` CHOP Execute``` DAT.  

The ```CHOP Execute``` DAT is designed to trigger specific functions whenever the target CHOP channel values have exhibited any of the parameter changes listed: 
- ```Off to On```
- ```While On```
- ```On to Off```
- ```While Off ```
- ```Value Change```

For this case we will only use ```Off to On```.  

Make sure the ```Off to On``` parameter is enabled. (A toggle button will only change from 0 to 1 and back)
Also make sure the `CHOPs` field refers back to the name of the null that your button is connected to,  otherwise it won't know what component should be controlling the communication. In this example `null1` is the CHOP connected to the button.

![Serial Communication Network](../imgs/chopExecSettings.png?raw=true "ChopExecute Script")

Make the chopExec DAT Active and write a python script that will execute when the button goes from off to on. The code below will send one byte (the character 'x') to Arduino when the button is clicked to the on position. Set the `terminator` option to '=' to only send the specified byte and no extra characters. See the [serial DAT reference](https://docs.derivative.ca/SerialDAT_Class) for more info.

    def onOffToOn(channel, sampleIndex, val, prev):
        op('serial1').send('x',terminator='=')
        return


![Serial Communication Network](../imgs/chopExecActive.png?raw=true "ChopExecute Script")

Once this is set up, your button will now be configured to open and close the serial port, and initiate communicaton with the Arduino.

### Serial DAT: Receive data
Click on the `serial` DAT.  Set the parameters to read incoming information as such:

- `Row/Callback Format`: "One Per Message"
- `Port`: Portnames will differ by computer, it should match what you see in your Arduino IDE (e.g. COM4, usbmodem14201, etc)
- `Baud Rate`: 9600 (Baud should match your arduino Baud)
- `Data Bits`: 8 (Default)
- `Parity`: None (Default)
- `DTR`: Enable (Default)
- `RTS`: Disable (Default)

![Serial DAT Settings](../imgs/serialDATsettings.png?raw=true "Serial DAT Settings")

### Send a byte to Arduino

Create a `constant` CHOP and connect it to a `null` this will eventually be the value to send back to arduino. 

![Constant null](../imgs/constantNull.png?raw=true "Serial callbacks Dat")

This value can be constant or changing as long as it is between 0-255. It can also eventually be used to control the Arduino's outputs.

![Constant Value](../imgs/constant.png?raw=true "Serial callbacks Dat")


### Serial Callback: Send When a Message is Received
Next activate the `serial_callbacks` scripting DAT.  

![Serial Callbacks](../imgs/serialCallbacksDat.png?raw=true "Serial callbacks Dat")

If it is not exposed, click the pink and white arrow icon on the bottom right side of the `serial` DAT

![Serial Callbacks](../imgs/callbackDropdown.png?raw=true "Serial callbacks Dat")

Make the `serial_callbacks` DAT active (editable) by clicking it's + icon and edit the `onReceive` function.  This function will execute each time the `serial` DAT receives a byte.

Inside the function, make a variable (mine is called "out") and assign it the constant value thats captured in the `null` CHOP you just made.

        out = int(op('null2')['chan1'])

Next write an if statement to check and see if TouchDesigner has received a complete message from Arduino.

Arduino is printing an ASCII '\n' after it prints all sensor values.  In decimal format an ASCII '\n' equals byte 10. ( [See the ASCII Table]("https://www.asciitable.com/") )

Based on how we set up the `serial` DAT, TouchDesigner will store the received bytes in an array (the `bytes` parameter of the `onReceive` function).  So if the last byte received by Touchdesigner is equal to 10 that means TD should have received all of the sensor values from arduino. That means we should "request" more values from Arduino, by sending our outgoing byte.

If the last byte is 10, use the `serial1` operator to send a Byte to the Arduino:
        
        if bytes[len(bytes)-1] == 10:
            op('serial1').sendBytes(out)
            


Full Code for the `serial_callbacks` DAT:

    def onReceive(dat, rowIndex, message, bytes):
        
        out = int(op('null2')['chan1'])
    
        if bytes[len(bytes)-1] == 10:
            op('serial1').sendBytes(out)
        
        return

![Serial Callbacks](../imgs/serialCallbacks.png?raw=true "Serial callbacks")




Make sure the Arduino code is uploaded, and the DATs are up correctly, then click the button to open the port, and your `serial` DAT should look something like this:

![Serial DAT](../imgs/serialDAT.png?raw=true "Serial DAT")

Once the sensor data is coming in reliably we can connect the output of our `serial` DAT to the input of a `convert` DAT and use the `convert` to separate the values.

![Serial To Convert](../imgs/serialtoConvert.png?raw=true "Serial to Convert")

The `convert` parameters define how you will reformat the incoming values.  Make sure that you are converting to a table, this will take values and split them into individual cells.  Input a comma ',' in the `split cells at` field to define which delimiters you are using in between values.  You do not need to input '\n', the `serial` DAT takes care of the control character.

![Serial To Convert](../imgs/convertSettings.png?raw=true "Convert Settings")
![Convert Parameters](../imgs/splitcells.png?raw=true "Convert Settings")

Once we have the values separated, connect the output of your `convert` to a `null` and connect the `null` to a `DAT to` CHOP. This will turn the split DAT values into a channel (CHOP) format.  Once you have values in a CHOP you can then export and connect channel values to the other parts of your TouchDesigner network

![Serial To Convert](../imgs/convertNullDatTo.png?raw=true "dat to CHOP")

In order to properly sort the information from the `null` chop, configure the `datto` CHOP parameters as follows:
- `DAT`: null6 (name of the DAT it should be reading)
- `Select Rows` : by Index
- `Start Row Index` : 1 ( row 0 is headings, so skip it)
- `Select Cols` : by Index
- `Start Col Index` : 0
- `End Col Index` : 2  <-- (if you have a different amount of sensors use the last index of what you actually have)
- `Output` : Channel per Column
- `First Row is` : Values
- `First Column is`: Values

![Dat Settings](../imgs/dattodetail.png?raw=true "DatTo Settings")

![Serial To Convert](../imgs/datto.png?raw=true "DatTo Settings")


The `datTo` CHOP should now have 3 channels each representing one of the sensor values. And can be processed or exported to other parts of TD

Please see the [parsing page](parsing.md) for a few basic strategies and examples of how to apply CHOP data to other parts of TouchDesigner.

## Control Arduino Physical Outputs with TD

Earlier in the tutorial we [sent a byte ](#send-a-byte-to-arduino) to arduino as a way of controling the flow of communication. 

This byte was just a constant value of `1`, however we can edit our circuit, code, and network to use that byte of information to control a physical output, in this case, the brightness of an LED.

### Add an LED to the Arduino Circuit
Connect a 220 ohm (anything from 220 - 1K Ω should work) to digital pin 2 on the Arduino.  

Note: Any PWM pin on the arduino. PWM pins are marked with `~` symbol. See the [Arduino Nano 33 IoT Pinout Diagram](https://docs.arduino.cc/resources/pinouts/ABX00027-full-pinout.pdf) for full detail.  

![2 Pots Light Sensor LED](../imgs/2potLightSensorLED.png?raw=true "2 way communication circuit")

### Update the Arduino code

Make a global brightness variable for your LED:

    int brightness = 255;

In the setup configure pin 2 as an output using the `pinMode()` function

    pinMode(2 , OUTPUT);

In the loop, assign the incoming value to the `brightness` variable. This is value TouchDesigner is sending to arduino. Use the `analogWrite()` function and the `brightness` variable to change the LED's brightness. 

Note: `analogWrite()` only accepts values from 0-255. At 0 the LED will be off at 255 the LED will appear at full brightness. In the next section we'll update TouchDesigner to send the correct range of values.


Full Code below:

    int sensor1;
    int sensor2;
    int sensor3;
    int brightness = 255;

    void setup() {
        Serial.begin(9600);
        pinMode(2 , OUTPUT);
    }

    void loop() {
        sensor1 = analogRead(A0);
        sensor2 = analogRead(A1);
        sensor3 = analogRead(A2);

        if (Serial.available() > 0) {
            int incoming = Serial.read();
           
            brightness = incoming; // transfer value from TD
           
            Serial.print(sensor1);
            Serial.print(",");
            Serial.print(sensor2);
            Serial.print(",");
            Serial.print(sensor3);
            Serial.print('\n');
        }

        analogWrite(2 , brightness); // use brightness to fade LED

    }


### Add a Control Slider

Navigate to the `constant` and `null` CHOPs we were using before. 

![Constant null](../imgs/constantNull.png?raw=true "Serial callbacks Dat")

We are going to create a slider to replace the `constant` and send changing values to Arduino.

![Slider](../imgs/mathRangeSliderConnected.png?raw=true "Slider to Select")


Add a `slider` COMP and connect it to a `select` CHOP

![Slider](../imgs/slider.png?raw=true "Slider to Select")

Make the slider active, move the slider and look at the range of values it outputs
![Slider](../imgs/slider.gif?raw=true "Slider gif")

Also notice how the channel name is different from the other CHOP channesl.  Our other CHOPs generate channels named `chan1`,`chan2`, etc, but the slider has generated a channed with the name `v1`. 

To avoid any conflicts with our previous set up, rename the channel in the `select` tab of the `select` CHOP

![Select rename](../imgs/selectRename.png?raw=true "Select rename")

By default TouchDesigner uses normalized values (between 0.0 - 1.0) for nearly all operators.  This means if we want to use the slider to control the brightness of the LED we need to map the slider's range of 0-1 to 0-255 so that it can control the full range of brightness of the LED.

Connect the `select` CHOP to a `math` CHOP

![Slider](../imgs/mathRangeFull.png?raw=true "Slider to Select")

Change the parameters of the `math` CHOP.  First navigate to the `OP` Tab, and look for the `Integer` dropdown.  Select `Round`

![Slider](../imgs/mathInt.png?raw=true "Slider to Select")

Navigate to the `Range` Tab and set the following fields:

 ```From Range:``` minimum to 0, maximum to 1.  This is the incoming range of the slider.

```To Range: 0 255``` minimum to 0, maximum to 255.  This is the outgoing mapped value that we want to send to Arduino.

![Slider](../imgs/mathRange.png?raw=true "Slider to Select")

Make slider active and observe how the range changes:

![Slider](../imgs/mathSlider.gif?raw=true "Slider to Select")

Lastly connect the `math` CHOP to the `null` CHOP from earlier.  Make sure the `constant`CHOP from before is also disconnected.

![Slider to Arduino](../imgs/mathToNull.png?raw=true "Slider to Arduino")


If all is set up correctly you should be able to use the button to connect and disconnect with the arduino.  You should see the values being parsed. And you should be able to use the slider to change to change the brightness of the LED all at the same time.


## Related Resources
* [ITP Physical Computing Site](https://itp.nyu.edu/physcomp/)
* [ITP Two Way Serial wth p5js](https://itp.nyu.edu/physcomp/labs/labs-serial-communication/lab-two-way-duplex-webserial-communication/)




[Just the Docs]: https://just-the-docs.github.io/just-the-docs/
[GitHub Pages]: https://docs.github.com/en/pages
[README]: https://github.com/just-the-docs/just-the-docs-template/blob/main/README.md
[Jekyll]: https://jekyllrb.com
[GitHub Pages / Actions workflow]: https://github.blog/changelog/2022-07-27-github-pages-custom-github-actions-workflows-beta/
[use this template]: https://github.com/just-the-docs/just-the-docs-template/generate
