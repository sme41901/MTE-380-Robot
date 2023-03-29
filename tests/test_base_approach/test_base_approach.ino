// #include "Wire.h"
// #include <MPU6050_light.h>
#include "robot_traversal.h"


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
  rampUp(180, 2);

  delay(10000);


 

  // while(read_distance(trigPinLeft, echoPinLeft) > 5 || read_distance(trigPinRight, echoPinRight) > 5){
  //   Serial.println("Approaching");
  //   approach(90);
  // }

  Serial.println("Stopped");
  stop();
  

}

void rampUp(float power, int direction) {
  float rampConst = 1000;
  float powerConst = 255;
  float timeOffset = millis();
  // Serial.print("Time start = ");
  // Serial.println(timeOffset);

  while((timeOffset + power*rampConst/powerConst) > float (millis()))
  {
    int rampPower = (millis() - timeOffset)*powerConst/rampConst;
    // set motor speed via pwm
    move(rampPower, power, direction);
    // debug
    /*Serial.print("Ramp power = ");
    Serial.println(rampPower);
    Serial.print("time: ");
    Serial.println(millis());
    Serial.println((power*rampConst)/powerConst);*/

    delay(150);    
  }
  move(power, 0, 2);

  
}

void rampDown(float power, float initialPower, int direction) {
  if(initialPower > power) {
    float rampConst = 1000;
    float powerConst = 255;
    float timeOffset = millis();
    // Serial.print("Time start = ");
    // Serial.println(timeOffset);
    while((timeOffset + (initialPower-power)*rampConst/powerConst) > float (millis()))
    {
      float rampPower = initialPower - (millis() - timeOffset)*powerConst/rampConst;
      // set motor speed via pwm
      move(rampPower, initialPower, direction);

      // delay(150);    
    }
    float timeEnd = millis();
    // Serial.print("hello");
    // Serial.print("Time end = ");
    // Serial.println(timeEnd);

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
    analogWrite(pwmRight, (power*(1.045)));
    // turn on motor and move forward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
  }
  if(direction == 3){
    analogWrite(pwmLeft, power);
    analogWrite(pwmRight, initialPower);
    // turn on motor and move forward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
  }
  if(direction == 4){
    analogWrite(pwmLeft, initialPower);
    analogWrite(pwmRight, power);
    // turn on motor and move forward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
  }
  
}

// double read_distance(int trigPin, int echoPin){
//   long duration;
//   double distance, totalDistance, averageDistance;  

//   int loops = 1;

//   for(int i = 0; i < loops; i++){
//     digitalWrite(trigPin, LOW);

//     delayMicroseconds(2);

//     digitalWrite(trigPin, HIGH);

//     delayMicroseconds(10);

//     digitalWrite(trigPin, LOW);

//     duration = pulseIn(echoPin, HIGH);

//     distance = (duration * 0.034 / 2);  

//     totalDistance += distance;
//   }

//   averageDistance = totalDistance / loops;

//   return averageDistance;
// }

void approach(float power){
  float tolerance = 5;

  if(abs(read_distance(trigPinLeft, echoPinLeft) - read_distance(trigPinRight, echoPinRight))  > tolerance){
      while(abs(read_distance(trigPinLeft, echoPinLeft) - read_distance(trigPinRight, echoPinRight))  > tolerance){
        float leftSensor = read_distance(trigPinLeft, echoPinLeft);
        float rightSensor = read_distance(trigPinRight, echoPinRight);

        Serial.print("Left Sensor: ");
        Serial.println(leftSensor);
        Serial.print("Right Sensor: ");
        Serial.println(rightSensor);

        if (leftSensor < rightSensor){
          // decrease power in LM
          float multiplier = (leftSensor / rightSensor);
          rampDown(multiplier*power, power, 3);
        } else if (rightSensor < leftSensor) {
          // decrease power in RM
          float multiplier = (rightSensor / leftSensor);
          rampDown(multiplier*power, power, 4);
        }

      }

      rampUp(power, 2);
  }

}
