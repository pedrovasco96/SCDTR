clear all
clc
 
a = arduino('COM3','uno');
light=0;
x = 0:10:250;

for i=1:26
  Ra=14000;
  sensv = readVoltage(a, 'A0');
  
  pause(0.1);

  %sensv=5*(sensv/1023);

  sensr=10000*(5/sensv-1);

  LUX(i)=(Ra^(10/7))*10*(sensr^(-10/7));
  
  vet(i)=light;
  writePWMVoltage(a,9,light);
  
  light=light+50/255;
  
end
  
writePWMVoltage(a,9,0);

figure();
plot(x,LUX);
title('Stair input');
ylabel('LUX');
xlabel('PWM Values');