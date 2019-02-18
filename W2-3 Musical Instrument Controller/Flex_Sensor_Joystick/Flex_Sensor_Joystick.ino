#include <MIDIUSB.h>
int flexSensorPin0 = A0;

int finger0 = 0;
int lastFinger0 = 0;

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

  int flexSensor0 = analogRead(flexSensorPin0);

  //maps sensor reading to 1 and 0
  if (flexSensor0 < 170) {   //(Flex sensor reading 300straight-150 right angle)
    finger0 = 1;
  } else {
    finger0 = 0;
  }
  Serial.println(finger0);

  // flex sensor plays the note or stops it:
  if (finger0 != lastFinger0) {
    delay(100);
    if (finger0 == 1) {
      midiCommand(0x90, 0x45, 0x7F);
    } else {
      midiCommand(0x80, 0x45, 0);
    }
    lastFinger0 = finger0;
  }

}

// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}
