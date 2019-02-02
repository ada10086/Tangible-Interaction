#include <LiquidCrystal.h>
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


#include <RTCZero.h>
RTCZero rtc;
/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 16;

/* Change these values to set the current initial date */
const byte day = 15;
const byte month = 6;
const byte year = 15;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  rtc.begin();
  rtc.setTime(hours, minutes, seconds);
  //rtc.setDate(day, month, year);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  print2digits(rtc.getHours());
  lcd.print(":");
  print2digits(rtc.getMinutes());
  lcd.print(":");
  print2digits(rtc.getSeconds());
  
  delay(1000);

}

void print2digits(int number) {
  if (number < 10) {
    lcd.print("0"); // print a 0 before if the number is < than 10
  }
  lcd.print(number);
}
