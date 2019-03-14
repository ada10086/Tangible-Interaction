#include <Keyboard.h>

// use this option for OSX:
char keyLeft = KEY_LEFT_ARROW;
char keyRight = KEY_RIGHT_ARROW;
char keyUp = KEY_UP_ARROW;
char keyDown = KEY_DOWN_ARROW;
char keyShift = KEY_LEFT_SHIFT;

int period = 0;
bool isWaitingForKeyRelease = false;
char keyToRelease;
unsigned long int waitStartTime;


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Used for software SPI
#define LIS3DH_CLK 12
#define LIS3DH_MISO 10
#define LIS3DH_MOSI 8
// Used for hardware & software SPI
#define LIS3DH_CS 7


// software SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

void setup() {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

  Serial.begin(9600);
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!

  
  pinMode(6, INPUT_PULLUP); //gas
  pinMode(5, INPUT_PULLUP);  //jump
  pinMode(4,INPUT_PULLUP); //boost
}

void loop() {
  int gas = digitalRead(6);
  int jump = digitalRead(5);
  int boost = digitalRead(4);

  sensors_event_t event;
  lis.getEvent(&event);
  Serial.print("\t\tX: "); Serial.println(event.acceleration.x); 

 // keyboard                
  if (gas == 1) {
    Keyboard.press(keyUp);
  } else {
    Keyboard.release(keyUp);
  }
  
  if (boost == 1) {
    Keyboard.press(keyShift); 
  } else {
    Keyboard.release(keyShift);
  }

  if (jump == 1) {
    Keyboard.write(32);         
//    delay(50);
  }
  
  int t = 0.5 * pow(1.75, abs(event.acceleration.x)) + 40;     //40-174

  if (event.acceleration.x > 2) {
    Keyboard.press(keyLeft);
    delay(t);
    Keyboard.release(keyLeft);
    delay(40);
  }
  else if (event.acceleration.x < -2) {
    Keyboard.press(keyRight);
    delay(t);
    Keyboard.release(keyRight);
    delay(40);
  }
  else {
    Keyboard.release(keyLeft);
    Keyboard.release(keyRight);
  }

  ////    // (-2, 2) neutral key release, (-5, -2), (2 - 5)small turn key press + release, (-10,-5)(5,10) sharp turn key hold
//    if (event.acceleration.x > 2 && event.acceleration.x < 5) {
//      Keyboard.press(keyLeft);
//      delay(40);
//      Keyboard.release(keyLeft);
//      delay(40);
//    } else if (event.acceleration.x >= 5) {
//      Keyboard.press(keyLeft);
//      delay(80);
//      Keyboard.release(keyLeft);
//      delay(40);
//    }
//    else if (event.acceleration.x > -5 && event.acceleration.x < -2) {
//      Keyboard.press(keyRight);
//      delay(40);
//      Keyboard.release(keyRight);
//      delay(40);
//    } else if (event.acceleration.x <= -5) {
//      Keyboard.press(keyRight);
//      delay(80);
//      Keyboard.release(keyRight);
//      delay(40);
//    }
//    else {
//      Keyboard.release(keyLeft);
//      Keyboard.release(keyRight);
//    }
}
