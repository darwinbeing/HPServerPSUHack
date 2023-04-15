# **HPServerPSUHack**

![alt text][image11]

---

## HP HSTNS-PL11/PL30/PL42

HP HSTNS-PL11/PL30/PL42 1200Watt Hot Plug Power Supply Hack for BMW Flashing and Programming.
The HPE PSU is a powerful and versatile power supply unit that can be modified or hacked to suit a wide range of needs,With some basic electronics knowledge and proper safety precautions, it's possible to make this PSU even more useful and versatile for your specific applications.

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
[image19]: ./resources/PL42BMWLoadTest.jpg "PL42 Load Test"
[image20]: ./resources/PL11_T3_Compensator.png "PL11 T3 Compensator"
[image21]: ./resources/PL11_OVP.png "PL11 OVP"
[image22]: ./resources/PLXXBMWLoadTest.jpg "PLXX BMW Load Test"
[image23]: ./resources/PL30_OVP_HW.png "PL30 OVP HW"
[image24]: ./resources/PL11_Vout_HW.png "PL11 Output HW"
[image25]: ./resources/PL11_OVP_HW.png "PL11 OVP HW"

### Activate PSU
To activate the PSU, add a 330 ohm resistor between Pin 33 and 36.  
33: ENABLE#  
36: PRESENT  
![alt text][image14]


### In-Circuit Serial Programming and Debugging Pinout

![alt text][image7]

Pin1 - PGEC  
Pin2 - PGED  
Pin3 - $\overline{\textrm{MCLR}}$  
Pin4 - GND  
Pin5 - Vcc(3.3V)  

![alt text][image4]
![alt text][image5]

* [Molex Connector - 21112-0504](https://www.mouser.com/ProductDetail/538-218112-0504)
* [218112-0504 Datasheet](https://www.mouser.com/datasheet/2/276/3/2181120504_CABLE_ASSEMBLIES-2864576.pdf)


### Modify PL30/PL42 Output & OVP Voltage
* Type III Compensator Using Op-Amp  

Locate the Type III compensator in the HP PL30 power supply by inspecting the PCB for the Op-Amp [TSV994](https://www.st.com/resource/en/datasheet/tsv994.pdf)
![alt text][image13]

To increase the voltage to 14.09V, connect a parallel resistor 'R' with the original 220 ohms resistor (labeled '221'), with a value of 1.5 kilo-ohms
![alt text][image8]
![alt text][image9]
![alt text][image15]

![alt text][image10]
![alt text][image23]

Assuming the default Output is 12.32V.  
There are two approaches to prevent the OVP (Over Voltage Protection) from being triggered, one is to add a resistor in parallel with the 01B resistor, the other is to modify the firmware of the MCU(dsPIC33FJ64GS606).  
$$\frac{R_{p1}}{R_{p1}+39.2+10+4420}{\ast}V_{out}=0.58,\ R_{p1}=\frac{R_1{\ast}220}{R_1+220}$$
$$\frac{1}{1+3.92}{\ast}12.32=\frac{R_{p2}}{R_{p2}+3.92}{\ast}V_{out},\ R_{p2}=\frac{R_2{\ast}1}{R_2+1}$$
$$If \ V_{out}=14.06V,\ R_1{\approx}1.54Kohms,\ R_2{\approx}5.64Kohms$$
$$If \ V_{out}=14.28V,\ R_1{\approx}1.33Kohms,\ R_2{\approx}5Kohms$$
$$If \ V_{out}=14.4V,\ R_1{\approx}1.27Kohms,\ R_2{\approx}4.7Kohms$$

The modification mentioned above is not meant to increase the OVP threshold, but rather to trick the dsPIC33FJ64GS606 MCU into thinking that the output voltage has not been adjusted.

 
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

**PL30 Rev10 Firmware With Output 14.06V**  
* [PL30 DSPIC33FJ64GS606 Output 14.06V](firmware/PL30/10/Patch/DSPIC33FJ64GS606.hex)

**PL42 Rev01 Firmware With Output 14.28V**  
* [PL42 DSPIC33FJ64GS606 Output 14.28V](firmware/PL42/01/Patch/DSPIC33FJ64GS606.hex)

### Modify PL11 Output & OVP Voltage
![alt text][image20]
![alt text][image24]

![alt text][image21]
![alt text][image25]

There are two approaches to prevent the OVP (Over Voltage Protection) from being triggered, one is to connect a resistor in parallel, the other is to modify the firmware of the MCU(PIC16F883).  
$$\frac{{VR}+R_{p3}}{{VR}+R_{p3}+39.2+10+4420}{\ast}V_{out}=2.5,\ R_{p3}=\frac{R_3{\ast}1100}{R_3+1100},\ 0<{VR}<90$$
$$If \ R_3=4.7Kohms,\ V_{out} \in (13.9,15)$$
$$If \ R_3=6.8Kohms,\ V_{out} \in (13.3,14.3)$$

$$\frac{R_{p3}+VR}{R_{p3}+VR+39.2+10+4420}{\ast}V_{out}=2.5,\ R_{p3}=\frac{R_3{\ast}1100}{R_3+1100},\ VR=40$$
$$\frac{1.74}{1.74+5.11}{\ast}12.29=\frac{R_{p4}}{R_{p4}+5.11}{\ast}V_{out},\ R_{p4}=\frac{R_4{\ast}1.74}{R_4+1.74}$$
$$If \ V_{out}=14.35V,\ R_3{\approx}5K1ohms,\ R_4{\approx}7K68ohms$$
$$If \ V_{out}=14.2V,\ R_3{\approx}5K49ohms,\ R_4{\approx}8K25ohms$$
$$If \ V_{out}=14V,\ R_3{\approx}6K04ohms,\ R_4{\approx}9K31ohms$$
The modification above is not to increase the OVP threshold, but to make the MCU believe that the output voltage has not been adjusted.

$$\frac{1.74}{1.74+5.11}{\ast}V_{ovp}=(\frac{1}{4}+\frac{15}{32}){\ast}V_{dd}$$
$$If \ V_{dd}=4.9V,\ V_{ovp}{\approx}13.86V$$
$$If \ V_{dd}=5V,\ V_{ovp}{\approx}14.12V$$
Vdd is the supply voltage of the PIC16F883. By default configuration, The OVP voltage is dependent on the MCU supply voltage, which is approximately 13.86 volts in this case.

**PL11 Rev12 Firmware With OVP Disabled**  
* [PL11 PIC16F883 OVP Disabled](firmware/PL11/12/Patch/PIC16F883.hex)

### Load Test
When conducting load testing with BMW ignition ON and AC running, the fan operates at its maximum speed. The output voltage reads 14.28V with no load. However, when under load, there is a voltage drop caused by the impedance in the wires, which is expected.

Technically speaking, I would recommend employing Nichrome 80 14 gauge AWG resistance wire for the construction of a dummy load.

***PL11/PL30/PL42 BMW Load With Fan on MAX,Audio,High Beam activated***
![alt text][image22]
