/*
 * MIDIUSB_test.ino
 *
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 * Modified by David Rios 2024
 */

#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <frequencyToNote.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>


// GLOBAL VARIABLES

int pButtons[3];
int buttons[3];
int buttonPins[3] = { 2, 3, 4 };
int notes[] = { 62, 65, 69 };

int pKnob;
int knob;
int threshold = 3;

int sensorSamples = 25;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pButtons[i] = digitalRead(buttonPins[i]);
    buttons[i] = digitalRead(buttonPins[i]);
  }
  pKnob = analogRead(A0);
  knob = analogRead(A0);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    buttons[i] = digitalRead(buttonPins[i]);
  }
  for (int i = 0; i < 3; i++) {
    if (pButtons[i] != buttons[i]) {
      if (buttons[i] == 0) {
        noteOn(0, notes[i], 100);
        MidiUSB.flush();
        Serial.print("note on: ");
        Serial.println(notes[i]);
      }
      if (buttons[i] == 1) {
        noteOff(0, notes[i], 100);
        MidiUSB.flush();
        Serial.print("note off: ");
        Serial.println(notes[i]);
      }
      pButtons[i] = buttons[i];
    }
  }
  int sum = 0;
  for(int i=0; i<sensorSamples;i++){
    sum += analogRead(A0);
  }
  int avg = sum/sensorSamples; // average of sensor readings
  knob = map(avg,0,1023,0,127);


  if (abs(pKnob - knob) > threshold) {
    controlChange(1, 20, knob);
    pKnob = knob;
   // Serial.print("Controller 1: ");
   // Serial.println(knob);
  }
}

/// LIBRARY FUNCTIONS

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