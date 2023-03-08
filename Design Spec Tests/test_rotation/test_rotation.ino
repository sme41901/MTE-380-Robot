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
    Serial.print("Move CW");
    Serial.println(mpu.getAngleZ());
    rotate_counterclockwise();
    mpu.update();    
  } while(mpu.getAngleZ() < 180);
  stop_motors();
  do {
    Serial.print("Move CCW");
    Serial.println(mpu.getAngleZ());
    rotate_clockwise();
    mpu.update();  
  } while(mpu.getAngleZ() > 0);
  stop_motors();
}

void stop_motors(){
  digitalWrite(inALeft, LOW);
  digitalWrite(inBLeft, LOW);
  digitalWrite(inARight, LOW);
  digitalWrite(inBRight, LOW);
}

void rotate_clockwise(){
  analogWrite(pwmLeft, 50);
  analogWrite(pwmRight, 50);
  digitalWrite(inALeft, HIGH);
  digitalWrite(inBLeft, LOW);
  digitalWrite(inARight, LOW);
  digitalWrite(inBRight, HIGH);
}


void rotate_counterclockwise(){
  analogWrite(pwmLeft, 50);
  analogWrite(pwmRight, 50);
  digitalWrite(inALeft, LOW);
  digitalWrite(inBLeft, HIGH);
  digitalWrite(inARight, HIGH);
  digitalWrite(inBRight, LOW);
}

