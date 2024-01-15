---
title: Serial to TD Call and Response Digital
layout: default
nav_order: 4
parent: Serial Communication
---

# Serial Call and Response Example

If we want Arduino and Touchdesigner exchange messages back and forth, one way to exchange information is with a Call and Response. Essentially we are telling them to take turns exchanging information.  We'll write code that tells arduino to print sensor values to Touchdesigner, but only if Touchdesigner has first sent a byte to arduino. We later tell Touchdesigner to respond to Arduino, writing out bytes if it has received values.  In this way they will exchange information (bytes) evenly without either ever sending before the other is ready to receive. 

This example shows the necessary python scripts, DATs, and CHOPs to set up Serial Communication with an Arduino. This example uses the the free version of TouchDesigner 2022.31030 and the Arduino Nano 33 IoT.

## Arduino Circuit:
For this example I'm using a circut with 3 pushbuttons as my digital inputs.  This can be adjusted for any number of digital inputs. The circut uses the following components:
 - Arduino Nano 33 IoT
 - Solderless breadboard
 - 3 pushbutton switches
 - Hookup wire / jumper wire


![Breadboard of Arduino Nano 33 IoT with Buttons connected to D2, D3, D4](../imgs/arduinoButtons.png?raw=true "Breadboard Switches")

## Arduino Code:
If we want Arduino and Touchdesigner exchange messages back and forth. We'll write code that tells arduino to print sensor values to Touchdesigner, but only if Touchdesigner has first sent a byte to arduino. We later tell Touchdesigner to respond to Arduino, only if it has received values. In this way they will exchange information evenly without either ever sending before the other is ready to receive.


To begin, let's set up variables for our sensor values:

    int sensor1;
    int sensor2;
    int sensor3;

Next inside of your ```setup()``` Use ```Serial.begin()``` to start communication at a baud rate of 9600.  Remember the baud rate, it will be used later in your touchdesigner network.  
    
    void setup(){
        Serial.begin(9600);
        pinMode(2, INPUT_PULLUP);
        pinMode(3, INPUT_PULLUP);
        pinMode(4, INPUT_PULLUP);
    }
    
Inside of your ```loop()``` read your sensors using ```analogRead()```:
    
    void loop(){
        sensor1 = digitalRead(2);
        sensor2 = digitalRead(3);
        sensor3 = digitalRead(4);
    }

After reading your sensors (and while still inside the ```loop()```). Create and `if` statement that only executes if `Serial.available()` is greater than `0`.  

This means whatever you put inside of this statement will only execute if the Arduino has received information on the serial port (from Touchdesigner).

    void loop(){
        sensor1 = digitalRead(2);
        sensor2 = digitalRead(3);
        sensor3 = digitalRead(4);
    
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

    void setup(){
        Serial.begin(9600);
        pinMode(2, INPUT_PULLUP);
        pinMode(3, INPUT_PULLUP);
        pinMode(4, INPUT_PULLUP);
    }

    void loop() {
        sensor1 = digitalRead(2);
        sensor2 = digitalRead(3);
        sensor3 = digitalRead(4);

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

The `convert` parameters define how you will reformat the incoming values.  Make sure that you are converting to a table, this will take values and split them into individual cells.  Input a comma ',' in the `split cells at` field to define which delimiters you are using in between values.  You do not need to input '\n', the `serial` DAT takes care of the ending control character.

![Serial To Convert](../imgs/convertSettings.png?raw=true "Convert Settings")

Once we have the values separated, the output of your `convert` to a `DAT to` CHOP. This will turn the split DAT values into a channel (CHOP) format.  Once you have values in a CHOP you can then export and connect as we have in other examples.

![Serial To Convert](../imgs/datto.png?raw=true "dat to CHOP")

In order to properly sort the information from the `convert` chop, configure the `datto` CHOP parameters as follows:
- `Select Rows` : by Index
- `Select Cols` : by Index
- `Start Col Index` : 0
- `End Col Index` : 2  <-- (if you have a different amount of sensors use the last index of what you actually have)
- 'Output' : Channel per Column
- `First Row is` : Values
- `First Column is`: Values

![Serial To Convert](../imgs/dattodetail.png?raw=true "Convert Settings")

