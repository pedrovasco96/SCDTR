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
int ref_change;
float PWM;
int flag=1;
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
int led_active = 0;
int node;
int n_drec = 0;
float d_rec = 0;

float dn[N] = {0};

PID pid;

// Function that defines the appropriate reference LUX value
// This reference can be changed with the circuit potenciometer
float occupation()
{
  int ocp=analogRead(A1);
  if(addr!=1)
  {
    return ref_low;
  }
  else
  {
    return ref_low;
  }
}

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
  on = 0.0;

  if (addr == 0) {
    Kn[0] = 0.5;
    Kn[1] = 0.1;
    Ln = 20.0;
  }
  else if (addr == 1) {
    Kn[0] = 0.13;
    Kn[1] = 0.49;
    Ln = 20.0;
  }

  Serial.print("address: ");
  Serial.println(addr);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(ledPin, OUTPUT);

  // Inicialize the led with 0 PWM value
  analogWrite(ledPin, 0);
}

void loop() {

  node = addr;
  
  // Enables or disables the feedforward term
  /*if (Serial.available() > 0) {
    aux = Serial.parseInt();
    if(aux == 0 || aux == 1)
    {
      flag = aux;
      ref_change = 1;
    }
  }*/

  start_time = millis();



  ref=occupation();
  if(prev_ref != ref)
    ref_change = 1;

  prev_ref = ref;

 
  //reads LUX value
  LUX=LUX_value();

  Ln = ref;

  // defines the control value we should input
  light=pid.control_signal(ref, LUX);

  // limit the control value range
  light=constrain(light, 0, 255);

  analogWrite(ledPin, light);

  PWM=light*100.0/255.0;

  Serial.print(ref);
  Serial.print("\t");
  Serial.print(LUX);
  Serial.print("\t");
  Serial.print(PWM);
  Serial.print("\t");
  Serial.print(flag);
  Serial.print("\t");
  Serial.print(millis());

  if(ref_change == 1 && flag == 1){
    Serial.print(" ");
 //   Serial.print("FeedFoward");    
  }

  Serial.println();

  end_time = millis();
  time_ = end_time - start_time;
  delay(25-time_);

}

void receiveEvent(int howMany) {
  while (Wire.available() > 0) {
    char red = Wire.read();
    if (red == 'b') {
      n_done++;
      //  Serial.print("one finished ");
      // Serial.println(n_done);
    }
    else if (red == 'a') {
      led_active = 1;
      Serial.println("other led is active");
    }
    else if (red == 'c') {
      float buff = Wire.read();
      d_rec += buff;
      n_drec++;
    }
  }
}
