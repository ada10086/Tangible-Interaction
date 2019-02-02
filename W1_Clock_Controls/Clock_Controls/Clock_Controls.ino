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

//------------------------------------------------------

#include <RTCZero.h>
RTCZero rtc;      // instance of the realtime clock
//int thisSecond;   // variable to look for seconds chang

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 16;

/* Change these values to set the current initial date */
const byte day = 1;
const byte month = 1;
const byte year = 19;

byte setHH;   //0-23
byte setMM;   //0-59
byte setSS;   //0-59
byte setmon;  //1-12
byte setday;  //1-31
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
  newPos = myEnc.read() / 4; //newPos = change of startValue, increment/decrement by 1 per click

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


  }

  if (counter == 3) {    //set hour
    //        lcd.setCursor(1,1);
    //        lcd.blink();  // blink cursor (0,1)
    //        lcd.setCursor(0,1);
    //        lcd.blink();  //blink cursor(1,1)
    lcd.setCursor(0, 0);
    lcd.print("set HH      ");

    lcd.setCursor(8, 1);  //set cursor for HH
    startValue = rtc.getHours();
    if (newPos != oldPos) {
      if ((startValue + newPos) >= 0 && (startValue + newPos) < 24) { //if within boundary
        oldPos = newPos;
        print2digits(startValue + newPos); //output hour 0 - 23
        setHH = startValue + newPos;   //store value to setHH
      } else { //if reaches boundary
        print2digits(startValue + oldPos);   //keep outputing hour 0 or 23
        setHH = startValue + oldPos;    //store value to setHH
        myEnc.write(oldPos * 4);  //keep resetting to oldPos(last change of startValue) before next click
      }
    }
  }

  if (counter == 4) { //set min
    //        lcd.setCursor(1,1);
    //        lcd.blink();  // blink cursor (0,1)
    //        lcd.setCursor(0,1);
    //        lcd.blink();  //blink cursor(1,1)
    lcd.setCursor(0, 0);
    lcd.print("set MM      ");

    lcd.setCursor(11, 1);
    startValue = rtc.getMinutes();
    if (newPos != oldPos) {
      if ((startValue + newPos) >= 0 && (startValue + newPos) < 60) {
        oldPos = newPos;
        print2digits(startValue + newPos);
        setMM = startValue + newPos;
      } else {
        print2digits(startValue + oldPos);
        setMM = startValue + oldPos;
        myEnc.write(oldPos * 4);
      }
    }
  }

  if (counter == 5) {   //set sec
    //        lcd.setCursor(1,1);
    //        lcd.blink();  // blink cursor (0,1)
    //        lcd.setCursor(0,1);
    //        lcd.blink();  //blink cursor(1,1)
    lcd.setCursor(0, 0);
    lcd.print("set SS      ");

    lcd.setCursor(14, 1);
    startValue = rtc.getSeconds();
    if (newPos != oldPos) {
      if ((startValue + newPos) >= 0 && (startValue + newPos) < 60) {
        oldPos = newPos;
        print2digits(startValue + newPos);
        setSS = startValue + newPos;
      } else {
        print2digits(startValue + oldPos);
        setSS = startValue + oldPos;
        myEnc.write(oldPos * 4);
      }
    }
  }

  if (counter == 1) {
    //        lcd.setCursor(1,1);
    //        lcd.blink();  // blink cursor (0,1)
    //        lcd.setCursor(0,1);
    //        lcd.blink();  //blink cursor(1,1)
    lcd.setCursor(0, 0);
    lcd.print("set month   ");

    lcd.setCursor(0, 1);
    startValue = rtc.getMonth();
    if (newPos != oldPos) {
      if ((startValue + newPos) >= 1 && (startValue + newPos) < 13) {
        oldPos = newPos;
        print2digits(startValue + newPos);
        setmon = startValue + newPos;
      } else {
        print2digits(startValue + oldPos);
        setmon = startValue + oldPos;
        myEnc.write(oldPos * 4);
      }
    }
  }

    if (counter == 2) {
    //        lcd.setCursor(1,1);
    //        lcd.blink();  // blink cursor (0,1)
    //        lcd.setCursor(0,1);
    //        lcd.blink();  //blink cursor(1,1)
    lcd.setCursor(0, 0);
    lcd.print("set day     ");

    lcd.setCursor(3, 1);
    startValue = rtc.getDay();
    if (newPos != oldPos) {
      if ((startValue + newPos) >= 1 && (startValue + newPos) < 32) {
        oldPos = newPos;
        print2digits(startValue + newPos);
        setday = startValue + newPos;
      } else {
        print2digits(startValue + oldPos);
        setday = startValue + oldPos;
        myEnc.write(oldPos * 4);
      }
    }
  }
}


void print2digits(int number) {
  if (number < 10) {
    lcd.print("0"); // print a 0 before if the number is < than 10
  }
  lcd.print(number);
}


void onPressedCallBack() {   //press when finish setting hour, minute, or second
  Serial.println("Button has been pressed!");
  if (counter == 0) {
    counter += 1;
    Serial.println("Setting mode");    //counter = 1 to set hh
  } else if (counter == 1) {
    counter += 1;
    rtc.setMonth(setmon);
    Serial.println("HHset");     //counter = 2 to set mm
  } else if (counter == 2) {
    counter += 1;
    rtc.setDay(setday);
    Serial.println("MMset");     //counter = 3 to set ss
  } else if (counter == 3) {
    counter += 1;
    rtc.setHours(setHH);
    Serial.println("SSset");     //counter = 0 to display mode
  } else if (counter == 4) {
    counter += 1;
    rtc.setMinutes(setMM);
    Serial.println("Monthset");
  } else if (counter == 5) {
    rtc.setSeconds(setSS);
    Serial.println("Dayset");
    counter = 0;
  }
  myEnc.write(0);  //rest enc position to 0 for the next setting
  Serial.println(counter);
}
