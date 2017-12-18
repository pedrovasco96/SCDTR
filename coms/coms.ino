#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>


byte addr;
int i = 3;
int j = 0;
float z = 56.8;
char lel;

void setup() {
  Serial.begin(115200);

  addr = EEPROM.read(0);

  Wire.begin(addr);
  TWAR = (addr << 1) | 1;
  Wire.onReceive(receiveEvent);

  Serial.print("address: ");
  Serial.print(addr);
  Serial.println();



}

void loop() {
  if (Serial.available() > 0 ) {
    lel = Serial.read();
    if (addr == 0) {
      Wire.beginTransmission(0);
      Serial.print("Sending data ");
      Serial.print(lel);
      Serial.println(" to 1");
    }
    else if (addr == 1) {
      Wire.beginTransmission(0);
      Serial.print("Sending data ");
      Serial.print(lel);
      Serial.println(" to 0");
    }

    Wire.write(lel);
    Wire.endTransmission();
  }
}

void receiveEvent(int howMany) {
  while (Wire.available() > 0)  {
    char c = Wire.read();
    Serial.print("Received data: ");
    Serial.print(c);
    Serial.println();
  }
}

