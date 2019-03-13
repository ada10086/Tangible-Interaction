#include <Keyboard.h>

// use this option for OSX:
char keyLeft = KEY_LEFT_ARROW;
char keyRight = KEY_RIGHT_ARROW;
char keyUp = KEY_UP_ARROW;
char keyDown = KEY_DOWN_ARROW;
char keyShift = KEY_LEFT_SHIFT;

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



#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
 
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}



// software SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

void setup(void) {
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
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");

  
  pinMode(6, INPUT_PULLUP); //gas
  pinMode(5, INPUT_PULLUP);  //jump
  pinMode(4,INPUT_PULLUP); //boost
}

void loop() {
  int gas = digitalRead(6);
  int jump = digitalRead(5);
  int boost = digitalRead(4);

  lis.read();      // get X Y and Z data at once
  // Then print out the raw data
//  Serial.print("X:  "); Serial.print(lis.x); 
//  Serial.print("  \tY:  "); Serial.print(lis.y); 
//  Serial.print("  \tZ:  "); Serial.print(lis.z); 

  /* Or....get a new sensor event, normalized */ 
  sensors_event_t event;
  lis.getEvent(&event);
//    Serial.println(boost);
//  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tX: "); Serial.print(event.acceleration.x); 
//  Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
//  Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
//  Serial.println(" m/s^2 ");
//  Serial.println();

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
  
  int t = 0.5 * pow(1.75, abs(event.acceleration.x)) + 40;     //40-174

  if (event.acceleration.x > 2) {
    Keyboard.press(keyLeft);
//    delay(t);
    Keyboard.release(keyLeft);
//    delay(40);
  }
  else if (event.acceleration.x < -2) {
    Keyboard.press(keyRight);
//    delay(t);
    Keyboard.release(keyRight);
//    delay(40);
  }
  else {
    Keyboard.release(keyLeft);
    Keyboard.release(keyRight);
  }
}
