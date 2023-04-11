# **HPServerPSUHack**

![alt text][image11]

---

## HP HSTNS-PL30

HP HSTNS-PL30 1200Watt Hot Plug Power Supply Hack for BMW Flashing and Programming.


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


### Enable PSU
Add a 330ohms resistor between Pin 33 and 36 to enable PSU.  
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


### Tweak Output Voltage
* Type III Compensator Using Op-Amp  

Find the Type III Compensator Using Op-Amp [TSV994](https://www.st.com/resource/en/datasheet/tsv994.pdf) in the HP PL30 power supply from the PCB
![alt text][image13]

Connector a parallel resistor "R" together with original resistor(220ohms, "221") to increase up to 14.09V,Its value is 1k5
![alt text][image8]
![alt text][image9]
![alt text][image15]

### Tweak Over Voltage Protection
![alt text][image10]
Assuming the default OVP is 14V.  
There are two methods to modify the OVP. The first method is to connect a resistor in parallel with the 01B resistor, and the second method is to modify the firmware of the MCU(dsPIC33FJ64GS606).  
$$\frac{1}{1+3.92}{\ast}14=\frac{R_p}{R_p+3.92}{\ast}V_{ovp},\ R_p=\frac{R{\ast}1}{R+1}$$
$$If \ V_{ovp}=15V,\ R{\approx}11Kohms$$
$$If \ V_{ovp}=16V,\ R{\approx}5.6Kohms$$
 
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

**PL11 Rev12 Firmware With OVP Disabled**  
* [PL11 PIC16F883 OVP Disabled](firmware/PL11/12/Patch/PIC16F883.hex)

### Load Test
Load testing with BMW ignition ON, fan runs at max with AC on, the output voltage is 14.28 without load. However, when loaded, there is a voltage drop due to impedance in the wires, which is reasonable.

***PL30 BMW Load With Fan On***
![alt text][image16]
![alt text][image17]

***PL11 BMW Load With Fan On***
![alt text][image18]

***PL42 BMW Load With Fan MAX,Audio,High Beam On***
![alt text][image19]