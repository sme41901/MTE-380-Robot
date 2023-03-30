// #include <FiniteStateMachine.h>
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

//constants
const int wallToRamp = 151;
const int courseWidth = 248;
const int courseHeight = 220;
const float halfWallToRamp = 75.5;
const float offset = 14.5;
const String currentState = "BASE_SEARCH";

bool inState;

void setup() {
  inState = false;

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


void loop(){

  if(inState == false){

    inState = true;  

    if(currentState == "WALL_APPROACH"){     
      wall_approach();
    } else if(currentState == "WALL_ASCEND"){
      wall_up();
    } else if(currentState == "WALL_UPHILL"){
      wall_down();
    } else if(currentState == "WALL_APEX"){
      wall_apex();
    } else if(currentState == "WALL_DESCEND"){
      wall_descend();
    } else if(currentState == "WALL_DISENGAGE"){ // when US reads less than 5
      wall_disengage();
    } else if(currentState == "BASE_SEARCH"){ //when IMU is 0 horizotal
      base_search();
    } else if(currentState == "BASE_APPROACH"){
      base_approach();
    }

  }
}


void reset_imu(){
    Serial.println(F("Resetting IMU"));
    delay(1000);
    mpu.calcOffsets();
    Serial.println("IMU Reset");
}


void wall_approach(){
  move_forward(20);
  while(average_distance() < 50){
  }

  inState = false;
}

void wall_engage(){
  move_forward(50);
  while(average_distance()  > 5){
    straighten();
  }

  inState = false;
}


void wall_up(){ 
  move_forward(100);
  while(average_distance() < 20){
    // straighten();
  }

  move_forward(50);
  while(average_distance() < 20){
    // straighten();
  }

  inState = false;
}


void wall_down(){
  move_forward(50); 
  while(average_distance() > 5){

    // if(/*velocity > 2 && acceleration > 0*/){
    // //MOTOR LEFT REDUCE TORQUE
    // //MOTOR RIGHT REDUCE TORQUE
    // }
  }

  inState = false;
}

void base_search(){ // flip

  move_backward(10);

  while(average_distance() > (courseHeight -  (wallToRamp / 2))){
  }

  motor_stop();

  float expectedX = courseHeight - (wallToRamp / 2);

  float x = average_distance();

  if(x < expectedX){
    motor_stop();
    return;
  }

  reset_imu();
  
  rotate_clockwise();
  while(mpu.getAngleZ() < 90){ 
    mpu.update();
  }

  motor_stop();

  
  float yRight = average_distance();
  float yLeft = courseWidth - average_distance() - (2 * offset);
  float maxAngleLeft = atan(x/yLeft);
  float maxAngleRight = atan(x/yRight);

  reset_imu();

  rotate_counterclockwise();

  //REGION 1

  while(mpu.getAngleZ() < maxAngleRight){ 
    if(average_distance() < yRight ){
      motor_stop();
      break;
    }
  }

  //REGION 2

  while( mpu.getAngleZ() < (180 - maxAngleLeft) ){
    if(average_distance()  < x ){
      motor_stop();
      break;
    }
  }

  //REGION 3

  while( mpu.getAngleZ() < 180 ){
    if(average_distance() < yLeft){
      motor_stop();
      break;
    }
  }

  motor_stop();

  //FACE FORWARD

  reset_imu();

  rotate_clockwise();

  while( mpu.getAngleZ() != 90 ){
  }

  motor_stop();

  move_backward(50); 
  while(average_distance() != (courseHeight - wallToRamp)){ 
  }

  motor_stop();

  x -= (wallToRamp / 2); // or x = /* HEIGHT OF COURSE */ - average_distance() - /*  OFFSET */

  maxAngleLeft = atan(x/yLeft);
  maxAngleRight = atan(x/yRight);

  rotate_counterclockwise();
  while(mpu.getAngleZ() != 0 ){
  }

  motor_stop();

  rotate_clockwise();

  //REGION 3

  while( mpu.getAngleZ() < maxAngleLeft ){
    if(average_distance() < yLeft ){
      motor_stop();
      break;
    }
  }

  //REGION 2

  while( mpu.getAngleZ() < (180-maxAngleRight) ){
    if(average_distance() < x ){
      motor_stop();
      break;
    }
  }

  //REGION 1

  while( mpu.getAngleZ() < 180  ){
    if(average_distance() < yRight ){
      motor_stop();
      break;
    }
  }

  inState = false;

}


// void base_search(){ //no flip

//   rampUp(50, 2);

//   while(average_distance() < (wallToRamp / 2)){
//   }

//   motor_stop();

//   float x = courseHeight - average_distance() - offset;
  

//   while( mpu.getAngleZ() < 90 ){
//     rotate_clockwise();
//     // rampUp()
//   }

//   motor_stop();

  
//   float yLeft = average_distance() - offset;
//   float yRight = courseWidth - average_distance() + offset;
//   float maxAngleLeft = atan(x/yLeft);
//   float maxAngleRight = atan(x/yRight);

//   reset_imu();

//   //REGION 1

//   while( mpu.getAngleZ() < maxAngleLeft ){
//     if(average_distance() < yLeft ){
//       motor_stop();
//       break;
//     }
//   }

//   //REGION 2

//   while( mpu.getAngleZ() < (180-maxAngleRight) ){
//     if(average_distance() < x ){
//       motor_stop();
//       break;
//     }
//   }

//   //REGION 3

//   while( mpu.getAngleZ() < 180  ){
//     if(average_distance() < yRight ){
//       motor_stop();
//       break;
//     }
//   }

//   //FACE FORWARD

//   rotate_counterclockwise();

//   while(mpu.getAngleZ() != 90){
//   }

//   motor_stop();

//   //rampUp();
  
//   while(average_distance() != (courseHeight - wallToRamp)){ 
//   }

//   //rampDown();

//   x -= wallToRamp / 2; 

//   maxAngleLeft = atan(x/yLeft);
//   maxAngleRight = atan(x/yRight);

//   //rampUp();

//   while(mpu.getAngleZ() != 0){
//   }

//   //rampDown();

//       //REGION 1

//   while( mpu.getAngleZ() < maxAngleLeft ){
//     if(average_distance() < yLeft ){
//       motor_stop();
//       break;
//     }
//   }

//   //REGION 2

//   while( mpu.getAngleZ() < (180-maxAngleRight) ){
//     if(average_distance() < x ){
//       motor_stop();
//       break;
//     }
//   }

//   //REGION 3

//   while( mpu.getAngleZ() < 180  ){
//     if(average_distance() < yRight ){
//       motor_stop();
//       break;
//     }
//   }

//   inState = false;

// }

void base_found(){
  move_backward(50);
  
  while(average_distance() > 5){
    // straighten();
  }

  motor_stop();
}











