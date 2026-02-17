clc;
clear all;
close all;
%% Voltage Mode Controller Design for LLC Resonant Converter 

%% LLC series resonant converter - Hard ware Specification
Ls = 62e-6;                                         % -->Enter Series inductance or Resonant Inductance
Lm = 268e-6;                                        % -->Enter Magnetizing inductance
Cs = 9.4e-9;                                        % -->Enter Resonant capacitance
Cf = (6*330e-6 + 40e-06);                           % -->Enter Output capcitance
rd = 0.00725 + 0.01;                                % -->Enter Secondary MOSFET and secondary side lump resistance
rc = 15e-3/6;                                       % -->Enter Output filter capacitor ESR 
rs = 15e-3;                                         % -->Enter Series resiatance of resonance inductor - DCR
n = 17;                                             % -->Enter Transformer turns ration - Np/Ns

%% LLC Operating Point
Duty = 0.5;                                         % -->Enter Duty ratio
Vin = 400;                                          % -->Enter Input voltage
Po_desired = 200;                                   % -->Enter Output power
Vo = 12;                                            % -->Enter Output voltage
Io = Po_desired/Vo;                                 % Output current
R = Vo/Io;                                          % Load resistance 
Re = (8*(n^2)/(pi^2))*(rd+R);                       % AC equivalent secondary resistance refer to primary 

%% Finding switching frequency based on Gain-Frequency Curve
Vq = (Vin/2);                                       % Half-bridge voltage

W0 = 1/sqrt(Ls*Cs) ;                                % Resonnat frequency
f0 = W0/(2*pi);                                     % Resonance frequency in Hz

m = (Lm+Ls)/Ls;                                     % LLC inductance ratio
Qfac = sqrt(Ls/Cs)/Re;                              % Q-factor of LLC tank circuit for specific load resistance
GainR = n*(Vo+(rd*Io))/Vq;                          % Requied resonance tank gain

% Finding ratio of switching frequency to resonance frequency
Aa = (Qfac^2)*((m-1)^2);
Bb = (m^2)-(2*(Qfac^2)*((m-1)^2))-(((m-1)^2)/(GainR^2));
Cc = ((Qfac^2)*((m-1)^2))-(2*m);
Dd = 1;
Fxp = [Aa 0 Bb 0 Cc 0 Dd];
Fxr = roots(Fxp);
Fxreal = real(Fxr);
Fx = max(Fxreal);                                   % Ratio of switching frequency to resonance frequency
fs = Fx*f0;                                         % Switching frequency related to load resistance
Ws = 2*pi*fs;                                       % Switching frequency in rad/sec

%% Sampling time and Digital Delay Transfer Function
Ts = 1/200000;                                              % Sampling time
s=tf('s');
Delay_Total = 8.55e-6;                                      % ADC_latency + Computational_delay + GateDriveDelay + ComparatorLatency + DAC Delay
% 1st Order Pade's Approximation for delays
H_outDelay=(1-s*(Delay_Total/2))/(1+s*(Delay_Total/2));     % Transfer function of total delay 

%% Steady-State parameters; Derived in the application note
Ves = (2*Vin/pi);

X = [ rs+Re     Ls*Ws       1           0           -Re         0       ;
      -Ls*Ws    rs+Re       0           1           0           -Re     ;
      1         0           0           -Cs*Ws      0           0       ;
      0         1           Cs*Ws       0           0           0       ;
      Re        0           0           0           -Re         -Lm*Ws  ;
      0         Re          0           0           Lm*Ws       -Re   ] ;
    
U0 = [ Ves ; 0 ; 0 ; 0; 0 ; 0 ];

Y = linsolve(X,U0) ;

Is = Y(1); Ic = Y(2) ; Vs = Y(3) ; Vc = Y(4) ; Ims = Y(5) ; Imc = Y(6) ;

Ipc = Ic-Imc;                                               % Cosine component of transformer primary current
Ips = Is-Ims ;                                              % Sine component of transformer primary current
Ipp = sqrt(Ips^2+Ipc^2);                                    % Transformer primary current
Isp = n*Ipp ;                                               % Transformer secondary current
Vcf = 2*Isp*R/pi;                                           % Filter capacitor steady state voltage

%% Small-Signal coefficients; Derived in the application note
rc1 = R*rc/(R+rc);                                          % in the application note rc1 is represented as r'c

Hip = ((4*n*rc1*Vcf*Ipc^2)/(pi*rc*Ipp^3))+((8*(rd+rc1)*n^2)/(pi^2));
Hic = -(4*n*rc1*Vcf*Ips*Ipc)/(pi*rc*Ipp^3);
Hvcf = (4*n*rc1*Ips)/(pi*rc*Ipp);
Gip = -(4*n*rc1*Vcf*Ips*Ipc)/(pi*rc*Ipp^3);
Gic = ((4*n*rc1*Vcf*Ips^2)/(pi*rc*Ipp^3))+((8*(rd+rc1)*n^2)/(pi^2));
Gvcf = (4*n*rc1*Ipc)/(pi*rc*Ipp);

Kis = (2*n*Ips)/(pi*Ipp);         
Kic = (2*n*Ipc)/(pi*Ipp);  

%% LLC EDF Model system matrices 
A = [ -((Hip+rs)/Ls)        -(Ws+(Hic/Ls))      -1/Ls       0       (Hip/Ls)                (Hic/Ls)                -(Hvcf/Ls)     ;
       (Ws-(Gip/Ls))        -((Gic+rs)/Ls)      0           -1/Ls   (Gip/Ls)                (Gic/Ls)                -(Gvcf/Ls)     ;
       1/Cs                 0                   0           -Ws     0                       0                       0              ;
       0                    1/Cs                Ws          0       0                       0                       0              ;
       (Hip/Lm)             (Hic/Lm)            0           0       -(Hip/Lm)               -(Ws+(Hic/Lm))          (Hvcf/Lm)      ;
       (Gip/Lm)             (Gic/Lm)            0           0       (Ws-(Gip/Lm))           -(Gic/Lm)               (Gvcf/Lm)      ;
       ((Kis*rc1)/(Cf*rc))  ((Kic*rc1)/(Cf*rc)) 0           0       -((Kis*rc1)/(Cf*rc))    -((Kic*rc1)/(Cf*rc))    -rc1/(R*Cf*rc)  ];
   
B = [  -Ic*W0      ;
        Is*W0      ;
       -Vc*W0      ;
        Vs*W0      ;
      -Imc*W0      ;
       Ims*W0      ;
          0      ] ;
      
C = [ (Kis*rc1)    (Kic*rc1)    0    0     -(Kis*rc1)     -(Kic*rc1)    (rc1/rc) ] ;

D = [ 0 ] ;

%% EDF model transfer function
sys = ss(A,B,C,D);
EDF = tf(sys);                                              % LLC converter frequency to output voltage transfer function

P = pole(EDF);                                              % Extraction of poles of LLC converter transfer function                                      
[N,K] = zero(EDF);                                          % Extraction of zeros of LLC converter transfer function 
Gp = minreal(zpk(N,P,K));                                   % LLC converter frequency to output voltage transfer function

figure(1);                                                  % Plant bode plot
P = bodeoptions;
P.Grid = 'on';
P.FreqUnits = 'Hz'; 
P.PhaseWrapping = 'off' ;
bode(Gp,P); hold on;

%% PWM gain
Gpwm = 1;                                                   % Digital PWM gain                         

%% Sensor and ADC gain
R9 = 1.50e3 ;                                               % From the LLC RD BoM
R8 = 4.99e3 ;                                               % From the LLC RD BoM
C = 2000e-12;                                               % From the LLC RD BoM
Kad = 3.33  ;                                               % ADC module Vdd supply
Gp_sensor = R9/(R9+R8);                                     % Output voltage measurment ckt gain (Low pass filter)
Req = R8*R9/(R8+R9);                 
Gpfc = Gp_sensor/(Req*C*s+1);                               % Low-pass filter transfer function
GsenseADC = Gpfc/Kad ;                                      % ADC and Sensor gain       

%% Voltage Mode Compensator design (2P2Z)
% Chosen Gain Crossover frequency
% Compensator one pole is to cancel the effect of Wesr.
% place the compensator two zeros  at complex pole location to cancel the effect of of complex zeros 

fc = 10000;                                                 % Cross-over frequency for compesator design
w = 2*pi*fc ;

[Ngp,Kgp] = zero(Gp*GsenseADC);                             % Extracting zeros of plant and sensor gain          
Pgp = pole(Gp*GsenseADC);                                   % Extracting poles of plant and sensor gain   

Pgp = Pgp( Pgp<=0 );                                        % Extracting left-hand side poles 
Ngp = Ngp( Ngp<=0 );                                        % Extracting left-hand side zeros  

Nreal = real(Ngp);                                          % Extracting real part of poles
Preal = real(Pgp);                                          % Extracting real part of zeros

[Nmax,Index_N] = max(Nreal);                                % Finding index of dominant zero
[Pmax,Index_P] = max(Preal);                                % Finding index of dominant pole

% Compensator poles-zeros selection 
Wesr = Ngp(Index_N);  
Wz1 = Pgp(Index_P);                                         % Compensator zero is placed at one complex pole
Wz2 = Pgp(Index_P+1);                                       % Compensator zero is placed at another complex pole
Wp1 = Wesr ;                                                % Compensator pole is placed at ESR zero of output capacitance

Gcv = (s-Wz1)*(s-Wz2)/(s*(s-Wp1));                          % Compensator transfer function

% Plant Gain calculation at Crossover frequency :
[Gv_fc phi_fc] = bode(Gp*GsenseADC*Gpwm,w);                 % Plant gain and phase at cross-over frequency

%Compensator gain calculation at cross-over frequency 
[Gcv_fc phic_fc] =  bode(Gcv,w);                            % Controller gain and phase at cross-over frequency
 
% At Crossover frequency K*Gc*Gp = 1 :
Kc = 1/(Gv_fc*Gcv_fc);                                      % Compesator intergrator gain calculation

% Overall compensator transfer function
Gcv = Kc*Gcv;                                               % Compensator transfer function

% Discretization of compensator transfer function using tustin or bi-linear transfromation
Gcv_d = c2d(Gcv,Ts,'tustin');

% Current-Loop-gain calculation
Gvol = minreal(Gp*Gcv*GsenseADC*H_outDelay*Gpwm);

figure(2);                                                  % Loop-gain bode plot
P = bodeoptions;
P.Grid = 'on';
P.FreqUnits = 'Hz'; 
P.PhaseWrapping = 'off';
bode(-Gvol, P); hold on;                                    % Negative sign indicates negative LLC control to output change 
margin(-Gvol);                                              % (switching frequency increase-output voltage decrease)  
            

%% Voltage to Frequency factor for simulink model
Vab = 400/2       ;        %nominal input voltage on primary Tx  
ws = 2*pi*fs      ;        % Resonant frequency
Xr = ws*Ls        ;        % Impedance of series Inductance
Xm = ws*Lm        ;        % Impedance of magnetizing Inductance
XC = 1/(ws*Cs)    ;        % Impedance of series capacitance
p = (Xr - XC + Xm)/Xm;     % Overall impedance
fr = 208e3        ;        % Resonant frequency
frequencytovoltagefactor  =  (2*pi^2*fr^3*Cs*Lm)/Vab/p^2 ;


