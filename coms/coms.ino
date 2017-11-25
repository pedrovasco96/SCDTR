#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>


byte addr;
char c;

void setup() {
  Serial.begin(115200);

  addr = EEPROM.read(0);

  Wire.begin(addr);
  Wire.onReceive(receiveEvent);

  Serial.print("address :");
  Serial.print(addr);
  Serial.println();
  


}

void loop() {
  if(Serial.available() > 0 ) {
    c = Serial.read();
    if(addr == 0){
      Wire.beginTransmission(1); 
      Serial.print("Sending data: ");
      Serial.write(c);
      Serial.println("\t to: 1");
    } 
    else if(addr == 1){
      Wire.beginTransmission(0);
      Serial.print("Sending data: ");
      Serial.write(c);
      Serial.println("\t to: 0");
    }
       
    Wire.write(c);  
    Wire.endTransmission();
  }
}

void receiveEvent(int howMany){
  while(Wire.available() > 0)  {
  c = Wire.read(); 
  Serial.print("Received data :");
  Serial.write(c); 
  }
}

