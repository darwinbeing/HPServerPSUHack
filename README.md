
![alt text][image19]

---

## HP HSTNS-PL11/PL30/PD44/PD52

HP HSTNS-PL11/PL30/PD44/PD52 1200 W hot-plug power supply hack for vehicle flashing and programming.

The HPE PSU is a powerful and versatile power supply unit that can be modified to suit a wide range of needs. With some basic electronics knowledge and proper safety precautions, it is possible to make this PSU even more useful for your specific applications.
Modified HPE PSUs are suitable for RC battery charging, radio power supply, and vehicle flashing/powering applications.

[//]: # (Image References)
[image1]: ./resources/mb_front.png "MB Front"
[image2]: ./resources/sb1_front.png "SB1 Front"
[image3]: ./resources/sb1_back.png "SB1 Back"
[image4]: ./resources/pickit3.png "Pickit3"
[image5]: ./resources/connector.png "Connector"
[image6]: ./resources/cable.png "Cable"
[image7]: ./resources/debug.png "Debug"
[image8]: ./resources/vout_fb_sch.png "+12V Feedback Schematic"
[image9]: ./resources/vout_fb_pcb.png "+12V Feedback PCB"
[image10]: ./resources/OVP.png "Over Voltage Protection"
[image11]: ./resources/PL30.png "HP HSTNS-PL30"
[image12]: ./resources/VR.png "10K VR(53CAD-D28-B15L)"
[image13]: ./resources/T3_compensator.png "Type III Compensator"
[image14]: ./resources/present.png "Enable PSU"
[image15]: ./resources/tweak_output.png "Tweak Output Voltage"
[image16]: ./resources/load_test_voltage.png "Load Test Voltage"
[image17]: ./resources/load_test_current.png "Load Test Current"
[image18]: ./resources/PL11_BMW_LoadWithFanMax.png "PL11 Load Test"
[image19]: ./resources/PD52BMWLoadTest.png "PD52 Load Test"
[image20]: ./resources/PL11_T3_Compensator.png "PL11 T3 Compensator"
[image21]: ./resources/PL11_OVP.png "PL11 OVP"
[image22]: ./resources/PLXXBMWLoadTest.jpg "PLXX BMW Load Test"
[image23]: ./resources/PL30_OVP_HW.png "PL30 OVP HW"
[image24]: ./resources/PL11_Vout_HW.png "PL11 Output HW"
[image25]: ./resources/PL11_OVP_HW.png "PL11 OVP HW"
[image26]: ./resources/PL30_OCP_HW.png "PL30 OCP HW"
[image27]: ./resources/PL11_OCP_HW.png "PL11 OCP HW"
[image28]: ./resources/PL30PicoWattMeter.png "PL30 PICO Watt Meter"
[image29]: ./resources/PL30_EN.png "PL30 Enable"
[image30]: ./resources/pickit5.png "Pickit5"
[image31]: ./resources/PL30_OVP_BOTTOM.png "PL30 OVP BOTTOM"
[image32]: ./resources/HP-HSTNS-PD44_MOD_SCH.png "PD44 MOD SCH"
[image33]: ./resources/PD44_EN.png "PD44 EN"
[image34]: ./resources/HP-HSTNS-PD44_MOD.png "PD44 MOD"
[image35]: ./resources/PD44_MOD_14V4.png "PD44 MOD 14.4V"
[image36]: ./resources/PL30_CSA.png "Current Sense Amplifier"
[image37]: ./resources/PL30_FeedbackLoop.png "ACMC Feedback Loop"
[image38]: ./resources/PL30_NoHWMod.png "PL30 Mod Firmware Only"

### Activate PSU
To activate the PSU, connect a low-value resistor between Pin 33 and Pin 36, or simply short them together.  
33: ENABLE#  
36: PRESENT  
![alt text][image29]
![alt text][image14]


### In-Circuit Serial Programming and Debugging Pinout

![alt text][image7]

LITEON PSU  
Pin1 - PGEC  
Pin2 - PGED  
Pin3 - $\overline{\textrm{MCLR}}$  
Pin4 - GND  
Pin5 - Vcc (3.3 V)  

Delta PSU  
Pin1 - GND  
Pin2 - $\overline{\textrm{MCLR}}$  
Pin3 - PGED  
Pin4 - PGEC  
Pin5 - Vcc (3.3 V)  

![alt text][image30]

* [Molex Connector - 218112-0503](https://www.digikey.com/en/products/detail/molex/2181120503/14309216?s=N4IgTCBcDa4IwA45zAWgAwFZ0GYQF0BfIA)
* [Molex Connector - 218113-0501](https://www.digikey.com/en/products/detail/molex/2181130501/14309204?s=N4IgTCBcDa4IwA45wMwFoAMBWDcQF0BfIA)


### Modify PL30 Output & OVP Voltage
* Type III Compensator Using Op-Amp  

A Type III compensator is a compensation network used in control systems to improve stability and performance.   
Locate the Type III compensator in the HP PL30 power supply by examining the PCB for the Op-Amp [TSV994](https://www.st.com/resource/en/datasheet/tsv994.pdf)
![alt text][image13]

To increase the output voltage, one can either increase the reference voltage or decrease the resistance value of R4.
![alt text][image8]
![alt text][image15]
![alt text][image10]

Assume the default output is 12.32 V.  
There are two approaches to prevent the OVP (Over Voltage Protection) from being triggered, one is to add a resistor in parallel with the 01B resistor, the other is to modify the firmware of the MCU(dsPIC33FJ64GS606).  
```math
\begin{gather*}
\frac{R_{p1}}{R_{p1}+39.2+10+4420}{\ast}V_{out}=0.58,\ R_{p1}=\frac{R_1{\ast}220}{R_1+220} \\
\frac{1}{1+3.92}{\ast}12.32=\frac{R_{p2}}{R_{p2}+3.92}{\ast}V_{out},\ R_{p2}=\frac{R_2{\ast}1}{R_2+1} \\
If \ V_{out}=14.4V,\ R_1{\approx}1.27Kohms,\ R_2{\approx}4.7Kohms \\
If \ V_{out}=15.2V,\ R_1{\approx}910(Ohms),\ R_2{\approx}3.4Kohms \\
If \ V_{out}=16V,\ R_1{\approx}715(Ohms),\ R_2{\approx}2.67Kohms
\end{gather*}
```
* [WolframAlpha Solver Output 15.2 V](https://www.wolframalpha.com/input?i=R_11%2F%28R_11%2B39.2%2B10%2B4420%29*V_1%3D0.58%2CR_11%3D220*R_1%2F%28R_1%2B220%29%2C1%2F%281%2B3.92%29*12.32%3DR_12%2F%28R_12%2B3.92%29*V_1%2CR_12%3DR_2*1%2F%281%2BR_2%29%2CV_1%3D15.2)

R1 is used to adjust the output voltage of the power supply, while R2 is used to adjust the threshold of the Over Voltage Protection (OVP).  
The modification mentioned above is not meant to raise the OVP threshold, but rather to make the dsPIC33FJ64GS606 MCU sense the output voltage as if it had not been adjusted.

### PL30 Current Sense Amplifier

![alt text][image36]
The figure above shows a current-sense amplifier circuit for high-side current measurement based on a Texas Instruments OPA2277 op amp and discrete gain-setting resistors.  
C1,C2,C4 Filter capacitors, GAIN=400.

### Patch Firmware Without Hardware Modifications

**PL30 Rev10 With Output 14.4 V Firmware**  
* [Download PL30 DSPIC33FJ64GS606](firmware/PL30/10/Patch/DSPIC33FJ64GS606_NoHWMod.hex)

![alt text][image38]

**PL30 Rev12 AGM Battery Charger 20 A Firmware**  
* [Download AGM Battery Charger 20 A CC](firmware/PL30/12/Patch/DSPIC33FJ64GS606_CC20.hex)

### PL30 Analog/Digital Control Scheme ACMC
![alt text][image37]
The average current feedback loop is established by cascading a dedicated voltage and current loop compensator, each tied to its respective feedback signal. Just like in peak current mode control, the outer voltage loop compensator output provides the reference for the inner current loop, where a second compensation filter adjusts the average inductor current by adjusting the modulated switch node control signal.

In the PL30, the outer voltage loop is a TYPE III analog compensator. This choice might have been made due to insufficient dsPIC MCU resources to implement a digital solution, or possibly because the analog compensator provides a faster response.

In later power supply designs, with upgraded processors offering more resources, a 3P3Z digital compensator replaced the analog TYPE III compensator, providing improved flexibility and control precision.

### PL30 System Signal and Pin Configuration

[Read more details](PL30_pc.md)

At system startup, the secondary-side microcontroller (MCU) transmits the following byte sequence to the primary-side controller:

EA 18 18 18 EA 18
18 18 50 20 00 45
9B 18 18 50 20 00
44 9C 18 18 50 20
00 45 9B 18 18 05
00 00 18 18 18 18
EA 18 18 18 05 02
FE 18 18 18 18 05
04 FC 18 18 18 18
05 06 FA 18 18 18
18 05 08 F8 18 18
18 18 05 1E E2 18
18 18 18 05 22 DE
18 18 18 18 50 28
00 00 D8 18 18 50
2C 00 7E 56 18 18

### ⚡ OVP Voltage Scaling Logic

        mov.w   0x0344, W0        ; Read ADC value (10-bit) into W0
        mov.w   #0x6ac2, W2       ; Load Q15 coefficient (182/(182+39.2+10+4420))/(220/(220+39.2+10+4420)) 0.834 into W2

        ; 16x16 unsigned multiply → 32-bit result in W1:W0 (High:Low)
        mul.uu  W0, W2, W0        ; Perform unsigned multiply, result: W1 = high word, W0 = low word

        ; Shift right by 15 bits to scale back to Q0 (integer)
        ; Equivalent to: (W1 << 1) | (W0 >> 15)
        sl      W1, #0x1, W1      ; W1 = W1 << 1
        lsr.w   W0, #0xf, W0      ; W0 = W0 >> 15
        ior.w   W0, W1, W1        ; Combine bits: W1 = (W1 << 1) | (W0 >> 15)

        ; Final result in W1
        ; W1 = Y = X * 0.834

### PL30 PICO Watt Meter
![alt text][image28]
The image above shows the Raspberry Pi Pico reading data from the HP HSTNS-PL30 power supply via I2C, measuring electrical parameters such as Amps, Volts, Watts, and Watt-hours, and displaying the results on the PIMORONI Pico Display Pack.

| PICO        | PL30 |
| ----------- | ----------- |
| VSYS(39)    | 5 V (output of the 12 V standby-input DC-DC converter)       |
| GND(38)     | GND       |
| GP4/SDA(6)  | 31        |
| GP5/SCL(7)  | 32        |

**PICO Firmware**  
* [RPI PICO FW](firmware/PICO)  
* [RPI PICO Watt Meter Source](https://github.com/darwinbeing/zpsu_mon)

Note: 
- Modify the voltage divider resistor: 220 Ω → 182 Ω.
- Update PSU FW
- Update PICO FW

A - Enable/Disable PSU  
X - CV/CC Switch  
B - Increase CC setting  
Y - Decrease CC setting  

### Modify PL11 Output & OVP Voltage
![alt text][image20]
![alt text][image24]

![alt text][image21]
![alt text][image25]

There are two approaches to prevent the OVP (Over Voltage Protection) from being triggered, one is to connect a resistor in parallel, the other is to modify the firmware of the MCU(PIC16F883).  
```math
\begin{gather*}
\frac{{VR}+R_{p3}}{{VR}+R_{p3}+39.2+10+4420}{\ast}V_{out}=2.5,\ R_{p3}=\frac{R_3{\ast}1100}{R_3+1100},\ 0<{VR}<90 \\
If \ R_3=4.7Kohms,\ V_{out} \in (13.9,15) \\
If \ R_3=6.8Kohms,\ V_{out} \in (13.3,14.3) \\
\frac{R_{p3}+VR}{R_{p3}+VR+39.2+10+4420}{\ast}V_{out}=2.5,\ R_{p3}=\frac{R_3{\ast}1100}{R_3+1100},\ VR=40 \\
\frac{1.74}{1.74+5.11}{\ast}12.29=\frac{R_{p4}}{R_{p4}+5.11}{\ast}V_{out},\ R_{p4}=\frac{R_4{\ast}1.74}{R_4+1.74} \\
If \ V_{out}=14.4V,\ R_3{\approx}5K1ohms,\ R_4{\approx}7K68ohms \\
If \ V_{out}=15.2V,\ R_3{\approx}3K57ohms,\ R_4{\approx}5K49ohms \\
If \ V_{out}=16V,\ R_3{\approx}2K7ohms,\ R_4{\approx}4K3ohms
\end{gather*}
```
* [WolframAlpha Solver Output 15.2 V](https://www.wolframalpha.com/input?i=%28R_11%2BR_5%29%2F%28R_11%2BR_5%2B39.2%2B10%2B4420%29*V_1%3D2.5%2CR_11%3D1100*R_3%2F%28R_3%2B1100%29%2C1.74%2F%281.74%2B5.11%29*12.29%3DR_12%2F%28R_12%2B5.11%29*V_1%2CR_12%3DR_4*1.74%2F%281.74%2BR_4%29%2CR_5%3D40%2CV_1%3D15.2)

R3 is used to adjust the output voltage of the power supply, while R4 is used to adjust the threshold of the Over Voltage Protection (OVP).  
The modification above is not meant to raise the OVP threshold, but rather to make the MCU sense the output voltage as if it had not been adjusted.

$$
\begin{gather*}
\frac{1.74}{1.74+5.11}{\ast}V_{ovp}=(\frac{1}{4}+\frac{15}{32}){\ast}V_{dd} \\
If \ V_{dd}=4.9V,\ V_{ovp}{\approx}13.86V \\
If \ V_{dd}=5V,\ V_{ovp}{\approx}14.12V
\end{gather*}
$$

Vdd is the supply voltage of the PIC16F883. In the default configuration, the OVP voltage depends on the MCU supply voltage, which is approximately 13.86 V in this case.

**PL11 Rev12 Firmware With OVP Disabled**  
* [PL11 PIC16F883 OVP Disabled](firmware/PL11/12/Patch/PIC16F883.hex)


### HSTNS-PD44 Digital Control Analysis

#### 1. System Overview

The firmware implements a Gain-Scheduled 2P2Z digital compensator with frequency-dependent gain scaling.
The controller consists of:  
- Second-order feedback (2 poles)  
- Second-order error compensation (2 zeros)  
- Dynamic gain scheduling based on previous output value  

```math
\begin{gather*}
u[n] = a_1{\ast}u[n-1] + a_2{\ast}u[n-2] + k_{AGC}(b_0{\ast}e[n] + b_1{\ast}e[n-1] + b_2{\ast}e[n-2]) \\
\end{gather*}
```

#### 2. 2P2Z Transfer Function

$$H(z) = \frac{-1.952 + 2.425 z^{-1} - 0.691 z^{-2}}{1 - 0.257 z^{-1} + 0.007 z^{-2}}$$

Coefficients (Q15 fixed-point):

| Parameter | Float | Q15 Hex | Role |
|-----------|-------|---------|------|
| $b_0$ | -1.95197 | 0xFFFF0626 | e[n] |
| $b_1$ | 2.424896 | 0x00013663 | e[n-1] |
| $b_2$ | -0.69126 | 0xFFFFA785 | e[n-2] |
| $a_1$ | 0.25742 | 0x20F3 | y[n-1] |
| $a_2$ | -0.00742 | 0xFF0D | y[n-2] |

Zeros: $z_1 = 0.443$, $z_2 = 0.799$

Poles: $z_1 = 0.226$, $z_2 = 0.031$

Difference equation:

$$y[n] = 0.257 \cdot y[n-1] - 0.007 \cdot y[n-2] - 1.952 \cdot e[n] + 2.425 \cdot e[n-1] - 0.691 \cdot e[n-2]$$

The firmware implements a 2P2Z (2-pole, 2-zero) digital compensator for LLC resonant converter frequency control.

The controller consists of:
- Two poles ($a_1$, $a_2$): determine closed-loop stability and transient response
- Two zeros ($b_0$, $b_1$, $b_2$): compensate the LLC power stage poles, providing phase margin
- Gain scheduling: adaptive gain scaling based on operating frequency, increasing loop gain at lower frequencies where LLC transfer function gain drops

#### 3. I2C protocol

Frequency: 5 kHz  
Address: 0x5F  
Data Frame: 00 REG CS  
Read voltage: 0x00 0x20 0x22  
TX: 00 20 22   RX: 58 0C 9C

REG Range  
0x01  
0x10 ~ 0x17  
0x20 ~ 0x24  
0x32 ~ 0x49  
0x60 ~ 0x62  
0xF0 ~ 0xF6 

#### 4. Decompiling HSTNS-PD44 800 W PSU Firmware to C

Implement PSU initialization and closed-loop 12.3 V output via firmware reverse engineering

Decompiled HSTNS-PD44 800 W LLC converter firmware (dsPIC33FJ64GS606) 
with Claude AI assistance. Reconstructed startup sequence, PWM configuration, 
and 2P2Z digital compensator for closed-loop voltage regulation at 12.3 V output.
 
* [HSTNS PD44 Firmware SRC](https://github.com/darwinbeing/hstns-pd44-firmware)

#### Reverse Engineer a Schematic
![alt text][image32]
#### Activate PSU
![alt text][image33]
#### Modify Output & OVP
![alt text][image34]

```math
\begin{gather*}
\frac{5.23}{5.23+20}{\ast}12.3=\frac{5.23}{5.23+R_1}{\ast}V_{out} \\
\frac{5.23}{5.23+20}{\ast}12.3=\frac{5.23}{5.23+R_2}{\ast}V_{out} \\
If \ V_{out}=13.8V,\ R_1{\approx}23.2Kohms,\ R_2{\approx}23.2Kohms \\
If \ V_{out}=14.25V,\ R_1{\approx}24Kohms,\ R_2{\approx}24Kohms \\
If \ V_{out}=14.4V,\ R_1{\approx}24.3Kohms,\ R_2{\approx}24.3Kohms
\end{gather*}
```

#### HSTNS-PD44 CC Firmware

Modify output/OVP divider network and update firmware  
* [Download PD44 DSPIC33FJ64GS606](firmware/PD44/05F/Patch/DSPIC33FJ64GS606_CC14420.hex)


### Load Test
When conducting load testing with the ignition ON and AC running, the cooling fan operates at full speed. The open-circuit output voltage measures 14.28 V. Under load conditions, a voltage drop is observed due to wire impedance — this behavior is expected and consistent with normal operation. For constructing the dummy load, 12 AWG Nichrome 80 resistance wire is recommended.

***PL11/PL30/PD44/PD52 Load With Fan on MAX,Audio,High Beam activated***
![alt text][image22]

### Video
**HP HSTNS-PL11 Modification**  
[![HP HSTNS-PL11 Mod](https://img.youtube.com/vi/_pB6r_7EpHQ/0.jpg)](https://youtu.be/_pB6r_7EpHQ "HP HSTNS-PL11")

**HP HSTNS-PL30 Modification**  
[![HP HSTNS-PL30 Mod](https://img.youtube.com/vi/eh7FzcV9E4g/0.jpg)](https://youtu.be/eh7FzcV9E4g "HP HSTNS-PL30")

**HP HSTNS-PL11 OCP Modification**  
[![HP HSTNS-PL11 OCP Demo](https://img.youtube.com/vi/BLK8ttS5fhQ/0.jpg)](https://youtu.be/BLK8ttS5fhQ "HP HSTNS-PL11 OCP")

**HP HSTNS-PL30 PICO Watt Meter**  
[![HP HSTNS-PL130 Watt Meter](https://img.youtube.com/vi/YhXmV2Y7lFA/0.jpg)](https://youtu.be/YhXmV2Y7lFA "HP HSTNS-PL30 Watt Meter")

**HP HSTNS-PL33 Modification**  
<a href="https://youtu.be/sOMTlh2ckdg" title="HP HSTNS-PL33 Mod">
 <img src="https://img.youtube.com/vi/sOMTlh2ckdg/0.jpg" alt="HP HSTNS-PL33" />
</a>

**HP HSTNS-PL28 Modification**  
[![HP HSTNS-PL28 Mod](https://img.youtube.com/vi/eTXfFXV7vOA/0.jpg)](https://youtu.be/eTXfFXV7vOA "HP HSTNS-PL28")

**HP HSTNS-PD19 Modification**  
[![HP HSTNS-PD19 Mod](https://img.youtube.com/vi/oVpzPtx0xMU/0.jpg)](https://youtu.be/oVpzPtx0xMU "HP HSTNS-PD19")

**HP HSTNS-PD52 Modification**  
[![HP HSTNS-PD52 Mod](https://img.youtube.com/vi/Jvvv-1Kfa74/0.jpg)](https://youtu.be/Jvvv-1Kfa74 "HP HSTNS-PD52")
