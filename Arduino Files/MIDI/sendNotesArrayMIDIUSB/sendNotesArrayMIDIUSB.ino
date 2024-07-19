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

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pButtons[i] = digitalRead(buttonPins[i]);
    buttons[i] = digitalRead(buttonPins[i]);
  }
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
        Serial.println(notes[i]);
      }
      if (buttons[i] == 1) {
        noteOff(0, notes[i], 100);
        MidiUSB.flush();
        Serial.println(notes[i]);
      }
      pButtons[i] = buttons[i];
    }
  }

  // controlChange(0, 10, 65); // Set the value of controller 10 on channel 0 to 65
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