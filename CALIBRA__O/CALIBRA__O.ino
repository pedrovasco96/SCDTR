#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>

#define N 2


int addr;
int c = 1;
int d = 0;
int i = 0;
int j = 0;
char red;
int led_active=0;
int n_done=0;

float R = 160;
float start_time = 0;
float curr_time = 0;

float al[N] = {-0.7, -1.1};
float bl[N] = {4.802, 6.1};

float R_p = R*100.0/255.0;
float o = 0;
float buff = 0;
float K[N]={0};

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

    for(i=1;i<=N;i++){
      // le valor LDR
      if(addr==i){
        n_done = 0;
        analogWrite(9,R);
        Serial.println("led ligated");
        delay(1000);
        buff = LUX_value();
        K[i-1]=R_p/buff;
        
        Wire.beginTransmission(0);
        Wire.write('a');  
        Wire.endTransmission();    
        Serial.println("Permission granted"); 
      
        start_time=millis();
        while(n_done<N-1){
          delay(50);
          curr_time=millis();
          if(curr_time-start_time >= 5000){
            i--;
            break;
          }
            
        }
        
        analogWrite(9,0);
           
        }
      else{
        while(led_active==0){
          delay(50);
        }
        Serial.println("got permission");
        delay(1000);
        buff = LUX_value();
        K[i-1]=R/buff;
        Wire.beginTransmission(i);
        Wire.write('b');  
        Wire.endTransmission();
        led_active==0;
      }
    }
    
    for(i=0;i<N;i++){
      Serial.println(K[i]);
    }

    delay(1000);
    o = LUX_value();
    
    Serial.println(o);
    while(1);
}


void receiveEvent(int howMany){
  while(Wire.available() > 0){
    red = Wire.read();
    if(red=='b'){
      n_done++;  
      Serial.print("one finished ");
      Serial.println(n_done);
    }
    else if(red=='a'){
      led_active=1;
      Serial.println("other led is active");
    }
  }
}


// This function computes the value in LUX in the box read by the LDR
float LUX_value()
{
  // read the value from the photoresistor:
  float sensv = analogRead(A0);

  // define voltage in LDR
  sensv=5*(sensv/1023);

  // define resistance in LDR
  float sensr=10000*(5/sensv-1);

  // converting to Lux
  return pow(10,-bl[addr-1]/al[addr-1])*(pow(sensr,1/al[addr-1]));
}


