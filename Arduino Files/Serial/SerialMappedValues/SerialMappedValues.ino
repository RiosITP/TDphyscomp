void setup() {
  Serial.begin(9600);
}

void loop() {
  // read potentiometer
  int potentiometer = analogRead(A0); 

  int mappedValues = map(potentiometer, 0, 1023, 0, 255);
  // print values in the serial monitor
  Serial.println(potentiometer);
}
