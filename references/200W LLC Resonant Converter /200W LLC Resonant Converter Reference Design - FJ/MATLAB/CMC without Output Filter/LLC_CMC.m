clc
clear all
close all
% Current Mode Controller Design for LLC Resonant Converter Reference Design
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

Js=Ips/Ipp ;

Jc=Ipc/Ipp ;

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

C= [ Kis*rc1    Kic*rc1    0    0     -Kis*rc1     -Kic*rc1    rc1/(rc)       ;
       Js         Jc       0    0        0             0        0            ];

D = [ 0  ;
      0  ];

%% State-Space model to transfer function
sys = ss(A,B,C,D);
H =tf(sys);  % Plant Transfer functions
%% Extracting the transfer function by neglecing the poles and zeros beyond
%% the  switching frequency
i = 1:1:2 ;
an = size(i);
imax = an(2);
 for p = 1:1:imax     
    P = pole(H(p));
    [N,K] = zero(H(p));
    K1 = 1 ;
    c =0 ;
    j = 1:1:size(P) ;
    an = size(j);  
    jmax = an(2);
    for k=1:1:jmax
        if ((abs(P(k))< Ws))
            c = c+1 ;
            P1(c) =   P(k)    ;            
        else            
            K1 = K1*(abs(P(k)));
         end;           
    end;
    
    K2 = 1 ;
    j = 1:1:size(N) ;
    c = 0;
    an = size(j);  
    jmax = an(2);
    for k=1:jmax
        if ((abs(N(k))< Ws))
            c= c+1 ;
            N1(c) =   N(k)    ;            
        else
            K2 = K2*(abs(N(k)));      
        end;                      
    end;    
   
    % DC (Low-Frequency) Gain calculation    
    K = K*K2/K1 ;  
    G(p) = minreal(zpk(N1,P1,abs(K)));
    if (p == 1)
        Gvw = G(p);
    elseif (p == 2) 
        Giw  = G(p);
    end; 
    
    N1 = 0 ;
    D1 = 0 ;
 end

figure(1)
P = bodeoptions;
P.Grid = 'on';
P.FreqUnits = 'Hz'; 
P.PhaseWrapping = 'off' ;
bode(Giw,P);
% 
%%  Current sensor gain
% It is Low pass Filter therefore it's magnitude is not considered and frequency response effect is neglected while designing compensator
s = tf('s');
Rfc=10;
Cfc = 12.5e-6 ;
Gfc =1/(Rfc*Cfc*s+1);

%% Current Loop Compensator design (2P2Z)
% Chosen Gain Crossover frequency is 5000Hz
% Compensator one pole is to cancel the effect of Wesr.
% place the compensator two zeros  at complex pole location to cancel the effect of of complex zeros and get the proper shape of open-loop transfer 
% function

fc=5000;
w = 2*pi*fc ;

[N,K]=zero(Giw);

D = pole(Giw);
j = length(D);
k = length(N);

% Compensator poles-zeros selection 
Wesr = -N(k);  
Wz1 = D(j-1) ;                                                              % compensator zero is placed at one complex pole
Wz2 = D(j) ;                                                                % compensator zero is placed at another complex pole
Wp1 = Wesr ;                                                                % compensator pole is placed at ESR zero of output capacitance

Gci = (s-Wz1)*(s-Wz2)/(s*(s+Wp1)) ; % compensator transfer function

% Plant Gain calculation at Crossover frequency :
[Giw_fc phi_fc] = bode(Giw,w);

%Compensator gain calculation at cross-over frequency 
[Gci_fc phic_fc] =  bode(Gci,w) ;
 
% At Crossover frequency K*Gc*Gp = 1 :
Kc = 1/(Giw_fc*Gci_fc);

% Overall compensator transfer function
Gci = Kc*Gci ;

% Discretization of compensator transfer function using tustin or bi-linear transfromation
Gci_d = c2d(Gci,Ts,'tustin') ;

% Current-Loop-gain calculation
Gil = minreal(Giw*Gci) ;
figure(2)
margin(Gil)
% 
%Closed-Loop Transfer function (Current Loop)
Gcl = minreal(feedback(Gil,1));

%%  feedback or sensor circuit gain
% Corner frequency of output sensor is 4.228e5 which is higher than 200KHz
s = tf('s');
R9 = 1.50e3 ;   % from the LLC RD BoM
R8 = 4.99e3 ;   % from the LLC RD BoM
Cfv = 2000e-12; % from the LLC RD BoM
Kadc = 3.3  ;   % ADC module Vdd Supply
Gfv = 1/Kadc;   % Output Voltage measurment ckt gain (Low Pass filter)
Rfv = R8*R9/(R8+R9);
Gfv = Gfv/(Rfv*Cfv*s+1);  % Low Pass Filter Transfer Function

%% Voltage-to-Current Transfer function
%   Choosen Cross-over frquecy is 2800Hz (PI compensator)
%   PI compensator Zero is placed to compensate the phase shift to obtain enough phase margins.
%   Integrator is placed to enhance the gain at low frequencies component.
 
fcv=3000;
w_cv = 2*pi*fcv ;
Gvi=Gvw/Giw ;                                                               % Voltate-to-Inductor current Transfer Function

Gvl = Gcl*Gvi*Gfv ;
figure(3)
margin(Gvl)

Wv=27500;                                                                 % 

Gcv =(s+Wv)/s ;                                                           % compensator transfer function 

% Compensator gain calculation at cross-over frequency 
[Gcv_fcv phic_fcv] =  bode(Gcv,w_cv) ;
 
% Plant Gain calculation at Crossover frequency :
[Gvl_fcv phi_fcv] = bode(Gvl,w_cv);
  
% At Crossover frequency K*Gc*Gp*G_sensor = 1 :
Kc = 1/(Gvl_fcv*Gcv_fcv);
 
% Overall compensator transfer function
Gcv = Kc*Gcv ;

% Controller gains calculation
Kp=Kc;
Ki=Wv*2*Ts;
Gvs = Gcv*Gvl;
figure(4)
bode(Gvs);

%% Voltage to Frequency factor for simulink model

Vab = 400/2       ;        %nominal input voltage on primary Tx  
ws = 2*pi*fs      ;        % Resonant frequency
Xr = ws*Ls        ;        % Impedance of series Inductance
Xm = ws*Lm        ;        % Impedance of magnetizing Inductance
XC = 1/(ws*Cs)    ;        % Impedance of series capacitance
p = (Xr - XC + Xm)/Xm;     % Overall impedance
fr = 208e3        ;        % Resonant frequency
frequencytocurrentfactor  =  (2*pi^2*fr^3*Cs*Lm)/Vab/p^2 ;

%%



      