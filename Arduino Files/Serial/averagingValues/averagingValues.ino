int potentiometerAverage;
int avgSamples = 25;

void setup() {
  Serial.begin(9600);
  int prevPot = analogRead(A0);
}

void loop() {
  // reset total each loop
  int total = 0;

  // for loop to add up multiple readings each loop
  for (int i = 0; i < avgSamples; i++) {
    // add to the total
    total += analogRead(A0);
  }
  // take the average of the total
  potentiometerAverage = total / avgSamples;
  // print values in the serial monitor
  Serial.println(potentiometerAverage);
}
