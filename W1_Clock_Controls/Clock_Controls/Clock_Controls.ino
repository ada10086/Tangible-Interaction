//attempt to combine LCD, encoder, RTC, button 
//issue: button press once would enter onPressedForDuration(long press callback), instead of single press callback
// something to do with lcd, or RTC

//------------------------------------------------------
#include <LiquidCrystal.h>
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//-------------------------------------------------------

#include <EasyButton.h>  //pull up resistor
#define BUTTON_PIN 6
EasyButton button(BUTTON_PIN);

int counter = -1 ;  //for setting hh:mm:ss
//String mode = "UTC time";

//-------------------------------------------------------

#include <Encoder.h>
Encoder myEnc(7, 8);
int oldPos  = -999;
int newPos;
int startValue = 6; //HH,MM,SS value before change, set arbitrarily

//------------------------------------------------------

#include <RTCZero.h>
RTCZero rtc;      // instance of the realtime clock
//int thisSecond;   // variable to look for seconds chang

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 16;

/* Change these values to set the current initial date */
const byte day = 15;
const byte month = 6;
const byte year = 15;

byte setHH;   //0-23
byte setMM;
byte setSS;
//------------------------------------------------------

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);    // set up the LCD's number of columns(16) and rows(2)
  
  rtc.begin();
  //rtc.setDate(day, month, year);
  rtc.setTime(hours, minutes, seconds);
  
  myEnc.write(0);    //reset to position 0

  button.begin();  //initialize button
//  button.onPressedFor(2000, onPressedForDuration);  //when button is pressed and held for 2sec, call onPressedForDuration()
  button.onPressed(onPressedCallBack);
  //button.onSequence(2 /* number of presses */, 2000 /* timeout */, onSequenceMatched /* callback */);

}

void loop() {
  button.read();
  newPos = myEnc.read() / 4; //newPos = change of startValue, increment/decrement by 1 per click

  //  if (thisSecond != rtc.getSeconds()) {
  //    // on the zero second, skip the rest of the loop():
  //    if (rtc.getSeconds() == 0) return;

  //display time
  if (counter == -1) {
    lcd.setCursor(0, 1);
    print2digits(rtc.getHours());
    lcd.print(":");
    print2digits(rtc.getMinutes());
    lcd.print(":");
    print2digits(rtc.getSeconds());
  }

  if (counter == 0) {    //set hour - blink HH cursor, rotate encoder to select and display HH(0-12)
//    lcd.setCursor(0, 1); // blink cursor (0,1)
//    lcd.blink();
//    lcd.setCursor(1, 1); //blink cursor(1,1)
//    lcd.blink();

    lcd.setCursor(0,0);
    lcd.print("set HH");

    lcd.setCursor(0, 1);  //set cursor for HH
    startValue = rtc.getHours();
    if (newPos != oldPos) {
      if ((startValue + newPos) >= 0 && (startValue + newPos) < 24) { //if within boundary
        oldPos = newPos;
        print2digits(startValue + newPos); //output hour 0 - 23
        setHH = startValue + newPos;   //store value to setHH
      } else { //if reaches boundary
        print2digits(startValue + oldPos);   //keep outputing month 0 or 23
        setHH = startValue + oldPos;    //store value to setHH
        myEnc.write(oldPos * 4);  //keep resetting to oldPos(last change of startValue) before next click
      }
    }
  } else if (counter == 1) {
    print2digits(rtc.getHours());   //finish setting HH and display HH
    //set minute
  } else if (counter == 2) {
    //set second
  }

  //    // save current time for next loop:
  //    thisSecond = rtc.getSeconds();
  //  }
}

void print2digits(int number) {
  if (number < 10) {
    lcd.print("0"); // print a 0 before if the number is < than 10
  }
  lcd.print(number);
}

//called when the button is held for x seconds.
void onPressedForDuration() {
  counter = 0;
  myEnc.write(0); //rest enc position to 0
  Serial.println("Enter setting mode! Setting hour");
  Serial.println(counter);
}

void onPressedCallBack() {   //press to set hh, mm, ss
  Serial.println("Button has been pressed!");
  if (counter >= 0 && counter <= 3) {
    Serial.println(counter);
    if (counter == 0) {
      rtc.setHours(setHH);
    } else if (counter == 1) {
      Serial.println("hour is set, setting minute");
    } else if (counter == 2) {
      Serial.println("minute is set, setting second");
    } else {
      Serial.println("second is set,setting finished!");
      counter = -1;
    }
    myEnc.write(0);  //rest enc position to 0 for the next setting
    counter += 1;
  }
}

//void onSequenceMatched() {
//  if(mode == "UTC time"){
//    mode = "Decimal time";
//  }else{
//    mode = "UTC time";
//  }
//  Serial.println(mode); //current mode
//}
