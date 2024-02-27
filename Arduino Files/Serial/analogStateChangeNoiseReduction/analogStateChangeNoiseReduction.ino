int noise = 15;
int prevPot;

void setup() {
  Serial.begin(9600);
  int prevPot = analogRead(A0);
}

void loop() {
  // reset total each loop
  int pot = analogRead(A0);

  if (abs(prevPot - pot) > noise) {
    Serial.println(pot);
    prevPot = pot;
  }
}
