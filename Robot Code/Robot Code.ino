#include <FiniteStateMachine.h>
#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);

//define states and working functions
//motor states
State STOP = State(stop);
State WALL_APPROACH = State(wall_approach);
State WALL_ENGAGE = State(wall_engage);
State WALL_UP = State(wall_up);
State WALL_DOWN = State(wall_down);
State WALL_DISENGAGE = State(wall_disengage);
State BASE_SEARCH = State(base_search);
State BASE_FOUND = State(base_found);

//initiate state machine
FSM Motor = FSM(STOP);

//Define Ultrasonic Pins
const int trigPinLeft = 7;
const int echoPinLeft = 8;
const int trigPinLeft = 9;
const int echoPinLeft = 10;

//Define Motor Pins
const int inALeft = 1;
const int inBLeft = 2;
const int pwmLeft = 3;
const int inARight = 4;
const int inBRight = 5;
const int pwmRight = 6;


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
}

void loop(){

  if(inState == false){

    inState = true;  

    if(Motor.isInState(STOP)){    
      Motor.transitionTo(WALL_APPROACH);
    } else if(Motor.isInState(WALL_APPROACH)){
      Motor.transitionTo(WALL_ENGAGE);
    } else if(Motor.isInState(WALL_ENGAGE)){    
      Motor.transitionTo(WALL_UP);
    } else if(Motor.isInState(WALL_UP)){     
      Motor.transitionTo(WALL_DOWN);   
    } else if(Motor.isInState(WALL_DOWN){   
      Motor.transitionTo(BASE_SEARCH);
    } else if(Motor.isInState(BASE_SEARCH){     
      Motor.transitionTo(BASE_FOUND);
    }


  }
}

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

    distance = (durationLeft * 0.034 / 2);  

    totalDistance += distance;
  }

  averageDistance = totalDistance / 100;

  return averageDistance;
}

double average_distance(){
  return 1.0247 * ( read_distance(trigPinLeft, echoPinLeft) + read_distance(trigPinRight, echoPinRight) ) / 2;
}

void move_forward(){
    analogWrite(pwmLeft, 100);
    analogWrite(pwmRight, 100);
    //counter clockwise?
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW);
    //clockwise?
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
  if(read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) > (1 + /*RELATIVE ERROR */) || read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) < (1 - /*RELATIVE ERROR */)){
      while(read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) > (1 + /*RELATIVE ERROR */) || read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) < (1 - /*RELATIVE ERROR */)){
        if (read_distance(trigPinLeft, echoPinLeft) < read_distance(trigPinRight, echoPinRight)){
          // decrease power in RM
          int multiplier = read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight);
          analogWrite(pwmLeft, 100);
          analogWrite(pwmRight, (multiplier*100));
        }
        else {
          // decrease power in LM
          int multiplier = read_distance(trigPinRight, echoPinRight) / read_distance(trigPinLeft, echoPinLeft);
          analogWrite(pwmLeft, (multiplier*100));
          analogWrite(pwmRight, 100);
        }

      }
  }


  //whats the multiplier to fix for Absolute error? If same as relative error then can use above block ^^^^ if not then use below to create new logic
  
  if(abs(read_distance(trigPinLeft, echoPinLeft) - read_distance(trigPinRight, echoPinRight)) > /*ABSOLUTE ERROR*/){
    if (read_distance(trigPinLeft, echoPinLeft) < read_distance(trigPinRight, echoPinRight)){
      // decrease power in RM
      int multiplier = read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight);
      analogWrite(pwmLeft, 100);
      analogWrite(pwmRight, (multiplier*100));
    }
    else {
      // decrease power in LM
      int multiplier = read_distance(trigPinRight, echoPinRight) / read_distance(trigPinLeft, echoPinLeft);
      analogWrite(pwmLeft, (multiplier*100));
      analogWrite(pwmRight, 100);
    }
    
  }

  move_forward();


}

void scanning(){

  rotate_clockwise();

  //REGION 1

  while(/* (angle < maxAngleLeft)  */){
    if(average_distance() /* < yLeft */){
      motor_stop();
      break;
    }
  }

  //REGION 2

  while(/* (angle < (180-maxAngleRight) */){
    if(average_distance() /* < x */){
      motor_stop();
      break;
    }
  }

  //REGION 3

  while(/* angle < 180  */){
    rotate_clockwise();
    if(average_distance()/* < yRight */){
      motor_stop();
      break;
    }
  }

  motor_stop();
}


void wall_approach(){
    move_forward();
   while(average_distance() < 10){
   }

   inState = false;
}

void wall_engage(){
  move_forward();
  while(average_distance()  > 5){
    straighten();
  }

  inState = false;
}


void wall_up(){ 
  move_forward();
  while(average_distance() < 100){
    straighten();
  }

  inState = false;
}


void wall_down(){
  move_forward(); // 50% speed
  while(average_distance() > 5){

    if(/*velocity > 2 && acceleration > 0*/){
    //MOTOR LEFT REDUCE TORQUE
    //MOTOR RIGHT REDUCE TORQUE
    }
  }

  inState = false;
}

void base_search(){

  const twoThirdsRamp = 696969;

  move_forward();

  while(average_distance() < twoThirdsRamp){
  }

  motor_stop();

  x = /* HEIGHT OF COURSE */ - average_distance() - /*  OFFSET */
  

  while(/* angle < 90 */){
    rotate_clockwise();
  }

  motor_stop();

  
  yLeft = average_distance() - /*OFFSET*/
  yRight = /*WIDTH OF COURSE*/ - average_distance() + /* OFFSET */
  maxAngleLeft = arctan(x/yLeft);
  maxAngleRight = arctan(x/yRight);

  //RESET IMU

  scanning();

  
  for(int i = 0; i < 3; i++){

  //FACE FORWARD

  rotate_counterclockwise();
  while(/*angle != 90*/){
  }

  motor_stop();

  //MOVE 2/3 LENGTH OF RAMP UP

  move_forward(); 
  while(average_distance() != (twoThirdsRamp * (i+2)){ 
  }

  motor_stop();

  x -= twoThirdsRamp; // or x = /* HEIGHT OF COURSE */ - average_distance() - /*  OFFSET */

  maxAngleLeft = arctan(x/y1);
  maxAngleRight = arctan(x/y2);

  rotate_counterclockwise();
  while(/*angle != 0 */){
  }

  motor_stop();

  scanning();

  }
  

  inState = false;

}

void base_found(){
  move_forward();
  while(average_distance() > 5){
  }

  motor_stop();
}








