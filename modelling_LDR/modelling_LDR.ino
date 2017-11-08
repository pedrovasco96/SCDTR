const int ledPin = 9;
int light = 0;
int i;
int flag=0;
float Ra=12650;
float LUX;
float a = -0.7;
float b = 4.802;
float p=pow(10,-b/a);

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
}

void loop() {
  if(flag==0){
    for(i=0; i<=255; i+=1){    
      analogWrite(ledPin, i);

      delay(50);
    
      // read the value from the photoresistor:
      float sensv = analogRead(A0); 
  
      // define voltage in LDR
      sensv=5*(sensv/1023);
  
      // define resistance in LDR
      float sensr=10000*(5/sensv-1);
      
      // converting to Lux
      LUX=p*(pow(sensr,1/a));

      // print out the value to the serial monitor
      //Serial.print("V Value: ");
      Serial.print(LUX);
      //Serial.print(" \t R Value: ");
      //Serial.print(sensr);
      Serial.print("\t");
      Serial.println(i);
    }
  }

  flag=1;
  analogWrite(ledPin, 0);
}
