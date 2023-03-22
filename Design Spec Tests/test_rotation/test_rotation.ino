#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
unsigned long timer = 0;
//Define Motor Pins
const int inALeft = 2;
const int inBLeft = 4;
const int pwmLeft = 3;
const int inARight = 13;
const int inBRight = 12;
const int pwmRight = 11;

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

  Serial.begin(9600);
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
  do {
    Serial.print("Move CW ");
    Serial.println(mpu.getAngleZ());
    rampUp(255, 3, 5000);
    mpu.update();    
  } while(mpu.getAngleZ() < 180);
  rampDown(0, 3, 5000, 255);
  delay(2000);
  do {
    Serial.print("Move CCW ");
    Serial.println(mpu.getAngleZ());
    rampUp(255, 4, 5000);
    mpu.update();  
  } while(mpu.getAngleZ() > 0);
  rampDown(0, 4, 5000, 255);
  delay(2000);  
}

void rampUp(float power, int direction, float rampTime) {
  float rampConst = rampTime;
  float powerConst = 255;
  float timeOffset = millis();
  /*Serial.print("Time offset = ");
  Serial.println(timeOffset);*/
  while((timeOffset + power*rampConst/powerConst) > float (millis()))
  {
    int rampPower = (millis() - timeOffset)*powerConst/rampConst;
    // set motor speed via pwm
    move(rampPower, direction);
    // debug
    /*Serial.print("Ramp power = ");
    Serial.println(rampPower);
    Serial.print("time: ");
    Serial.println(millis());
    Serial.println((power*rampConst)/powerConst);
    delay(15);*/
  }
  move(power, direction);
}

void rampDown(float power, int direction, float rampTime, float initialPower) {
  if(initialPower > power) {
    float rampConst = rampTime;
    float powerConst = 255;
    float timeOffset = millis();
    /*Serial.print("Time offset = ");
    Serial.println(timeOffset);*/
    while((timeOffset + (initialPower-power)*rampConst/powerConst) > float (millis()))
    {
      int rampPower = initialPower - (millis() - timeOffset)*powerConst/rampConst;
      // set motor speed via pwm
      move(rampPower, direction);
      
      // debug
      /*Serial.print("Ramp power = ");
      Serial.println(rampPower);
      Serial.print("time: ");
      Serial.println(millis());
      Serial.println((power*rampConst)/powerConst);
      delay(15);  */  
    }
    move(power, direction);
  }
}

void move(int power, int direction) {
  // set motor speed via pwm
  analogWrite(pwmLeft, power);
  analogWrite(pwmRight, power);
  // direction = 0 stop
  if(direction == 0) {
  // turn off motors
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, LOW);
  }
  if(direction == 1) {
  // turn on motor and move backward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
  }
  if(direction == 2) {
    // turn on motor and move forward
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, HIGH);
    digitalWrite(inBRight, LOW);
  }
  if(direction == 3) {
    // turn on motor and rotate CW
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
  }
  if(direction == 4) {
    // turn on motor and rotate CCW
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, HIGH);
    digitalWrite(inBRight, LOW);
  }
  
}

void stop() {
  // set motor speed via pwm
  move(0, 0);
}

