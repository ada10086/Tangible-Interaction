//------------------------------------------------------
#include <LiquidCrystal.h>
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//----------------------------------------------------

#include <EasyButton.h>
#define BUTTON_PIN 6 //pull up resistance
EasyButton button(BUTTON_PIN);

int counter = 0 ; //for setting hour, minute, second, month, day

//----------------------------------------------------

#include <Encoder.h>
Encoder myEnc(7, 8);
int oldPos  = -999;
int newPos;
int startValue = 6; //HH,MM,SS,Month,Day value before change, set arbitrarily
byte setValue;

//------------------------------------------------------

#include <RTCZero.h>
RTCZero rtc;      // instance of the realtime clock

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 16;

/* Change these values to set the current initial date */
const byte day = 1;
const byte month = 1;
const byte year = 19;

//------------------------------------------------------


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);    // set up the LCD's number of columns and rows

  rtc.begin();
  rtc.setDate(day, month, year);
  rtc.setTime(hours, minutes, seconds);

  myEnc.write(0);    //reset to position 0

  button.begin();  //initialize button
  button.onPressed(onPressedCallBack);
}

void loop() {
  button.read();
  newPos = myEnc.read() / 4;  //newPos = change of startValue, increment/decrement by 1 per click

  if (counter == 0) {    //display time

    lcd.setCursor(0, 0);
    lcd.print("The time is:");
    lcd.setCursor(0, 1);
    print2digits(rtc.getMonth());
    lcd.print("/");
    print2digits(rtc.getDay());
    lcd.print("   ");
    print2digits(rtc.getHours());
    lcd.print(":");
    print2digits(rtc.getMinutes());
    lcd.print(":");
    print2digits(rtc.getSeconds());

  } else {      //set time

    //setDateTime (int _counter, String _setxx, int _cursorDigit, int _minValue, int _maxValue)
    setDateTime (1, "set month   ", 0,  1,  12);
    setDateTime (2, "set day     ", 3,  1,  31);
    setDateTime (3, "set hours   ", 8,  0,  23);
    setDateTime (4, "set minutes ", 11,  0,  59);
    setDateTime (5, "set seconds ", 14,  0,  59);
    
  }
}


void print2digits(int number) {
  if (number < 10) {
    lcd.print("0"); // print a 0 before if the number is < than 10
  }
  lcd.print(number);
}

void onPressedCallBack() {       //press when finish setting month, day, hour, minute, or second
  Serial.println("Button has been pressed!");
  switch (counter) {
    case 0:
      counter += 1;
      Serial.println("Setting mode");
      break;
    case 1:
      counter += 1;
      rtc.setMonth(setValue);
      Serial.println("Month set");
      break;
    case 2:
      counter += 1;
      rtc.setDay(setValue);
      Serial.println("Day set");
      break;
    case 3:
      counter += 1;
      rtc.setHours(setValue);
      Serial.println("Hours set");
      break;
    case 4:
      counter += 1;
      rtc.setMinutes(setValue);
      Serial.println("Minutes set");
      break;
    case 5:
      rtc.setSeconds(setValue);
      Serial.println("Seconds set, setting finished");
      counter = 0;
      break;
  }
  myEnc.write(0);  //rest enc position to 0 for the next setting
  Serial.println(counter);
}


void setDateTime (int _counter, String _setxx, int _cursorDigit, int _minValue, int _maxValue) {
  if (counter == _counter) {
    //            lcd.setCursor(1,1);
    //            lcd.blink();  // blink cursor (0,1)
    lcd.setCursor(0, 0);
    lcd.print(_setxx);

    lcd.setCursor(_cursorDigit, 1);
    int startValueArray[] = {rtc.getMonth(), rtc.getDay(), rtc.getHours(), rtc.getMinutes(), rtc.getSeconds()};
    startValue = startValueArray[_counter - 1];
    if (newPos != oldPos) {
      if ((startValue + newPos) >= _minValue && (startValue + newPos) <= _maxValue) {
        oldPos = newPos;
        setValue = startValue + newPos;
        print2digits(setValue);
      } else {
        setValue = startValue + oldPos;
        print2digits(setValue);
        myEnc.write(oldPos * 4);
      }
    }
  }
}
