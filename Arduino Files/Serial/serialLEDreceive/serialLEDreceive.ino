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