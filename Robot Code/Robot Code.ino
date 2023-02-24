#include <FiniteStateMachine.h>

//define states and working functions
//motor states
State STOP = State(stop);
State WALL_ENGAGE = State(wall_engage);
State WALL_UP = State(wall_up);
State WALL_DOWN = State(wall_down);
State WALL_DISENGAGE = State(wall_disengage);
State BASE_SEARCH = State(base_search);
State BASE_FOUND = State(base_found);

//initiate state machines
FSM Motor = FSM(STOP);

const int trigPinLeft = 7;
const int echoPinLeft = 8;
const int trigPinLeft = 9;
const int echoPinLeft = 10;

bool inState;

void setup() {
  inState = false;
  pinMode(trigPinLeft, OUTPUT); 
  pinMode(echoPinLeft, INPUT); 
  pinMode(trigPinRight, OUTPUT); 
  pinMode(echoPinRight, INPUT);
}

void loop(){

  if(inState == false){//inState only used if loop() can run again before function inside finishes executing

    inState = true;  

    if(Motor.isInState(STOP)){    
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


void wall_engage(){
  while((read_distance(trigPinLeft, echoPinLeft) + read_distance(trigPinRight, echoPinRight)) / 2 > 5){
    //MOTOR LEFT FULL SPEED
    //MOTOR RIGHT FULL SPEED
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


/*
1. Start distance to boundary is gonna be small, distance to ground when engaged is also small 
2. Make sure we can distinguish between when were close to the boundary at start and when we engage in the wall
3. Flip boolean when we go away from boundary so that we know when we're engaging to the wall
*/

void wall_up(){ 
  while(/*ULTRASONIC SENSOR < 100 */){
    //MOTOR LEFT FULL SPEED
    //MOTOR RIGHT FULL SPEED
    straighten();
  }

  inState = false;
}


void wall_down(){
  while(/*ULTRASONIC SENSOR > 5*/){
    //MOTOR LEFT HALF SPEED
    //MOTOR RIGHT HALF SPEED

    if(/*velocity > 2 && acceleration > 0*/){
    //MOTOR LEFT REDUCE TORQUE
    //MOTOR RIGHT REDUCE TORQUE
    }

    //can't check straighten() because ceiling is not flat, unreliable, but if we are to fix it we would need to reverse direction of one of the motors (negative) in the straighten() function
  }

  inState = false;
}

void base_search(){

  while(/* ULTRASONIC SENSOR < 10 */){
    //FORWARD
  }

  x = // HEIGHT OF COURSE - (ULTRASONIC SENSOR + OFFSET)
  
  //RESET IMU 

  while(/* angle < 90 */){
    //CLOCKWISE
  }

  
  yLeft = //ULTRASONIC SENSOR + OFFSET
  yRight = //WIDTH OF COURSE - (ULTRASONIC SENSOR + 2*OFFSET)
  maxAngleLeft = arctan(x/y1);
  maxAngleRight = arctan(x/y2);

  //RESET IMU

  while(/* angle < (EFFECTUAL ANGLE / 2) */){
    //CLOCKWISE
  }

  //REGION 1

  while(/* (angle < (180 - (EFFECTUAL ANGLE / 2))) && (angle < maxAngleLeft)  */){
    //CLOCKWISE
    if(/*ultrasonic distance < (yLeft/cos(angle))*/){
      break;
    }
  }

  //REGION 2

  while(/* (angle < (180 - (EFFECTUAL ANGLE / 2))) && (angle < 90)  */){
    //CLOCKWISE
    if(/*ultrasonic distance < (x/cos(90-angle))*/){
      break;
    }
  }

  //REGION 3

  while(/* (angle < (180 - (EFFECTUAL ANGLE / 2))) && (angle < (180 - maxAngleRight))  */){
    //CLOCKWISE
    if(/*ultrasonic distance < (x/cos(angle-90))*/){
      break;
    }
  }

  //REGION 4

  while(/* (angle < (180 - (EFFECTUAL ANGLE / 2))))  */){
    //CLOCKWISE
    if(/*ultrasonic distance < (yRight/cos(180-angle))*/){
      break;
    }
  }

  //FACE FORWARD

  while(/*angle != 90*/){
    //COUNTER CLOCKWISE
  }

  //MOVE 10CM UP

  while(/* ULTRASONIC SENSOR != 20 */){
    //FORWARD
  }


  x = //x - 10

  maxAngleLeft = arctan(x/y1);
  maxAngleRight = arctan(x/y2);

  while(/*angle != 0 */){
    //COUNTER CLOCKWISE
  }

  while(/* angle < maxAngleLeft  */){
    //CLOCKWISE
    if(/*ultrasonic distance < (yLeft/cos(angle))*/){
      break;
    }
  }

  while(/* (angle < 90)  */){
    //CLOCKWISE
    if(/*ultrasonic distance < (x/cos(90-angle))*/){
      break;
    }
  }

  while(/* ( angle < (180 - maxAngleRight)  */){
    //CLOCKWISE
    if(/*ultrasonic distance < (x/cos(angle-90))*/){
      break;
    }
  }

  while(/* (angle < 180)  */){
    //CLOCKWISE
    if(/*ultrasonic distance < (yRight/cos(180-angle))*/){
      break;
    }
  }




    
  inState = false;

}

void base_found(){
  while(/* ULTRASONIC SENSOR > 5*/){
    //MOTOR LEFT FULL SPEED 
    //MOTOR RIGHT FULL SPEED    
  }

  //MOTOR LEFT STOP 
  //MOTOR RIGHT STOP
  
}






