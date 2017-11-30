#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>


byte addr;
int c;
int i = 3;
int j= 0; 

void setup() {
  Serial.begin(115200);

  addr = EEPROM.read(0);

  Wire.begin(addr);
  Wire.onReceive(receiveEvent);

  Serial.print("address: ");
  Serial.print(addr);
  Serial.println();
  


}

void loop() {
  if(Serial.available() > 0 ) {
    c = Serial.parseInt();
    if(addr == 0){
      Wire.beginTransmission(1); 
      Serial.print("Sending data ");
      Serial.print(c);
      Serial.println(" to 1");
    } 
    else if(addr == 1){
      Wire.beginTransmission(0);
      Serial.print("Sending data ");
      Serial.print(c);
      Serial.println(" to 0");
    }
       
    Wire.write(c);  
    Wire.endTransmission();
  }
}

void receiveEvent(int howMany){
  while(Wire.available() > 0)  {
    c = Wire.read(); 
    Serial.print("Received data: ");
    Serial.print(c);
    Serial.println(); 
  }
}

