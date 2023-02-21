int motorRightDirection = 8;
int motorRightPwm = 9;
int motorLeftDirection = 10;
int motorLeftPwm = 11;

bool direction = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(motorLeftDirection, OUTPUT);
  pinMode(motorLeftPwm, OUTPUT);
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
  motorBackward();
  delay(5000);
  motorStop();
  delay(5000);
}

void motorForward() {
  digitalWrite(motorLeftDirection, LOW);
  analogWrite(motorLeftPwm, 10);  
  digitalWrite(motorRightDirection, HIGH);
  analogWrite(motorRightPwm, 10);
}

void motorBackward() {
  digitalWrite(motorLeftDirection, HIGH);
  analogWrite(motorLeftPwm, 10);  
  // the right motor is not switching directions
  digitalWrite(motorRightDirection, LOW);
  analogWrite(motorRightPwm, 10);  
}

void motorStop() {
  analogWrite(motorLeftPwm, 255);
  analogWrite(motorRightPwm, 255);
}

// int i = 0;
// unsigned long time = 0;
// bool flag = HIGH;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   pinMode(10, OUTPUT); //direction control PIN 10 with direction wire 
//   pinMode(11, OUTPUT); //PWM PIN 11  with PWM wire
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if (millis() - time > 5000)  {
//     flag = !flag;
//     digitalWrite(10, flag);
//     time = millis();
//   }
//   if (Serial.available())  {
//     analogWrite(11, Serial.parseInt());  //input speed (must be int)
//     delay(200);
//   }
//   for(int j = 0;j<8;j++)  {
//     i += pulseIn(9, HIGH, 500000); //SIGNAL OUTPUT PIN 9 with  white line,cycle = 2*i,1s = 1000000us，Signal cycle pulse number：27*2
//   }
//   i = i >> 3;
//   Serial.print(111111 / i); //speed   r/min  (60*1000000/(45*6*2*i))
//   Serial.println("  r/min");
//   i = 0;
// }