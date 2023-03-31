// This is a header file containing functions and definitions 
#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);

// DEFINTIONS 
const int inALeft = 2;
const int inBLeft = 4;
const int pwmLeft = 3;
const int inARight = 13;
const int inBRight = 12;
const int pwmRight = 11;

// const int trigPinLeft = 10;
// const int echoPinLeft = 9;
// const int trigPinRight = 6;
// const int echoPinRight = 5;

const int trigPinLeft = 6;
const int echoPinLeft = 5;
const int trigPinRight = 10;
const int echoPinRight = 9;



// const int inALeft = 4;
// const int inBLeft = 2;
// const int pwmLeft = 3;
// const int inARight = 12;
// const int inBRight = 13;
// const int pwmRight = 11;
const int forward = 1;
const int backward = 2;
const int CW = 3;
const int CCW = 4;
const int adjustLeft = 5;
const int adjustRight = 6;
const int slowLeft = 7;
const int slowRight = 8;
const float motorMultiplier = 1.167;

// FUNCTIONS FROM:  test_motor_controller_VNH5019_and_ramp_functions

void stop(int currentDirection) {
    int power = 255 * 0.05;
    int counterTime = 100;
    if (currentDirection == forward) {
        analogWrite(pwmLeft, power);
        analogWrite(pwmRight, power);
        // turn on motor and move backward
        digitalWrite(inALeft, LOW);
        digitalWrite(inBLeft, HIGH);
        digitalWrite(inARight, LOW);
        digitalWrite(inBRight, HIGH);
        delay(counterTime);
    }
    if (currentDirection == backward) {
        analogWrite(pwmLeft, power);
        analogWrite(pwmRight, power);
        // turn on motor and move forward
        digitalWrite(inALeft, HIGH);
        digitalWrite(inBLeft, LOW);
        digitalWrite(inARight, HIGH);
        digitalWrite(inBRight, LOW);
        delay(counterTime);
    }
    if (currentDirection == CW) {
        analogWrite(pwmLeft, power);
        analogWrite(pwmRight, power);
        // turn on motor and move CCW
        digitalWrite(inALeft, HIGH);
        digitalWrite(inBLeft, LOW);
        digitalWrite(inARight, LOW);
        digitalWrite(inBRight, HIGH);
        delay(counterTime);
    }
    if (currentDirection == CCW) {
        analogWrite(pwmLeft, power);
        analogWrite(pwmRight, power);
        // turn on motor and move CW
        digitalWrite(inALeft, LOW);
        digitalWrite(inBLeft, HIGH);
        digitalWrite(inARight, HIGH);
        digitalWrite(inBRight, LOW);
        delay(counterTime);
    }
    // turn motors off
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, LOW);
}

void bridgeControl(int power, int direction) {
    if (direction == forward) {
        // turn on motor and move forward
            // set motor speed via pwm
        analogWrite(pwmLeft, power);
        analogWrite(pwmRight, power);
        digitalWrite(inALeft, HIGH);
        digitalWrite(inBLeft, LOW);
        digitalWrite(inARight, HIGH);
        digitalWrite(inBRight, LOW);
    }
    if (direction == backward) {
        // turn on motor and move backward
            // set motor speed via pwm
        analogWrite(pwmLeft, power);
        analogWrite(pwmRight, power);
        digitalWrite(inALeft, LOW);
        digitalWrite(inBLeft, HIGH);
        digitalWrite(inARight, LOW);
        digitalWrite(inBRight, HIGH);
    }
    if (direction == CCW) {
        // turn on motor and move CCW
          // set motor speed via pwm
        analogWrite(pwmLeft, power);
        analogWrite(pwmRight, power);
        digitalWrite(inALeft, LOW);
        digitalWrite(inBLeft, HIGH);
        digitalWrite(inARight, HIGH);
        digitalWrite(inBRight, LOW);
    }
    if (direction == CW) {
        // turn on motor and move CW
            // set motor speed via pwm
        analogWrite(pwmLeft, power);
        analogWrite(pwmRight, power);
        digitalWrite(inALeft, HIGH);
        digitalWrite(inBLeft, LOW);
        digitalWrite(inARight, LOW);
        digitalWrite(inBRight, HIGH);
    }
    if (direction == adjustLeft) {
        analogWrite(pwmLeft, power);   
        digitalWrite(inALeft, HIGH);
        digitalWrite(inBLeft, LOW);   
    }
    if (direction == adjustRight) {
        analogWrite(pwmRight, power);   
        digitalWrite(inARight, HIGH);
        digitalWrite(inBRight, LOW);   
    }
}

//98% applied to pwmright at 1% power moving bkwd
//going down the wall, give 1 to pwmright and give 5 to pwmleft
void bridgeControlApproach(float power, float initialPower, int direction) {
    // set motor speed via pwm
    // analogWrite(pwmLeft, power/**0.945*/);
    // analogWrite(pwmRight, power);
    if(direction == forward || backward || CCW || CW){
        analogWrite(pwmLeft, power/**0.945*/);
        analogWrite(pwmRight, power);
    }
    if (direction == forward) {
        // turn on motor and move forward
        digitalWrite(inALeft, HIGH);
        digitalWrite(inBLeft, LOW);
        digitalWrite(inARight, HIGH);
        digitalWrite(inBRight, LOW);
    }
    if (direction == backward) {
        // turn on motor and move backward
        digitalWrite(inALeft, LOW);
        digitalWrite(inBLeft, HIGH);
        digitalWrite(inARight, LOW);
        digitalWrite(inBRight, HIGH);
    }
    if (direction == CCW) {
        // turn on motor and move CCW
        digitalWrite(inALeft, LOW);
        digitalWrite(inBLeft, HIGH);
        digitalWrite(inARight, HIGH);
        digitalWrite(inBRight, LOW);
    }
    if (direction == CW) {
        // turn on motor and move CW
        digitalWrite(inALeft, HIGH);
        digitalWrite(inBLeft, LOW);
        digitalWrite(inARight, LOW);
        digitalWrite(inBRight, HIGH);
    }
    if (direction == slowLeft) {
      analogWrite(pwmLeft, power);
      analogWrite(pwmRight, initialPower*motorMultiplier);
      digitalWrite(inALeft, HIGH);
      digitalWrite(inBLeft, LOW);
      digitalWrite(inARight, HIGH);
      digitalWrite(inBRight, LOW);
    }
    if (direction == slowRight) {
      analogWrite(pwmLeft, initialPower);
      analogWrite(pwmRight, power*motorMultiplier);
      digitalWrite(inALeft, HIGH);
      digitalWrite(inBLeft, LOW);
      digitalWrite(inARight, HIGH);
      digitalWrite(inBRight, LOW);
    }
}

void rampUp(float power, int direction, float rampTime, float angle) {
    bool rampUpComplete = true;
    float rampConst = rampTime;
    float powerConst = 255;
    float timeOffset = millis();
    int rampPower = 0;
    //Serial.print("Time offset = ");
    //Serial.println(timeOffset);
    while ((timeOffset + power * rampConst / powerConst) > float(millis()))
    {
        rampPower = (millis() - timeOffset) * powerConst / rampConst;
        // if(direction == CW || direction == CCW) {
        //   Serial.print("Current Angle: ");
        //   Serial.println(mpu.getAngleZ());
        //   mpu.update();
        //   //TODO: implement the check for acquireAngle here
        //   // It would like the following:
        //   //if(acquireAngle == regionAngle)
        //   if(mpu.getAngleZ() == angle/2) {
        //     rampUpComplete = false;
        //     rampDown(0, direction, (float (millis()) - timeOffset), rampPower);
        //     break;
        //   }
        // }
        // set motor speed via pwm
        bridgeControl(rampPower, direction);
        // debug
        /*Serial.print("Ramp power = ");
        Serial.println(rampPower);
        Serial.print("time: ");
        Serial.println(millis());
        Serial.println((power*rampConst)/powerConst);
        delay(15);*/
    }
    // if (mpu.getAngleZ() < angle / 2)    
      // if(direction == CCW && rampUpComplete) {
      //   //TODO: implement the check for acquireAngle here
      //   // It would like the following:
      //   //if(acquireAngle == regionAngle)
      //   while(mpu.getAngleZ() < angle) 
      //   {
      //     Serial.print("Current Angle: ");
      //     Serial.println(mpu.getAngleZ());
      //     mpu.update();
      //     bridgeControl(power, direction);
      //   }
      //   rampDown(0, direction, rampTime, rampPower);    
      // } 
      // if(direction == CW && rampUpComplete) {
      //   while(mpu.getAngleZ() > angle) 
      //   {
      //     Serial.print("Current Angle: ");
      //     Serial.println(mpu.getAngleZ());
      //     mpu.update();
      //     bridgeControl(power, direction);
      //   }
      //   rampDown(0, direction, rampTime, rampPower);  
      // }  
    if (direction == forward || direction == backward) {
        bridgeControl(power, direction);
    }
    ;
}

void rampDownApproach(float power, int direction, float rampTime, float initialPower) {
    if (initialPower > power) {
        float rampConst = rampTime;
        float powerConst = 255;
        float timeOffset = millis();
        //Serial.print("Time offset = ");
        //Serial.println(timeOffset);
        while ((timeOffset + (initialPower - power) * rampConst / powerConst) > float(millis()))
        {
            int rampPower = initialPower - (millis() - timeOffset) * powerConst / rampConst;
            // Serial.print("Current Angle: ");
            // Serial.println(mpu.getAngleZ());
            // mpu.update();
            // set motor speed via pwm
            Serial.print("UPDATED POWER: ");
            Serial.println(rampPower);
            Serial.print("OLD POWER: ");
            Serial.println(initialPower);
            bridgeControlApproach(rampPower, initialPower, direction);

            // debug
            /*Serial.print("Ramp power = ");
            Serial.println(rampPower);
            Serial.print("time: ");
            Serial.println(millis());
            Serial.println((power*rampConst)/powerConst);
            delay(15);  */
        }
        
        Serial.println("DONE POWERING");
        bridgeControlApproach(power, initialPower, direction);
    }
}

// FUNCTIONS FROM: test_base_approach

double read_distance(int trigPin, int echoPin) {
    long duration;
    double distance, totalDistance, averageDistance;

    digitalWrite(trigPin, LOW);

    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);

    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = (duration * 0.034 / 2);

    return distance;
}

void approach_straighten(float power){
  float tolerance = 10;

  float leftSensor = read_distance(trigPinLeft, echoPinLeft);
  float rightSensor = read_distance(trigPinRight, echoPinRight);

  //debugging
  // Serial.print("Left Sensor: ");
  // Serial.println(leftSensor);
  // Serial.print("Right Sensor: ");
  // Serial.println(rightSensor);

  if(abs(read_distance(trigPinLeft, echoPinLeft) - read_distance(trigPinRight, echoPinRight))  > tolerance){
      while(abs(read_distance(trigPinLeft, echoPinLeft) - read_distance(trigPinRight, echoPinRight))  > tolerance){
        float leftSensor = read_distance(trigPinLeft, echoPinLeft);
        float rightSensor = read_distance(trigPinRight, echoPinRight);

        //debugging
        // Serial.print("Left Sensor: ");
        // Serial.println(leftSensor);
        // Serial.print("Right Sensor: ");
        // Serial.println(rightSensor);

        if (leftSensor < rightSensor){
          // decrease power in LM
          float multiplier = (leftSensor / rightSensor);
          // rampDown(power, 3);
          rampDownApproach(multiplier*power, slowLeft, 1000, power); //new
        } else if (rightSensor < leftSensor) {
          // decrease power in RM
          float multiplier = (rightSensor / leftSensor);
          // rampDown(multiplier*power, power, 4);
          rampDownApproach(multiplier*power, slowRight, 1000, power); //new
        }

      }

      rampUp(power, forward, 1000, 0);
  }

}

void base_approach(){
  
  Serial.println("1....2..");
  delay(3000);
  Serial.println("3!");

  Serial.println("RAMP UP START");
  // rampUp(80, 2);
  rampUp(80, forward, 1000, 0); //new 
 
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

  // stop();
  stop(forward); //new

  delay(5000);

  // rampUp(80, 2);
  rampUp(80, forward, 1000, 0); //new 

  Serial.println("STARTING APPROACH");

  while(read_distance(trigPinLeft, echoPinLeft) > 10 || read_distance(trigPinRight, echoPinRight) > 10){
      approach_straighten(80);
  }
  
  // stop();
  stop(forward); //new
  Serial.println("Stopped");

}



