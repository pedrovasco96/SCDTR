#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>


byte addr;
int i = 10;
int j = 40;
float z = 56.8;
char lel;
char c;

void setup() {
  Serial.begin(115200);

  addr = EEPROM.read(0);

  Wire.begin(addr);
  TWAR = (addr << 1) | 1;
  Wire.onReceive(receiveEvent);

  Serial.print("address: ");
  Serial.println(addr);




}

void loop() {
  for (int pila = 0; pila < 2; pila++) {
    if (addr == pila) {
      if (Serial.available() > 0 ) {
        lel = Serial.read();
        Wire.beginTransmission(0);
        Wire.write('z');
        Wire.write(lel);
        Wire.endTransmission();
      }
      else {
        Wire.beginTransmission(0);
        Wire.write('a');
        Wire.write(addr);
        Wire.write('d');
        Wire.write(i);
        Wire.write('l');
        Wire.write(j);
        Wire.endTransmission();
      }

      i += 10;
      j += 10;

      if (i == 100)
        i = 10;
      if (j == 100)
        j = 10;

    }
  }
  delay(100);
}

void receiveEvent(int howMany) {
  while (Wire.available() > 0)  {
    char red = Wire.read();
    if (red == 'z') {
      c = Wire.read();
      Serial.print("Received data: ");
      Serial.println(c);
    }
    else {
      Serial.print("Received data: ");
      Serial.print(red);
      c = Wire.read();
      Serial.print(c);
      c = Wire.read();
      Serial.print(c);
      c = Wire.read();
      Serial.print(c);
      c = Wire.read();
      Serial.print(c);
      c = Wire.read();
      Serial.println(c);


    }
  }
}

