const int ledPin = 9;
int light = 127.5;
int i;
int flag=0;
float Ra=12650;
float LUX;
float sensr;
float a = -0.7;
float b = 4.802;
unsigned long sampint=15;
unsigned long starttime=0;
unsigned long endtime=0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
}

void loop() {

  if(flag==0){
    for(i=0;i<=1;i+=1){
      starttime = millis();
      
      analogWrite(ledPin, 0);

      // read the value from the photoresistor:
      float sensv = analogRead(A0); 
  
      // define voltage in LDR
      sensv=5*(sensv/1023);

      // define resistance in LDR
      sensr=10000*(5/sensv-1);
      
      // converting to Lux
      LUX=(pow(10,-b/a))*(pow(sensr,1/a));

      Serial.print(LUX);
      Serial.print(" \t");
      Serial.println(i*15);
      
      endtime = millis();
      
      delay(sampint-(endtime-starttime));
    }
    
    for(i=0; i<=50; i+=1){
      starttime = millis();    
      analogWrite(ledPin, light);
    
      // read the value from the photoresistor:
      float sensv = analogRead(A0); 
  
      // define voltage in LDR
      sensv=5*(sensv/1023);
  
      // define resistance in LDR
      sensr=10000*(5/sensv-1);
      
      // converting to Lux
      LUX=(pow(10,-b/a))*(pow(sensr,1/a));

      // print out the value to the serial monitor
      Serial.print("LUX Value: ");
      Serial.print(LUX);
      Serial.print(" \t");
      Serial.println(i*15+30);

      endtime = millis();

      delay(sampint-(endtime-starttime));
    }
  }

  flag=1;
  analogWrite(ledPin, 0);
}
