#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>


byte addr;
int i = 3;
int j= 0; 
float z = 56.8;
char lel;

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
    lel = Serial.read();
    if(addr == 0){
      Wire.beginTransmission(1); 
      Serial.print("Sending data ");
      Serial.print(z);
      Serial.println(" to 2");
    } 
    else if(addr == 1){
      Wire.beginTransmission(0);
      Serial.print("Sending data ");
      Serial.print(z);
      Serial.println(" to 1");
    }
       
    Wire.write(z);  
    Wire.endTransmission();
  }
}

void receiveEvent(int howMany){
  while(Wire.available() > 0)  {
    float c = Wire.read(); 
    Serial.print("Received data: ");
    Serial.print(c);
    Serial.println(); 
  }
}

