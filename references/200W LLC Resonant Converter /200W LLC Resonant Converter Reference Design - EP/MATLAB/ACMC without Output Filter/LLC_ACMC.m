clc;
clear all;
close all;
%% Avergae Current Mode Controller Design for LLC Resonant Converter

%% LLC series resonant converter - Hard ware Specification
Ls = 62e-6;                                         % -->Enter Series inductance or Resonant Inductance
Lm = 268e-6;                                        % -->Enter Magnetizing inductance
Cs = 9.4e-9;                                        % -->Enter Resonant capacitance
Cf = (5*330e-6 + 40e-06);                           % -->Enter Output capcitance
rd = 0.00725+0.008;                                 % -->Enter Secondary MOSFET and secondary side lump resistance
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

%% Sensor Circuit Parameters
% Current sensor parameters
Rfc = 10;                                           % -->Enter Low-pass filter R
Cfc = 2e-6 ;                                        % -->Enter Low-pass filter C
Gct = 1/50;                                         % -->Enter CT gain
Gopamp = (1+(576/300));                             % -->Enter Opamp gain;           
Gpd = 20;                                           % -->Enter CT burdon resistor;
Gavg = 2/pi;
% Voltage sensor parameters
R9 = 1.50e3 ;                                       % -->Enter voltage divider resistor-1
R8 = 5.02e3 ;                                       % -->Enter voltage divider resistor-2
Cfv = 2000e-12;                                     % -->Enter LP filter capacitor

%% ADC and PWM module gains
Gpwm = 1;                                           % -->Enter Digital PWM gain  
Kadc = 3.33  ;                                      % -->Enter ADC module Vdd Supply

%% Enter controller design parameters for desired controller 
fci = 5540;                                         % -->Enter cross-over frequency for inner loop controller 
fcv = 1130;                                         % -->Enter cross-over frequency for outer loop controller

TsC = 1/200000;                                     % -->Enter sampling time for inner current controller discretization
TsV = 1/50000;                                      % -->Enter sampling time for outer voltage controller discretization

Delay_Total = 3.558e-06;                            % -->Enter ADC_latency + Computational_delay 

T_zoh1 = TsV;                                       % -->Enter Total hold time for outer voltage reference due to slove sampling of outer loop 
T_zoh2 = TsC;                                       % -->Enter Total hold time for inner current reference due to slove sampling of inner loop 

% Inner loop controller parameters need to be adjusted for desired PM (See Plot-2 to adjust PM of inner loop-gain)
z1shift = 0.6661;                                   % -->Enter inner controller zero shifting
z2shift = 0.3737;                                   % -->Enter inner controller zero shifting
p1shift = 0.06132;                                  % -->Enter inner controller pole shifting            

% Outer loop controller parameter need to be adjusted for desired PM (see Plot-3 to adjust PM of outer loop-gain)  
Wv = 2500;                                          % -->Enter outer controller zero placement

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
s=tf('s');

% 1st Order Pade's Approximation for delays
H_outDelay=(1-s*(Delay_Total/2))/(1+s*(Delay_Total/2));         % Transfer function of total delay 

% Zero order hold due to out loop lower sampling rate
exp_Ts_coeff = s*T_zoh1/2;
exp_Ts1 = (1-exp_Ts_coeff+exp_Ts_coeff^2-exp_Ts_coeff^3)/(1+exp_Ts_coeff+exp_Ts_coeff^2+exp_Ts_coeff^3); % Higher-order hold equation 

H_zoh1 = minreal((1 - exp_Ts1)/(s*T_zoh1));                     % Trasnfer function of ZOH

exp_Ts_coeff = s*T_zoh2/2;
exp_Ts2 = (1-exp_Ts_coeff+exp_Ts_coeff^2-exp_Ts_coeff^3)/(1+exp_Ts_coeff+exp_Ts_coeff^2+exp_Ts_coeff^3); % Higher-order hold equation 

H_zoh2 = minreal((1 - exp_Ts2)/(s*T_zoh2));                     % Trasnfer function of ZOH

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

Ipc = Ic-Imc;                                       % Cosine component of transformer primary current
Ips = Is-Ims ;                                      % Sine component of transformer primary current
Ipp = sqrt(Ips^2+Ipc^2);                            % Transformer primary current
Ir = sqrt(Is^2+Ic^2);
Im = sqrt(Ims^2+Imc^2);
Isp = n*Ipp ;                                       % Transformer secondary current
Vcf = 2*Isp*R/pi;                                   % Filter capacitor steady state voltage

%% Small-Signal coefficients; Derived in the application note
rc1 = R*rc/(R+rc);                                  % rc' of application note is represented as rc1

Hip = ((4*n*rc1*Vcf*Ipc^2)/(pi*rc*Ipp^3))+((8*(rd+rc1)*n^2)/(pi^2));
Hic = -(4*n*rc1*Vcf*Ips*Ipc)/(pi*rc*Ipp^3);
Hvcf = (4*n*rc1*Ips)/(pi*rc*Ipp);
Gip = -(4*n*rc1*Vcf*Ips*Ipc)/(pi*rc*Ipp^3);
Gic = ((4*n*rc1*Vcf*Ips^2)/(pi*rc*Ipp^3))+((8*(rd+rc1)*n^2)/(pi^2));
Gvcf = (4*n*rc1*Ipc)/(pi*rc*Ipp);

Kis = (2*n*Ips)/(pi*Ipp);         
Kic = (2*n*Ipc)/(pi*Ipp);  

Js= (2/pi)*Is/sqrt(Is^2+Ic^2);
Jc= (2/pi)*Ic/sqrt(Is^2+Ic^2);

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
      
C= [ Kis*rc1    Kic*rc1    0    0     -Kis*rc1     -Kic*rc1    rc1/(rc)       ;
       Js         Jc       0    0        0             0        0            ];

D = [ 0  ;
      0  ];

%% EDF model transfer function
sys = ss(A,B,C,D);
EDF = tf(sys);                                              

Gvw = EDF(1);                                       % LLC converter frequency to output voltage transfer function
Giw = EDF(2);                                       % LLC converter frequency to tank current transfer function

P = pole(Giw);                                      % Extraction of poles of LLC converter transfer function                                      
[N,K] = zero(Giw);                                  % Extraction of zeros of LLC converter transfer function 
Giw = minreal(zpk(N,P,K));                          % LLC converter frequency to output voltage transfer function

figure(1);                                          % Plant bode plot
P = bodeoptions;
P.Grid = 'on';
P.FreqUnits = 'Hz'; 
P.PhaseWrapping = 'off' ;
bode(Giw,P); hold on;

%%  Current sensor gain             
Gif =1/(Rfc*Cfc*s+1);                               % Low pass-filter gain
Gisense = Gif*Gct*Gopamp*Gpd/Kadc;                  % Total current sensor gain

%% Current Loop Compensator design (2P2Z)
% Chosen Gain Crossover frequency
% Compensator one pole is to cancel the effect of Wesr.
% place the compensator two zeros  at complex pole location to cancel the effect of of complex zeros 
% and get the proper shape of open-loop transfer function
wc = 2*pi*fci ;

%poles of plant and sensor gain
[Niw,Kiw] = zero(-Giw*Gisense);                     % Extracting zeros of plant and sensor gain          
Piw = pole(-Giw*Gisense);                           % Extracting poles of plant and sensor gain   

Piw = Piw( Piw<=0 );                                % Extracting left-hand side poles 
Niw = Niw( Niw<=0 );                                % Extracting left-hand side zeros  

Nreal = real(Niw);                                  % Extracting real part of poles
Preal = real(Piw);                                  % Extracting real part of zeros

[Nmax,Index_N] = max(Nreal);                        % Finding index of dominant zero
[Pmax,Index_P] = max(Preal);                        % Finding index of dominant pole

% Compensator poles-zeros selection 
Wesr = Nreal(Index_N);  

Wz1 = Preal(Index_P)/z1shift;                       % Compensator zero is placed at one complex pole
Wz2 = Preal(Index_P)/z2shift;                       % Compensator zero is placed at another complex pole
Wp1 = Wesr/p1shift ;                                % Compensator pole is placed at ESR zero of output capacitance

Gci = (s-Wz1)*(s-Wz2)/(s*(s-Wp1));                  % Compensator transfer function

% Plant Gain calculation at Crossover frequency :
[Giw_fc Phiw_fc] = bode(-Giw*Gisense,wc);           % Plant gain and phase at cross-over frequency

%Compensator gain calculation at cross-over frequency 
[Gci_fc Phci_fc] =  bode(Gci,wc);                   % Controller gain and phase at cross-over frequency
 
% At Crossover frequency K*Gc*Gp = 1
Kc = 1/(Giw_fc*Gci_fc);                             % Compesator intergrator gain calculation

% Overall compensator transfer function
Gci = Kc*Gci ;                                      % Compensator transfer function

% Discretization of compensator transfer function using tustin or bi-linear transfromation
Gci_d = c2d(Gci,TsC,'tustin') ;

% Current-Loop-gain calculation
Giol = minreal(-Giw*Gisense*Gci) ;                  % Open-loop gain 

figure(2);                                          % Loop-gain bode plot
P = bodeoptions;
P.Grid = 'on';
P.FreqUnits = 'Hz'; 
P.PhaseWrapping = 'off';
bode(Giol, P); hold on;
margin(Giol);    

%% Closed-Loop Transfer function (Current Loop)
Gcl = (-Gci*Giw*H_outDelay*Gpwm)/(1-(Gisense*Gci*H_outDelay*H_zoh2*Giw*Gpwm));            % with system delay and hold
% Gcl = (-Gci*Giw*Gpwm)/(1-(Gisense*Gci*Giw*Gpwm));                                       % without system dealy and hold


%%  feedback or sensor circuit gain
Gp_sensor = R9/(R9+R8);                             % Output Voltage measurment ckt gain (Low Pass filter)
Rfv = R8*R9/(R8+R9);                 
Gpfc = Gp_sensor/(Rfv*Cfv*s+1);                     % Low Pass Filter Transfer Function
Gfv = Gpfc/Kadc ;                                   % ADC and Sensor Gain     
 
%% Voltage-to-Current Transfer function
Giv = -Gvw/-Giw ;                                   % Voltate-to-Inductor current Transfer Function

%% Current Loop Compensator design (PI) 
w_cv = 2*pi*fcv;                                    % Cross-over frequency in rad/sec

Gvl = Gcl*Giv*Gfv;                                  % Multiplication of inner closed-loop gain, outer plant gain and voltage sensor gain
                  
Gcv =(s+Wv)/s ;                                     % compensator transfer function 

% Compensator gain calculation at cross-over frequency 
[Gcv_fcv phic_fcv] =  bode(Gcv,w_cv) ;
 
% Plant Gain calculation at Crossover frequency :
[Gvl_fcv phi_fcv] = bode(Gvl,w_cv);
  
% At Crossover frequency K*Gc*Gp*G_sensor = 1 :
Kc = 1/(Gvl_fcv*Gcv_fcv);
 
% Overall compensator transfer function
Gcv = Kc*Gcv ;                                      % Continuous time outer loop controller

Gcv_d = c2d(Gcv,TsV,'tustin') ;                     % Outer loop controller discrete equation

Gvs = Gcv*Gvl*H_zoh1*H_outDelay;                    % Outer loop-gain 

figure(3);                                          % Loop-gain bode plot
P = bodeoptions;
P.Grid = 'on';
P.FreqUnits = 'Hz'; 
P.PhaseWrapping = 'off' ;
bode(Gvs, P); hold on;
margin(Gvs);

%% Voltage to Frequency factor for simulink model
Vab = 400/2       ;        %nominal input voltage on primary Tx  
ws = 2*pi*fs      ;        % Resonant frequency
Xr = ws*Ls        ;        % Impedance of series Inductance
Xm = ws*Lm        ;        % Impedance of magnetizing Inductance
XC = 1/(ws*Cs)    ;        % Impedance of series capacitance
p = (Xr - XC + Xm)/Xm;     % Overall impedance
fr = 208e3        ;        % Resonant frequency
frequencytovoltagefactor  =  (2*pi^2*fr^3*Cs*Lm)/Vab/p^2 ;