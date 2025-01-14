
![alt text][image19]

---

## HP HSTNS-PL11/PL30/PL42

HP HSTNS-PL11/PL30/PL42 1200Watt Hot Plug Power Supply Hack for Vehicle Flashing and Programming.
The HPE PSU is a powerful and versatile power supply unit that can be modified or hacked to suit a wide range of needs,With some basic electronics knowledge and proper safety precautions, it's possible to make this PSU even more useful and versatile for your specific applications.
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
[image36]: ./resources/current_sense.png "Current Sense Amplifier"
[image37]: ./resources/PL30_FeedbackLoop.png "ACMC Feedback Loop"

### Activate PSU
To activate the PSU, add a small resistance resistor between Pin 33 and 36 or simply short them together.  
33: ENABLE#  
36: PRESENT  
![alt text][image29]
![alt text][image14]


### In-Circuit Serial Programming and Debugging Pinout

![alt text][image7]

Pin1 - PGEC  
Pin2 - PGED  
Pin3 - $\overline{\textrm{MCLR}}$  
Pin4 - GND  
Pin5 - Vcc(3.3V)  

![alt text][image30]

* [Molex Connector - 21112-0504](https://www.mouser.com/ProductDetail/538-218112-0504)
* [218112-0504 Datasheet](https://www.mouser.com/datasheet/2/276/3/2181120504_CABLE_ASSEMBLIES-2864576.pdf)


### Modify PL30/PL42 Output & OVP Voltage
* Type III Compensator Using Op-Amp  

Type III compensators are a type of compensation network used in control systems to improve stability and performance.   
Locate the Type III compensator in the HP PL30 power supply by examining the PCB for the Op-Amp [TSV994](https://www.st.com/resource/en/datasheet/tsv994.pdf)
![alt text][image13]

To increase the output voltage, one can either increase the reference voltage or decrease the resistance value of R4.
![alt text][image8]
![alt text][image9]

![alt text][image15]
![alt text][image10]
![alt text][image31]

Assuming the default Output is 12.32V.  
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
* [WolframAlpha Solver Output 15.2V](https://www.wolframalpha.com/input?i=R_11%2F%28R_11%2B39.2%2B10%2B4420%29*V_1%3D0.58%2CR_11%3D220*R_1%2F%28R_1%2B220%29%2C1%2F%281%2B3.92%29*12.32%3DR_12%2F%28R_12%2B3.92%29*V_1%2CR_12%3DR_2*1%2F%281%2BR_2%29%2CV_1%3D15.2)

R1 is used to adjust the output voltage of the power supply, while R2 is used to adjust the threshold of the Over Voltage Protection (OVP).  
The modification mentioned above is not meant to increase the OVP threshold, but rather to trick the dsPIC33FJ64GS606 MCU into thinking that the output voltage has not been adjusted.

### PL30 Current Sense Amplifier

![alt text][image36]
Figure above shows a current sense amplifier circuit for high side current measurements based on a Texas Instruments OPA2277 op amp and discrete gain setting resistors.  
C1,C2,C4 Filter capacitors, GAIN=400.

 
### Rev10 Firmware
[Original DSPIC33FJ64GS606](firmware/PL30/10/DSPIC33FJ64GS606.hex)

### Patch Firmware
```
--- PL30/10/DSPIC33FJ64GS606.hex	2023-03-12 14:10:17.000000000 -0700
+++ PL30/10/Patch/DSPIC33FJ64GS606.hex	2023-04-11 10:05:24.000000000 -0700
@@ -1802,7 +1802,7 @@
 :10708000D06E88000603A90091C0A9004400F90051
 :107090004F047800CF0278004F027800CF017800CB
 :1070A0004F017800CF0078004F00780000400600C4
-:1070B000811F7800851F7800211A80000150880008
+:1070B000811F7800851F7800E134070001508800A7
 :1070C000155280008582400011508000818252005C
 :1070D00015528800C2AADE0015508800311A8000BF
 :1070E00051528800D5E9AE00060037004501200066
@@ -4747,7 +4747,7 @@
 :10288000D06E88000603A90091C0A9004400F90099
 :102890004F047800CF0278004F027800CF01780013
 :1028A0004F017800CF0078004F007800004006000C
-:1028B000811F7800851F7800211A80000150880050
+:1028B000811F7800851F7800E1060700015088001D
 :1028C00015528000858240001150800081825200A4
 :1028D00015528800C2AADE0015508800311A800007
 :1028E00051528800D5E9AE000600370045012000AE
@@ -5188,8 +5188,8 @@
 :104410008080A900FFFF37000000FE00FFFFFF00C3
 :10442000FFFFFF00FFFFFF00FFFFFF00FFFFFF0098
 :10443000FFFFFF00FFFFFF00FFFFFF00FFFFFF0088
-:10444000FFFFFF00FFFFFF00FFFFFF00FFFFFF0078
-:10445000FFFFFF00FFFFFF00FFFFFF00FFFFFF0068
+:104440000000F800211A8000C0072000808F500073
+:1044500001003600808050000000F90000000600D6
 :10446000FFFFFF00FFFFFF00FFFFFF00FFFFFF0058
 :10447000FFFFFF00FFFFFF00FFFFFF00FFFFFF0048
 :10448000FFFFFF00FFFFFF00FFFFFF00FFFFFF0038
```
The code snippet above is PL30 Output set to 14.28V  

**PL30 Rev10 Firmware With Output 14.4V(Without Hardware Modifications)**  
* [PL30 DSPIC33FJ64GS606 FirmwareOnly](firmware/PL30/10/Patch/DSPIC33FJ64GS606_NoHWMod.hex)

**PL42 Rev01 Firmware With Output 14.4V(Without Hardware Modifications)**  
* [PL42 DSPIC33FJ64GS606 FirmwareOnly](firmware/PL42/01/Patch/DSPIC33FJ64GS606.hex)


### PL30 Feedback Loop
![alt text][image37]
The most common approach to establishing a bi-directional control stage is by using Average Current Mode Control (ACMC). This control mode employs a multi-loop system consisting of one outer voltage loop and one inner current loop. The outer voltage loop, implemented as a Type III analog compensator in hardware, regulates a constant output voltage by providing a dynamic current reference to the inner current loop. The inner current loop, realized as a digital compensator designed with a PID controller and implemented within a dsPIC MCU, then adjusts the PWM to achieve precise control.

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
* [WolframAlpha Solver Output 15.2V](https://www.wolframalpha.com/input?i=%28R_11%2BR_5%29%2F%28R_11%2BR_5%2B39.2%2B10%2B4420%29*V_1%3D2.5%2CR_11%3D1100*R_3%2F%28R_3%2B1100%29%2C1.74%2F%281.74%2B5.11%29*12.29%3DR_12%2F%28R_12%2B5.11%29*V_1%2CR_12%3DR_4*1.74%2F%281.74%2BR_4%29%2CR_5%3D40%2CV_1%3D15.2)

R3 is used to adjust the output voltage of the power supply, while R4 is used to adjust the threshold of the Over Voltage Protection (OVP).  
The modification above is not to increase the OVP threshold, but to make the MCU believe that the output voltage has not been adjusted.

$$
\begin{gather*}
\frac{1.74}{1.74+5.11}{\ast}V_{ovp}=(\frac{1}{4}+\frac{15}{32}){\ast}V_{dd} \\
If \ V_{dd}=4.9V,\ V_{ovp}{\approx}13.86V \\
If \ V_{dd}=5V,\ V_{ovp}{\approx}14.12V
\end{gather*}
$$

Vdd is the supply voltage of the PIC16F883. By default configuration, The OVP voltage is dependent on the MCU supply voltage, which is approximately 13.86 volts in this case.

**PL11 Rev12 Firmware With OVP Disabled**  
* [PL11 PIC16F883 OVP Disabled](firmware/PL11/12/Patch/PIC16F883.hex)
### PL11 OCP
![alt text][image27]

The output voltage of the CT amplified is divided down to 5V and fed into the MCU(PIC16F883). Adding a resistor in parallel across the 8.2k（labeled '8201') resistor can reduce the maximum output current.
A more effective approach would be to measure the voltage at OC_DET along with the actual output voltage of amplifier, and calculate the appropriate divider ratio.  
Shorting the 8.2k resistor would cap the maximum output current at around 30A.  
The current flowing through the CT(Current Transformer) is transformed into a voltage signal and subsequently amplified using an amplifier. The amplified voltage is directly proportional to the output current $`V=\frac{I}{10}`$.

### PL30 PICO Watt Meter
![alt text][image28]
The image above shows the Raspberry Pi Pico reading data from the HP HSTNS-PL30 power supply via I2C, measuring electrical parameters such as Amps, Volts, Watts, and Watt-hours, and displaying the results on the PIMORONI Pico Display Pack.

| PICO        | PL30 |
| ----------- | ----------- |
| VSYS(39)    | 3.3V       |
| GND(38)     | GND       |
| GP4/SDA(6)  | 31        |
| GP5/SCL(7)  | 32        |

***BOMs***  
1. Raspberry Pi Pico  
2. Pico Display Pack  

**PICO Firmware 14.48V**  
* [RPI PICO FW](firmware/PICO/zephyr.uf2)
* [RPI PICO Watt Meter Source](https://github.com/darwinbeing/zpsu_mon)

### Modify HP HSTNS-PD44 800W TITANIUM LLC PSU

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
#### PD44 14.4V
![alt text][image35]

### Load Test
When conducting load testing with ignition ON and AC running, the fan operates at its maximum speed. The output voltage reads 14.28V with no load. However, when under load, there is a voltage drop caused by the impedance in the wires, which is expected.
Technically speaking, I would recommend employing Nichrome 80 12 gauge AWG resistance wire for the construction of a dummy load.

***PL11/PL30/PL42 Load With Fan on MAX,Audio,High Beam activated***
![alt text][image22]

### Video
**HP HSTNS-PL11 Modification**  
[![HP HSTNS-PL11 Mod](https://img.youtube.com/vi/_pB6r_7EpHQ/0.jpg)](https://youtu.be/_pB6r_7EpHQ "HP HSTNS-PL11")

**HP HSTNS-PL30/PL42 Modification**  
[![HP HSTNS-PL30/PL42 Mod](https://img.youtube.com/vi/eh7FzcV9E4g/0.jpg)](https://youtu.be/eh7FzcV9E4g "HP HSTNS-PL30/PL42")

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
