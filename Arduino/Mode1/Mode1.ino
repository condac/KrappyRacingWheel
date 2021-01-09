// Mode 1. Single arduino for all buttons, 2 rotary and 5 analog.
// Buttons are done with a button matrix, see scematics: !TODO!


//#define GYRO_STEERING
#define ROTARY_ENCODER
#include <Joystick.h>

#include <Arduino.h>
#ifdef GYRO_STEERING
#include <TinyMPU6050.h>
#endif
#define REV_STEERING 1
#define UPSIDEDOWN 0


#ifdef ROTARY_ENCODER
#include "rotaryEncoder.h"
#include <MD_REncoder.h>

// set up encoder object
MD_REncoder R1 = MD_REncoder(0, 1);
MD_REncoder R2 = MD_REncoder(2, 3);

#define matrix1 18
#define matrix2 19
#define matrix3 20
#define matrix4 21

#define mbutt1 14
#define mbutt2 15
#define mbutt3 16

#define MATRIXLINES 4
#define MATRIXBUTTONS 3
#define MATRIXBUTTONSTOTAL MATRIXLINES*MATRIXBUTTONS

bool mbuttons[MATRIXBUTTONSTOTAL+1];
int mlines[MATRIXLINES];
int mbutts[MATRIXBUTTONS];


#endif


double totalTurnAngle = 360; // 360 is 180 to each side. typical road car have ~900

long shiftPulse = 50;
bool setupMode = false;
/*
 *  Constructing MPU-6050
 */
#ifdef GYRO_STEERING
MPU6050 mpu (Wire);
#endif
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  32, 0,                  // Button Count, Hat Switch Count
  true, true, true,     // X and Y, but no Z Axis
  true, true, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, true, false);  // No accelerator, brake, or steering

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(0,INPUT_PULLUP);
  pinMode(1,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(15,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);
  pinMode(16,INPUT_PULLUP);
  setupMatrix();
  R1.begin();
  R2.begin();
  Serial.println("started..");
  delay(1000);
  Serial.print("1");
  //delay(1000); 
  //Serial.print("2");
  //delay(1000); 
  //Serial.print("3");
  //delay(1000); 
  //Serial.print("4");
  //delay(1000); 
  //Serial.print("5");
  // wait 5s so we can reprogram device incase of flooding on serial port
    // Initialize Joystick Library
  // Initialization

  #ifdef GYRO_STEERING
  mpu.Initialize();
  // Calibration
  Serial.println("=====================================");
  //Serial.println("Starting calibration...");
  //mpu.Calibrate();
  #endif

  Joystick.begin(false);
  //Joystick.setXAxisRange(-32767, 32767);
  
  Joystick.setXAxisRange(-512, 512);
  Joystick.setYAxisRange(-512, 512);
  Joystick.setZAxisRange(-512, 512);
  
  Joystick.setRxAxisRange(-512, 512);
  Joystick.setRyAxisRange(-512, 512);
  Joystick.setRzAxisRange(-512, 512);
}

void setupMatrix() {

  mlines[0] = matrix1;
  mlines[1] = matrix2;
  mlines[2] = matrix3;
  mlines[3] = matrix4;

  mbutts[0] = mbutt1;
  mbutts[1] = mbutt2;
  mbutts[2] = mbutt3;

  for (int i=0;i<MATRIXLINES;i++) {
    pinMode(mlines[i],INPUT_PULLUP);
  }
  for (int i=0;i<MATRIXBUTTONS;i++) {
    pinMode(mbutts[i],INPUT_PULLUP);
  }

}

long counter = 0;
long time1 = 0;
int button = 0; 
bool currentButtonState;
double prevang = 0.0;
int turns = 0;

void loop() {
  mode1();

}

void handleMatrix() {
  for (int i=0;i<MATRIXLINES;i++) {
    pinMode(mlines[i],OUTPUT);
    digitalWrite(mlines[i],LOW);
    for (int x=0;x<MATRIXBUTTONS;x++) {
      mbuttons[i*MATRIXBUTTONS+x+1] = !digitalRead(mbutts[x]);
    }
    pinMode(mlines[i],INPUT_PULLUP);
    digitalWrite(mlines[i],HIGH);
  }

  
}

void handleButton(int pin, int buttonNr, bool reverse, long pulse) {
  if (pin <0) {
    bool state = mbuttons[-pin];
    if (reverse) {
      state = !state;
    }
    if (state) {
      pulseButton(buttonNr, pulse);  
    }
    //Joystick.setButton(buttonNr, state);
  } else {
    bool state = false;
    state = digitalRead(pin);
    if (reverse) {
      state = !state;
    }
    if (state) {
      pulseButton(buttonNr, pulse);  
    }
    //Joystick.setButton(buttonNr, state);
  }
}

void handleMultiWheelButton(int aPort, int button1, int button2, int startButton, int sections) {
  int value = analogRead(aPort);
  sections = sections;
  if(sections == 0) {
    sections = 1;
  }
  int divider = 1024/sections;
  //divider = divider + 1;
  value = value/divider;

  handleButton(button1, value*2+startButton, false, shiftPulse);
  handleButton(button2, value*2+startButton+1, false, shiftPulse);
}
void handleEncoder(MD_REncoder R, int buttonNr) {
  uint8_t x = R.read();
  if (x) {
    Serial.print(x); 
        Serial.println("kaka"); 
  }
  if (x==16) {
    Joystick.setButton(buttonNr, true);
    Joystick.setButton(buttonNr+1, false);
  } else if (x==32) {
    Joystick.setButton(buttonNr, false);
    Joystick.setButton(buttonNr+1, true);
  } else {
    Joystick.setButton(buttonNr, false);
    Joystick.setButton(buttonNr+1, false);
  }
}
int getPosition(int aPort, int sections) {
  int divider = 1023/sections;
  divider = divider +1;
  int val = analogRead(aPort);
  int out = 0;
  int test = divider;
  //Serial.print("val");
  //Serial.print(val);
  
  //Serial.print("test");
  
  //Serial.println(test);
  while (val > test) {
    

    test = test + divider;
    out = out + 1;
  //    Serial.print("val");
  //Serial.print(val);
  //Serial.print("test");
  //Serial.println(test);
  }
  
  //Serial.print("getpos return:");
  
  //Serial.println(out);
  return out;
}

long rotarySticky = 0;
long stick = 15;
int ERSlast[2];
long ERSsticky[2];
int ERScycle[2];
long ERSstick = 30;
long ERSpause = 250;

void handleERS(int x, int aPin, int button1, int button2) {
  
  int pos = getPosition(aPin,6);
  //Serial.println(pos);
  if (ERSlast[x] != pos) {
    //Serial.print("ers changed");
    //Serial.print(pos);
    //Serial.print(" ");
    
    //Serial.println(ERSlast[x]);
    if (ERScycle[x] == 0) { // we are not doing anything yet
      //Serial.println("cycle 0");
      if (ERSlast[x] < pos) {
        // ERS is turned up
        ERScycle[x] = 1;
        pulseButton(button2,ERSstick);
        //Joystick.setButton(button2, true);
        ERSsticky[x] = millis()+ERSstick;
        ERSlast[x]++;
      } else {
        // ERS is turned down
        ERScycle[x] = 1;
        pulseButton(button1,ERSstick);
        
        //Joystick.setButton(button1, true);
        ERSsticky[x] = millis()+ERSstick;
        ERSlast[x]--;
      }
    }
  }
  
  if (ERScycle[x] == 1) {
    if (ERSsticky[x] < millis()) {
      //Joystick.setButton(button1, false);
      //Joystick.setButton(button2, false);
      ERSsticky[x] = millis()+ERSpause;
      ERScycle[x] = 2;
    }
  }
  if (ERScycle[x] == 2) {
    if (ERSsticky[x] < millis()) {
      //Joystick.setButton(button1, false);
      //Joystick.setButton(button2, false);
      ERScycle[x] = 0;
    }
  }
}
bool falseOnce = false;
void mode1() {
  
  // read 2 gear shifters
  handleButton(5,0,false, shiftPulse);
  handleButton(7,1,false, shiftPulse);


  // Read 2 encoders

  //handleEncoder(R1,2);
  //handleEncoder(R2,4);
  uint8_t x = R1.read();
  if (x && setupMode) {
    Serial.print(x);
    Serial.println("banan"); 
  }
  if (x==16) {
    pulseButton(2,stick);
  } else if (x==32) {
    pulseButton(2+1,stick);
  } 
  
  x = R2.read();
  if (x && setupMode) {
    Serial.print(x);
    Serial.println("banan"); 
  }
  if (x==16) {
    pulseButton(4,stick);
    //Joystick.setButton(4, true);
    //Joystick.setButton(4+1, false);
    //rotarySticky = millis()+stick;
    //falseOnce = true;
    
  } else if (x==32) {
    pulseButton(4+1,stick);
    //Joystick.setButton(4, false);
    //Joystick.setButton(4+1, true);
    //rotarySticky = millis()+stick;
    //falseOnce = true;
  } else {
    //if (rotarySticky < millis() && falseOnce) {
    //  Joystick.setButton(4, false);
    //  Joystick.setButton(4+1, false);
    //  falseOnce = false;
    //}
  }
  //getRotationType1(0, 1, 1)
  //getRotationType1(int pinIn1, int pinIn2, int nr)

  // read button matrix
  handleMatrix();
  handleButton(-1,6,false,shiftPulse);
  handleButton(-2,7,false,shiftPulse);
  handleButton(-3,8,false,shiftPulse);
  //Joystick.setButton(6, mbuttons[1]);
  //Joystick.setButton(7, mbuttons[2]);
  //Joystick.setButton(8, mbuttons[3]);
  
  handleButton(-4,9,false,shiftPulse);
  handleButton(-5,10,false,shiftPulse);
  handleButton(-6,11,false,shiftPulse);
  //Joystick.setButton(9, mbuttons[4]);
  //Joystick.setButton(10, mbuttons[5]);
  //Joystick.setButton(11, mbuttons[6]);

  handleButton(-7,12,false,shiftPulse);
  handleButton(-8,13,false,shiftPulse);
  //handleButton(-9,14,false,shiftPulse); multiwheel button
  //Joystick.setButton(12, mbuttons[7]);
  //Joystick.setButton(13, mbuttons[8]);
  //Joystick.setButton(14, mbuttons[9]);

  handleButton(-10,14,false,shiftPulse);
  handleButton(-11,15,false,shiftPulse);
  //handleButton(-12,17,false,shiftPulse); // miltiwheelbutton
  //Joystick.setButton(14, mbuttons[10]);
  //Joystick.setButton(15, mbuttons[11]);
  //Joystick.setButton(17, mbuttons[12]);
  
  
  // Read 4 analog inputs
  int a0 = analogRead(A6);
  a0 = (a0-512);
  int a1 = analogRead(A7);
  a1 = (a1-512);
  int a2 = analogRead(A9);
  a2 = (a2-512);
  int a3 = analogRead(A10);
  a3 = (a3-512);
  Joystick.setXAxis(0);
  Joystick.setYAxis(a1);
  Joystick.setZAxis(a2);
  Joystick.setRxAxis(a3);
  Joystick.setRyAxis(a0);
  Joystick.setRzAxis(0);
  // read multiwheel dial

  handleMultiWheelButton(A8, -9, -12, 16 , 5);

  // test ERS analog mode
  
  handleERS(0,A10,4,5);

  // handle pulsed buttons
  pulseLoop();
  // send state
  Joystick.sendState();
  counter++;

  if (time1<millis() && setupMode) {
    Serial.println(counter);
    time1 = millis()+200;
    counter = 0;

    Serial.print("matrixbuttons = ");
    for (int i=0;i<MATRIXBUTTONSTOTAL;i++) {
      Serial.print(" ");
      Serial.print(mbuttons[i+1]);
    }
    Serial.println("");
    
  }
}


#define PULSEARRAY 32
long pulseArray[PULSEARRAY];
long serialStick;

void pulseButton(int button, long pulse) {
    
  pulseArray[button] = millis() + pulse; 
  if ( button == 26) {
    if (serialStick<millis()) {
      Serial.println("sb-;");
      serialStick = millis() + 200;
    }
    
  }
  else if ( button == 27) {
    if (serialStick<millis()) {
      Serial.println("sb+;");
      serialStick = millis() + 200;
    }
  }
  else {
    serialStick = false;
  }

}
void pulseLoop() {
  long mil = millis();
  for (int i=0;i<PULSEARRAY;i++) {
    if (pulseArray[i] != 0) {
      if(pulseArray[i] > mil) {
        Joystick.setButton(i, true);
      } else {
        Joystick.setButton(i,false);
      }
    }
  }

}
