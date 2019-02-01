//double press button in 0.5 second to toggle between UTC time and decimal time
//hold down button 2 seconds to enter setting mode
//in setting mode, press once to confirm setting and move on to the next (hr, min, sec)

#include <EasyButton.h>
#define BUTTON_PIN 9 //pull up resistance

// Instance of the button.
EasyButton button(BUTTON_PIN);

int counter1 = -1 ; //for setting hour, minute, second
String mode = "UTC time";

void setup() {
  Serial.begin(9600);
  button.begin();  //initialize button

  //when button is pressed and held for 2sec, call onPressedForDuration()
  button.onPressedFor(2000, onPressedForDuration);
  button.onPressed(onPressedCallBack);
  button.onSequence(2 /* number of presses */, 2000 /* timeout */, onSequenceMatched /* callback */);

}

void loop() {
  button.read();
  //  button.onSequence(2 /* number of presses */, 500 /* timeout */, onSequenceMatched /* callback */);

  if (counter1 == 0) {
    //set hour
  } else if (counter1 == 1) {
    //set minute
  } else if (counter1 == 2) {
    //set second
  }
}

// Callback function to be called when the button is pressed and held.
void onPressedForDuration() {
  counter1 = 0;
  Serial.println("Enter setting mode! Setting hour");
}

void onPressedCallBack() {   //press when finish setting hour, minute, or second
  if (counter1 >= 0 && counter1 <= 3) {
    counter1 += 1;
    //    Serial.println("Button has been pressed!");
    Serial.println(counter1);
    if (counter1 == 1) {
      Serial.println("hour is set, setting minute");
    } else if (counter1 == 2) {
      Serial.println("minute is set, setting second");
    } else {
      Serial.println("second is set,setting finished!");
      counter1 = -1;
    }
  }
}

void onSequenceMatched() {
  if(mode == "UTC time"){
    mode = "Decimal time";
  }else{
    mode = "UTC time";
  }
  Serial.println(mode); //current mode
}
