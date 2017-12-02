#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>

#define N 2

byte addr;
int c;
int i = 0;
int j= 0;
int sensv;

double K[N][N]={0};
byte adr[N]={0,1};

void setup() {
  Serial.begin(115200);

  addr = EEPROM.read(1);

  Wire.begin(addr);
  Wire.onReceive(receiveEvent);

  Serial.print("address: ");
  Serial.print(addr);
  Serial.println();
}

void loop() {

  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      // Liga led
      if(addr==adr[j])
      {
        analogWrite(9,100);
      }
      // le valor LDR
      if(addr==adr[i])
      {
        delay(100);
        sensv=analogRead(A0);
        // Se for endereço zero escreve no serial
        if(addr==0)
        {
          Serial.println(sensv);
        }
        // Se for endereço diferente de zero envia para o zero
        else
        {
          Wire.beginTransmission(0);
          Wire.write(sensv);  
          Wire.endTransmission();
        }
        //compute k[i][j]
      }
      else
      {
        delay(100);
      }
      // desliga LED
      if(addr==adr[j])
      {
        analogWrite(9,0);
      }
    }
  }
  delay(2000);
}

void receiveEvent(int howMany){
  while(Wire.available() > 0)  {
    sensv = Wire.read(); 
    Serial.print("Received data: ");
    Serial.print(sensv);
    Serial.println(); 
  }
}
