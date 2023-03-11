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

### Enable PSU
Add a 330ohms resistor between Pin 33 and 36 to enable PSU.  
33: ENABLE#  
36: PRESENT  


### In-Circuit Serial Programming and Debugging Pinout

![alt text][image7]

Pin1 - PGEC  
Pin2 - PGED  
Pin3 - <span style="text-decoration:overline">MCLR</span>  
Pin4 - GND  
Pin5 - Vcc(3.3V)  

![alt text][image4]
![alt text][image5]

* [Molex Connector - 218112-0504](https://www.mouser.com/ProductDetail/538-218112-0504)
* [218112-0504 Datasheet](https://www.mouser.com/datasheet/2/276/3/2181120504_CABLE_ASSEMBLIES-2864576.pdf)

### Rev10 Firmware

* [Original DSPIC33FJ64GS606](firmware/Rev10/DSPIC33FJ64GS606.hex)

### Tweak Output Voltage
Connector a parallel resistor "R" together with original resistor(220ohms, "221") to increase up to 14.09V,Its value is 1k5
![alt text][image8]
![alt text][image9]
![alt text][image12]

### Tweak Over Voltage Protection
![alt text][image10]
Assuming the default OVP is 14V.  
There are two methods to modify the OVP. The first method is to connect a resistor in parallel with the 01B resistor, and the second method is to modify the firmware of the MCU(dsPIC33FJ64GS606).
The attached file is the firmware that I have modified with an OVP set to 15V.

* [DSPIC33FJ64GS606 OVP 15V](firmware/Rev10/Patch/DSPIC33FJ64GS606.hex)
