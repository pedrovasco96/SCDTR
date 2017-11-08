const int ledPin = 9;
int light = 128;
int i;
int flag=0;
float Ra=12650;
float LUX;
float sensr;
float a = -0.7;
float b = 4.802;
float p=pow(10,-b/a);
float start_time = 0;
float end_time = 0;
float time_ = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
}

void loop() {
  if(flag==0){
    
    while(micros()<20000){
      
      analogWrite(ledPin, 0);

      // read the value from the photoresistor:
      float sensv = analogRead(A0); 
  
      // define voltage in LDR
      sensv=5*(sensv/1023);

      // define resistance in LDR
      sensr=10000*(5/sensv-1);
      
      // converting to Lux
      LUX=p*(pow(sensr,1/a));

      //Serial.print(sensv);
      Serial.print(LUX);
      Serial.print(" \t");
      Serial.println(float(micros())/1000);
    }

    //Serial.println("    ");
    while(micros()<20000+480000){

      start_time = micros();
   
      analogWrite(ledPin, light);
    
      // read the value from the photoresistor:
      float sensv = analogRead(A0); 
  
      // define voltage in LDR
      sensv=5*(sensv/1023);
  
      // define resistance in LDR
      sensr=10000*(5/sensv-1);
      
      // converting to Lux
      LUX=p*(pow(sensr,1/a));

      end_time = micros();

      time_ = end_time - start_time;

      delay(70 - time_/1000);

      // print out the value to the serial monitor
      //Serial.print("V Value: ");
      //Serial.print(sensv);
      Serial.print(LUX);
      Serial.print(" \t");
      Serial.println(float(micros())/1000);
    }
  }
  
  flag=1;
  
  analogWrite(ledPin, 0);
}
