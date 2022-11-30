#include <LiquidCrystal.h>
#include <Servo.h>
const int SENSOR_PIN = 8; // the Arduino's input pin that connects to the touch sensor's SIGNAL pin  
const int sensor_pin = A0; // soil moisture 
int lastState = LOW;      // the previous state from the input pin
int currentState;         // the current reading from the input pin
int trigPin1=6;           // Ultrasonic sensor pin
int echoPin1=7;           // ultrasonic sensor pin  
Servo myservo;           // servo 5v-red , gnd-brown , orange-9
int pos;


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  lcd.setCursor(0, 1);
  myservo.attach(9)
; 
}

void loop() {
  int count = 0;
  long duration1, distance1;
  digitalWrite(trigPin1, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;

   if (distance1 >= 241 || distance1 <= 0){
    lcd.setCursor(0, 0);
    lcd.print("Out of range !");
    lcd.setCursor(0, 1);
    delay(1500);
  }
  else { 
    lcd.setCursor(0, 0);
    lcd.print ( "Distance = ");
    lcd.print ( distance1);
    lcd.println("cm");
    lcd.setCursor(0, 1);
    delay(500);
    currentState = digitalRead(SENSOR_PIN);
    float moisture_percentage;
    int sensor_analog;
    sensor_analog = analogRead(sensor_pin);
    moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );


      // CODE for opening Lid and starting conveyor belt
  
  
for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    
    myservo.write(pos);    
              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(1000);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  


  
  if( currentState == HIGH && moisture_percentage<10.0)
   {    
    lcd.setCursor(0, 1);
    lcd.print("Dry Waste  \n     ");
  currentState=LOW;
   }
   
   else if(moisture_percentage>5.0 && currentState==LOW)
   { 
      lcd.setCursor(0, 0);
      lcd.print("Wet Waste         ");
      lcd.setCursor(0,1);
      lcd.print("Moisture = \n");
      lcd.print(moisture_percentage);
      delay(400);
     // lcd.clear();


   
//------------------------------------------------------------ wet side code -----------------------------------------------------------



for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  
  delay(200);
  
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  
  

//-------------------------------------------------------------------------------------------------------------------------------------------


  //delay(1000);
   }

// IF soil and touch sensor is not getting any input 
   else if(moisture_percentage<5.0 && currentState==LOW){
     lcd.setCursor(0, 1);
     lcd.print("Idle State \n");
   }
   // IF both soil and touch is detecting inputs 
   else if  (moisture_percentage>5.0 && currentState==HIGH){
    lcd.setCursor(0, 1);
    lcd.print("Invalid waste");
   }
   else{
     Serial.println("Unidentified waste");
   }
  }

}