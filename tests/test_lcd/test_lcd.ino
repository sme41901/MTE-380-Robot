#include <LiquidCrystal.h>
LiquidCrystal lcd(1,2,4,5,6,7);
const int trigPin = 9;
const int echoPin = 8;
long duration;
int distance;
float speedSound = 0.034;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.begin(16,2);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:  
  lcd.print("Arduino");
  delay(3000);
  lcd.setCursor(2, 1);
  lcd.print("LCD Tutorial");
  delay(3000);
  lcd.clear(); // Clears the display 
  lcd.blink(); //Displays the blinking LCD cursor 
  delay(4000); 
  lcd.setCursor(7,1); 
  delay(3000); 
  lcd.noBlink(); // Turns off the blinking LCD cursor 
  lcd.cursor(); // Displays an underscore (line) at the position to which the next character will be written 
  delay(4000); 
  lcd.noCursor(); // Hides the LCD cursor 
  lcd.clear(); // Clears the LCD screen 

}
