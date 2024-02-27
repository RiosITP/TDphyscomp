int currentButtonState;
int prevButtonState;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  // set an initial reading to compare with current reading
  prevButtonState = digitalRead(2);
}

void loop() {
  // read the button's current state
  currentButtonState = digitalRead(2);
  // compare previous state to current state.
  // if there is a difference, it means it has changed state
  if (prevButtonState != currentButtonState) {
    // if its currently zero, its been pressed
    if (currentButtonState == 0) {
      Serial.println("Pressed");
    }
    // if its currently one, its been released
    if (currentButtonState == 1) {
      Serial.println("Released");
    }
    // update the previous value to reflect the change
    prevButtonState = currentButtonState;
  }
}