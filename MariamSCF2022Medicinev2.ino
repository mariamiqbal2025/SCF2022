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
PEMENOL CHIP CONFIGURATION:
pin 100 song number 1001.mp3 "Please take medication" connected to pin 5 on MKR1000 main chip 
pin 102 song number 1003.mp3 "Medication dispensed" connected to pin 4 on MKR1000 main chip
pin 104 song number 1005.mp3 "Medication picked up" connected to pin 3 on MKR1000 main chip
  
*/

#include <Servo.h>

Servo M1;  // create servo object to control a servo
Servo M2;  // create servo object to control a servo

unsigned long int day=86400000; //milliseconds in a day
unsigned long int hour=3600000; //milliseconds in a hour
unsigned long int minute=60000; //milliseconds in a min
unsigned long int second=1000; //milliseconds in a second

int overdoseProtection=0; // is the tray not empty? 
unsigned long int timeInterval = 5; // 5 minutes interval for medication dosage, how often you should take the medication
unsigned long int timeCheck= 1; // 1min  after timeCheck hrs, check if medication was taken 
unsigned long int lastDoseTime= 0; // last time dose was dispensed

boolean voicealert= false; // starts the program with the voice alert off 
int count =0 ; // count # of times voiceAlert was used
int maxCount=3; //maximum amount of times voiceAlert will happen before a text message will be sent to loved ones. 

int timelapsedinmins =0; // time lapsed in minutes 

void setup() {
  delay(10000); //delay to show the console display
  
  lastDoseTime= timeprint(millis()); // initializes the var to the time when you start the program 
  Serial.println("Start time:");
  Serial.println(lastDoseTime);
  /*
   * pin 100 song number 1001.mp3 "Please take medication" connected to pin 5 on MKR1000 main chip 
    pin 102 song number 1003.mp3 "Medication dispensed" connected to pin 4 on MKR1000 main chip
    pin 104 song number 1005.mp3 "Medication picked up" connected to pin 3 on MKR1000 main chip
   * 
   */
  pinMode(9, INPUT); // pin 9 is connected to the IR sensor output
  pinMode(5, OUTPUT); // pin5 is connected to PEMENOL board for voice alert audio
  pinMode(4, OUTPUT); // pin4 is connected to PEMENOL board for voice alert audio
  pinMode(3, OUTPUT); // pin3 is connected to PEMENOL board for voice alert audio
  
  //delay(10000); // Delay 10 seconds 
  M1.attach(6);  // attaches the servo on pin 6 to the servo object Motor 1
   M2.attach(7);  // attaches the servo on pin 7to the servo object Motor 2
   M1.write(0);
   M2.write(0);
   digitalWrite(5,HIGH);// pin5 is connected to PEMENOL board for voice alert audio; initialize to no voice;
     delay(10000); // Delay 10 seconds 
}

void loop() {
  Serial.print("Overdose : ");
  Serial.println(overdoseProtection);
  delay(1000);
if(overdoseProtection==0 ){ 
  // release medications 
  releaseDose(); 
  //record the time when the dose was released. 
  
  Serial.println("Net Dose  time: ");
  lastDoseTime=timeprint(millis()); 
  
  count=0; 
}
delay(1000);
  //delay(60000); // Delay a minute
// read obstacle IR sensor
readIRSensor();  
timelapsedinmins=timeprint(millis())-lastDoseTime; 
Serial.print("Time elapsed in min:");
Serial.println(timelapsedinmins);

if(overdoseProtection==1 && ((timelapsedinmins>=timeCheck) || count>=1)){
  voiceAlert();
  Serial.println("Waiting after voice alert for 30seconds"); 
  delay(30000);
  if(count>maxCount){
    sendtextMessage(); 
  }
}










  
}

void releaseDose(){
  timelapsedinmins=timeprint(millis())-lastDoseTime; 
  Serial.print("Release dose : Time elapsed in min:");
  Serial.println(timelapsedinmins);
   if((timelapsedinmins-timeInterval<=0) || count==0){
      M1.write(0);                  // Rotate 90 degree
      delay(1000);                           // waits for the servo to get there
      M1.write(90);
      delay(1000);

      M2.write(0);                  // Rotate 90 degree
      delay(1000);                           // waits for the servo to get there
      M2.write(90);
      delay(1000);
  
      overdoseProtection=1; //there is something on the tray, activate overdoseProtection
      Serial.println("Medicine dispensed"); 
      Medicationdispensed(); 
   }
   else 
   Serial.println("Medicine not dispensed yet."); 
}
void readIRSensor(){
  if(digitalRead(9)==LOW){
      Serial.println("Medicine is still on the tray");
      overdoseProtection=1;
  
}
  else{
    Serial.println("Medicine picked up!");
    Medicationpickedup();
    overdoseProtection=0; 
  }  



  
}
void voiceAlert(){

    digitalWrite(5,LOW); // pin 5 is configured to the speaker system + playback chip 
    delay(1000);
    digitalWrite(5,HIGH);
    Serial.print("VOICE ALERT ACTIVATED : "); 
    Serial.println(count);
    count++; 
    


}

void Medicationdispensed(){

    digitalWrite(4,LOW); // pin 4 is configured to the speaker system + playback chip 
    delay(1000);
    digitalWrite(4,HIGH);
    Serial.print("Medication dispensed: ");  
 


}
void Medicationpickedup(){

    digitalWrite(3,LOW); // pin 3 is configured to the speaker system + playback chip 
    delay(1000);
    digitalWrite(3,HIGH);
    Serial.print("Medication picked up! ");  
 


}

void sendtextMessage(){
  Serial.println("Your loved one needs to take their medicine."); 
}

int timeprint(unsigned long int mills){

  int days = mills/day;
  int hours=(mills%day)/hour;
  int minutes=((mills%day)%hour)/minute;
  int seconds=(((mills%day)%hour)%minute)/second;
  Serial.print("Time now:" );
  Serial.print(" day:");
  Serial.print(days);
  Serial.print(" Hours:");
  Serial.print(hours);
  Serial.print(" Minutes:");
  Serial.print(minutes);
  Serial.print(" Seconds:");
  Serial.println(seconds);
  
  return minutes;
  
}
