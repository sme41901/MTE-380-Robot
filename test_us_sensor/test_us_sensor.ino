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

const int trigPin = 11;
const int echoPin = 10;
// defines variables
long duration;
int distance;
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}
