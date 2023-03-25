#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
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
int yOffset /*does not change*/, xOffset;
//4ifs for the 4 regions and pass the region to the acquireTarget

void setup() {
  // put your setup code here, to run once:
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
  
  Serial.begin(115200);
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { } // stop everything if could not connect to MPU6050
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  // rotate test
  
  // delay(2000);
  // rotate(255*0.13, CCW, 1000, (180*0.95), false);
  // mpu.update();
  // delay(5000);
  // mpu.update();

//clean and disengage
delay(2000);
move(255*0.8, forward, 1000, -1, 100000);

//Speed test
  // delay(2000);  
  // move(255*0.27, forward, 2500, 0, 6000);
  // delay(5000);    
  
  // while(keepMoving){
  //   move(255, 2);
  //   delay(500);
  //   rampDown(255*0.4, 2, 1000, 255);
  //   delay(5000);
  // }
}
//talked to edward and come up with design specification test plan
//condition, robot rotates and goes from vertical to horizontal, with the 2 magnets no longer engaged with the wall

//TODO rotate function (angle,power,direction) should be boolean if true then give acquire the angle at which 
// you need to remember the ramp power from ramping up in case X 
// with search and acquire post
//TODO IMU power adjustment function
//TODO inverse fwd and inv bkwd
//burst function
void move(float power, int direction, float rampTime, float slowdown, unsigned long duration) {
  // direction = 0 stop
  // if(direction == stop) {
  // // turn off motors
  //   digitalWrite(inALeft, LOW);
  //   digitalWrite(inBLeft, LOW);
  //   digitalWrite(inARight, LOW);
  //   digitalWrite(inBRight, LOW);
  // }
  if(direction == forward) {
    // turn on motor and move forward
    rampUp(power, forward, rampTime, NULL);
    delay(duration);
    if(slowdown > -1){
      rampDown(slowdown, forward, rampTime, power);
    }
  }
  if(direction == backward) {
    // turn on motor and move backward
    rampUp(power, backward, rampTime, NULL);
    delay(duration);
    if(slowdown > -1){
      rampDown(slowdown, forward, rampTime, power);
    }
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
  Serial.print("START ANGLE: ");
  Serial.println(startAngle);
  //CCW rotation
  if(thetaAngle > 0) {
    while((rampUpStartTime + power*rampTimeConst/powerConst) > float (millis()) && (((thetaAngle/2 + startAngle)) > mpu.getAngleZ()) /* && acquireTarget is false*/)
    {
      mpu.update();
      rampUpPower = (float (millis()) - rampUpStartTime)*powerConst/rampTimeConst;    
      bridgeControl(rampUpPower, direction);
      rampUpTime = float (millis()) - rampUpStartTime;
    }
    mpu.update();
    rampUpAngle = mpu.getAngleZ() - startAngle;  
    Serial.print("RAMP UP ANGLE: ");
    Serial.println(rampUpAngle);
    Serial.print("RAMP UP TIME: ");
    Serial.println(rampUpTime);
    // check if robot finished ramping up already
    if((thetaAngle/2 + startAngle) > mpu.getAngleZ()) {
      while(mpu.getAngleZ() < (thetaAngle + startAngle - rampUpAngle)) {
        mpu.update();
        bridgeControl(power, direction);      
        Serial.print("CONSTANT SPEED ANGLE: ");
        Serial.println(mpu.getAngleZ());
      }
      rampUpPower = power;
    }
    float rampDownStartTime = millis();
    Serial.print("RAMP DOWN START TIME: ");
    Serial.println(rampDownStartTime); 
    Serial.print("RAMP UP POWER: ");
    Serial.println(rampUpPower);
    while((rampDownStartTime + rampUpTime) > float (millis()) && (thetaAngle + startAngle) > mpu.getAngleZ()) 
    {
      mpu.update();
      rampDownPower = rampUpPower - (float (millis()) - rampDownStartTime)*powerConst/(rampUpTime);
      if(rampDownPower < 0) {
        rampDownPower = 0;
      }
      bridgeControl(rampDownPower, direction);
      Serial.print("RAMP DOWN ");
      Serial.println(rampDownPower);
      Serial.print("RAMP DOWN ANGLE: ");
      Serial.println(mpu.getAngleZ());
    }
    bridgeControl(0, direction);
    float rampDownTime = float (millis()) - rampDownStartTime;
    Serial.print("RAMP DOWN TIME: ");
    Serial.println(rampDownTime);
    stop(direction);
  }
  //CW rotation
  if(thetaAngle < 0) {
    while((rampUpStartTime + power*rampTimeConst/powerConst) > float (millis()) && (((thetaAngle/2 + startAngle)) < mpu.getAngleZ()) /* && acquireTarget is false*/)
    {
      mpu.update();
      rampUpPower = (float (millis()) - rampUpStartTime)*powerConst/rampTimeConst;    
      bridgeControl(rampUpPower, direction);
      rampUpTime = float (millis()) - rampUpStartTime;
    }
    mpu.update();
    rampUpAngle = startAngle - mpu.getAngleZ();  
    Serial.print("RAMP UP ANGLE: ");
    Serial.println(rampUpAngle);
    Serial.print("RAMP UP TIME: ");
    Serial.println(rampUpTime);
    // check if robot finished ramping up already
    if(((thetaAngle/2 + startAngle)) < mpu.getAngleZ()) {
      while(mpu.getAngleZ() > (thetaAngle + startAngle + rampUpAngle)) {
        mpu.update();
        bridgeControl(power, direction);      
        Serial.print("CONSTANT SPEED ANGLE: ");
        Serial.println(mpu.getAngleZ());
      }
      rampUpPower = power;
    }
    float rampDownStartTime = millis();
    Serial.print("RAMP DOWN START TIME: ");
    Serial.println(rampDownStartTime); 
    Serial.print("RAMP UP POWER: ");
    Serial.println(rampUpPower);
    while((rampDownStartTime + rampUpTime) > float (millis()) && (thetaAngle + startAngle) < mpu.getAngleZ()) 
    {
      mpu.update();
      rampDownPower = rampUpPower - (float (millis()) - rampDownStartTime)*powerConst/(rampUpTime);
      if(rampDownPower < 0) {
        rampDownPower = 0;
      }
      bridgeControl(rampDownPower, direction);
      Serial.print("RAMP DOWN ");
      Serial.println(rampDownPower);
      Serial.print("RAMP DOWN ANGLE: ");
      Serial.println(mpu.getAngleZ());
    }
    bridgeControl(0, direction);
    float rampDownTime = float (millis()) - rampDownStartTime;
    Serial.print("RAMP DOWN TIME: ");
    Serial.println(rampDownTime);
    stop(direction);
  }  
}

//void acquireTarget(int stage, int region)

void rampUp(float power, int direction, float rampTime, float angle) {
  bool rampUpComplete = true;
  float rampConst = rampTime;
  float powerConst = 255;
  float timeOffset = millis();
  int rampPower = 0;
  //Serial.print("Time offset = ");
  //Serial.println(timeOffset);
  while((timeOffset + power*rampConst/powerConst) > float (millis()))
  {
    rampPower = (millis() - timeOffset)*powerConst/rampConst;
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
  if(direction == forward || direction == backward){
    bridgeControl(power, direction);    
  }  
;}

void rampDown(float power, int direction, float rampTime, float initialPower) {
  if(initialPower > power) {
    float rampConst = rampTime;
    float powerConst = 255;
    float timeOffset = millis();
    //Serial.print("Time offset = ");
    //Serial.println(timeOffset);
    while((timeOffset + (initialPower-power)*rampConst/powerConst) > float (millis()))
    {
      int rampPower = initialPower - (millis() - timeOffset)*powerConst/rampConst;
      Serial.print("Current Angle: ");
      Serial.println(mpu.getAngleZ());
      mpu.update();
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

void stop(int currentDirection) {
  int power = 255*0.05;
  int counterTime = 100;
  if(currentDirection == forward) {        
    analogWrite(pwmLeft, power);
    analogWrite(pwmRight, power);  
    // turn on motor and move backward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
    delay(counterTime);    
  }
  if(currentDirection == backward) {        
    analogWrite(pwmLeft, power);
    analogWrite(pwmRight, power);  
    // turn on motor and move forward
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, HIGH);
    digitalWrite(inBRight, LOW);
    delay(counterTime);    
  }
  if(currentDirection == CW) {
    analogWrite(pwmLeft, power);
    analogWrite(pwmRight, power);  
    // turn on motor and move CCW
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW); 
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);   
    delay(counterTime);   
  }
  if(currentDirection == CCW) {
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
  // set motor speed via pwm
  analogWrite(pwmLeft, power/**0.945*/);
  analogWrite(pwmRight, power);
  if(direction == backward) {
    // turn on motor and move forward
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, HIGH);
    digitalWrite(inBRight, LOW);
  }
  if(direction == forward) {
    // turn on motor and move backward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
  }
  if(direction == CW) {
    // turn on motor and move CW
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, HIGH);
    digitalWrite(inBRight, LOW);
  }
  if(direction == CCW) {
    // turn on motor and move CCW
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW); 
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);   
  }
}