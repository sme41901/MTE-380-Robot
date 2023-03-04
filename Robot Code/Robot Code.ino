#include <FiniteStateMachine.h>

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

  if(inState == false){//inState only used if loop() can run again before function inside finishes executing

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
  return ( read_distance(trigPinLeft, echoPinLeft) + read_distance(trigPinRight, echoPinRight) ) / 2;
}

void move_forward(){
    analogWrite(pwmLeft, 100);
    analogWrite(pwmRight, 100);
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, HIGH);
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

void wall_approach(){
   while(average_distance() < 10){
    move_forward();
   }

   inState = false;
}

void wall_engage(){
  while(average_distance()  > 5){
    move_forward();
    straighten();
  }

  inState = false;
}


void straighten(){
  if(read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) > (1 + /*RELATIVE ERROR */) || read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) < (1 - /*RELATIVE ERROR */))
      while(read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) > (1 + /*RELATIVE ERROR */) || read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight) < (1 - /*RELATIVE ERROR */)){
        if (read_distance(trigPinLeft, echoPinLeft) < read_distance(trigPinRight, echoPinRight)){
          // decrease power in RM
          // (read_distance(trigPinLeft, echoPinLeft) / read_distance(trigPinRight, echoPinRight)) * RM
        }
        else{
          // decrease power in LM
          // (read_distance(trigPinRight, echoPinRight) / read_distance(trigPinLeft, echoPinLeft) ) * LM
        }

      }
  }
  
  if(/*abs(left sensor - right sensor) > 2*/){
    //reduce power in one motor
   }
}


void wall_up(){ 
  while(average_distance() < 100){
    move_forward();
    straighten();
  }

  inState = false;
}


void wall_down(){
  while(average_distance() > 5){
    move_forward(); // 50% speed

    if(/*velocity > 2 && acceleration > 0*/){
    //MOTOR LEFT REDUCE TORQUE
    //MOTOR RIGHT REDUCE TORQUE
    }

    //can't check straighten() because ceiling is not flat, unreliable, but if we are to fix it we would need to reverse direction of one of the motors (negative) in the straighten() function
  }

  inState = false;
}

void base_search(){

  while(average_distance() < 10){
    move_forward();
  }

  x = /* HEIGHT OF COURSE */ - /* ultrasonic distance */ + /*OFFSET*/
  
  
  //RESET IMU 

  while(/* angle < 90 */){
    rotate_clockwise();
  }

  
  yLeft = average_distance() + /*OFFSET*/
  yRight = /*WIDTH OF COURSE*/ - /*ultrasonic distance*/ + /* 2*OFFSET */
  maxAngleLeft = arctan(x/y1);
  maxAngleRight = arctan(x/y2);

  //RESET IMU

  while(/* angle < (EFFECTUAL ANGLE / 2) */){
    rotate_clockwise();
  }

  //REGION 1

  while(/* (angle < (180 - (EFFECTUAL ANGLE / 2))) && (angle < maxAngleLeft)  */){
    rotate_clockwise();
    if(/*ultrasonic distance < (yLeft/cos(angle))*/){
      break;
    }
  }

  //REGION 2

  while(/* (angle < (180 - (EFFECTUAL ANGLE / 2))) && (angle < 90)  */){
    rotate_clockwise();
    if(/*ultrasonic distance < (x/cos(90-angle))*/){
      break;
    }
  }

  //REGION 3

  while(/* (angle < (180 - (EFFECTUAL ANGLE / 2))) && (angle < (180 - maxAngleRight))  */){
    rotate_clockwise();
    if(/*ultrasonic distance < (x/cos(angle-90))*/){
      break;
    }
  }

  //REGION 4

  while(/* (angle < (180 - (EFFECTUAL ANGLE / 2))))  */){
    rotate_clockwise();
    if(/*ultrasonic distance < (yRight/cos(180-angle))*/){
      break;
    }
  }

  
  
  
  for(int i = 0; i < 3; i++){

  //FACE FORWARD

  while(/*angle != 90*/){
    rotate_counterclockwise();
  }

  //MOVE 10CM UP

  while(/* ULTRASONIC SENSOR != 20 */){
    move_forward();
  }

  x -= 10;

  maxAngleLeft = arctan(x/y1);
  maxAngleRight = arctan(x/y2);

  while(/*angle != 0 */){
    rotate_counterclockwise();
  }

  while(/* angle < maxAngleLeft  */){
    rotate_clockwise();
    if(/*ultrasonic distance < (yLeft/cos(angle))*/){
      break;
    }
  }

  while(/* (angle < 90)  */){
    rotate_clockwise();
    if(/*ultrasonic distance < (x/cos(90-angle))*/){
      break;
    }
  }

  while(/* ( angle < (180 - maxAngleRight)  */){
    rotate_clockwise();
    if(/*ultrasonic distance < (x/cos(angle-90))*/){
      break;
    }
  }

  while(/* (angle < 180)  */){
    rotate_clockwise();
    if(/*ultrasonic distance < (yRight/cos(180-angle))*/){
      break;
    }
  }


  }

  inState = false;

}

void base_found(){
  while(/* ULTRASONIC SENSOR > 5*/){
    move_forward();   
  }

  //MOTOR LEFT STOP 
  //MOTOR RIGHT STOP
  
}






