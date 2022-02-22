/*
Author: Mariam Iqbal from the great state of Massachusetts 
Advanced Math and Science Academy Charter School In Marlborough
9th grade

 Project Description: A medicine dispenser for dementia patients living on their own that dispenses the proper dose of a drug at the proper time, reminding a patient to take it 
 and messaging loved ones information and updates. 
  Serveo Motor connections 
  Red - 5V
  Orange - Signal
  Brown - Ground 

  IR Sensor connection 

 
  
*/

#include <Servo.h>

Servo M1;  // create servo object to control a servo
Servo M2;  // create servo object to control a servo



boolean overdoseProtection=false; // is the tray not empty? 
unsigned long int timeInterval = 10000; // interval for medication dosage, how often you should take the medication
unsigned long int timeCheck= 10000; // after timeCheck hrs, check if medication was taken 
unsigned long int lastDoseTime= 0; // last time dose was dispensed

boolean voicealert= false; // starts the program with the voice alert off 
int count =0 ; // count # of times voiceAlert was used
int maxCount=3; //maximum amount of times voiceAlert will happen before a text message will be sent to loved ones. 

 

void setup() {
  lastDoseTime= millis(); // initializes the var to the time when you start the program 
  
  pinMode(9, INPUT); // pin 9 is connected to the IR sensor output
  pinMode(5, OUTPUT); // pin5 is connected to PEMENOL board for voice alert audio
  
  
  M1.attach(6);  // attaches the servo on pin 6 to the servo object Motor 1
   M2.attach(7);  // attaches the servo on pin 7to the servo object Motor 2
   M1.write(90);
   M2.write(90);
   digitalWrite(5,HIGH);// pin5 is connected to PEMENOL board for voice alert audio; initialize to no voice;
}

void loop() {
if(overdoseProtection==false ){ 
  // release medications 
  releaseDose(); 
  //record the time when the dose was released. 
  lastDoseTime=millis(); 
  count=0; 
}

// read obstacle IR sensor
readIRSensor();  
Serial.print("Time lapsed:");
Serial.println(lastDoseTime-millis());
if(overdoseProtection=true && ((lastDoseTime-millis()>=timeCheck) || count>=1)){
  voiceAlert(); 
  if(count>maxCount){
    sendtextMessage(); 
  }
}










  
}

void releaseDose(){
   if(((millis()- lastDoseTime >= timeInterval)) || count==0){
      M1.write(0);                  // Rotate 90 degree
      delay(1000);                           // waits for the servo to get there
      M1.write(90);
      delay(1000);

      M2.write(0);                  // Rotate 90 degree
      delay(1000);                           // waits for the servo to get there
      M2.write(90);
      delay(1000);
  
      overdoseProtection=true; //there is something on the tray, activate overdoseProtection
      Serial.println("Medicine dispensed"); 
   }
   else 
   Serial.println("Medicine not dispensed yet."); 
}
void readIRSensor(){
  if(digitalRead(9)==LOW){
      Serial.println("Medicine is still on the tray");
      
  
}
  else{
    Serial.println("Medicine picked up!");
    
    overdoseProtection=false; 
  }  

delay(1000);

  
}
void voiceAlert(){

    digitalWrite(5,LOW); // pin 5 is configured to the speaker system + playback chip 
    delay(11000);
    digitalWrite(5,HIGH);
    Serial.println("VOICE ALERT ACTIVATED"); 
    count++; 
 


}

void sendtextMessage(){
  Serial.println("Your loved one needs to take their medicine."); 
}
