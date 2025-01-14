<p><a href="https://www.microchip.com" rel="nofollow"><img src="https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png" alt="Microchip Technology" style="max-width:100%;"></a></p>

Repo Name: p33c-peripheral-sfr-abstraction-drivers

## Set of dsPIC33C Peripheral Register Abstraction Layer (PRAL) Drivers
**Special Funtion Register Abstraction Declarations and Low-Level Read/Write Functions**

### Description:
Peripheral Special Function Register Abstraction Layer Drivers (PRAL) are used to provide a simple and direct way of accessing peripheral instances which are not known at compile time.
These drivers provide an abstracted version of the dedicated register set of a peripheral type as data structure. Pointer based handlers are used to assign a generic data structure to a dedicated register set in user code which will get resolved at runtime.
The API of these drivers provide simple low level access functions

  - GetHandle(uint16_t Instance)  
    Returns the memory address of the first register of the register set of a dedicated peripheral instance. Mapping this memory address to a user-defined peripheral register abstraction data structure allows direct access to each peripheral SFR and subsequently to its register bits. The peripheral registers are auto-extracted from the first instance of the peripheral from the XC16 device header file of the recently selected device making sure all supported register bits are properly represented.  
<br>

  - ConfigRead(uint16_t Instance)  
	Returns a peripheral register abstraction data structure loaded with the recent register settings of the given peripheral instance. This function can be used to read and verify register contents or copy & paste configurations across register instances.  
<br>

  - ConfigWrite(uint16_t Instance, P33C_xxx_SFR_t Config)  
	Writes an initialized peripheral register abstraction data structure to a given peripheral instance. This allows quick loading of peripheral configuration templates from user-defined, pre-initialized peripheral SFR abstraction data structure objects.  
<br>


### Usage Examples:  
Different Peripheral Register Macro Abstraction drivers may have different functions, depending on the architecture and structure of the peripheral registers. However, each of them has at least the three fundamental functions GetHandle, ConfigRead and ConfigWrite.

The following code examples for the high-speed PWM driver show how these three fundamental functions are used. 

#### a) GetHandle(uint16_t Instance) Example
This example declares a generic PWM generator register pointer object which gets assigned to the dedicated PWM generator #3. After the assignment the generic data structure object is used to directly read from/write to the SFRs of the dedicated high-speed PWM generator. 
These read/write operations are equivalent to hard-coded register manipulation instructions but are now mappable to any PWM generator available without having to change/refactor user code.

```c
    #include "xc16_pral.h"

	(...)

	struct P33C_PWM_GENERATOR_s* pg;
    
	// Capture SFR memory address of PWM generator #3
	pg = p33c_PwmGenerator_GetHandle(3);
	
	// Access PWM generator #3 configuration register
	pg->PGxIOCONH.bits.PMOD = 1; // PWM generator #3 outputs operate in Independent mode
	pg->PGxCONL.bits.ON = 1; // enabling PWM generator #3
	
```

#### b) ConfigRead(uint16_t Instance) Example
This example declares a generic PWM generator register object which is used to read most recent settings from the special function registers of a given high-speed PWM generator instance. 

```c
    #include "xc16_pral.h"

	(...)

	struct P33C_PWM_GENERATOR_s pg;
    
	// Read SFR memory address of PWM generator #3
	pg = p33c_PwmGenerator_ConfigRead(3);
	
```

#### c) ConfigWrite(uint16_t Instance) Example
This example declares a generic PWM generator register object which is used to write a user-defined, generic high-speed PWM generator register settings to the special function registers of a given high-speed PWM generator instance. 

```c
    #include "xc16_pral.h"

	(...)

	struct P33C_PWM_GENERATOR_s pg;

	// Initialize abstracted PWM configuration
	pg.PGxCONL.bits.MODSEL = 0b000; // PWM timebase counter mode selection = Independent Edge PWM mode
	pg.PGxCONL.bits.CLKSEL = 0b01; // Clock selection = MCLKSEL[1:0] control bits
	pg.PGxCONL.bits.HREN = 1 // enable high resolution mode

	// Write configuration to PWM generator #3 configuration registers
	p33c_PwmGenerator_ConfigWrite(3, pg);
```



### Contents
Peripheral Register Macro Abstraction Drivers in this Repository:

  - p33c_adc:	(in development)
  - p33c_ccp:	Input Capture/Output Compare/Timer peripheral (SCCP / MCCP) driver
  - p33c_dac:	Analog high-speed comparator with integrated Digital-To-Analog converter driver
  - p33c_dsp:	CPU configuration register set abstraction driver
  - p33c_gpio:	General purpose I/O register set abstraction driver
  - p33c_pps:	Peripheral Pin Select (PPS) driver
  - p33c_pwm:	High-speed PWM register set abstraction driver
  
Extension Files:

  - p33c_macros:	 Set of CPU in-line macros not covered by Built-In Functions
  - p33smps_devices: Additional defines for enhanced filtering of product families

### Required Tools:
  - (none)

### Additional Resources:
  - (none)

---
##### History:
  - | 10/14/2020  | 1.0  |  M91406  | Created Repository
  - | 10/22/2020  | 1.1  |  M91406  | Updated CCP driver and added peripheral instance selection macros to PPS driver
