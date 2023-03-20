#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);


//Define Ultrasonic Pins
const int echoPinLeft = 5;
const int trigPinLeft = 6;
const int trigPinRight = 9;
const int echoPinRight = 10;

//Define Motor Pins
const int inALeft = 2;
const int inBLeft = 4;
const int pwmLeft = 3;
const int inARight = 13;
const int inBRight = 12;
const int pwmRight = 11;

void setup() {

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
  rampUp(50, 2);

  approach();


  

}

void rampUp(float power, int direction) {
  float rampConst = 1000;
  float powerConst = 255;
  float timeOffset = millis();
  Serial.print("Time offset = ");
  Serial.println(timeOffset);
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
    Serial.println((power*rampConst)/powerConst);*/

    delay(150);    
  }
  move(power, 2);

  
}

void move(int power, int direction) {
  // set motor speed via pwm
  analogWrite(pwmLeft, power);
  analogWrite(pwmRight, power);
  // direction = 0 go forward
  if(direction == 0) {
  // turn on motor and move forward
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
  
}

void approach(){
  
  

}
