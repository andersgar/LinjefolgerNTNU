#include "Car.h"

Car car("NTNU-IOT", "");
bool lineMode = 0;
int prevError = 0;
bool turning = 0;

///Preferences setup
int maxSpeed = 90;
bool lineColor = BLACK;
float turnConf = 0.75;
float turnConf2 = 1.8;
int proxConf = 10;
bool avoiding = 0; // Toggles the avoiding on/off

void setup() {
  car.initCar();
  car.calibrateGyro(2048);
}

void loop() {
  int lineValue = car.data[LINE].value;
  car.sendData(3, maxSpeed);
  
  if(lineMode && !turning){
    float speedDifference = (lineValue*turnConf)+ turnConf2 * (lineValue - prevError);
    car.drive(maxSpeed + speedDifference, maxSpeed - speedDifference);
    prevError = lineValue;
  }

  if (car.data[PROXIMITY].flag) {
    int proDATA = car.data[PROXIMITY].value;
    car.data[PROXIMITY].flag = false;
    car.sendData(2, proDATA);
    if(proDATA >= proxConf && avoiding){
      triangle(DOWN);
    }
  }
 
  if (car.data[ENCODERS].flag) {
    float encDATA = car.data[ENCODERS].value;
    float tidmeasure = car.data[READ_TIME].value;
    //Hjulenes omkrets er 2*3.14*(19.5 mm) = 122.46 mm eller 0.12246 m, hvis man deler 75*12 (gear ratio and frequency of magnet rotation) på encDATA får man hvor mange omkretsen/hjulene gikk rundt hver 70 ms eller car.data[READ_TIME].value, får man fartsmålingen
    float gangerOmkrets = encDATA / (75 * 12);
    float fartDATA = (100 * 122.246 * gangerOmkrets) / tidmeasure; // i mm per sekund, men grafer m/s, så vi må gange med 100 for cm / s
    //Serial.println(fartDATA);
    car.data[ENCODERS].flag = false; // Merk at enkoderene teller 12 ganger per motorrotasjon. Hjulene roterer  ́en gang per 75te motorrotasjon. Hjulenes diamater, inkludert beltene, er 39mm. 
    car.sendData(1, fartDATA);
  }
}

void w(bool button) {
  if (button == DOWN && !turning) {
    car.drive(maxSpeed, maxSpeed);
  }
  if (button == UP) {
    car.drive(0, 0);
  }
}

void a(bool button) {
  if (button == DOWN) {
    car.drive(-maxSpeed/1.7, maxSpeed/1.7);
  }
  if (button == UP) {
    car.drive(0, 0);
  }
}

void s(bool button) {
  if (button == DOWN) {
    car.drive(-maxSpeed, -maxSpeed);
  }
  if (button == UP) {
    car.drive(0, 0);
  }
}

void d(bool button) {
  if (button == DOWN) {
    car.drive(maxSpeed/1.7, -maxSpeed/1.7);
  }
  if (button == UP) {
    car.drive(0, 0);
  }
}

void e(bool button) {
  if (button == DOWN) {
    maxSpeed = maxSpeed + 5;
  }
  if (button == UP) {
  }
}

void q(bool button) {
  if (button == DOWN) {
    maxSpeed = maxSpeed - 5;
  }
}
/*
void halfTurn { // Does a 180-degree turn
  turning = 1;  // "turning" overrides the line-follower's control of the car
  car.drive(maxSpeed, -maxSpeed);
  delay(550);
  if(lineMode == 1){ // If it was previously following a line sets motors to maxSpeed
      car.drive(maxSpeed, maxSpeed);
  }else{ // Otherwise it stops the car
      car.drive(0, 0);
  }
  turning = 0;
}

bool triangleSwitch = 1 // Allows alternating functionality of the "triangle" button
void triangle(bool button) {
  if (button == DOWN) {
      if triangleSwitch {
          avoiding = 1;
          triangleSwitch = !triangleSwitch; // The other functionality runs at the next button press
      }
      else {
          avoiding = 0;
          triangleSwitch = !triangleSwitch; // The other functionality runs at the next button press
      }
  }
}
*/
void triangle(bool button) {
  if (button == DOWN) {
    turning = 1;
    car.drive(maxSpeed, -maxSpeed);
    delay(550);
    if(lineMode == 1){
      car.drive(maxSpeed, maxSpeed);
    }else{
      car.drive(0, 0);
    }
    turning = 0;
  }
}

void circle(bool button) {
  if(button == DOWN){
    car.calibrateLine(lineColor);
  }
}

bool squareSwitch = 1; // Allows alternating functionality of the "square" button
void square(bool button) {
  if (button == DOWN) {
    if (squareSwitch) {
      lineMode = 1;
      maxSpeed = 80; //resetter fart ved start av lineMode
      squareSwitch = !squareSwitch; // The other functionality runs at the next button press
    }
    else {
      lineMode = 0;
      car.drive(0, 0);
      squareSwitch = !squareSwitch; // The other functionality runs at the next button press
      maxSpeed = 90;
    }
  }
}