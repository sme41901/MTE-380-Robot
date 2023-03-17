const int inALeft = 2;
const int inBLeft = 4;
const int pwmLeft = 3;
const int inARight = 13;
const int inBRight = 12;
const int pwmRight = 11;
int speedValue = 127;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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

void loop() {
  // put your main code here, to run repeatedly:
  rampDown(55, 255, 2);
  // stop();
  // move(10,1);
  // stop();
  delay(3000);
}

void rampUp(float power, int direction) {
  float rampConst = 1000;
  float powerConst = 255;
  float timeOffset = millis();
  Serial.print("Time offset = ");
  Serial.println(timeOffset);
  while((timeOffset + power*rampConst/powerConst) > float (millis()))
  {
    int rampPower = (millis() - timeOffset)*powerConst/rampConst;
    // set motor speed via pwm
    move(rampPower, direction);
    // debug
    /*Serial.print("Ramp power = ");
    Serial.println(rampPower);
    Serial.print("time: ");
    Serial.println(millis());
    Serial.println((power*rampConst)/powerConst);*/

    delay(150);    
  }
  move(power, 2);
}

void rampDown(float power, float initialPower, int direction) {
  if(initialPower > power) {
    float rampConst = 6000;
    float powerConst = 255;
    float timeOffset = millis();
    Serial.print("Time offset = ");
    Serial.println(timeOffset);
    while((timeOffset + (initialPower-power)*rampConst/powerConst) > float (millis()))
    {
      int rampPower = initialPower - (millis() - timeOffset)*powerConst/rampConst;
      // set motor speed via pwm
      move(rampPower, direction);
      
      // debug
      /*Serial.print("Ramp power = ");
      Serial.println(rampPower);
      Serial.print("time: ");
      Serial.println(millis());
      Serial.println((power*rampConst)/powerConst);*/

      delay(150);    
    }
    move(power, 2);
  }
}

void move(int power, int direction) {
  // set motor speed via pwm
  analogWrite(pwmLeft, power);
  analogWrite(pwmRight, power);
  // direction = 0 go forward
  if(direction == 0) {
  // turn on motor and move forward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, LOW);
  }
  if(direction == 1) {
  // turn on motor and move backward
    digitalWrite(inALeft, LOW);
    digitalWrite(inBLeft, HIGH);
    digitalWrite(inARight, LOW);
    digitalWrite(inBRight, HIGH);
  }
  if(direction == 2) {
    // turn on motor and move forward
    digitalWrite(inALeft, HIGH);
    digitalWrite(inBLeft, LOW);
    digitalWrite(inARight, HIGH);
    digitalWrite(inBRight, LOW);
  }
  
}

void stop() {
  // set motor speed via pwm
  move(0, 0);
}

void directionControl(int speed) {
  // set motor speed via pwm
  analogWrite(pwmLeft, speed);
  analogWrite(pwmRight, speed);
  // turn on motor and turn forward
  digitalWrite(inALeft, HIGH);
  digitalWrite(inBLeft, LOW);
  digitalWrite(inARight, HIGH);
  digitalWrite(inBRight, LOW);

  delay(2000);
  digitalWrite(inALeft, LOW);
  digitalWrite(inBLeft, LOW);
  digitalWrite(inARight, LOW);
  digitalWrite(inBRight, LOW);
  delay(2000);
  digitalWrite(inALeft, LOW);
  digitalWrite(inBLeft, HIGH);
  digitalWrite(inARight, LOW);
  digitalWrite(inBRight, HIGH);
  delay(2000);
  digitalWrite(inALeft, LOW);
  digitalWrite(inBLeft, LOW);
  digitalWrite(inARight, LOW);
  digitalWrite(inBRight, LOW);
  delay(2000);
}
