
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
[image36]: ./resources/PL30_CSA.png "Current Sense Amplifier"
[image37]: ./resources/PL30_FeedbackLoop.png "ACMC Feedback Loop"
[image38]: ./resources/PL30_NoHWMod.png "PL30 Mod Firmware Only"

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

### Patch Firmware Without Hardware Modifications

**PL30 Rev10 With Output 14.4V Firmware**  
* [Download PL30 DSPIC33FJ64GS606](firmware/PL30/10/Patch/DSPIC33FJ64GS606_NoHWMod.hex)

![alt text][image38]

**PL30 Rev12 AGM Battery Charger 20A Firmware**  
* [Download AGM Battery Charger 20A CC](firmware/PL30/12/Patch/DSPIC33FJ64GS606_CC20.hex)

**PL42 Rev01 With Output 14.4V Firmware**  
* [Download PL42 DSPIC33FJ64GS606](firmware/PL42/01/Patch/DSPIC33FJ64GS606.hex)


### PL30 Analog/Digital Control Scheme ACMC
![alt text][image37]
The average current feedback loop is established by cascading a dedicated voltage and current loop compensator, each tied to its respective feedback signal. Just like in peak current mode control, the outer voltage loop compensator output provides the reference for the inner current loop, where a second compensation filter adjusts the average inductor current by adjusting the modulated switch node control signal.

In the PL30, the outer voltage loop is a TYPE III analog compensator. This choice might have been made due to insufficient dsPIC MCU resources to implement a digital solution, or possibly because the analog compensator provides a faster response.

In later power supply designs, with upgraded processors offering more resources, a 3P3Z digital compensator replaced the analog TYPE III compensator, providing improved flexibility and control precision.

### PL30 System Signal and Pin Configuration


| Pin | Name                            | Description                                   | Note                                       |
|-----|---------------------------------|-----------------------------------------------|--------------------------------------------|
| 1   | PWM3H                           | 29kHz 75% 3.3V Feedback                       |                                            |
| 2   | PWM4L                           | FAN Drive 25kHz 10%                           | FAN CON 4                                  |
| 3   | PWM4H                           | TYPE III Compensator Vref                     |                                            |
| 4   | RG6                             | FAN Power                                     | ??, 0V                                     |
| 5   | RG7                             | LS/SD                                         |                                            |
| 6   | RG8                             | Optoisolator Viper17                          | BR L217 TOP, 0V                            |
| 7   | ~MCLR                           | 47R                                           |                                            |
| 8   | RG9                             | LED                                           | HIGH: ON, LOW: OFF                         |
| 9   | VSS                             | GND                                           |                                            |
| 10  | VDD                             | VCC                                           | 3.3V                                       |
| 11  | AN5/CMP3B/AQEB1/CN7/RB5         | NC                                            |                                            |
| 12  | AN4                             | 12VSB                                         | 2.446V                                     |
| 13  | AN3                             | IS_FB Primary                                 | 100mV, OUTPUT SEC 20A                      |
| 14  | AN2                             | OVP                                           |                                            |
| 15  | AN1                             | High-Side Shunt Current Secondary             |                                            |
| 16  | AN0                             | Output Voltage Feedback                       |                                            |
| 17  | AN6                             | PSU_ON Detect                                 |                                            |
| 18  | AN7                             | PSU_ON Detect                                 |                                            |
| 19  | AVDD                            | VCC                                           |                                            |
| 20  | AVSS                            | GND                                           |                                            |
| 21  | AN8                             | Standby Voltage                               |                                            |
| 22  | AN9                             | Standby Output Current Sense                  |                                            |
| 23  | AN10                            | Secondary Ambient Temperature                 |                                            |
| 24  | AN11                            | Secondary Semiconductor Temperature           |                                            |
| 25  | VSS                             | GND                                           |                                            |
| 26  | VDD                             | VCC                                           | 3.3V                                       |
| 27  | AN12                            | Transformer Center Tap                        | Np/Ns = Vp/Vct                             |
| 28  | AN13                            |                                               |                                            |
| 29  | AN14/CMP3D/SS1/U2RTS/RB14       | NC                                            |                                            |
| 30  | AN15/CMP4D/CN12/RB15            | NC                                            |                                            |
| 31  | SDA2                            | I2C Bus                                       | EEPROM M24256-BW                           |
| 32  | SCL2                            | I2C Bus                                       | EEPROM M24256-BW                           |
| 33  | U1TX                            | ADUM2201                                      | → Primary BaudRate = 92593                 |
| 34  | U1RX                            | ADUM2201                                      | → Primary                                  |
| 35  | U1RTS/SCK1/INT0/RF6             | NC                                            |                                            |
| 36  | SDA1                            | I2C1 External Communication                   |                                            |
| 37  | SCL1                            | I2C1 External Communication                   |                                            |
| 38  | VDD                             | VCC                                           | 3.3V                                       |
| 39  | RC12                            | dsPIC33FJ16GS502                              | RB11 L357T                                 |
| 40  | RC15                            | L217 Optoisolator Bottom                      |                                            |
| 41  | VSS                             | GND                                           |                                            |
| 42  | IC1/FLT1/SYNCI1/INT1/RD8        | FAN CON PIN 3                                 |                                            |
| 43  | IC2/FLT2/U1CTS/INT2/RD9         | 24-2                                          | TO P200 1                                  |
| 44  | IC3/INDX1/FLT3/INT3/RD10        | 24-4                                          | TO P200 3                                  |
| 45  | IC4/QEA1/FLT4/INT4/RD11         | 24-6                                          | TO P200 5                                  |
| 46  | RD0                             | TS391 12VSB OCP                               |                                            |
| 47  | PGED2                           |                                               |                                            |
| 48  | PGEC2                           |                                               |                                            |
| 49  | OC2/SYNCO2/FLT6/RD1             | UNKNOWN OUTPUT                                |                                            |
| 50  | RD2                             | Main Transformer                              | 3.3V Control                               |
| 51  | OC4/SYNCO1/RD3                  | NC                                            |                                            |
| 52  | RD4                             | Input Standby                                 | 16V Output to 3.3V DCT                     |
| 53  | PWM6H                           | UCC27424 PIN2                                 | MOSFET SR, 3.3V                            |
| 54  | PWM5L/CN15/RD6                  | NC                                            |                                            |
| 55  | PWM5H/UPDN1/CN16/RD7            | NC                                            |                                            |
| 56  | VCAP                            |                                               | A low-ESR (< 0.5 Ohms) capacitor           |
| 57  | VDD                             | VCC                                           | 3.3V                                       |
| 58  | RF0                             | SDA                                           | AT24C01D IC EEPROM 1KBIT I2C 1MHZ SOT23-5  |
| 59  | RF1                             | SCL                                           | AT24C01D IC EEPROM 1KBIT I2C 1MHZ SOT23-5  |
| 60  | PWM1L                           | Q1                                            |                                            |
| 61  | PWM1H                           | Q2                                            |                                            |
| 62  | PWM2L                           | Q3                                            |                                            |
| 63  | PWM2H                           | Q4                                            |                                            |
| 64  | PWM3L/RE4                       | NC                                            |                                            |


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
| VSYS(39)    | 5V (12V Standby Input DC DC Converter Output 5V)       |
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
