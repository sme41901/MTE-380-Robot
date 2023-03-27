// #include <LiquidCrystal.h>
// LiquidCrystal lcd(1,2,4,5,6,7);
// const int trigPin = 9;
// const int echoPin = 8;
// long duration;
// int distance;
// float speedSound = 0.034;

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);
//   //lcd.begin(16,2);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   // clear trig pin
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);

//   // Set the trigPin High for 10 microseconds
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);

//   // read echoPin, returns the sound wave travel time in microseconds
//   duration = pulseIn(echoPin, HIGH);

//   // calculate the distance
//   distance = duration*speedSound/2;

//   lcd.setCursor(0, 0); // Sets the location at which subsequent text written to the LCD will be displayed
//   lcd.print("Distance: "); // Prints string "Distance" on the LCD
//   lcd.print(distance); // Prints the distance value from the sensor
//   lcd.print(" cm");
//   delay(1000);
//   lcd.clear(); // Clears the LCD screen
// }

const int trigPinLeft = 6;
const int echoPinLeft = 5;
const int trigPinRight = 10;
const int echoPinRight = 9;
// defines variables
long durationLeft;
int distanceLeft;
long durationRight;
int distanceRight;
void setup() {
  pinMode(trigPinLeft, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinLeft, INPUT); // Sets the echoPin as an Input
  pinMode(trigPinRight, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinRight, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPinLeft, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinLeft, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationLeft = pulseIn(echoPinLeft, HIGH);
  // Calculating the distance
  distanceLeft = durationLeft * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance left: ");
  Serial.println(distanceLeft);
  // Clears the trigPin
  digitalWrite(trigPinRight, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinRight, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationRight = pulseIn(echoPinRight, HIGH);
  // Calculating the distance
  distanceRight = durationRight * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance right: ");
  Serial.println(distanceRight);
}
