int motorRightDirection = 53;
int motorRightPwm = 3;

bool direction = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(motorRightDirection, OUTPUT);
  pinMode(motorRightPwm, OUTPUT);
  motorStop();
}

void loop() {
  // put your main code here, to run repeatedly:
  motorForward();
  delay(5000);
  motorStop();
  delay(5000);
 
}

void motorForward() {
  digitalWrite(motorRightDirection, HIGH);
  analogWrite(motorRightPwm, 128);
}

void motorBackward() {
  // the right motor is not switching directions
  digitalWrite(motorRightDirection, LOW);
  analogWrite(motorRightPwm, 0);  
}

void motorStop() {
  analogWrite(motorRightPwm, 255);
}