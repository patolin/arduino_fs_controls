// Flight Simulator Throthe Quadrant
// 2023-01-09 Patricio Reinoso
// Requires Joystick 2.0 Library from https://github.com/MHeironimus/ArduinoJoystickLibrary
//--------------------------------------------------------------------

#include <Joystick.h>
#define DT 3
#define CLK 2

#define encoderUp 4
#define encoderDn 5
#define encoderDelay 10

Joystick_ Joystick;


int currentState;
int initState;
int counter  = 0;
unsigned long bebounceDelay = 0;
bool btnEncoder1 = LOW;
bool btnEncoder2 = LOW;
int switches[4] = {6,7,8,9};
int buttons[4] = {15,14,16,10};
const int pinToButtonMap = 4;

void setup() {
  // Initialize Button Pins
  for (int i=0; i<=3; i++) {
    pinMode(switches[i], INPUT_PULLUP);
    pinMode(buttons[i], INPUT_PULLUP);
  }

  // encoder setup for interrupt on change
  pinMode(DT, INPUT);
  pinMode(CLK, INPUT);
  initState = digitalRead(CLK);
  attachInterrupt(digitalPinToInterrupt(DT), encoder_value, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CLK), encoder_value, CHANGE);
  counter=127;

  // axis ranges
  Joystick.setThrottleRange(0, 1023);
  Joystick.setZAxisRange(0, 1023);
  Joystick.setRzAxisRange(0, 1023);
  Joystick.setRxAxisRange(0, 255);

  // Initialize Joystick Library
  Joystick.begin();
}

void loop() {

  bool oldEncoderUp;
  bool oldEncoderDn;

  for (int i=0; i<=3; i++) {
    Joystick.setButton(i, !digitalRead(switches[i]));
    Joystick.setButton(i+6, !digitalRead(buttons[i]));
  }

  if (oldEncoderUp != btnEncoder1) {
    Joystick.setButton(4,HIGH);
    delay(encoderDelay);
    
  } else {
    Joystick.setButton(4,LOW);
  }
  btnEncoder1 = LOW;
  oldEncoderUp = btnEncoder1;

  if (oldEncoderDn != btnEncoder2) {
    Joystick.setButton(5,HIGH);
    delay(encoderDelay);
  } else {
    Joystick.setButton(5,LOW);
    
  }
  btnEncoder2 = LOW;
  oldEncoderDn = btnEncoder2;
 
  Joystick.setZAxis(1023-analogRead(A0)); 
  delay(5);
  Joystick.setThrottle(1023-analogRead(A1));
  delay(5);
  Joystick.setRzAxis(analogRead(A2));
  delay(5);
  Joystick.setRxAxis(counter);
  delay(50);
}

void encoder_value() {
  noInterrupts();
  currentState = digitalRead(CLK);
  if (currentState != initState  && currentState == 1) {
    if (digitalRead(DT) != currentState) {
        btnEncoder1 = HIGH;
        delay(encoderDelay);
      //btnEncoder1 = !btnEncoder1;

    } else {
        btnEncoder2 = HIGH;
        delay(encoderDelay);
      //btnEncoder2 = !btnEncoder2;
    }
  }
  initState = currentState;
  interrupts();
}
