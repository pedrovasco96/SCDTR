clear all
clc
 
arduino=serial('COM3','BaudRate',9600);
 
fopen(arduino);
 
x=linspace(1,250,250);
   
for i=1:length(x)
	y(i)=fscanf(arduino,'%d');
end
	
fclose(arduino);
disp('making plot..')
plot(x,y);