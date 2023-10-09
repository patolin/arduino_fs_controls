// Simple example application that shows how to read four Arduino
// digital pins and map them to the USB Joystick library.
//
// Ground digital pins 9, 10, 11, and 12 to press the joystick 
// buttons 0, 1, 2, and 3.
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// by Matthew Heironimus
// 2015-11-20
//--------------------------------------------------------------------

#include <Joystick.h>
#define DT 3
#define CLK 2
Joystick_ Joystick;


int currentState;
int initState;
int counter  = 0;
unsigned long bebounceDelay = 0;
bool btnEncoder1 = LOW;
bool btnEncoder2 = LOW;

void setup() {
  // Initialize Button Pins
  // pinMode(2, INPUT);
  // pinMode(3, INPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  pinMode(DT, INPUT);
  pinMode(CLK, INPUT);

  initState = digitalRead(CLK);
  attachInterrupt(digitalPinToInterrupt(DT), encoder_value, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CLK), encoder_value, CHANGE);

  Joystick.setThrottleRange(0, 1023);
  Joystick.setZAxisRange(0, 1023);
  Joystick.setRzAxisRange(0, 1023);
  Joystick.setRxAxisRange(0, 255);

  // Initialize Joystick Library
  Joystick.begin();
  Serial.begin (9600);
  counter=127;


}

int readAnalog(int port) {
  return analogRead(port);
}

// int readEncoder() {

//   aState = digitalRead(8);
//   if (aState != aLastState) {
//     if (digitalRead(9) != aState) {
//       //if (cntRotary<1023) { cntRotary++; }
//       cntRotary--; 
//     } else {
//       //if (cntRotary>0) { cntRotary--; }
//        cntRotary++;
//     }
//   }
//   aLastState = aState;
//   return cntRotary;
// }
// Constant that maps the phyical pin to the joystick button.
const int pinToButtonMap = 4;




// Last state of the button
int lastButtonState[6] = {0,0,0,0,0,0};



void loop() {


  Joystick.setButton(0, !digitalRead(6));
  Joystick.setButton(1, !digitalRead(7));
  Joystick.setButton(2, !digitalRead(8));
  Joystick.setButton(3, !digitalRead(9));

  Joystick.setButton(6, !digitalRead(15));
  Joystick.setButton(7, !digitalRead(14));
  Joystick.setButton(8, !digitalRead(16));
  Joystick.setButton(9, !digitalRead(10));


  // // Read pin values
  // for (int index = 0; index < 4; index++)
  // {
  //   int currentButtonState = !digitalRead(index + pinToButtonMap);
  //   if (currentButtonState != lastButtonState[index])
  //   {
  //     Joystick.setButton(index, currentButtonState);
  //     lastButtonState[index] = currentButtonState;
  //   }
  // }
  
  // encoder
    int currentButtonState = btnEncoder1;
    if (currentButtonState != lastButtonState[4])
    {
      Joystick.setButton(4, HIGH);
      delay(50);
      Joystick.setButton(4, LOW);
      lastButtonState[4] = currentButtonState;
    }

    currentButtonState = btnEncoder2;
    if (currentButtonState != lastButtonState[5])
    {
      Joystick.setButton(5, HIGH);
      delay(50);
      Joystick.setButton(5, LOW);
      lastButtonState[5] = currentButtonState;
    }
  // *************

  Joystick.setZAxis(1023-readAnalog(A0)); 
  delay(10);
  Joystick.setThrottle(1023-readAnalog(A1));
  delay(10);
  Joystick.setRzAxis(readAnalog(A2));
  delay(10);
  Joystick.setRxAxis(counter);
  delay(50);
}

void encoder_value() {
  currentState = digitalRead(CLK);
  if (currentState != initState  && currentState == 1) {
    if (digitalRead(DT) != currentState) {
      if (counter<255) { counter ++; }
      btnEncoder1 = !btnEncoder1;

    } else {
      if (counter>0) { counter --; }
      btnEncoder2 = !btnEncoder2;
    }
  }
  initState = currentState;
}
