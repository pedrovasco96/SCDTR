#include "pid3.h"

using namespace std;

PID::PID(){
  Kp=0.1419;
  Ki=606.06;
  T=0.025;
  b = 0.2; //has values between 0 and 1, should be step experimentally to reduce overshoot
  K1=Kp*b;
  K2=Kp*Ki*T/2;
  k0=7;
  e=0;
  p=0;
  i=0;
  u=0;
  final_u=0;
  uff=0;
  LUX_ant=0;
  i_ant=0;
  e_ant=0;
}

PID::~PID(){
}

float PID::control_signal(float ref, float LUX)
{
  // defines error
  e=ref-LUX;


  if(flag==1 && ref_change == 1){
    uff = k0*ref;
    ref_change = 0;
  }
  else{
    // proporcional control
     p=K1*ref-Kp*LUX;
     
    // integral control
    i=i_ant+K2*(e+e_ant);
  
    //Anti-WindUp
    i = constrain(i, -uff-5, uff+5);
  } 


  u=p+i+uff;

    //deadzone
    if  ( u >=  2)
    final_u = u-2;
  else  if  (u  <=  -2)
    final_u = u-2;
  else
    final_u = 0;


/*  Serial.print("i:");
  Serial.print(i);
  Serial.print(" u:");
  Serial.print(u);
  Serial.print(" "); */

  i_ant=i;
  e_ant=e;

  return final_u;
}
