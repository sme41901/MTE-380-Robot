// #include <FiniteStateMachine.h>
#include "robot_traversal.h"

//constants
const int wallToRamp = 151;
const int courseHeight = 220;
const float halfWallToRamp = 75.5;
const float offset = 14.5;
const String currentState = "WALL_APPROACH";

void setup() {

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

  //sensors
  pinMode(trigPinLeft, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinLeft, INPUT); // Sets the echoPin as an Input
  pinMode(trigPinRight, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinRight, INPUT); // Sets the echoPin as an Input

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

void wall_approach(){
  Serial.println("IN WALL APPROACH");
  Serial.println("=========================");
  Serial.println(mpu.getAngleY());
  Serial.println("=========================");
  // while(mpu.getAngleY() > (-84.91 + 10) || mpu.getAngleY() < (-84.91 - 10) ){
  while(mpu.getAngleY() > -84.91 ){
    Serial.println(mpu.getAngleY());
    mpu.update();
    move(255, forward, 1000, 0, 6000);
    Serial.println(currentState);
  }
  Serial.println("OUT WALL APPROACH");
  currentState = "WALL_ASCEND";
}

void wall_climb(){ 
  // while(mpu.getAngleY() < (-34.84 + 10) || mpu.getAngleY() > (-34.84 - 10) )
  while(mpu.getAngleY() < -34.84  ){
    
    mpu.update();
    move(255*0.8, forward, 1000, 0, 3000);
    Serial.println(currentState);
  }
  currentState = "WALL_UPHILL";
}

void wall_uphill(){ 
  while(mpu.getAngleY() < 33.86){
    mpu.update();
    move(255*0.4, forward, 1000, 0, 3000);
    Serial.println(currentState);
  }
  currentState = "WALL_DOWNHILL";
}


void wall_downhill(){
  while(mpu.getAngleY() < 84.91){
        mpu.update();
    move(255*0.4, forward, 1000, 0, 6000);
    Serial.println(currentState);
  }
  currentState = "WALL_DESCEND";
}

void wall_descend(){
  while(readDistance(trigPinLeft, echoPinLeft) < 10 || readDistance(trigPinRight, echoPinRight) < 10)
  {
    mpu.update();
    move(255*0.01, backward, 1000, 0, 6000);
    Serial.println(currentState);
  }  
  currentState = "WALL_DISENGAGE";
}

void wall_disengage(){
  while(mpu.getAngleY() > 0 )
  {
        mpu.update();
    move(255*0.01, backward, 1000, 0, 6000);
    Serial.println(currentState);
  }  
  currentState = "BASE_SEARCH";
}

void base_search(){
  delay(2000);
  int i = 1;
  resetCurrentStage();
  // yDistanceFromWallToBoundary = (readDistance(trigPinLeft, echoPinLeft) + readDistance(trigPinRight, echoPinRight)) / 2;
  // Serial.print("y dist: ");
  // Serial.println(yDistanceFromWallToBoundary);
  // else { 
  while(currentStage < numberOfStages /*&& !postFound*/) {   
    
    Serial.println(currentState); 
    if(currentStage > 0)
    {
      i = 2;
    }
    yDistanceFromWallToBoundary = (readDistance(trigPinLeft, echoPinLeft) + readDistance(trigPinRight, echoPinRight)) / 2;
    moveForwardUS(yDistanceFromWallToBoundary, yDistanceFromWallToBoundary - i*searchDistanceIncrement);     
    rotate(255*0.12, CW, 1000, -90*0.925, false);
    Serial.print("CURRENT ANGLE : ");
    Serial.println(round(mpu.getAngleZ()));     
    mpu.update();
    delay(1000);
    xDistanceFromWallToBoundary = (readDistance(trigPinLeft, echoPinLeft) + readDistance(trigPinRight, echoPinRight)) / 2;    
    mpu.update();
    rotate(255*0.12, CCW, 1000, 180*0.945, false);
    mpu.update();
        Serial.print("CURRENT ANGLE : ");
    Serial.println(round(mpu.getAngleZ())); 
    delay(1000);
    //if(!postFound)
    rotate(255*0.12, CW, 1000, -90*0.925, false);
    mpu.update();    
        Serial.print("CURRENT ANGLE : ");
    Serial.println(round(mpu.getAngleZ())); 
    increaseCurrentStage();  
      // only move up if we are not on the final stage    
    //}
  }
  currentState = "BASE_FOUND";
}

void loop(){ 
    mpu.update();
    if(currentState == "WALL_APPROACH"){     
      wall_approach();
    } else if(currentState == "WALL_ASCEND"){
      wall_climb();
    } else if(currentState == "WALL_UPHILL"){
      wall_uphill();
    } else if(currentState == "WALL_DOWNHILL"){
      wall_downhill();
    } else if(currentState == "WALL_DESCEND"){
      wall_descend();
    } else if(currentState == "WALL_DISENGAGE"){ // when US reads less than 5
      wall_disengage();
    } else if(currentState == "BASE_SEARCH"){ //when IMU is 0 horizotal
      base_search();
    } else if(currentState == "BASE_FOUND"){
      stop(forward);
    }
  }



// void reset_imu(){
//     Serial.println(F("Resetting IMU"));
//     delay(1000);
//     mpu.calcOffsets();
//     Serial.println("IMU Reset");
// }

// void base_search(){ // flip

//   move_backward(10);

//   while(average_distance() > (courseHeight -  (wallToRamp / 2))){
//   }

//   motor_stop();

//   float expectedX = courseHeight - (wallToRamp / 2);

//   float x = average_distance();

//   if(x < expectedX){
//     motor_stop();
//     return;
//   }

//   reset_imu();
  
//   rotate_clockwise();
//   while(mpu.getAngleZ() < 90){ 
//     mpu.update();
//   }

//   motor_stop();

  
//   float yRight = average_distance();
//   float yLeft = courseWidth - average_distance() - (2 * offset);
//   float maxAngleLeft = atan(x/yLeft);
//   float maxAngleRight = atan(x/yRight);

//   reset_imu();

//   rotate_counterclockwise();

//   //REGION 1

//   while(mpu.getAngleZ() < maxAngleRight){ 
//     if(average_distance() < yRight ){
//       motor_stop();
//       break;
//     }
//   }

//   //REGION 2

//   while( mpu.getAngleZ() < (180 - maxAngleLeft) ){
//     if(average_distance()  < x ){
//       motor_stop();
//       break;
//     }
//   }

//   //REGION 3

//   while( mpu.getAngleZ() < 180 ){
//     if(average_distance() < yLeft){
//       motor_stop();
//       break;
//     }
//   }

//   motor_stop();

//   //FACE FORWARD

//   reset_imu();

//   rotate_clockwise();

//   while( mpu.getAngleZ() != 90 ){
//   }

//   motor_stop();

//   move_backward(50); 
//   while(average_distance() != (courseHeight - wallToRamp)){ 
//   }

//   motor_stop();

//   x -= (wallToRamp / 2); // or x = /* HEIGHT OF COURSE */ - average_distance() - /*  OFFSET */

//   maxAngleLeft = atan(x/yLeft);
//   maxAngleRight = atan(x/yRight);

//   rotate_counterclockwise();
//   while(mpu.getAngleZ() != 0 ){
//   }

//   motor_stop();

//   rotate_clockwise();

//   //REGION 3

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

//   //REGION 1

//   while( mpu.getAngleZ() < 180  ){
//     if(average_distance() < yRight ){
//       motor_stop();
//       break;
//     }
//   }

//   currentState = "B"

// }


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

// void base_approach(){
//   Serial.println("1....2..");
//   delay(3000);
//   Serial.println("3!");

//   Serial.println("RAMP UP START");
//   // rampUp(80, 2);
//   rampUp(80, forward, 1000, 0); //new 
 
//   bool postFound = false;
//   int numMeasurements = 0;

//   while(!postFound){
    
//       Serial.println("POST NOT FOUND");
//       float leftSensor = read_distance(trigPinLeft, echoPinLeft);
//       float rightSensor = read_distance(trigPinRight, echoPinRight);
//       Serial.println(leftSensor);
//       Serial.println(rightSensor);
//       if(abs(leftSensor - rightSensor) < 10){
//         numMeasurements++;
//       }

//       if(numMeasurements == 10){
//         postFound = true;
//       }

//       delay(50);

//   }

//   Serial.println("POST FOUND");

//   // stop();
//   stop(forward); //new

//   delay(5000);

//   // rampUp(80, 2);
//   rampUp(80, forward, 1000, 0); //new 

//   Serial.println("STARTING APPROACH");

//   while(read_distance(trigPinLeft, echoPinLeft) > 15 || read_distance(trigPinRight, echoPinRight) > 15){
//       approach(80);
//   }
  
//   // stop();
//   stop(forward); //new
//   Serial.println("Stopped");
// }











