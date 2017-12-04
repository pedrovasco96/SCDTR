#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>

#define N 2

byte addr;
int c;
int i = 0;
int j = 0;
int sensv;
float start_time;
float end_time;


double K[N][N]={0};
byte adr[N+1]={0,1,2};

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

  for(i=1;i<=N;i++){
    // Liga led
    if(addr==adr[i])
    {
      analogWrite(9,100);
    }
    delay(100);
    for(j=1;j<=N;j++){
      // le valor LDR
      if(addr==adr[j])
      {
        sensv=analogRead(A0);
        // Se for endereço zero escreve no serial
        if(addr==1)
        {
          Serial.println(sensv);
        }
        // Se for endereço diferente de zero envia para o zero
        else
        {
          Wire.beginTransmission(1);
          Wire.write(sensv);  
          Wire.endTransmission();
        }
        //K[i][j]=100/sensv;
      }
    }
    delay(500);
    // desliga LED
    if(addr==adr[i])
    {
      analogWrite(9,0);
    }
  }
  delay(5000);
}

void receiveEvent(int howMany){
  while(Wire.available() > 0)  {
    sensv = Wire.read(); 
    Serial.print("Received data: ");
    Serial.print(sensv);
    Serial.println(); 
  }
}
