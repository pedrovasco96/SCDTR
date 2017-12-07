#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>

#define N 2

byte addr;
int c = 1;
int d = 0;
int i = 0;
int j = 0;
int red = 0;
int led_active=0;
int n_done = 0;
int sensv;
float start_time;
float end_time;


double K[N]={0};
byte curr = 1;

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

    for(i=1;j<=N;j++){
      // le valor LDR
      if(addr==i){
        n_done = 0;
        analogWrite(9,100);
        Serial.println("led ligated");
        delay(500);
        K[i-1]=analogRead(A0);
        Wire.beginTransmission(0);
        Wire.write(d);  
        Wire.endTransmission();
        
        while(n_done<N){
        }
        
        analogWrite(9,0);
           
        }
      else{
        while(led_active==0){
        }
        Serial.print("got permission");
        delay(500);
        K[i-1]=analogRead(A0);
        Wire.beginTransmission(i);
        Wire.write(c);  
        Wire.endTransmission();
        led_active==0;
      }
    }
    for(i=0;i<N;i++){
      Serial.println(K[i]);
    }
    while(1);
}


void receiveEvent(int howMany){
  while(Wire.available() > 0){
    red = Wire.read();
    if(red==1){
      n_done++;  
      Serial.println("one finished");
    }
    else if(red==0){
      led_active=1;
    }
  }
}
