//setting month value 1-12 with rotary encoder by incrementing/decrementing 1 per click

#include <Encoder.h>

Encoder myEnc(7, 8);
long oldPos  = -999;
int startValue = 6;   //June, month value before change, set arbitrarily 1-12

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
  myEnc.write(0);   //reset to position 0
}


void loop() {
  //start rotate
  long newPos = myEnc.read() / 4; //newPos = change of startValue, increment/decrement by 1 per click

  //method 1: reading cannot go over boundaries
  //  if (newPos != oldPos) {
  //    if ((startValue + newPos) > 0 && (startValue + newPos) < 13) { //if within boundary
  //      oldPos = newPos;
  //      Serial.println(startValue + newPos); //output month 1 - 12
  //    } else { //if reaches boundary
  //      Serial.println(startValue + oldPos);   //keep outputing month 1 or 12
  //      myEnc.write(oldPos * 4);  //keep resetting to oldPos(last change of startValue) before next click
  //    }
  //  }

  //method 2: when over boundaries, go back to min value
  if (newPos != oldPos) {
    oldPos = newPos;
    if ((startValue + newPos) > 12) {
      newPos -= 12;
      myEnc.write((newPos - 12) * 4);
    } else if ((startValue + newPos) < 1) {
      myEnc.write((newPos + 12) * 4);
      newPos += 12;
    }
    Serial.println(startValue + newPos); //output month 1 - 12
  }
}
