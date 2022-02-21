/*

  Serveo Motor connections 
  Red - 5V
  Orange - Signal
  Brown - Ground 

  IR Sensor connection 
  
*/

#include <Servo.h>

Servo M1;  // create servo object to control a servo
Servo M2;  // create servo object to control a servo

int m1pos=0; 
int m2pos=0;
 

void setup() {

  pinMode(9, INPUT); // pin 9 is connected to the IR sensor output
  pinMode(5, OUTPUT); // pin5 is connected to PEMENOL board for voice alert audio
  
  
  M1.attach(6);  // attaches the servo on pin 6 to the servo object Motor 1
   M2.attach(7);  // attaches the servo on pin 7to the servo object Motor 2
   M1.write(90);
   M2.write(90);
   digitalWrite(5,HIGH);// pin5 is connected to PEMENOL board for voice alert audio; initialize to no voice;
}

void loop() {

if(digitalRead(9)==LOW){
  Serial.println("Medicine picked up");
  digitalWrite(5,LOW);
  delay(11000);
   digitalWrite(5,HIGH);
}
  else
 Serial.println("Medicine not picked up");
  

delay(1000);

 
  M1.write(0);                  // Rotate 90 degree
  delay(1000);                           // waits for the servo to get there
  M1.write(90);
  delay(1000);

   M2.write(0);                  // Rotate 90 degree
  delay(1000);                           // waits for the servo to get there
  M2.write(90);
  delay(1000);
  
}
