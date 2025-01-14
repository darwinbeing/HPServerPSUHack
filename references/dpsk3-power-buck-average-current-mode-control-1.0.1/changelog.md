# dpsk3-power-buck-average-current-mode-control, release v1.0.1

### Features Added\Updated

In MPLAB&reg; PowerSmart&trade; Digital Control Library Designer, release 0.9.14.676 labels of user extension function hooks changed, causing the compiler to fail when new control code is generated. This firmware release ensures compatibility with the latest version of this code generating tool.

When you download this version of the firmware, please also update your version of PowerSmart&trade; Digital Control Library Designer to version 0.9.14 or higher:
[MPLAB&reg; PowerSmart&trade; Digital Control Library Designer v0.9.14](https://microchip-pic-avr-tools.github.io/powersmart-dcld/)


# dpsk3-power-buck-average-current-mode-control, release v1.0.0

### Release Highlights
This is the initial release of the average current mode control code example demonstrating how to establish a 
digital dual-loop average current mode control implementation regulating the output voltage of a synchronous buck converter. 

This example is based on the code example demonstrating basic voltage mode control of the synchronous buck converter on the 
Digital Power Starter Kit 3. The firmware remains almost identical with the exception of the current loop controller configuration
in file `app_power_config.c` and the control loop library extension `i_loop`.

### Features Added\Updated
This code example runs automatically after power has been applied to the development board. In this initial version 
the on-board LED of the dsPIC33CK Digital Power Plug-In Module is toggled with a interval period of 300 ms, when the 
controller is running at maximum speed of 100 MIPS. After startup, PWM generator #1 is generating a 500 kHz complementary 
waveform at the PWM1H/PWM1L outputs. The output voltage feedback is read through ADC input #13 and processed through the 
digital control loop in the respective interrupt service routine. The voltage loop calculates the reference current required
to provide the demanded output power to maintain a constant output voltage based on the voltage error detected. This reference
value is picked up the the inner current loop, where it gets compared to the most recent feedback input value of the inductor
current. The deviation between both is then fed into the current loop compensation filter and passed through the anti-windup 
limiter. At the end of the control loop execution the result is written to the PWM duty cycle register, eventually regulating 
the output voltage to the nominal 3.3 V output voltage.

During operation the most recent values of input voltage, output voltage, output current and temperature are displayed on 
the on-board LCD. The screens can be switched by pressing the `User` button for more than 1 second.

### Supported Hardware Revisions:
Digital Power Starter Kit 3, Revision 3.0 (Part-No. 330017-3)
Visit [www.microchip.com/330017-3](https://www.microchip.com/330017-3) for more information
