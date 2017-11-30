#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>


byte addr;
int c;
int sensv;
int i = 3;
int j= 0; 

void setup() {
  Serial.begin(9600);

  pinMode(A0, INPUT);
  pinMode(9, OUTPUT);

  addr = EEPROM.read(0);

  Wire.begin(addr);
  Wire.onReceive(receiveEvent);

  Serial.print("address: ");
  Serial.print(addr);
  Serial.println();
  
}

void loop() {

  if(addr == 1){
    analogWrite(9,100);
    sensv=analogRead(A0);
    Wire.beginTransmission(0);
    Wire.write(sensv);  
    Wire.endTransmission(); 
  }
  // addr comunicates with serial
  if(addr == 0){
    sensv=analogRead(A0);
    Serial.println(sensv);
    analogWrite(9,100);
  } 
  delay(5000);
  if(addr == 1){
    analogWrite(9,0);
    sensv=analogRead(A0);
    Wire.beginTransmission(0);
    Wire.write(sensv);  
    Wire.endTransmission(); 
  }
  if(addr == 0){
    sensv=analogRead(A0);
    Serial.println(sensv);
    analogWrite(9,0);
  } 
   while(1)
   {}
       
}

void receiveEvent(int howMany){
  while(Wire.available() > 0)  {
    sensv = Wire.read(); 
    Serial.print("Received data: ");
    Serial.print(sensv);
    Serial.println(); 
  }
}

