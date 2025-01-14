# Fault Handler Function Driver

<span id="startDoc"> </span> <!-- start tag for internal references -->

### Class
PowerSmart™ Firmware Framework Component

### Summary
Generic fault handler supporting comparison of static and dynamic data with automated fault trip and release function hooks. It further provides interrupt service routines for uninterruptible interrupts (traps) and a trap logger, capturing the status of CPU registers at the time when an uninterruptible interrupt is triggered (e.g. divide-by-zero, address error, oscillator failure, etc.). 

### Table Of Contents

- [Introduction](#intro)
- [Library Usage Guidelines](#lib_use)
- [API Quick-Start Guide](#api_guide)
- [API Public Data Object](#api_objects)
- [History](#history)


<span id="intro"> </span>
### Introduction

#### Description
This generic fault handler driver declares a fault object data structure, which can be used in user code to define various different fault objects, which will get monitored and managed by  fault handler functions.

The main Execute function of this fault handler is restricted to detecting fault conditions, defined as filtered threshold violations by comparing the value of the defined fault object source variable against user-defined trip and release thresholds. Fault condition is triggered when the monitored value of a variable or register is exceeding a defined threshold n consecutive times. Consequently, a fault condition is cleared when the monitored value of the variable is found to be within a valid range m consecutive times.

The numbers n and m used for triggering/clearing a fault condition is set as parameter in user code and effectively provide filtering options influencing the sensitivity of the respective fault condition detection.

#### Fault Object Types:

* Static: One 16-bit wide SFR or user variable compared against 16-bit wide constant
* Dynamic: One 16-bit wide SFR or user variable compared against another 16-bit wide SFR or user variable 
* Each variable is/can be supported by bit masks supporting range or bit-wise comparisons

#### Compare Functions:

* None (no comparison takes place, fault object ignored)
* Greater Than
* Less Than
* Is Equal
* Is Not Equal
* Between Thresholds
* Outside Thresholds

#### Fault Indication/Fault Response:

* Immediate indication of fault condition through status bit in fault object status word 
(set/cleared when change in fault condition is detected)
* Immediate indication of fault status through status bit in fault object status word 
(set/cleared when fault trips/resets)
* Automated trigger of user-defined software function when a fault condition trips
* Automated trigger of user-defined software function when a fault condition is released

[[back](#startDoc)]

<span id="lib_use"> </span>
### Library Usage Guidance

This code library repository must be included in an independent directory within the target project. Changes to any of the files of this repository in the target project environment can be pushed back to this library repository from where these changes can be distributed to one, more or all target projects this library has been used in. The upgrade of library files in a particular target project needs to be performed individually, allowing to individually manage the device driver version used.

#### Adding Subtree Repository

##### Step 1 - Adding Subtree Repository to Target Project
###### a) Using Sourcetree Application

Open *Sourcetree* Application
* Open menu *Repository* → *Repository Settings*
* Click *Add* to open the Add Repository dialog

Inside the Repository dialog
* Enter *Remote Name* `subrepo-fault`
* Enter clone URL/path https://bitbucket.microchip.com/scm/mcu16asmpsl/subrepo-fault-handler.git
* Under *Optional extended integration* → *Remote Account* select your user account
* Click *OK* to close the dialog

###### b) Using Git Bash

Open Git Bash at root of target project repository and use the following command for adding the new subtree remote:

* `$ git remote add subrepo-fault https://bitbucket.microchip.com/scm/mcu16asmpsl/subrepo-fault-handler.git`

##### Step 2) Cloning Subtree Repository

Further using Git Bash, use the `subtree add` command to clone the contents of this code library to the target project

* `$ git subtree add --prefix=<code_library_target> subrepo-fault main  --squash`

with `<code_library_target>` = path to sub-folder within the target project directory structure this code library should be cloned to (e.g. my_project.X/sources/fault_handler/drivers).

#### 3) Pulling latest version from Library Repository
When a new version of this code library is available, you can pull it by using the `subtree pull` command in the Git Bash:

`$ git subtree pull --prefix=<code_library_target> subtree-fault main --squash`

with `<code_library_target>` = path to sub-folder within the target project directory structure this code library has been cloned to (e.g. my_project.X/sources/fault_handler/drivers).

#### 4) Pushing new version back to Library Repository
When changes have been made to files of this code library, the new version can be pushed back to the library repository by using the `subtree push` command in the Git Bash:

`$ git subtree push --prefix=<code_library_target> subtree-fault feature/version-update --squash`

with `<code_library_target>` = path to sub-folder within the target project directory structure this code library has been cloned to (e.g. my_project.X/sources/fault_handler/drivers).

<span style="color:red">
    <u><b>Note:</b></u><br>
    Pushing directly to the library project 'main' or 'develop' branches may be prohibited. 
    Hence, <i><u><b>changes can only be pushed to feature branches</b></u></i>. A Pull Request is required to review and merge changes to 'develop'. Once changes have been approved and merged int 'develop', they may be merged into branch 'main' and thus released as new version through another pull request. This new version of 'main' can be tagged with a new version number and pulled into target projects.
</span>


<br>&nbsp;

[[back](#startDoc)]

<span id="api_guide"> </span>
### API Quick-Start Guide

#### API Public Functions

The API of this function driver provides the following global functions:

```c
volatile uint16_t drv_FaultHandler_CheckObject(volatile struct FAULT_OBJECT_s* fltobj);
volatile uint16_t drv_TrapHandler_SoftTrapsInitialize(
                bool accumulator_a_overflow_trap_enable, 
                bool accumulator_b_overflow_trap_enable, 
                bool accumulator_catastrophic_overflow_trap_enable);
```

* **`uint16_t drv_FaultHandler_CheckObject(volatile struct FAULT_OBJECT_s* fltobj)`**<br>&nbsp;
This function executes the comparison between the defined fault object variable or register and its thresholds, tracking the most recent fault condition of the object by incrementing or clearing fault threshold violation counters.
This function takes a function call parameter of type *struct FAULT_OBJECT_s* (see blow) on which the fault condition analysis is performed and returns an integer return value indicating success (= 1) or failure (= 0) of the function execution. The return value ***does not*** reflect the fault condition but only verifies if the ***execution of the fault check function*** was successful. The result of the fault check function can be read from the status bits of the fault object fltobj of type `FAULT_OBECT_s`.<br>&nbsp;<br>


* **`uint16_t drv_TrapHandler_SoftTrapsInitialize( bool, bool, bool)`**<br>&nbsp;
This function initializes the use of so-called *Soft Traps*. Soft traps define conditions which can be considered critical enough to be treated like other uninterruptible interrupts triggered by hardware errors (e.g. address error). 
In this version the trap handler allows to treat DSP accumulator saturation events be treated as uninterruptible interrupt event or be ignored. This function can be used to enable/disable trap-treatment of saturation event (32-bit overflow condition) of accumulator a and/or b and also provides an additional, independent trap option for super-saturation events of any accumulator (40-bit overflow condition) <br>&nbsp;<br>


[[back](#startDoc)]

<span id="api_objects"> </span>
#### API Public Data Objects

##### 1) Fault Handler

The fault handler monitors a so-called fault object of type `FAULT_OBJECT_t`

```c
typedef struct FAULT_OBJECT_s {

	volatile struct FLT_OBJECT_STATUS_s  Status;           // Status word of this fault object
	volatile uint16_t Counter;                             // Fault event counter (controlled by FAULT HANDLER)
	volatile struct FLT_COMPARE_OBJECT_s SourceObject;     // Object which should be monitored
	volatile struct FLT_COMPARE_OBJECT_s ReferenceObject;  // Reference object the source should be compared with
    volatile struct FLT_EVENT_RESPONSE_s TripResponse;     // Settings defining the fault trip event
    volatile struct FLT_EVENT_RESPONSE_s RecoveryResponse; // Settings defining the fault recovery event

} FAULT_OBJECT_t; // Generic fault object
```
<br>&nbsp;

##### Sub Structure  `struct FLT_OBJECT_STATUS_s`

This data structure defines the fault object status.

```c
typedef struct FLT_OBJECT_STATUS_s{

    union {
	struct {
		volatile bool FaultStatus : 1;                      // Bit 0: Flag bit indicating if FAULT has been tripped
		volatile bool FaultActive : 1;                      // Bit 1: Flag bit indicating if fault condition has been detected but FAULT has not been tripped yet
		volatile unsigned : 6;                              // Bit <7:2>: (reserved)
		volatile enum FLT_COMPARE_TYPE_e CompareType: 3;	// Bit <10:8>: Fault check comparison type control bits
		volatile unsigned : 4;                              // Bit 14: (reserved)
		volatile bool Enabled : 1;                          // Bit 15: Control bit enabling/disabling monitoring of the fault object
	} __attribute__((packed)) bits;            // Fault object status bit field for single bit access  

	volatile uint16_t value;		// Fault object status word  
    };
    
} FLT_OBJECT_STATUS_t;	// Fault object status
```
<br>&nbsp;

##### Sub Structure  `struct FLT_COMPARE_OBJECT_s`

This data structure defines the data object which will be monitored by the fault handler.

```c
typedef struct FLT_COMPARE_OBJECT_s {
    
    volatile uint16_t* ptrObject;   // Pointer to register or variable which should be monitored 
    volatile uint16_t bitMask;      // Bit mask will be &-ed with source as value (use 0xFFFF for full value comparison)
    
} FLT_COMPARE_OBJECT_t;	// Fault compare object
```
<br>&nbsp;

##### Sub Structure  `struct FLT_EVENT_RESPONSE_s`

This data structure defines the fault monitor event response object.

```c
typedef struct FLT_EVENT_RESPONSE_s {
    
    volatile uint16_t compareThreshold;             // Signal level at which the fault condition will be detected
    volatile uint16_t eventThreshold;               // Bit mask will be &-ed with source as value (use 0xFFFF for full value comparison)
    volatile uint16_t (*ptrResponseFunction)(void); // pointer to a user-defined function called when a defined fault monitoring event is detected
    
} FLT_EVENT_RESPONSE_t;	// Fault monitor event response object
```
<br>&nbsp;

##### 2) Trap Handler

The trap handler provides a public variable of type `struct TRAP_LOGGER_s`. This data structure holds information about the most recent status of uninterruptible interrupts. As any occurring trap may result in an immediate warm CPU reset, this variable is declared with the attribute `__perisistent__` to keep the stored information available in RAM after the CPU reset to allow the user software to analyze which trap has caused the reset and what the CPU and task conditions were at the event.

```c
typedef struct TRAP_LOGGER_s 
{
    volatile struct TRAPLOG_STATUS_s status;    // Status word of the Trap Logger object
    volatile uint16_t reset_count;              // Counter of CPU RESET events (read/write)
    volatile enum TRAP_ID_e trap_id;            // Trap-ID of the captured incident
    volatile uint16_t trap_count;               // Counter tracking the number of occurrences
    volatile struct TRAP_FLAGS_s trap_flags;    // Complete list of trap flags (showing all trap flags)
    volatile struct CPU_RCON_s rcon_reg;        // Captures the RESET CONTROL register
    volatile struct CPU_INTTREG_s inttreg;      // Interrupt Vector and Priority register capture
    volatile struct TASK_INFO_s task_capture;   // Information of last task executed
    
} TRAP_LOGGER_t; // Global data structure for trap event capturing
```
<br>&nbsp;

##### Sub Structure  `struct TRAPLOG_STATUS_s`

If the control bit `cpu_reset_trigger` in data structure `TRAPLOG_STATUS_s` is enabled in user code, the CPU is reset automatically after the trap condition has been captured. 

```c
typedef struct TRAPLOG_STATUS_s 
{
    union {
    struct {

        // Control bits
        volatile bool cpu_reset_trigger : 1;    // Bit 0: Control bit to trigger software-enforced CPU reset
        volatile unsigned : 1;                  // Bit 1: (reserved)
        volatile unsigned : 1;                  // Bit 2: (reserved)
        volatile unsigned : 1;                  // Bit 3: (reserved)
        volatile unsigned : 1;                  // Bit 4: (reserved)
        volatile unsigned : 1;                  // Bit 5: (reserved)
        volatile unsigned : 1;                  // Bit 6: (reserved)
        volatile unsigned : 1;                  // Bit 7: (reserved)

        // Status bits
        volatile bool sw_reset : 1;             // Bit 8:  Flag indicating CPU was reset by software (read only)
        volatile unsigned : 1;                  // Bit 9:  (reserved)
        volatile unsigned : 1;                  // Bit 10: (reserved)
        volatile unsigned : 1;                  // Bit 11: (reserved)
        volatile unsigned : 1;                  // Bit 12: (reserved)
        volatile unsigned : 1;                  // Bit 13: (reserved)
        volatile unsigned : 1;                  // Bit 14: (reserved)
        volatile unsigned : 1;                  // Bit 15: (reserved)

    }__attribute__((packed)) bits;

	volatile uint16_t value;
    };
    
} TRAPLOG_STATUS_t;
```
<br>&nbsp;

##### Sub Structure  `struct TRAP_FLAGS_s`

This 32-bit wide data structure holds status bits for every trap type supported by dsPIC33. This bit-field can be used to identify which trap triggered the CPU reset.

```c
typedef struct TRAP_FLAGS_s
{
    union {
    struct {

        volatile unsigned OVAERR    :1; // Bit #0:  Accumulator A Overflow Trap Flag bit
        volatile unsigned OVBERR    :1; // Bit #1:  Accumulator B Overflow Trap Flag bit
        volatile unsigned COVAERR   :1; // Bit #2:  Accumulator A Catastrophic Overflow Trap Flag bit
        volatile unsigned COVBERR   :1; // Bit #3:  Accumulator B Catastrophic Overflow Trap Flag bit
        volatile unsigned SFTACERR  :1; // Bit #4:  Shift Accumulator Error Status bit
        volatile unsigned DIV0ERR   :1; // Bit #5:  Divide-by-Zero Error Status bit
        volatile unsigned MATHERR   :1; // Bit #6:  Math Error Status bit
        volatile unsigned ADDRERR   :1; // Bit #7:  Address Error Trap Status bit
        volatile unsigned STKERR    :1; // Bit #8:  Stack Error Trap Status bit
        volatile unsigned OSCFAIL   :1; // Bit #9:  Oscillator Failure Trap Status bit
        volatile unsigned SWTRAP    :1; // Bit #10: Software Trap Status bit
        volatile unsigned NAE       :1; // Bit #11: NVM Address Error Soft Trap Status bit
        volatile unsigned DOOVR     :1; // Bit #12: DO Stack Overflow Soft Trap Status bit
        volatile unsigned APLL      :1; // Bit #13: Auxiliary PLL Loss of Lock Soft Trap Status bit
        volatile unsigned SGHT      :1; // Bit #14: Software Generated Hard Trap Status bit
        volatile unsigned DMACERR   :1; // Bit #15: DMA Trap Status bit

        volatile unsigned ECCDBE    :1; // Bit #16: ECC Double-Bit Error Trap Status bit
        volatile unsigned CAN       :1; // Bit #17: CAN Address Error Soft Trap Status bit
        volatile unsigned CAN2      :1; // Bit #18: CAN2 Address Error Soft Trap Status bit
        volatile unsigned           :13; // Bit <19:31> (reserved)

    }__attribute__((packed))bits;

    volatile uint32_t value;
    };
    
}TRAP_FLAGS_t;
```
<br>&nbsp;

##### Sub Structure `struct TASK_INFO_s`

The `TASK_INFO_s` data structure allows logging of user code information. To make best use of this feature, it is recommended to define unique IDs for tasks in the user application to help identify the code module which was active when a trap occurred. This data structure further allows the definition of a fault ID and operating mode ID to further enhance troubleshooting information.

*Example:*

With `traplog` being a global variable, any user code can set the task info fields `task_id`,`fault_id`, `op_mode` at any point at any time. By setting this information in every critical function/task of the user code, this information will be continuously updated. When a trap occurs, the information in these data fields will be captured and be available after the CPU reset. User code should evaluate the contents and status of `traplog` at the beginning of *main()* to make sure this information is captured and analyzed before overwritten by software processes.

```c
typedef struct TASK_INFO_s 
{
    volatile uint16_t task_id; // Task ID of last executed task
    volatile uint16_t fault_id; // Fault ID causing a catastrophic fault event
    volatile uint16_t op_mode; // Operating Mode ID of task manager

} TASK_INFO_t;
```
<br>&nbsp;

##### Sub Structures `struct CPU_RCON_s` and `struct CPU_INTTREG_s`

These data structures are shadow copies of the CPU RESET register RCON and Interrupt Control and Status register INTTREG. Both registers will be captured at a trap event and stored in the persistent `traglog` data object.

```c
typedef struct CPU_INTTREG_s
{
    union {
    struct {
        volatile unsigned VECNUM:8;	// Bit #0-7:  Pending Interrupt Number List
        volatile unsigned ILR	:4;	// Bit #8-11: New Interrupt Priority Level
        volatile unsigned		:1;	// Bit #12: Reserved
        volatile unsigned VHOLD :1; // Bit #13: Vector Number Capture Enable bit
        volatile unsigned       :1;	// Bit #14: Reserved
        volatile unsigned       :1;	// Bit #15: Reserved
    }__attribute__((packed))bits;

	volatile uint16_t value;
    };
    
} CPU_INTTREG_t;

typedef struct CPU_RCON_s 
{
    union {
    struct {

        volatile unsigned por	:1;	// Bit #0:  Power-on Reset Flag bit
        volatile unsigned bor	:1;	// Bit #1:  Brown-out Reset Flag bit
        volatile unsigned idle	:1;	// Bit #2:  Wake-up from Idle Flag bit
        volatile unsigned sleep	:1;	// Bit #3:  Wake-up from Sleep Flag bit
        volatile unsigned wdto	:1;	// Bit #4:  Watchdog Timer Time-out Flag bit
        volatile unsigned swdten:1;	// Bit #5:  Software Enable/Disable of WDT bit
        volatile unsigned swr	:1;	// Bit #6:  Software Reset Flag (Instruction) bit
        volatile unsigned extr	:1;	// Bit #7:  External Reset Pin (MCLR) bit
        volatile unsigned vregs	:1;	// Bit #8:  Voltage Regulator Standby During Sleep bit
        volatile unsigned cm    :1;	// Bit #9:  Configuration Mismatch Flag bit
        volatile unsigned		:1;	// Bit #10: Reserved
        volatile unsigned vregsf:1;	// Bit #11: Flash Voltage Regulator Standby During Sleep bit
        volatile unsigned		:1;	// Bit #12: Reserved
        volatile unsigned		:1;	// Bit #13: Reserved
        volatile unsigned iopuwr:1;	// Bit #14: Illegal Opcode or Uninitialized W Access Reset Flag bit
        volatile unsigned trapr :1;	// Bit #15: Trap Reset Flag bit

    }__attribute__((packed))bits;

	volatile uint16_t value;
    };
    
} CPU_RCON_t;

```
<br>&nbsp;



[[back](#startDoc)]

---
<span id="history"> </span>
##### History:
* 03/13/2020 v1.0 (M91406) Initial release
* 11/02/2020 v2.0 (M91406) Enhanced version with generic trap handler

---
© 2021, Microchip Technology Inc.

