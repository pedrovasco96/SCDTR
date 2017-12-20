#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>


byte addr;
int i = 3;
int j = 0;
float z = 56.8;
char lel;
int bufa;
float ref, ref_high, ref_low, ref_change, calib;

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
  if (Serial.available() > 0) {
    char aux = Serial.read();

    if (aux == 's' || aux == 'u')
    {
      int target = Serial.parseInt();
      if (target == 0) {
        if (aux == 's')
          ref = ref_high;
        else
          ref = ref_low;
      }
      Wire.beginTransmission(0);
      Wire.write(aux);
      Wire.write(target);
      Wire.endTransmission();

      ref_change = 1;
    }
    else if (aux == 'r') {
      calib = 1;
      Wire.beginTransmission(0);
      Wire.write(aux);
      Wire.endTransmission();

    }
  }
}

void receiveEvent(int howMany) {
  while (Wire.available() > 0)  {
    char red = Wire.read();
    if (red == 's') {
      bufa = Wire.read();
      if (addr == bufa) {
        Serial.println(red);
        Serial.println(bufa);
      }

    }
    else if (red == 'u') {
      bufa = Wire.read();
      if (addr == bufa) {
        Serial.println(red);
        Serial.println(bufa);
      }
    }
  }
}

