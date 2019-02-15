//play multiple notes simultaneously
#include <MIDIUSB.h>
#define PIN_FlexSensor0 A0
#define PIN_FlexSensor1 A1
//#define PIN_FlexSensor2 A2;
//#define PIN_FlexSensor3 A3;
//#define PIN_FlexSensor4 A4;

// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}

class Finger {
  private:
    int flexSensorPin;
    int finger;
    int lastFinger;
    int flexSensor;
    byte midiNote;
  public:
    Finger(int flexSensorPin, byte midiNote) {
      this -> flexSensorPin = flexSensorPin;
      this -> midiNote = midiNote;
    }
    void triggerRelease() {
      flexSensor = analogRead(flexSensorPin);

      //maps sensor reading to 1 and 0
      if (flexSensor < 300) {   //(Flex sensor reading 500straight-200 right angle)
        finger = 1;
      } else {
        finger = 0;
      }
      Serial.println(finger);

      // flex sensor plays the note or stops it:
      if (finger != lastFinger) {
        delay(5);
        if (finger == 1) {
          midiCommand(0x90, midiNote, 0x7F);
        } else {
          midiCommand(0x80, midiNote, 0);
        }
        lastFinger = finger;
      }
    }
};

Finger finger0(PIN_FlexSensor0, 0x36);
Finger finger1(PIN_FlexSensor1, 0x38);


void setup() {
  Serial.begin(9600);

}

void loop() {
  int xSensor = analogRead(A5);
  delay(1);
  int x = map(xSensor, 0, 1023, 0, 3) - 1;

  // x is the pitch bend axis:
  if (x != 0) {
    int pitchBendSensor = xSensor << 5;          // shift so top bit is bit 14
    byte msb = highByte(pitchBendSensor);        // get the high bits
    byte lsb = lowByte(pitchBendSensor >> 1);    // get the low 8 bits
    bitWrite(lsb, 7, 0);                         // clear the top bit of the low byte
    midiCommand(0xE0, lsb, msb);                 // send the pitch bend message
  }

  finger0.triggerRelease();
  finger1.triggerRelease();

}
