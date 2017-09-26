#include <Arduino.h>
#include <Joystick.h> // include joystick library

// Create the Joystick
Joystick_ joystick;

// define pins
#define PotPin A0
#define IrPin A2
#define ReedPin 8
#define RightBrake 3
#define LeftBrake 4
#define Reset 9

//rpm variables
int reedVal;
long timer = 0;// time between one full rotation (in ms)
float mph = 0.00;
float kmh = mph x 1.609344;
throttle = map(kmh, 0, 25, 0 , 125);
float circumference;
int maxReedCounter = 100;//min time (in ms) of one rotation (for debouncing)
int reedCounter;



void setup() {
        //activate internal pull ups
        pinMode(RightBrake, INPUT_PULLUP);
        pinMode(LeftBrake, INPUT_PULLUP);
        pinMode(Reset, INPUT_PULLUP);
        pinMode(ReedPin, INPUT);
        int maxReedCounter = 100;//min time (in ms) of one rotation (for debouncing)
        circumference = 2*3.14*radius;

        // TIMER SETUP- the timer interrupt allows preceise timed measurements of the reed switch
        cli();//stop interrupts

        //set timer1 interrupt at 1kHz
        TCCR1A = 0;// set entire TCCR1A register to 0
        TCCR1B = 0;// same for TCCR1B
        TCNT1  = 0;
        // set timer count for 1khz increments
        OCR1A = 1999;// = (1/1000) / ((1/(16*10^6))*8) - 1
        // turn on CTC mode
        TCCR1B |= (1 << WGM12);
        // Set CS11 bit for 8 prescaler
        TCCR1B |= (1 << CS11);
        // enable timer compare interrupt
        TIMSK1 |= (1 << OCIE1A);

        sei();//allow interrupts
        //END TIMER SETUP

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

ISR(TIMER1_COMPA_vect) {//Interrupt at freq of 1kHz to measure reed switch
        reedVal = digitalRead(reed);//get val of A0
        if (reedVal) {//if reed switch is closed
                if (reedCounter == 0) {//min time between pulses has passed
                        mph = (56.8*float(circumference))/float(timer);//calculate miles per hour
                        timer = 0;//reset timer
                        reedCounter = maxReedCounter;//reset reedCounter
                }
                else{
                        if (reedCounter > 0) {//don't let reedCounter go negative
                                reedCounter -= 1;//decrement reedCounter
                        }
                }
        }
        else{//if reed switch is open
                if (reedCounter > 0) {//don't let reedCounter go negative
                        reedCounter -= 1;//decrement reedCounter
                }
        }
        if (timer > 2000) {
                mph = 0;//if no new pulses from reed switch- tire is still, set mph to 0
        }
        else{
                timer += 1;//increment timer
        }
}


// write values to axis
void loop() {
        joystick.setXAxis(analogRead(PotPin));
        joystick.setYAxis(analogRead(IrPin));
        joystick.setZAxis(throttle);
        Button(digitalRead(RightBrake), 1);
        Button(digitalRead(LeftBrake), 2);
        Button(digitalRead(Reset), 3);
}
