// Motor connections
int inA = 2;
int inB = 3;

// pwm connection
int pwmPin = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(pwmPin, OUTPUT);

  // keep motors off initially
  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  directionControl();
}

void directionControl() {
  // set motor speed via pwm
  analogWrite(pwmPin, 255);
  // turn on motor and turn forward
  digitalWrite(inA, HIGH);
  digitalWrite(inB, LOW);

  delay(5000);
  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);
  delay(5000);
  digitalWrite(inA, LOW);
  digitalWrite(inB, HIGH);
   delay(5000);
   digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);
  delay(5000);
}
