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
pin 102 song number 1003.mp3 "Medication dispensed" connected to pin 3 on MKR1000 main chip
pin 104 song number 1005.mp3 "Medication picked up" connected to pin 4 on MKR1000 main chip
  
*/

#include <Servo.h>
#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>

//Your Wifi router setup at home
char ssid[] = "MY2.4G";   //your network SSID (aka WiFi name)
char pass[] = "Nmamitn92#"; //your network password
int status = WL_IDLE_STATUS;
WiFiServer server(80);
//To send SMS using the s  IFTTT
const char* host = "maker.ifttt.com";
WiFiSSLClient sslClient;




Servo M1;  // create servo object to control a servo
Servo M2;  // create servo object to control a servo

unsigned long int day=86400000; //milliseconds in a day
unsigned long int hour=3600000; //milliseconds in a hour
unsigned long int minute=60000; //milliseconds in a min
unsigned long int second=1000; //milliseconds in a second
int overdoseProtection=0; // is the tray not empty? 
 int timeInterval = 1; // 1 minutes interval for medication dosage, how often you should take the medication
 int timeCheck= 1; // 1 min  after timeCheck hrs, check if medication was taken 
 int lastDoseTime= 0; // last time dose was dispensed
//boolean voicealert= false; // starts the program with the voice alert off 
int count =0 ; // count # of times voiceAlert was used
int maxCount=3; //maximum amount of times voiceAlert will happen before a text message will be sent to loved ones. 
int timelapsedinmins =0; // time lapsed in minutes 

int PIN_M1_Motor =6; // attaches the servo on pin 6 to the servo object Motor 1
int PIN_M2_Motor =7 ; //attaches the servo on pin 7to the servo object Motor 2
int PIN_IR_Sensor1 = 9; // pin 9 is connected to the IR sensor output
 /*
   * pin 100 song number 1001.mp3 "Please take medication" connected to pin 5 on MKR1000 main chip 
    pin 102 song number 1003.mp3 "Medication dispensed" connected to pin 4 on MKR1000 main chip
    pin 104 song number 1005.mp3 "Medication picked up" connected to pin 3 on MKR1000 main chip
   * 
 */
int PIN_VM_PTM =5;
int PIN_VM_MD =3;
int PIN_VM_MPU =4; 

void setup() {
  delay(10000); //delay to show the console display
  int r= Connect_to_Wifi(); // Connect to wifi for sendign text message 
   
  lastDoseTime= timeprint(millis()); // initializes the var to the time when you start the program 
  Serial.println("Start time:");
  Serial.println(lastDoseTime);
 
  pinMode(PIN_IR_Sensor1, INPUT); // pin 9 is connected to the IR sensor output
  pinMode(PIN_VM_PTM, OUTPUT); // pin5 is connected to PEMENOL board for voice alert audio
  pinMode(PIN_VM_MD, OUTPUT); // pin4 is connected to PEMENOL board for voice alert audio
  pinMode(PIN_VM_MPU, OUTPUT); // pin3 is connected to PEMENOL board for voice alert audio
  digitalWrite(PIN_VM_PTM,HIGH);// pin5 is connected to PEMENOL board for voice alert audio; initialize to no voice;
  digitalWrite(PIN_VM_MD,HIGH);// pin4 is connected to PEMENOL board for voice alert audio; initialize to no voice;
  digitalWrite(PIN_VM_MPU,HIGH);// pin3 is connected to PEMENOL board for voice alert audio; initialize to no voice;
  
   M1.attach(PIN_M1_Motor);  // attaches the servo on pin 6 to the servo object Motor 1
   M1.write(135);
   delay(1000);
   M2.attach(PIN_M2_Motor);  // attaches the servo on pin 7to the servo object Motor 2
   M2.write(135); 
   delay(1000); // Delay 1 seconds 
 
    
}

void loop() {
  Serial.print("Overdose : ");
  Serial.println(overdoseProtection);
  delay(1000);

  
if(overdoseProtection==0 ){ 
  // release medications 
  releaseDose(); 
  count=0; 
}
else{
delay(1000);   // Delay a secons
// read obstacle IR sensor
readIRSensor();  
timelapsedinmins=timeprint(millis())-lastDoseTime; 
Serial.print("Time elapsed in min:");
Serial.println(timelapsedinmins);

if(overdoseProtection==1 && ((timelapsedinmins>=timeCheck) || count>=1)){
  voiceAlert();
  Serial.println("Waiting after voice alert for 10seconds"); 
  delay(10000);
  if(count>maxCount){
    sendtextMessage(); 
  }
}

}
}

void releaseDose(){
  timelapsedinmins=timeprint(millis())-lastDoseTime; 
  Serial.print("Release dose : Time elapsed in min:");
  Serial.println(timelapsedinmins-timeInterval);
   if(timelapsedinmins-timeInterval>=0) {
      M1.write(135);                  // Rotate 90 degree
      delay(1000);                           // waits for the servo to get there
      M1.write(45);
      delay(1000);
      M1.write(135);
      delay(1000);
      
      M2.write(135);                  // Rotate 90 degree
      delay(250);                           // waits for the servo to get there
      M2.write(45);
      delay(250);
      M2.write(135);
       delay(1000);
  
      overdoseProtection=1; //there is something on the tray, activate overdoseProtection
      Serial.println("Medicine dispensed"); 
      Medicationdispensed();
       //record the time when the dose was released. 
      Serial.println("Last Dose  time: ");
      lastDoseTime=timeprint(millis()); // Reset and print the last dose time 
   }
   else 
   Serial.println("Medicine not dispensed yet."); 
}
void readIRSensor(){
  if(digitalRead(PIN_IR_Sensor1)==LOW){
      Serial.println("Medicine is still on the tray");
      overdoseProtection=1;
  
}
  else{
    Serial.println("Medicine picked up!");
    Medicationpickedup();
    overdoseProtection=0; 
    lastDoseTime= timeprint(millis()); //Reset the last dose time
  }  



  
}
void voiceAlert(){

    digitalWrite(PIN_VM_PTM,LOW); // pin 5 is configured to the speaker system + playback chip 
    delay(1000);
    digitalWrite(PIN_VM_PTM,HIGH);
    Serial.print("Voice Alert Activated: "); 
    Serial.println(count);
    count++; 
    


}

void Medicationdispensed(){

    digitalWrite(PIN_VM_MD,LOW); // pin 4 is configured to the speaker system + playback chip 
    delay(1000);
    digitalWrite(PIN_VM_MD,HIGH);
    Serial.print("Medication dispensed: ");  
    delay(10000);


}
void Medicationpickedup(){

    digitalWrite(PIN_VM_MPU,LOW); // pin 3 is configured to the speaker system + playback chip 
    delay(1000);
    digitalWrite(PIN_VM_MPU,HIGH);
    Serial.print("Medication picked up! ");  
    delay(10000);


}

void sendtextMessage(){
  Serial.println("Your loved one needs to take their medicine."); 
  //Send Text message
  ifttt_Send_SMS();
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



int  Connect_to_Wifi()

{
if (WiFi.status() == WL_NO_SHIELD) {
       // In case of connection issues wait 
        delay(3000);
        return 1;       
  }
      // attempt to connect to Wifi network
    while ( status != WL_CONNECTED) {
   Serial.println("Connecting to Wifi");
        status = WiFi.begin(ssid, pass);
        if(status != WL_CONNECTED){
          // wait 10 seconds for connection:
        delay(1000);
        }

    }

    Serial.println("Connected to Wifi");
    server.begin();
    printWifiStatus();

}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    //signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    // print where to go in a browser:
    Serial.print("Your IP address to use in the browser: ");
    Serial.println(ip);
}


//call to IFTTT recipe to send an email or sms to your mobile
void ifttt_Send_SMS() {
  String Medicine_not_pickedup = "Y";
  String  data = "{\"value1\":\"" + Medicine_not_pickedup   + "\"}";

  int i = sslClient.connect(host, 443);
  Serial.println("SSL connect return code");
  Serial.println(i);
  
  if (sslClient.connect(host, 443)) { //443 or 80
    //change this to your Maker setting from https://ifttt.com/services/maker/settings
    //sslClient.println("POST /trigger/moition_detected/with/key/XXXXXXXXXXXXXXXX HTTP/1.1");
    sslClient.println("POST  /trigger/{MedicineNotPicked}/with/key/eMqTu9G_LZ1WYzmVyZEN6P6t1kqVpGlSdrg4lYIIV6M  HTTP/1.1");
   // POST https://maker.ifttt.com/trigger/{event}/with/key/eMqTu9G_LZ1WYzmVyZEN6P6t1kqVpGlSdrg4lYIIV6M
    sslClient.println("Host: maker.ifttt.com");
    sslClient.println("Content-Type: application/json");
    sslClient.print("Content-Length: ");
    sslClient.println(data.length());
    sslClient.println();
    sslClient.print(data);
    sslClient.stop();
    Serial.println("IFTTT request Sucessful");
  }
  else {
    Serial.println("IFTTT request failed");
  }
}
