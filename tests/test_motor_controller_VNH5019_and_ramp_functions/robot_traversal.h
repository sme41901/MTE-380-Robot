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
const int forward = 1;
const int backward = 2;
const int CW = 3;
const int CCW = 4;
const int adjustLeft = 5;
const int adjustRight = 6;
//US sensors
const int trigPinLeft = 10;
const int echoPinLeft = 9;
const int trigPinRight = 6;
const int echoPinRight = 5;
int currentStage = 0;

int sensorToCentreOffset = 3;
const float rampLength = 151;

// these values get updated in void loop
float xDistanceFromWallToBoundary = 124;
float yDistanceFromWallToBoundary = 110;

const int numberOfStages = 3;
float searchDistanceIncrement = rampLength/numberOfStages;

void resetCurrentStage() {
  currentStage = 0;
}

void increaseCurrentStage() {
  currentStage++;
}

double readDistance(int trigPin, int echoPin){
  long duration;
  double distance, totalDistance, averageDistance;  

  for(int i = 0; i < 50; i++){
    digitalWrite(trigPin, LOW);

    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);

    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = (duration * 0.034 / 2);  

    totalDistance += distance;
  }

  averageDistance = totalDistance / 50;

  return averageDistance;
}

bool acquireTarget(bool checkForTarget, float currentAngle, float startAngle) {
  bool result = false;
  if(!checkForTarget) {
    return result;
  }    
  else {
    // stage 0: check in front of you
    // if(currentStage == 0) {
    //   if(readDistance(trigPinLeft, echoPinLeft) < yDistanceFromWallToBoundary || readDistance(trigPinRight, echoPinRight) < yDistanceFromWallToBoundary) {
    //     //approach        
    //   }
    // }
    // stage 1 to numberOfStages - 1
    // else { 
      double yDistanceFromRobotToWall =  yDistanceFromWallToBoundary - sensorToCentreOffset - (currentStage*searchDistanceIncrement);
      double xDistanceFromRobotToWall =  xDistanceFromWallToBoundary - sensorToCentreOffset;
      double regionOneAngle = atan2(yDistanceFromRobotToWall, xDistanceFromRobotToWall) * 180/PI;           
      double regionTwoAngle = 180 - 2*regionOneAngle;  
      // check region 1 for post
      if(currentAngle > (startAngle - regionOneAngle)) {
        // if(readDistance(trigPinLeft, echoPinLeft) < xDistanceFromRobotToWall || readDistance(trigPinRight, echoPinRight) < xDistanceFromRobotToWall) {
        //   return !result;
        // }
        Serial.print("region 1: ");
        Serial.println(currentAngle);
      } 
      //check region 2 for post
      else if(currentAngle > (startAngle - regionOneAngle - regionTwoAngle)) {
        // if(readDistance(trigPinLeft, echoPinLeft) < yDistanceFromRobotToWall || readDistance(trigPinRight, echoPinRight) < yDistanceFromRobotToWall) {
        //   return !result;
        // }
        Serial.print("region 2: ");
        Serial.println(currentAngle);
      }

      //check region 3 for post
      else if(currentAngle > (startAngle - (2*regionOneAngle) - regionTwoAngle)) {
        Serial.print("region 3: ");
        Serial.println(currentAngle);   
        // if(readDistance(trigPinLeft, echoPinLeft) < xDistanceFromRobotToWall || readDistance(trigPinRight, echoPinRight) < xDistanceFromRobotToWall) {
        //   return !result;
        // }
      }
    //}
  }
  return result;
}

void stop(int currentDirection) {
    int power = 255 * 0.05;
    int counterTime = 150;
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
    if (currentDirection == CCW) {
        analogWrite(pwmLeft, power);
        analogWrite(pwmRight, power);
        // turn on motor and move CCW
        digitalWrite(inALeft, HIGH);
        digitalWrite(inBLeft, LOW);
        digitalWrite(inARight, LOW);
        digitalWrite(inBRight, HIGH);
        delay(counterTime);
    }
    if (currentDirection == CW) {
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
//98% applied to pwmright at 1% power moving bkwd
//going down the wall, give 1 to pwmright and give 5 to pwmleft
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

void rotate(float power, int direction, float rampTime, float thetaAngle, bool checkForTarget) {
  float rampTimeConst = rampTime;
  float powerConst = 255;
  float rampUpStartTime = millis();
  int rampUpPower = 0, rampDownPower = 0;
  float rampUpAngle, rampUpTime;
  mpu.update();
  float startAngle = mpu.getAngleZ(); 
  float currentAngle = mpu.getAngleZ();  
  Serial.print("START ANGLE: ");
  Serial.println(startAngle);
  //CCW rotation
  if(thetaAngle > 0) {
    while((rampUpStartTime + power*rampTimeConst/powerConst) > float (millis()) && (((thetaAngle/2 + startAngle)) > mpu.getAngleZ()) && !acquireTarget(checkForTarget, currentAngle, startAngle))
    {
      mpu.update();
      currentAngle = mpu.getAngleZ();  
      rampUpPower = (float (millis()) - rampUpStartTime)*powerConst/rampTimeConst;    
      bridgeControl(rampUpPower, direction);
      rampUpTime = float (millis()) - rampUpStartTime;
      rampUpAngle = mpu.getAngleZ() - startAngle;   
      // Serial.print("CURRENT RAMP UP ANGLE: ");
      // Serial.println(currentAngle);     
    }
    mpu.update();
    // Serial.print("RAMP UP ANGLE: ");
    // Serial.println(rampUpAngle);
    // Serial.print("RAMP UP TIME: ");
    // Serial.println(rampUpTime);
    // check if robot finished ramping up already but still has not found the target
    if((thetaAngle/2 + startAngle) > mpu.getAngleZ()) {
      while((mpu.getAngleZ() < (thetaAngle + startAngle - rampUpAngle)) && !acquireTarget(checkForTarget, currentAngle, startAngle)) {
        mpu.update();
        currentAngle = mpu.getAngleZ(); 
        bridgeControl(power, direction);  
        // Serial.print("CURRENT CONST ANGLE: ");
        // Serial.println(currentAngle);               
        // Serial.print("CONSTANT SPEED ANGLE: ");
        // Serial.println(mpu.getAngleZ());
      }
      rampUpPower = power;
    }
    // check if robot has found the target
    if(acquireTarget(checkForTarget, currentAngle, startAngle)) {
      //approach      
    } 
    float rampDownStartTime = millis();
    // Serial.print("RAMP DOWN START TIME: ");
    // Serial.println(rampDownStartTime); 
    // Serial.print("RAMP UP POWER: ");
    // Serial.println(rampUpPower);
    while((rampDownStartTime + rampUpTime) > float (millis()) && (thetaAngle + startAngle) > mpu.getAngleZ() && !acquireTarget(checkForTarget, currentAngle, startAngle)) 
    {
      mpu.update();
      currentAngle = mpu.getAngleZ();
      rampDownPower = rampUpPower - (float (millis()) - rampDownStartTime)*powerConst/(rampUpTime);
      if(rampDownPower < 0) {
        rampDownPower = 0;
      }
      bridgeControl(rampDownPower, direction);
      //       Serial.print("CURRENT RAMP DOWN ANGLE: ");
      // Serial.println(currentAngle);   
      // Serial.print("RAMP DOWN POWER: ");
      // Serial.println(rampDownPower);
      // Serial.print("RAMP DOWN ANGLE: ");
      // Serial.println(mpu.getAngleZ());
    }
    bridgeControl(0, direction);
    float rampDownTime = float (millis()) - rampDownStartTime;
    // Serial.print("RAMP DOWN TIME: ");
    // Serial.println(rampDownTime);
    stop(direction);
    mpu.update();
    Serial.print("FINAL ANGLE: ");
    Serial.println(mpu.getAngleZ());
  }
  //CW rotation
  if(thetaAngle < 0) {
    while((rampUpStartTime + power*rampTimeConst/powerConst) > float (millis()) && (((thetaAngle/2 + startAngle)) < mpu.getAngleZ()) && !acquireTarget(checkForTarget, currentAngle, startAngle))
    {
      mpu.update();
      currentAngle = mpu.getAngleZ();  
      rampUpPower = (float (millis()) - rampUpStartTime)*powerConst/rampTimeConst;    
      bridgeControl(rampUpPower, direction);
      rampUpTime = float (millis()) - rampUpStartTime;
      rampUpAngle = startAngle - mpu.getAngleZ(); 
      //       Serial.print("CURRENT RAMP UP ANGLE: ");
      // Serial.println(currentAngle);   
      
    }
    mpu.update(); 
    // Serial.print("RAMP UP ANGLE: ");
    // Serial.println(rampUpAngle);
    // Serial.print("RAMP UP TIME: ");
    // Serial.println(rampUpTime);
    // check if robot finished ramping up already
    if((thetaAngle/2 + startAngle) < mpu.getAngleZ()) {
      while((mpu.getAngleZ() > (thetaAngle + startAngle + rampUpAngle) && !acquireTarget(checkForTarget, currentAngle, startAngle))) {
        mpu.update();
        currentAngle = mpu.getAngleZ(); 
        bridgeControl(power, direction);      
        // Serial.print("CONSTANT SPEED ANGLE: "); 
        // Serial.println(mpu.getAngleZ());
      //         Serial.print("CURRENT CONST ANGLE: ");
      // Serial.println(currentAngle);   
      }
      rampUpPower = power;
    }
    // check if robot sees the post
    if(acquireTarget(checkForTarget, currentAngle, startAngle)) {
      //approach      
    }    
    float rampDownStartTime = millis();
    // Serial.print("RAMP DOWN START TIME: ");
    // Serial.println(rampDownStartTime); 
    // Serial.print("RAMP UP POWER: ");
    // Serial.println(rampUpPower);
    while((rampDownStartTime + rampUpTime) > float (millis()) && (thetaAngle + startAngle) < mpu.getAngleZ() && !acquireTarget(checkForTarget, currentAngle, startAngle)) 
    {
      mpu.update();
      currentAngle = mpu.getAngleZ(); 
      rampDownPower = rampUpPower - (float (millis()) - rampDownStartTime)*powerConst/(rampUpTime);
      if(rampDownPower < 0) {
        rampDownPower = 0;
      }
      bridgeControl(rampDownPower, direction);
      //       Serial.print("CURRENT RAMP DOWN ANGLE: ");
      // Serial.println(currentAngle);   
      // Serial.print("RAMP DOWN ");
      // Serial.println(rampDownPower);
      // Serial.print("RAMP DOWN ANGLE: ");
      // Serial.println(mpu.getAngleZ());
    }
    bridgeControl(0, direction);
    float rampDownTime = float (millis()) - rampDownStartTime;
    // Serial.print("RAMP DOWN TIME: ");
    // Serial.println(rampDownTime);
    stop(direction);
    mpu.update();
    Serial.print("FINAL ANGLE: ");
    Serial.println(mpu.getAngleZ());
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
}

void rampDown(float power, int direction, float rampTime, float initialPower) {
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
            bridgeControl(rampPower, direction);

            // debug
            /*Serial.print("Ramp power = ");
            Serial.println(rampPower);
            Serial.print("time: ");
            Serial.println(millis());
            Serial.println((power*rampConst)/powerConst);
            delay(15);  */
        }
        bridgeControl(power, direction);
    }
}

void move(float power, int direction, float rampTime, float slowdown, unsigned long duration) {
    // direction = 0 stop
    // if(direction == stop) {
    // // turn off motors
    //   digitalWrite(inALeft, LOW);
    //   digitalWrite(inBLeft, LOW);
    //   digitalWrite(inARight, LOW);
    //   digitalWrite(inBRight, LOW);
    // }
    mpu.update();
    int startAngle = round(mpu.getAngleZ());
    Serial.print("START ANGLE : ");
    Serial.println(round(mpu.getAngleZ()));  
    if (direction == forward) {
        // turn on motor and move forward
        float multiplier = 10;
        rampUp(power, forward, rampTime, NULL);
        unsigned long startTime = millis();
        while((startTime + duration) > millis()) {
          mpu.update();        
          Serial.print("CURRENT ANGLE : ");
          Serial.println(round(mpu.getAngleZ()));  
          // if we stray by strayDegree, adjust the respective motor
          if(round(mpu.getAngleZ()) < startAngle)
          {
            while(round(mpu.getAngleZ()) < startAngle)
            {
              mpu.update();       
              bridgeControl(power + multiplier*round(mpu.getAngleZ()), adjustLeft);
              Serial.println("ADJUST LEFT");                
            }
            bridgeControl(power, adjustLeft);
          }       
          if(round(mpu.getAngleZ()) > startAngle)
          {
            while(round(mpu.getAngleZ()) > startAngle)
            {
              mpu.update(); 
              bridgeControl(power-multiplier*round(mpu.getAngleZ()), adjustRight);
              Serial.println("ADJUST RIGHT");                
            }
            bridgeControl(power, adjustRight);
          }                
        }        
        if (slowdown > -1) {
            rampDown(slowdown, forward, rampTime, power);
        }
    }
    if (direction == backward) {
        // turn on motor and move backward
        rampUp(power, backward, rampTime, NULL);
        delay(duration);
        if (slowdown > -1) {
            rampDown(slowdown, forward, rampTime, power);
        }
    }
}
// FUNCTIONS FROM: test_base_approach
double read_distance(int trigPin, int echoPin) {
    long duration;
    double distance, totalDistance, averageDistance;

    for (int i = 0; i < 20; i++) {
        digitalWrite(trigPin, LOW);

        delayMicroseconds(2);

        digitalWrite(trigPin, HIGH);

        delayMicroseconds(10);

        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);

        distance = (duration * 0.034 / 2);

        totalDistance += distance;
    }

    averageDistance = totalDistance / 20;

    // if(trigPin == trigPinLeft){
    //   Serial.print("Left Sensor: ");
    //   Serial.println(averageDistance);


    // }

    // if(trigPin == trigPinRight){
    //   Serial.print("Right Sensor: ");
    //   Serial.println(averageDistance);
    // }

    return averageDistance;
}
