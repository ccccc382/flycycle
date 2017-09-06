#include <Arduino.h>
#include <joystick.h>

// Create the Joystick
Joystick_ joystick;

#define PotPin 4
#define IrPin 5
#define ReedPin 6
#define RightBrake 7
#define LeftBrake 8



void setup() {
    //activate internal pull ups
    pinMode(RightBrake, INPUT_PULLUP);
    pinMode(LeftBrake, INPUT_PULLUP);

    //initialize joystick library
    joystick.begin();
}

//function to set button states
void Button(int input) {
  if (input == 1) {
    joystick.pressButton(input);
    joystick.releaseButton(input);
  }
  else {
    joystick.pressButton(input);
  }
}




void loop() {

  joystick.setXAxis(analogRead(PotPin));
  joystick.setYAxisT(analogRead(IrPin));
  joystick.setZAxis(analogRead(ReedPin));
  Button(RightBrake);
  Button(LeftBrake);
}
