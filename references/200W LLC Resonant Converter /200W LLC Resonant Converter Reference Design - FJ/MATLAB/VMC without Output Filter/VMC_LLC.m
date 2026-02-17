clc
clear all
close all
% Voltage Mode Controller Design for LLC Resonant Converter Reference Design
%% LLC series resonant converter - Hardware Specification

Ls = 62e-6    ;       % Series inductance or Resonant Inductance
Lm = 268e-6   ;       % Magnetizing inductance
Cs = 9.4e-9    ;      % Resonant Capacitance
Vin = 400      ;      % Input Voltage
Cf = 2000e-6  ;       % Output Capcitance
R = 144/200   ;       % Output load resiatance
rc = 15e-3  ;         % Output Filter Capacitor ESR 
rs = 14e-3   ;        % Series resiatance of Inductor - DCR
n = 17        ;       % Transformer turns ration - Np/Ns
fs = 205e3    ;       % Switching frequency
D = 0.5       ;       % Duty ratio
Ts = 1/(50000)  ;     % Sampling time
rc1 = R*rc/(R+rc);    % in the application note rc1 is represented as r'c

%% Steady-State parameters; Derived in the application note

Ves = 2*Vin/pi ;

Ws = 2*pi*fs ;

Re = 8*n^2*(1-rc1/R)*(rc/rc1)*R/pi^2 ;

W0 = 1/sqrt(Ls*Cs) ; % Resonnat Frequency
 
f0 = W0/(2*pi);


X = [ rs+Re     Ls*Ws     1       0      -Re           0       ;
       -Ls*Ws    rs+Re     0       1        0           -Re     ;
        1           0      0    -Cs*Ws      0           0       ;
        0           1     Cs*Ws    0        0           0       ;
        Re          0      0       0      -Re         -Lm*Ws    ;
        0           Re     0       0      Lm*Ws         -Re   ] ;
    
U0 = [ Ves ; 0 ; 0 ; 0; 0 ; 0 ];

Y = linsolve(X,U0) ;

Is = Y(1); Ic = Y(2) ; Vs = Y(3) ; Vc = Y(4) ; Ims = Y(5) ; Imc = Y(6) ;

Ipc =Ic-Imc  ; % Cosine Component of Transformer Primary Current

Ips = Is-Ims ; % Sine Component of Transformer Primary Current

Ipp = sqrt(Ips^2+Ipc^2);    % Transformer Primary Current

Isp = n*Ipp ;   % Transformer Secondary Current

Vcf = pi/4*Isp*Re/n^2 ; % Filter Capacitor Steady State Voltage

V0 = 2/pi*rc1*Ipp+ (rc1/rc)*Vcf; % Steady State Output Voltage

I0 = V0/R ; % Output Current

%% Small-Signal coefficients


Hip = 4*n*Vcf*Ipc^2/(pi*Ipp^3);

Hic = -4*n*Vcf*Ips*Ipc/(pi*Ipp^3);

Hvcf = 4*n*Ips/(pi*Ipp);

Gip = Hic ;

Gic = 4*n*Vcf*Ips^2/(pi*Ipp^3);

Gvcf = 4*n*Ipc/(pi*Ipp);

Kis = 2*n*Ips/(pi*Ipp);

Kic = 2*n*Ipc/(pi*Ipp);

A = [ -(Hip+rs)/Ls       -Ws-Hic/Ls       -1/Ls           0              Hip/Ls           Hic/Ls         -Hvcf/Ls       ;
        Ws-Gip/Ls        -(Gic+rs)/Ls       0           -1/Ls            Gip/Ls           Gic/Ls         -Gvcf/Ls       ;
        1/Cs                 0              0           -Ws                0                0               0           ;
          0                1/Cs            Ws            0                 0                0               0           ;
        Hip/Lm            Hic/Lm            0            0               -Hip/Lm          -Ws-Hic/Lm      Hvcf/Lm       ;
        Gip/Lm            Gic/Lm            0            0              Ws-Gip/Lm         -Gic/Lm         Gvcf/Lm       ;
        Kis*rc1/(Cf*rc)   Kic*rc1/(Cf*rc)   0            0            -Kis*rc1/(Cf*rc)  -Kic*rc1/(Cf*rc) -rc1/(Cf*rc)  ];
  
B = [  -Ic*W0      ;
        Is*W0      ;
       -Vc*W0      ;
        Vs*W0      ;
      -Imc*W0      ;
       Ims*W0      ;
          0      ] ;

C= [ Kis*rc1    Kic*rc1    0    0     -Kis*rc1     -Kic*rc1    rc1/(rc) ] ;

D = [ 0 ] ;

%% State-Space model to transfer function
sys = ss(A,B,C,D);
H =tf(sys);
Gp = H;     % Plant Transfer function

%% Extracting the transfer function by neglecing the poles and zeros beyond the  switching frequency

P = pole(Gp);
[N,K] = zero(Gp);
K1 = 1 ;
j = 1:1:7 ;
an = size(j);  
jmax = an(2);
for k=1:jmax
    if ((abs(P(k))< Ws))
        P(k) =   P(k)    ;
    else
        K1 = K1*(abs(P(k)));
        P(k) = 1 ;
    end;
    
end;
K2 = 1 ;
j = 1:1:5 ;
an = size(j);  
jmax = an(2);
for k=1:jmax
    if ((abs(N(k))< Ws))
        N(k) =   N(k)    ;
    else
        K2 = K2*(abs(N(k)));
        N(k) = 1 ;
        
    end;
end;

% DC (Low-Frequency) Gain calculation

K = K*K2/K1 ;  
Gp = minreal(zpk(N,P,K)); 
figure(1)
P = bodeoptions;
P.Grid = 'on';
P.FreqUnits = 'Hz'; 
P.PhaseWrapping = 'off' ;
bode(Gp);
% 
%%  feedback or sensor circuit gain
% Cross-over frequency of output sensor is 4.228e5 which is higher than 200KHz
s = tf('s');
R9 = 1.50e3 ;   % from the LLC RD BoM
R8 = 4.99e3 ;   % from the LLC RD BoM
C = 2000e-12;   % from the LLC RD BoM
Kad = 3.3  ;    % ADC module Vdd Supply
Gp_sensor = R9/(R9+R8);   % Output Voltage measurment ckt gain (Low Pass filter)
Req = R8*R9/(R8+R9);
Gpfc = Gp_sensor/(Req*C*s+1);  % Low Pass Filter Transfer Function
Gfp = Gp*Gpfc ;                
Pfp = pole(Gfp);        % Poles of Gfp
[Zfp Kfp] = zero(Gfp);  % Zeros and Gain of Gfp
% 
%% Compensator design (3P2Z in cotinuous domain equivalent to 3P3Z in discrete domain); 
% Chosen Gain Crossover frequency is 2000Hz
% Compensator one pole is to cancel the effect of Wesr.
% Compensator anohter pole is placed nearer to the switching frequency to get the attenuation at switching frequency.
% place the compensator two zeros  at complex pole location to cancel the effect of of complex zeros and get the proper shape of open-loop transfer function
% Compensator design is discussed in deatil in the application note

fc=2000;
w = 2*pi*fc ;

% Compensator poles-zeros selection

Wp = abs(Zfp(1));               % compensator pole is placed at ESR zero of output capacitance
Wz1 = -Pfp(1) ;                 % compensator zero is placed at one complex pole (s1 in app.note)
Wz2 = -Pfp(2) ;                 % compensator zero is placed at another complex pole (s2 in app.note)
Wpc = 0.8*Ws ;                  % compensator pole placed near the switching frequency

s = tf('s') ;
Gc = (s/Wz1+1)*(s/Wz2+1)/(s*(s/Wp+1)*(s/Wpc+1)) ; % compensator transfer function


% Plant Gain calculation at Crossover frequency :
[Gp_fc phi_fc] = bode(Gfp,w);


%Compensator gain calculation at cross-over frequency 
[Gc_fc phic_fc] =  bode(Gc,w) ;

% At Crossover frequency K*Gc*Gp = 1 :
Kc = 1/(Gp_fc*Gc_fc);

% Overall compensator transfer function
Gc = Kc*Gc ;

% Discretization of compensator transfer function using tustin or bi-linear transfromation

Gc_d = c2d(Gc,Ts,'tustin') ;   

% Loop-gain calculation
Gl = Gfp*Gc ;  

figure(2)
bode(Gl)

%% Voltage to Frequency factor for simulink model
Vab = 400/2       ;        %nominal input voltage on primary Tx  
ws = 2*pi*fs      ;        % Resonant frequency
Xr = ws*Ls        ;        % Impedance of series Inductance
Xm = ws*Lm        ;        % Impedance of magnetizing Inductance
XC = 1/(ws*Cs)    ;        % Impedance of series capacitance
p = (Xr - XC + Xm)/Xm;     % Overall impedance
fr = 208e3        ;        % Resonant frequency
frequencytovoltagefactor  =  (2*pi^2*fr^3*Cs*Lm)/Vab/p^2 ;

%%










      