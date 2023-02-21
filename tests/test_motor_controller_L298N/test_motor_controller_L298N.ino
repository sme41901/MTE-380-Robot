// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);

  // turn off motors
  motorsStop();
}

void loop() {
  // put your main code here, to run repeatedly:
  directionControl();
  delay(3000);
}

void motorsStop() {
  // turn off motors
  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

void directionControl() {
  // Set motors to max speed
  // for pwm pins values are 0 to 255
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Turn on motor A & B and go forward
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	delay(3000);

  motorsStop();  
	
	// Now change motor directions and reverse
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	delay(3000);

  motorsStop();

  // Turn left
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);  
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	delay(3000);

  motorsStop();

  // Turn right
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	delay(3000);

  motorsStop();
}
