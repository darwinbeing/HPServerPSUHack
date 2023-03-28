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
[image18]: ./resources/PL11_BMW_LoadWithFanMax.png "PLL Load Test"


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
[Original DSPIC33FJ64GS606](firmware/Rev10/DSPIC33FJ64GS606.hex)

### Patch Firmware
```
--- firmware/Rev10/DSPIC33FJ64GS606.hex	2023-03-12 14:10:17.000000000 -0700
+++ firmware/Rev10/Patch/DSPIC33FJ64GS606.hex 2023-03-10 19:01:22.000000000 -0800
@@ -1640,7 +1640,7 @@
 :106660000098A30005003A0067006000E20F5000A8
 :1066700002003200D749A80075FF0700000006009D
 :10668000F0708000E30F500012003A0011508000BB
-:1066900000372000808F50000E003600412180001E
+:10669000203B2000808F50000E00360041218000FA
 :1066A000303EB000808F50000A003600D6A9A80006
 :1066B0008A28A8008C48A800015080000050800063
 :1066C000C208DD004400DE0080804000A144880054
@@ -4585,7 +4585,7 @@
 :101E60000098A30005003A0067006000E20F5000F0
 :101E700002003200D749A80075FF070000000600E5
 :101E8000F0708000E30F500012003A001150800003
-:101E900000372000808F50000E0036004121800066
+:101E9000203B2000808F50000E0036004121800042
 :101EA000303EB000808F50000A003600D6A9A8004E
 :101EB0008A28A8008C48A8000150800000508000AB
 :101EC000C208DD004400DE0080804000A14488009C
 
Convert Intel Hex to dsPIC33F assembly code  
rom:003348 00 37 20 00     mov.w      #0x370,W0  
--->  
rom:003348 20 3b 20 00     mov.w      #0x3b2,W0  

rom:008f48 00 37 20 00     mov.w      #0x370,W0  
--->  
rom:008f48 20 3b 20 00     mov.w      #0x3b2,W0  
```
The code snippet above is PL30 OVP set to 15V  

**PL30 Rev10 Firmware With OVP 15V**  
* [PL30 DSPIC33FJ64GS606 OVP 15V](firmware/Rev10/Patch/DSPIC33FJ64GS606.hex)

**PL11 Rev12 Firmware With OVP Disabled**  
* [PL11 PIC16F883 OVP Disabled](firmware/Rev12/Patch/PIC16F883.hex)

### Load Test
Load testing with BMW ignition ON, fan runs at max with AC on, the output voltage is 14.09 without load. However, when loaded, there is a voltage drop due to impedance in the wires, which is reasonable.

***PL30 BMW Load With Fan On***
![alt text][image16]
![alt text][image17]

***PL11 BMW Load With Fan On***
![alt text][image18]
