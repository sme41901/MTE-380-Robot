// Motor connections
int inA = 2;
int inB = 3;

// pwm connection
int Pwm = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(Pwm, OUTPUT);

  // keep motors off initially
  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  directionControl();
  delay(1000);
}

void directionControl() {
  // set motor speed via pwm
  analogWrite(Pwm, 100);

  // turn on motor and turn forward
  digitalWrite(inA, HIGH);
  digitalWrite(inB, LOW);
}
