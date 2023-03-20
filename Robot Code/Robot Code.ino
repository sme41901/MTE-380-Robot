// #include <FiniteStateMachine.h>
#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);

//define states and working functions
//motor states
// State STOP = State(stop);
// State WALL_APPROACH = State(wall_approach);
// State WALL_ENGAGE = State(wall_engage);
// State WALL_UP = State(wall_up);
// State WALL_DOWN = State(wall_down);
// State WALL_DISENGAGE = State(wall_disengage);
// State BASE_SEARCH = State(base_search);
// State BASE_FOUND = State(base_found);

//initiate state machine
// FSM Motor = FSM(STOP);

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


// void loop(){

//   if(inState == false){

//     inState = true;  

//     if(Motor.isInState(STOP)){    
//       Motor.transitionTo(WALL_APPROACH);
//     } else if(Motor.isInState(WALL_APPROACH)){
//       Motor.transitionTo(WALL_ENGAGE);
//     } else if(Motor.isInState(WALL_ENGAGE)){    
//       Motor.transitionTo(WALL_UP);
//     } else if(Motor.isInState(WALL_UP)){     
//       Motor.transitionTo(WALL_DOWN);   
//     } else if(Motor.isInState(WALL_DOWN){   
//       Motor.transitionTo(BASE_SEARCH);
//     } 
//     if(currentState == "WALL_APPROACH"){     
//       wall_engage();
//     }
//     if(currentState == "BASE_SEARCH"){     
//       // Motor.transitionTo(BASE_FOUND);
//       base_search();
//     }



//   }
// }

void loop(){

  move_forward(50);

}

// void check_drift(){

//   move_forward(50);

//   int angle;
//   int totalAngle;
//   float averageAngle;

//   do{

//     for(int i = 0; i < 100; i++){
//       angle = mpu.getAngleZ();
//       totalAngle += angle;
//     }

//     averageAngle = totalAngle / 100;
//   } while(averageAngle < 5);

//   motor_stop();

//   Serial.print(averageAngle);

// }

double read_distance(int trigPin, int echoPin){
  long duration;
  double distance, totalDistance, averageDistance;  

  for(int i = 0; i < 100; i++){
    digitalWrite(trigPin, LOW);

    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);

    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = (duration * 0.034 / 2);  

    totalDistance += distance;
  }

  averageDistance = totalDistance / 100;

  return averageDistance;
}

double average_distance(){
  return 1.0247 * ( read_distance(trigPinLeft, echoPinLeft) + read_distance(trigPinRight, echoPinRight) ) / 2;
}

void move_forward(int power){
    analogWrite(pwmLeft, power);
    analogWrite(pwmRight, power);
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, HIGH);
    digitalWrite(inBRight, LOW);
}

void move_backward(int power){
    analogWrite(pwmLeft, power);
    analogWrite(pwmRight, power);
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
}

void rotate_clockwise(){
    analogWrite(pwmLeft, 50);
    analogWrite(pwmRight, 50);
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, HIGH);
    digitalWrite(inBRight, LOW);
}

void rotate_counterclockwise(){
    analogWrite(pwmLeft, 50);
    analogWrite(pwmRight, 50);
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
}

void motor_stop(){
    analogWrite(pwmLeft, 0);
    analogWrite(pwmRight, 0);
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, LOW);

}

void straighten(){
  if(read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) > (1 + 0.1) || read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) < (1 - 0.1) || abs(read_distance(trigPinLeft, echoPinLeft) - read_distance(trigPinRight, echoPinRight)) > 5){
      while(read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) > (1 + 0.1) || read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) < (1 - 0.1)){
        if (read_distance(trigPinLeft, echoPinLeft) < read_distance(trigPinRight, echoPinRight)){
          // decrease power in RM
          int multiplier = (read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight)) * 2;
          analogWrite(pwmLeft, 100);
          analogWrite(pwmRight, (multiplier*100));
        }
        else {
          // decrease power in LM
          int multiplier = (read_distance(trigPinRight, echoPinRight) / read_distance(trigPinLeft, echoPinLeft)) * 2;
          analogWrite(pwmLeft, (multiplier*100));
          analogWrite(pwmRight, 100);
        }

      }
  }

  move_forward(50);
}

void imu_straighten(){
  //clockwise = negative, decrease power in LM
  //counter clockwise = positive, decrease  power in RM
  while(1){
    if(mpu.getAngleZ() > 0){
      int multiplier = (100 - mpu.getAngleZ()) / 100;
      analogWrite(pwmLeft, 100);
      analogWrite(pwmRight, (multiplier*100));
    } 

    if(mpu.getAngleZ() < 0){
      int multiplier = (100 - mpu.getAngleZ()) / 100;
      analogWrite(pwmLeft, (multiplier*100));
       analogWrite(pwmRight, 100); 
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

//   inState = false;

// }


void base_search(){ //no flip

  move_forward(50);

  while(average_distance() < (wallToRamp / 2)){
  }

  motor_stop();

  float x = courseHeight - average_distance() - offset;
  

  while( mpu.getAngleZ() < 90 ){
    rotate_clockwise();
  }

  motor_stop();

  
  float yLeft = average_distance() - offset;
  float yRight = courseWidth - average_distance() + offset;
  float maxAngleLeft = atan(x/yLeft);
  float maxAngleRight = atan(x/yRight);

  reset_imu();

    //REGION 1

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

  //REGION 3

  while( mpu.getAngleZ() < 180  ){
    if(average_distance() < yRight ){
      motor_stop();
      break;
    }
  }

  //FACE FORWARD

  rotate_counterclockwise();

  while(mpu.getAngleZ() != 90){
  }

  motor_stop();

  //MOVE 2/3 LENGTH OF RAMP UP

  move_forward(50); 
  
  while(average_distance() != (courseHeight - wallToRamp)){ 
  }

  motor_stop();

  x -= wallToRamp / 2; 

  maxAngleLeft = atan(x/yLeft);
  maxAngleRight = atan(x/yRight);

  rotate_counterclockwise();
  while(mpu.getAngleZ() != 0){
  }

  motor_stop();

  inState = false;

}

void base_found(){
  move_backward(50);
  
  while(average_distance() > 5){
    // straighten();
  }

  motor_stop();
}











