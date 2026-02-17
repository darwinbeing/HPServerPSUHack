clear all

L = 4.7e-6            %filter inductance L4
C = 360e-6         %filter capacitance after L4
ESR = 0.014         %cap ESR
R = 0.1           %series DCR + track  + etc (experimentally determined by open loop droop characteristics) 
Ts = 1/50000       %sampling time shouldnt change significantly from no load to full load

                              %3 roots of the control system chosen
turnratio = 16.67;
VN = 14.3;            % base voltage refered to primary
Vinmin = 350/2;           %nominal input voltage 
Vin = 400/2;
Vnominal = 12;                    
                   
f1 = -1400*2*pi;
f2 = -900*2*pi;
%f3 = -600*2*pi

                    %pole to cancel the cap ESR 0
fp =  5000*2*pi ; %0.9/(ESR*C)

% A = [f1^2,f1,1 ;  f2^2,f2,1 ; f3^2,f3,1];
% B = [-L*C*f1^3;-L*C*f2^3;-L*C*f3^3];

A = [f1,1;f2,1]
% B = [(-R*C*f1^2 -L*C*f1^3);( -R*C*f1^2 - L*C*f2^3)]


 B = [-R*C*f1^2 -L*C*f1^3; -R*C*f2^2 -L*C*f1^3];
Y = inv(A)*B;
                    %calculate PID coeffs    
%Kd = Y(1)- 1.0*R*C
Kp = Y(1)
Ki = Y(2)



wr = 180000*2*pi;
Ls = 67e-6
LM = 270e-6
Cr = 9.4e-9
Tnominal = 1/180000; %period nominal


XL = wr*Ls;
XM = wr*LM;
XC = 1/(wr*Cr);

p = (XL - XC + XM)/XM;

voltagetotimefactor = (p^2*wr*LM*Cr)*pi;

multiplier  = voltagetotimefactor*turnratio/Vinmin;

modifier = Vinmin/Vin
%modifier = 1

% period_toapply = (pid_o/p + Vo - nominalVtoapply)*multiplier*modifier + period_nominal

%normalizing
TN = 1/(140e3);       %minimum freq period 130KHz
scalefactor = VN*multiplier/TN;
prescaler = 1/2;

%scaled fractional value output to be used in code . To be converted to Q15 
K1 = Kp*scalefactor    *prescaler;
K2 = Ki*Ts*scalefactor *prescaler;
%K3 = Kd/Ts*scalefactor *prescaler;
K4 = scalefactor *prescaler;

%treat output as duty. Finally post multiply with 7000 counts
%corresponding to base 7e-6 period 

%limit frequencies to 40000 Hz
flowpass  = 5000*2*pi  
alpha =exp(-flowpass*Ts)
beta  = 1 - alpha 

Kd =0

 a = [Kd Kp Ki];                %pid controller
 b = [1 0]
 
 c = [ESR*C 1]                  %effective plant after decoupling    
 d = [L*C (R*C + 0.1*ESR*C) 0.1]
 
 e = [1]                        %filter
 f = [1/fp 1]
 

 g = [L*C (R*C+Kd) Kp+0.1 Ki]
 h = [1 0]
 
 l = [L R]
 m = [C*ESR 1]
 
 
 Units = bodeoptions; % Set phase visiblity to off and frequency units to Hz in options
 Units.FreqUnits = 'Hz'; % Create plot with the options specified by P

  
 figure(1)              %bode plots for loop gain A*B for phase and gain margin with 10 % parameter variation
 X = tf(e,f)*tf(a,b)*tf(c,d)
 grid on
 bode(X, Units)
 
        
 Y = c2d(X,Ts,'zoh');
 hold on 
 bode(Y, Units)
 grid on
                        % closed loop bode plot
 figure(2)
 bode((X/(1+X)), Units);
 hold on
 grid on 
 bode((Y/(1+Y)), Units);
 
 figure(3)
 Z = (tf(a,b)*tf(e,f) +  tf(d,c))/tf(l,1)
 grid on
 bode(Z, Units)
 
w = 2*pi*linspace(150000,220000,1000);
for n = 1:length(w)
    
   Xm = w(n)*LM ;
   Xl = w(n)*Ls;
   Xc = -1/w(n)/Cr;
    
   H(n) = Xm/(Xc+Xl+Xm);
   
end

figure(4)
grid on
plot(2*pi./w,H)
 

R = 0.1
figure(5)

correctnum = [Kd Kp Ki];
correctden = [L*C R*C 0 0];
correctloop = tf(correctnum,correctden);
grid on
bode(correctloop, Units);


