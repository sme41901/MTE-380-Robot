#include "Wire.h"
#include <MPU6050_light.h>
#include "robot_traversal.h"

// MPU6050 mpu(Wire);


//Define Ultrasonic Pins
// const int trigPinLeft = 5;
// const int echoPinLeft = 6;
// const int trigPinRight = 9;
// const int echoPinRight = 10;

//Sean Pins
const int trigPinLeft = 10;
const int echoPinLeft = 9;
const int trigPinRight = 6;
const int echoPinRight = 5;

//Define Motor Pins
// const int inALeft = 2;
// const int inBLeft = 4;
// const int pwmLeft = 3;
// const int inARight = 13;
// const int inBRight = 12;
// const int pwmRight = 11;

const float currentPowerLeft;
const float currentPowerRight;

const float motorMultiplier = 1.067;

void setup() {

  Serial.begin(9600);

//Initialize Left Ultrasonic Sensor
  pinMode(trigPinLeft, OUTPUT); 
  pinMode(echoPinLeft, INPUT); 

//Initialize Right Ultrasonic Sensor
  pinMode(trigPinRight, OUTPUT); 
  pinMode(echoPinRight, INPUT);

//Initialize Left Motor
  pinMode(inALeft, OUTPUT);
  pinMode(inBLeft, OUTPUT);
  pinMode(pwmLeft, OUTPUT);

//Initialize Right Motor
  pinMode(inARight, OUTPUT);
  pinMode(inBRight, OUTPUT);
  pinMode(pwmRight, OUTPUT);

// keep motors off initially
  digitalWrite(inALeft, LOW);
  digitalWrite(inBLeft, LOW);
  digitalWrite(inARight, LOW);
  digitalWrite(inBRight, LOW);
}


void loop() {
  Serial.println("1....2..");
  delay(3000);
  Serial.println("3!");

  Serial.println("RAMP UP START");
  rampUp(80, 2);
  // rampUp(80, forward, 1000, 0) //new 
 
  bool postFound = false;
  int numMeasurements = 0;

  while(!postFound){
    
      Serial.println("POST NOT FOUND");
      float leftSensor = read_distance(trigPinLeft, echoPinLeft);
      float rightSensor = read_distance(trigPinRight, echoPinRight);
      Serial.println(leftSensor);
      Serial.println(rightSensor);
      if(abs(leftSensor - rightSensor) < 10){
        numMeasurements++;
      }

      if(numMeasurements == 10){
        postFound = true;
      }

      delay(50);

  }

  Serial.println("POST FOUND");

  stop();
  // stop(forward); //new

  delay(5000);

  rampUp(80, 2);
  // rampUp(80, forward, 1000, 0) //new 

  Serial.println("STARTING APPROACH");

  while(read_distance(trigPinLeft, echoPinLeft) > 15 && read_distance(trigPinRight, echoPinRight) > 15){
      approach(80);
  }
  
  stop();
  // stop(forward); //new
  Serial.println("Stopped");
}

// void rampUp(float power, int direction) {
//   float rampConst = 1000;
//   float powerConst = 255;
//   float timeOffset = millis();

//   while((timeOffset + power*rampConst/powerConst) > float (millis()))
//   {
//     int rampPower = (millis() - timeOffset)*powerConst/rampConst;
//     // set motor speed via pwm
//     move(rampPower, power, direction);
//     delay(150);    
//   }
//   move(power, 0, 2);

  
// }

void rampDown(float power, float initialPower, int direction) {
  if(initialPower > power) {
    float rampConst = 1000;
    float powerConst = 255;
    float timeOffset = millis();
    while((timeOffset + (initialPower-power)*rampConst/powerConst) > float (millis()))
    {
      float rampPower = initialPower - (millis() - timeOffset)*powerConst/rampConst;
      // set motor speed via pwm
      move(rampPower, initialPower, direction);

      // delay(150);    
    }

    if(direction == 3){
      move(power, initialPower, 3);
    }

    if(direction == 4){
      move(power, initialPower, 4);
    }
  }
}

void stop() {
  // set motor speed via pwm
  move(0, 0, 0);
}

void move(float power, float initialPower, int direction) {
  // set motor speed via pwm
  // analogWrite(pwmLeft, power);
  // analogWrite(pwmRight, power);
  // direction = 0 go forward
  if(direction == 0) {
  // stop motor
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, LOW);
  }
  if(direction == 1) {
    analogWrite(pwmLeft, power);
    analogWrite(pwmRight, power);
  // turn on motor and move backward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
  }
  if(direction == 2) {
    analogWrite(pwmLeft, power);
    analogWrite(pwmRight, power*motorMultiplier);
    // turn on motor and move forward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
    // digitalWrite(inALeft, HIGH);
    // digitalWrite(inBLeft, LOW);
    // digitalWrite(inARight, HIGH);
    // digitalWrite(inBRight, LOW);
  }
  if(direction == 3){
    analogWrite(pwmLeft, power);
    analogWrite(pwmRight, initialPower*motorMultiplier);
    // turn on motor and move forward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
    // digitalWrite(inALeft, HIGH);
    // digitalWrite(inBLeft, LOW);
    // digitalWrite(inARight, HIGH);
    // digitalWrite(inBRight, LOW);
  }
  if(direction == 4){
    analogWrite(pwmLeft, initialPower);
    analogWrite(pwmRight, power*motorMultiplier);
    // turn on motor and move forward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
    // digitalWrite(inALeft, HIGH);
    // digitalWrite(inBLeft, LOW);
    // digitalWrite(inARight, HIGH);
    // digitalWrite(inBRight, LOW);
  }
  
}

// void imu_straighten(float power){
//   //clockwise = negative, decrease power in LM
//   //counter clockwise = positive, decrease  power in RM
//   float currentAngle = mpu.getAngleZ();

//   while(abs(currentAngle) > 5){

//     if(mpu.getAngleZ() > 0){
//       float multiplier = (100 - currentAngle) / 100;
//       rampDown(multiplier*power, power, 4);
//     } 

//     if(mpu.getAngleZ() < 0){
//       float multiplier = (100 - currentAngle) / 100;
//       rampDown(multiplier*power, power, 3);
//     }
//   }
// }

void approach(float power){
  float tolerance = 10;

  float leftSensor = read_distance(trigPinLeft, echoPinLeft);
  float rightSensor = read_distance(trigPinRight, echoPinRight);

  //debugging
  Serial.print("Left Sensor: ");
  Serial.println(leftSensor);
  Serial.print("Right Sensor: ");
  Serial.println(rightSensor);

  if(abs(read_distance_bulk(trigPinLeft, echoPinLeft) - read_distance_bulk(trigPinRight, echoPinRight))  > tolerance){
      while(abs(read_distance(trigPinLeft, echoPinLeft) - read_distance(trigPinRight, echoPinRight))  > tolerance){
        float leftSensor = read_distance(trigPinLeft, echoPinLeft);
        float rightSensor = read_distance(trigPinRight, echoPinRight);

        //debugging
        Serial.print("Left Sensor: ");
        Serial.println(leftSensor);
        Serial.print("Right Sensor: ");
        Serial.println(rightSensor);

        if (leftSensor < rightSensor){
          // decrease power in LM
          float multiplier = (leftSensor / rightSensor);
          rampDown(power, 3);
          //rampDown(multiplier*power, slowLeft, 1000, power); //new
        } else if (rightSensor < leftSensor) {
          // decrease power in RM
          float multiplier = (rightSensor / leftSensor);
          rampDown(multiplier*power, power, 4);
          //rampDown(multiplier*power, slowRight, 1000, power); //new
        }

      }

      rampUp(power, 2);
  }

}
