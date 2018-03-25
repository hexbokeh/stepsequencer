#include <EEPROM.h>
#include <MIDI.h>
//#include<iostream>

//2018_03_08_07_02_001

/*
   Drum Sequencer
   Multidimentional
*/

int buttonPins[6] = {36, 35, 34, 33, 14, 15};
int ledPins[4] = {A8, A7, A6, A5};
boolean lastButtonStates[6];
boolean buttonStates[6];

boolean stepStates[3][4] =
{
  {true, true, false, false},
  {false, true, true, false},
  {false, false, true, true},
};

int midiNotes[4] = {60 - 24, 62 - 24, 68 - 24}; // change these values later.
int tempo = 0;
//int sequenceDefaultBrightness;

int manualSwitch = 31;

int currentChannel; // = 0 by default.
int ledChannelIndicators [3] = {2, 3, 4};

int currentStep = 0;
unsigned long lastStepTime = 0;

int sustainTime = 50; // in milliseconds.
unsigned long lastSustainTime;

void setup()
{
  for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++)
  {
    pinMode(buttonPins[i], INPUT);
  }
  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }
  for (int i = 0; i < sizeof(ledChannelIndicators) / sizeof(ledChannelIndicators[0]); i++)
  {
    pinMode(ledChannelIndicators[i], OUTPUT);
  }

  loadFromEEPROM(); // from values set when last connected to power.
  passArray(stepStates);

  pinMode(manualSwitch, INPUT);

  // just for when power is restored or new code is uploded.
  digitalWrite(ledChannelIndicators[0], HIGH);
}

void loop()
{

  //debug
  tempo = map(analogRead(A13), 0, 1023, 5, 1000);
  updateChannel();
  checkButtons(); // put this after sequence();?
  sequence();
  setLEDS();
}

void sequence()
{

  if (digitalRead(manualSwitch) == HIGH)
  {
    // tempo = map(analogRead(A13), 0, 1023, 5, 1000);
  }
  // debug
  else
  {
    tempo = 100; // some fixed value.
  }

  if (millis() > lastStepTime + tempo)
  {
    //if its time to go to the next step...

    lastStepTime = millis(); // I suspect the placement of this has something to do with timing variations.
    for (int i = 0; i < 4; i++)
    {
      usbMIDI.sendNoteOff(midiNotes[i], 0, 1);// should be i
    }

    currentStep++;         //increment to the next step
    currentStep %= 4;

    //set lastStepTime to the current time

    //setLEDs();

    for (int i = 0; i < sizeof(midiNotes) / sizeof(midiNotes[0]); i++)
    {
      if (stepStates[i][currentStep] == HIGH)// should be i
      {
        //        Serial.println("about to play MIDI note");
        playMIDINote(i); // should be i
      }
    }



    //debug
    Serial.print(tempo);
    Serial.print(" ");
    //    Serial.println();


    // test array function
    passArray(stepStates);
  }
}

void setLEDS()
{
  for (int i = 0; i < 4; i++)
  {
    if (i == currentStep) // if this led is part of the sequence, turn the light on.
    {
      //digitalWrite(ledPins[i], HIGH);
      analogWrite(ledPins[i], map(analogRead(A19), 0, 1023, 0, 255));
      //Serial.println("i = currentStep");
    }
    if (stepStates[currentChannel][i] == HIGH) // or should it be 'else if'?
    {
      analogWrite(ledPins[i], 255);
      //digitalWrite(ledPins[i], HIGH);
    }
    else
    {
      analogWrite(ledPins[i], 0);
      //digitalWrite(ledPins[i], LOW);
    }
  }
}

void playMIDINote(int noteVal)
{
  int tempStep = currentStep;
  lastSustainTime = millis();
  usbMIDI.sendNoteOn(midiNotes[noteVal], 100, 1); // maybe set the last to 1, for first MIDI channel?
  //  Serial.println("note on");

  if (millis() > lastSustainTime + tempo) // or +sustainTime
  {
    lastSustainTime = millis();
    usbMIDI.sendNoteOff(midiNotes[noteVal], 0, 1);
    Serial.println("note off");
  }
}
