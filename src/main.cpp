#include <Arduino.h>
#include <Joystick.h>

// Create the Joystick
Joystick_ joystick;

#define PotPin A0
#define IrPin A2
#define ReedPin 8
#define RightBrake 3
#define LeftBrake 4
#define reset 9



void setup() {
    //activate internal pull ups
    pinMode(RightBrake, INPUT_PULLUP);
    pinMode(LeftBrake, INPUT_PULLUP);
      pinMode(Reset, INPUT_PULLUP);

    //initialize joystick library
    joystick.begin();
}

//function to set button states
void Button(int input,int num) {
  if (input == 1) {
    joystick.pressButton(num);
  }
  else {
        joystick.releaseButton(num);
  }
}

void loop() {
  joystick.setXAxis(analogRead(PotPin));
  joystick.setYAxis(analogRead(IrPin));
  joystick.setZAxis(analogRead(ReedPin));
  Button(DigitalRead(RightBrake), 1);
  Button(DigitalRead(LeftBrake), 2);
  Button(DigitalRead(Reset), 3);
}
