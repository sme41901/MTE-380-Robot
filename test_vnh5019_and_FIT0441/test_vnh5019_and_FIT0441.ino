// Motor connections
int inA = 2;
int inB = 3;
int motorLeftDirection = 8;
int motorLeftPwm = 9;

// pwm connection
int rightMotorPwm = 5;
int rightMotorSpeed = 63;
int millisec = 1000;

void setup() {
  // put your setup code here, to run once:
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(rightMotorPwm, OUTPUT);

  pinMode(motorLeftDirection, OUTPUT);
  pinMode(motorLeftPwm, OUTPUT);

  // keep motors off initially
  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  turnOne();
  delay(millisec);
  motorStop();
  delay(millisec);
  turnTwo();
  delay(millisec);
  motorStop();
  delay(millisec);
  motorForward();
  delay(millisec);
  motorStop();
  delay(millisec);
  motorBackward();
  delay(millisec);  
  motorStop();
  delay(millisec);
}

void turnOne() {
  digitalWrite(motorLeftDirection, LOW);
  analogWrite(motorLeftPwm, 0);  
  analogWrite(rightMotorPwm, rightMotorSpeed);
  digitalWrite(inA, HIGH);
  digitalWrite(inB, LOW);

}

void turnTwo() {
  digitalWrite(motorLeftDirection, HIGH);
  analogWrite(motorLeftPwm, 0);  
  analogWrite(rightMotorPwm, rightMotorSpeed);
  digitalWrite(inA, LOW);
  digitalWrite(inB, HIGH); 
}

void motorBackward() {
  digitalWrite(motorLeftDirection, HIGH);
  analogWrite(motorLeftPwm, 0);  
  analogWrite(rightMotorPwm, rightMotorSpeed);
  digitalWrite(inA, HIGH);
  digitalWrite(inB, LOW);

}

void motorForward() {
  digitalWrite(motorLeftDirection, LOW);
  analogWrite(motorLeftPwm, 0);  
  analogWrite(rightMotorPwm, rightMotorSpeed);
  digitalWrite(inA, LOW);
  digitalWrite(inB, HIGH); 
}

void motorStop() {
  analogWrite(motorLeftPwm, 255);
  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);
}
