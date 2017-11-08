clear all
close all
clc
 
a = arduino;
light=0;
k=0;
xx= 0:0.5:124.5;
lightv = zeros(1,250);
sensvv = zeros(1,250);

for i=1:25
  Ra=14000;
  
  writePWMVoltage(a,'D9',light);
  
  for j=1:10
      k = k+1;
      sensvv(k) = readVoltage(a, 'A0');
      lightv(k)=light;
      pause(0.05);
  end
  
  %m = mean(sensvv);
  %sensv(i) = m;

  %sensv=5*(sensv/1023);

  %sensr=10000*(5/sensv-1);

  %LUX(i)=(Ra^(10/7))*10*(sensr^(-10/7));
  
  light=light+50/255;
  
end
  
writePWMVoltage(a,'D9',0);

figure();
plot(xx,sensvv);
title('Stair input Response');
ylabel('Voltage');
xlabel('Segundos (ms)');
figure();
plot(xx,lightv);
title('Stair input');
ylabel('Voltage');
xlabel('Segundos (ms)');