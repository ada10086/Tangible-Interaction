//incremeting and decremeting rotary encoder reading by 1

#include <Encoder.h>

Encoder myEnc(7, 8);
long oldPosition  = -999;

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
  myEnc.write(0);
}


void loop() {
  long newPosition = myEnc.read()/4;

  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition); 
  }
}
