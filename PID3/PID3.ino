#include "pid3.h"
#include <stdio.h>
#include <EEPROM.h>
#include <Wire.h>
#include <math.h>

#define N 2

const int ledPin = 9;
int light = 0;
float ref_low = 11.62; // 1/3 * MAX_LUX
float ref_high = 23.25; // 2/3 * MAX_LUX
float ref;
float prev_ref = 0;
int ref_change = 1;
int calib = 1;
int cons = 1;
float PWM;
int flag = 1;
int aux;
float LUX;
float sensr;
float start_time;
float end_time;
float time_;
float al[N] = { -0.7, -1.1};
float bl[N] = {4.802, 6.1};

int addr;
float cn;
float qn;
float Kn[N] = {0};
float on;
float Ln;
int n_done = 0;
volatile int led_active = 0;
int node;
volatile int n_drec = 0;
float d_rec = 0;

float dn[N] = {0};

float R = 160;
float R_p = R * 100.0 / 255.0;
float buff2 = 0;
int bufa;
float curr_time = 0;

PID pid;

// This function computes the value in LUX in the box read by the LDR
float LUX_value()
{
  // read the value from the photoresistor:
  float sensv = analogRead(A0);

  // define voltage in LDR
  sensv = 5 * (sensv / 1023);

  // define resistance in LDR
  float sensr = 10000 * (5 / sensv - 1);

  // converting to Lux
  return pow(10, -bl[addr] / al[addr]) * (pow(sensr, 1 / al[addr]));
}

void setup() {
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);

  addr = EEPROM.read(0);

  Wire.begin(addr);
  TWAR = (addr << 1) | 1;

  Wire.onReceive(receiveEvent);

  cn = 1.0;
  qn = 0.0;

  Serial.print("address: ");
  Serial.println(addr);

  ref = ref_low;


  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(ledPin, OUTPUT);

  // Inicialize the led with 0 PWM value
  analogWrite(ledPin, 0);
}

void loop() {

  // Detects changes in reference
  if (Serial.available() > 0) {
    char aux = Serial.read();

    if (aux == 's' || aux == 'u')
    {
      while (Serial.available() <= 0) {
        delay(1);
      }
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
    else if (aux == 'f') {
      if (cons == 0)
        cons = 1;
      else
        cons = 0;

      Wire.beginTransmission(0);
      Wire.write(aux);
      Wire.endTransmission();
    }
  }

  if (calib == 1)
  {
    led_active = 0;
    n_done = 0;
    analogWrite(ledPin, 0);
    for (int i = 0; i < N; i++) {
      // le valor LDR
      if (addr == i) {
        n_done = 0;
        analogWrite(9, R);
        Serial.println("led ligated");
        delay(1000);
        buff2 = LUX_value();
        //Serial.println(buff);
        Kn[i] = buff2 / R_p;

        Wire.beginTransmission(0);
        Wire.write('e');
        Wire.endTransmission();
        Serial.println("Permission granted");

        start_time = millis();
        while (n_done < N - 1) {
          curr_time = millis();
          if (curr_time - start_time >= 5000) {
            i--;
            break;
          }
        }
        analogWrite(9, 0);
      }
      else {
        delay(1000);
        while (led_active == 0);
        Serial.println("got permission");
        buff2 = LUX_value();
        //Serial.println(buff);
        Kn[i] = buff2 / R_p;
        Wire.beginTransmission(i);
        Wire.write('b');
        Wire.endTransmission();
        led_active == 0;
      }
    }
    for (int i = 0; i < N; i++) {
      Serial.println(Kn[i]);
    }
    delay(1000);
    on = LUX_value();
    Serial.println(on);
    calib = 0;
    ref = ref_low;
    ref_change = 1;
  }

  node = addr;

  start_time = millis();

  /*ref=occupation();
    if(prev_ref != ref)
    ref_change = 1;*/

  //prev_ref = ref;


  //reads LUX value
  LUX = LUX_value();

  Ln = ref;



  // defines the control value we should input
  light = pid.control_signal();

  // limit the control value range
  light = constrain(light, 0, 255);

  analogWrite(ledPin, light);

  PWM = light * 100.0 / 255.0;

  Serial.print(ref);
  Serial.print("\t");
  Serial.print(LUX);
  Serial.print("\t");
  Serial.print(PWM);
  Serial.print("\t");
  Serial.print(flag);
  Serial.print("\t");
  Serial.print(millis());
  Serial.println();



  end_time = millis();
  time_ = end_time - start_time;

  for (int i = 0; i < N; i++) {
    if (addr == i) {
      Wire.beginTransmission(0);
      Wire.write('a');
      Wire.endTransmission();
      Wire.beginTransmission(0);
      Wire.write(addr);
      Wire.endTransmission();
      Wire.beginTransmission(0);
      Wire.write('d');
      Wire.endTransmission();
      Wire.beginTransmission(0);
      Wire.write(int(PWM));
      Wire.endTransmission();
      Wire.beginTransmission(0);
      Wire.write('l');
      Wire.endTransmission();
      Wire.beginTransmission(0);
      Wire.write(int(LUX));
      Wire.endTransmission();
    }
  }

  delay(30 - time_);

}

void receiveEvent(int howMany) {
  while (Wire.available() > 0) {
    char red = Wire.read();
    if (red == 'b') {
      n_done++;
      //  Serial.print("one finished ");
      // Serial.println(n_done);
    }
    else if (red == 'e') {
      led_active = 1;
      Serial.println("other led is active");
    }
    else if (red == 'c') {
      float buff = Wire.read();
      d_rec += buff;
      n_drec++;
    }
    else if (red == 's') {
      bufa = Wire.read();
      if (addr == bufa)
        ref = ref_high;
      ref_change = 1;
    }
    else if (red == 'u') {
      bufa = Wire.read();
      if (addr == bufa)
        ref = ref_low;
      ref_change = 1;
    }
    else if (red == 'r') {
      calib = 1;
    }
    else if (red == 'f') {
      if (cons == 0)
        cons = 1;
      else
        cons = 0;
    }
  }
}
