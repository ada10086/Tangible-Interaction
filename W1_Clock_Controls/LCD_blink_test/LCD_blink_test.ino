//LCD displays "Hello World" + blink test

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//const int rs = 12, en =11, d4 = 5, d5 = 6, d6 = 3, d7 =2;   
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number 20
  lcd.print(20);
  
  lcd.setCursor(0,1);
  lcd.blink();  // blink cursor (0,1)
  lcd.setCursor(1,1);
  lcd.blink();  //blink cursor(1,1)
}
