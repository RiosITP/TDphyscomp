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

/// LIBRARY FUNCTIONS
// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}

// GLOBAL VARIABLES

int pButton1 = 1;
int pButton2 = 1;
int pButton3 = 1;


void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pButton1 = digitalRead(2);
  pButton2 = digitalRead(3);
  pButton3 = digitalRead(4);
}

void loop() {

  int button1 = digitalRead(2);
  int button2 = digitalRead(3);
  int button3 = digitalRead(4);

  // state change detection for button 1
  if (pButton1 != button1) {
    // if the button is pressed, send a note On
    if (button1 == 0) {
      // send a 48 note (Middle C) on over channel 1 with 100 velocity
      // Note: (arduino is 0 indexed, so arduino channel 0 is the DAW's channel 1)
      noteOn(0, 48, 100);
      MidiUSB.flush();
    }
    // if the button is released, send a note Off
    if (button1 == 1) {
      // turn off that note if you let go of the button
      noteOff(0, 48, 100);
      MidiUSB.flush();
    }
    // update button status to avoide continuously sending MIDI notes
    pButton1 = button1;
  }

  //repeat for the other buttons
  // state change detection for button 2
  if (pButton2 != button2) {
    if (button2 == 0) {
      // send a 52 (note: E)
      noteOn(0, 52, 100);
      MidiUSB.flush();
    }
    if (button2 == 1) {
      noteOff(0, 52, 100);
      MidiUSB.flush();
    }
    pButton2 = button2;
  }

  if (pButton3 != button3) {
    if (button3 == 0) {
      // send a 55 (note: G)
      noteOn(0, 55, 100);
      MidiUSB.flush();
    }
    if (button3 == 1) {
      noteOff(0, 55, 100);
      MidiUSB.flush();
    }
    pButton3 = button3;
  }
  // controlChange(0, 10, 65); // Set the value of controller 10 on channel 0 to 65
}