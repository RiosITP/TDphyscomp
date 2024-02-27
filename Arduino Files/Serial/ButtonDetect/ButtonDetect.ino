int buttonState;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  buttonState = digitalRead(2);
}

void loop() {
  // read the button's current state
  buttonState = digitalRead(2);
  // print values in the serial monitor
  Serial.println(buttonState);
}