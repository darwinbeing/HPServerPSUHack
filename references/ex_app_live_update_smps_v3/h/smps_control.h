/**************************************************************************
  Company:
    Microchip Technology Inc.
  File Name:
    smps_interfaces.h
    
    
  Summary:
    This header file lists the interfaces used by the Switch Mode Power
    Supply compensator library.
    
    
  Description:
    SMPS Control (Compensator) library interface header file
    
    This header file lists the type defines for structures used by the SMPS
    library. Library function definitions are also listed along with
    information regarding the arguments of each library function.          
  **************************************************************************/

// DOM-IGNORE-BEGIN
////////////////////////////////////////////////////////////////////////////////
// © 2015 Microchip Technology Inc.
//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
// derivatives created by any person or entity by or on your behalf, exclusively
// with Microchip’s products.  Microchip and its licensors retain all ownership
// and intellectual property rights in the accompanying software and in all
// derivatives here to.
//
// This software and any accompanying information is for suggestion only.  It
// does not modify Microchip’s standard warranty for its products.  You agree
// that you are solely responsible for testing the software and determining its
// suitability.  Microchip has no obligation to modify, test, certify, or
// support the software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
// EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
// PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP’S PRODUCTS,
// COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
//
// IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
// (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
// INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
// EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
// ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
// MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
// TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
// CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
// FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//
// MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
// TERMS.
////////////////////////////////////////////////////////////////////////////////
// DOM-IGNORE-END

#ifndef _SMPS_CONTROL_H		// Guards against multiple inclusion
#define _SMPS_CONTROL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types 
// *****************************************************************************
// *****************************************************************************

//**************************************************************************
/* Data type for the 3-pole 3-zero (3P3Z) controller

  Summary:
    Data type for the 3-pole 3-zero (3P3Z) controller
	
  Description:
    The 3P3Z controller is the digital implementation of the analog type III 
    controller. This is a filter which generates a compensator characteristic 
    considering three poles and three zeros. This controller requires four
    feedback errors multiplied by their associated coefficients plus the three
    latest controller output values multiplied by their associated coefficients
    along the delay line to provide proper compensation of the power converter.
    The coefficients are determined externally using simulation tools.
     
    The SMPS_3P3Z_T data structure contains a pointer to derived
    coefficients in X-space and pointer to error/control history samples in
    Y-space. User must declare variables for the derived coefficients and
    the error history samples.
    
    The abCoefficients referenced by the SMPS_3P3Z_T data structure are
    derived from the coefficients B0-B3 plus A1-A3. These will be declared
    in external arrays. The SMPS_3P3Z_T data structure just holds pointers
    to these arrays.
      
    The coefficients will be determined by simulation tools, their outputs are
    given as floating point numbers. These numbers will be copied into the
    declared arrays after they have been converted into 16-bit integer numbers.
                                                                 
*/
        
typedef struct
{
    int16_t*    aCoefficients ;         // Pointer to A coefficients located in X-space
    int16_t*    bCoefficients ;         // Pointer to B coefficients located in X-space
    int16_t*    controlHistory ;	// Pointer to 3 delay-line samples located in Y-space
                                        // with the first sample being the most recent
    int16_t*    errorHistory ;          // Pointer to 4 delay-line samples located in Y-space
                                        // with the first sample being the most recent
    uint16_t    preShift ;		// Normalization from ADC-resolution to Q15 (R/W)
    int16_t     postShift ;		// Normalization bit-shift from Q15 to PWM register resolution (R/W)
    int16_t     postScaler ;		// Controller output post-scaler  (R/W)
    uint16_t    minOutput ;		// Minimum output value used for clamping (R/W)
    uint16_t    maxOutput ;		// Maximum output value used for clamping (R/W)
} SMPS_3P3Z_T;

// *****************************************************************************
/* Data type for the 2-pole 2-zero (2P2Z) controller

  Summary:
	Data type for the 2-pole 2-zero (2P2Z) controller
	
  Description:
	The 2P2Z controller is the digital implementation of the analog type II 
	controller. This is a filter which generates a compensator characteristic 
	considering two poles and two zeros. This controller requires three feedback 
	error multiplied by their associated coefficients plus the two latest 
	controller output values multiplied by their associated coefficients along 
	the delay line to provide proper compensation of the power converter. 
	The coefficients are determined externally using simulation tools.

    The SMPS_2P2Z_T data structure contains a pointer to derived
    coefficients in X-space and pointer to error/control history samples in
    Y-space. User must declare variables for the derived coefficients and
    the error history samples.
    
    The abCoefficients referenced by the SMPS_2P2Z_T data structure are
    derived from the coefficients B0-B2 plus A1-A2. These will be declared
    in external arrays. The SMPS_2P2Z_T data structure and just holds
    pointers to these arrays.
    
    The coefficients will be determined by simulation tools, which output
    is given as floating point numbers. These numbers will be copied into
    the declared arrays after they have been converted into 16-bit integer
    numbers.                                                               
*/ 
typedef struct
{	
    int16_t*    aCoefficients ;         // Pointer to A coefficients located in X-space
    int16_t*    bCoefficients ;         // Pointer to B coefficients located in X-space
    int16_t*    controlHistory ;	// Pointer to 2 delay-line samples located in Y-space
                                        // with the first sample being the most recent
    int16_t*    errorHistory ;          // Pointer to 3 delay-line samples located in Y-space
                                        // with the first sample being the most recent
    uint16_t    preShift ;		// Normalization from ADC-resolution to Q15 (R/W)
    int16_t     postShift ;		// Normalization bit-shift from Q15 to PWM register resolution (R/W)
    int16_t     postScaler ;		// Controller output post-scaler  (R/W)
    uint16_t    minOutput ;		// Minimum output value used for clamping (R/W)
    uint16_t    maxOutput ;		// Maximum output value used for clamping (R/W) 
} SMPS_2P2Z_T ;

//***********************************************************************
/* Data type for the PID controller

  Summary:
    Data type for the PID controller 
	
  Description:
	Data type for the Proportional Integral Derivative (PID) controller
    
    This digital implementation of a PID controller is a filter which
    generates a compensator characteristic considering the values of the
    coefficients KA, KB, KC these coefficients will determine the
    converter's frequency response. These coefficients are determined
    externally using simulation tools.
        
    This function call includes the pointer to the controller data
    structure, pointer of the input source register, control reference
    value, and to the pointer to the output register.                   
 */
typedef struct {

    int16_t*  abcCoefficients;	// Pointer to A, B & C coefficients located in X-space
                                // These coefficients are derived from
                                // the PID gain values - Kp, Ki and Kd
    int16_t*  errorHistory;	// Pointer to 3 delay-line samples located in Y-space
				            // with the first sample being the most recent
    int16_t  controlHistory;    // Stores the most recent controller output (n-1)
    int16_t  postScaler;	// PID basic Coefficient scaling Factor
    int16_t  preShift ;		// Normalization from ADC-resolution to Q15 (R/W)
    int16_t  postShift;		// Normalization from DSP to PWM register
    uint16_t minOutput;		// Minimum output value used for clamping
    uint16_t maxOutput;		// Maximum output value used for clamping


} SMPS_PID_T;




// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/************************************************************************
  Function:
    void SMPS_3P3ZInitialize( SMPS_3P3Z_T *controller_data )
    
  Summary:
    This function clears the SMPS_3P3Z_T data history structure arrays
	
  Description:
    This function clears the SMPS_3P3Z_T data history structure arrays.
	It's recommended to clear the error-history and controller-history 
	arrays before 3P3Z controller implementation.
  Conditions:
    None.
  Input:
    SMPS_3P3Z_T * -  This parameter is a pointer to a SMPS_3P3Z_T type
                     structure
  Return:
    Void.
  Example:
    <code lang="c++">
    SMPS_3P3Z_T controller3P3Z;
    SMPS_3P3ZInitialize(&amp;controller3P3Z);
    </code>
                                                                         
  ************************************************************************/ 

void SMPS_Controller3P3ZInitialize(SMPS_3P3Z_T *controllerData );

/*****************************************************************************************
  Function:
    \ \ 
    
    
    <code lang="c++">
        void SMPS_Controller3P3ZUpdate(SMPS_3P3Z_T* controllerData,
                                     volatile uint16_t* controllerInputRegister,
                                     int16_t reference,
                                     volatile uint16_t* controllerOutputRegister);
    </code>
    
    
    
    
  Summary:
    This function calls the SMPS_Controller3P3ZUpdate controller
	
  Description:
    This function updates the 3P3Z controller and can be called as soon as the 
	coefficients have been loaded into their arrays.

  Conditions:
    Before the controller can be used, it has to be initialized. The data
    structure has to be filled by copying the pointers to the coefficient,
    error and controller history arrays to the structure and the physical
    clamping limits of the output value. In the function call pointers to
    the Input source register, reference value, and pointer to the output
    register need to be called.
	
  Input:
    SMPS_3P3Z_T * controllerData -        This parameter is a pointer to a
                                          SMPS_3P3Z_T type structure<p />
    uint16_t* controllerInputRegister -   This parameter is a pointer to the
                                          \input source register or variable
                                          being tracked by the 3P3Z (e.g.
                                          ADCBUF0).<p />
    int16_t reference -                   This parameter is a signed integer
                                          value that will be used by the
                                          controller as the feedback
                                          reference or set\-point.<p />
    uint16_t* controllerOutputRegister -  This parameter is a pointer to the
                                          Control loop target register of
                                          the calculated result(e.g. PDC1).
  Return:
    void
  Example:
    <code lang="c++">
    int16_t controlReference;
    SMPS_3P3Z_T controller3P3Z;
    SMPS_Controller3P3ZUpdate(&amp;controller3P3Z,&amp;ADCBUF0,controlReference,&amp;PDC1)
    </code>
                                                                                          
  *****************************************************************************************/ 


void SMPS_Controller3P3ZUpdate(SMPS_3P3Z_T* controllerData,
                               volatile uint16_t* controllerInputRegister,
			       int16_t reference,
			       volatile uint16_t* controllerOutputRegister);


void SMPS_Controller3P3Z(void);



/************************************************************************
  Function:
          void SMPS_2P2ZInitialize(void)
    
  Summary:
    This function clears the SMPS_2P2Z_T data structure arrays
	
  Description:
    This function clears the SMPS_2P2Z_T data history structure arrays.
	It's recommended to clear the error-history and controller-history 
	arrays before 2P2Z controller implementation.
	
  Conditions:
    None.
  Input:
    SMPS_2P2Z_T* -  This parameter is a pointer to a SMPS_2P2Z_T type
                    structure
  Return:
    void
  Example:
    <code lang="c++">
    SMPS_2P2Z_T controller2P2Z;
    SMPS_Controller2P2ZUpdateInitialize(&amp;controller2P2Z);
    </code>
                                                                         
  ************************************************************************/ 

void SMPS_Controller2P2ZInitialize(SMPS_2P2Z_T *controllerData);


/*****************************************************************************************
  Function:
    \ \ 
    
    
    <code>
        void SMPS_Controller2P2ZUpdate(SMPS_2P2Z_T* controllerData,
                                       volatile uint16_t* controllerInputRegister,
                                       int16_t reference,
                                       volatile uint16_t* controllerOutputRegister);
    
    </code>
  Summary:
    This function calls the SMPS_Controller2P2ZUpdate controller
	
  Description:
    This function updates the 2P2Z controller and can be called as soon as the 
	coefficients have been loaded into their arrays.
	
  Conditions:
    Before the controller can be used, it has to be initialized. The data
    structure has to be filled by copying the pointers to the coefficient,
    error and controller history arrays to the structure and the physical
    clamping limits of the output value. In the function call pointers to
    the Input source register, reference value, and pointer to the output
    register need to be called.
  Input:
    SMPS_2P2Z_T * controllerData -        This parameter is a pointer to a
                                          SMPS_2P2Z_T type structure<p />
    uint16_t* controllerInputRegister -   This parameter is a pointer to the
                                          \input source register or variable
                                          being tracked by the 2P2Z (e.g.
                                          ADCBUF0).<p />
    int16_t reference -                   This parameter is a signed integer
                                          value that will be used by the
                                          controller as the feedback
                                          reference or set\-point.<p />
    uint16_t* controllerOutputRegister -  This parameter is a pointer to the
                                          Control loop target register of
                                          the calculated result(e.g. PDC1).
  Return:
    void
  Example:
    <code lang="c++">
    int16_t controlReference;
    SMPS_2P2Z_T controller2P2Z;
    SMPS_Controller2P2ZUpdate(&amp;controller2P2Z,&amp;ADCBUF0,controlReference,&amp;PDC1)
    </code>
                                                                                          
  *****************************************************************************************/ 

void SMPS_Controller2P2ZUpdate(SMPS_2P2Z_T* controllerData,
                               volatile uint16_t* controllerInputRegister,
                               int16_t reference,
                               volatile uint16_t* controllerOutputRegister);


/****************************************************************************
  Function:
          void SMPS_PIDInitialize( SMPS_PID_T *controller_data )
    
  Summary:
    This function clears the SMPS_PID_T data structure arrays
	
  Description:
    This function clears the SMPS_PID_T data history structure arrays.
	It's recommended to clear the error-history and controller-history 
	arrays before PID controller implementation.
	
  Conditions:
    None.
  Input:
    SMPS_PID_T* -  This parameter is a pointer to a SMPS_PID_T type structure
  Return:
    void
  Example:
    <code lang="c++">
    SMPS_PID_T controllerPID;
    SMPS_ControllerPIDUpdateInitialize(&amp;controllerPID);
    </code>
                                                                             
  ****************************************************************************/ 
void SMPS_ControllerPIDInitialize(SMPS_PID_T *controllerData );

/***************************************************************************************
  Function:
    \ \ 
    
    
    <code>
      void SMPS_ControllerPIDUpdate(SMPS_PID_T* controllerData,
                                   volatile uint16_t* controllerInputRegister,
                                   int16_t reference,
                                   volatile uint16_t* controllerOutputRegister);
    </code>
    
    
    
    
  Summary:
    This function calls the SMPS_ControllerPIDUpdate controller
	
  Description:
    This function updates the PID controller and can be called as soon as the 
	coefficients have been loaded into their arrays.
	
  Conditions:
    Before the controller can be used, it has to be initialized. The data
    structure has to be filled by copying the pointers to the coefficient,
    error and controller history arrays to the structure and the physical
    clamping limits of the output value. In the function call pointers to
    the Input source register, reference value, and pointer to the output
    register need to be called.
	
  Input:
    SMPS_PID_T * controllerData -         This parameter is a pointer to a
                                          SMPS_PID_T type structure<p />
    uint16_t* controllerInputRegister -   This parameter is a pointer to the
                                          \input source register or variable
                                          being tracked by the PID (e.g.
                                          ADCBUF0).<p />
    int16_t reference -                   This parameter is a signed integer
                                          value that will be used by the
                                          controller as the feedback
                                          reference or set\-point.<p />
    uint16_t* controllerOutputRegister -  This parameter is a pointer to the
                                          Control loop target register of
                                          the calculated result(e.g. PDC1).
  Return:
    void
  Example:
    <code lang="c++">
    int16_t controlReference;
    SMPS_PID_T controllerPID;
    SMPS_ControllerPIDUpdate(&amp;controllerPID,&amp;ADCBUF0,controlReference,&amp;PDC1)
    </code>
                                                                                        
  ***************************************************************************************/
void SMPS_ControllerPIDUpdate(SMPS_PID_T* controllerData,
                              volatile uint16_t* controllerInputRegister,
                              int16_t reference,
                              volatile uint16_t* controllerOutputRegister);


#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

#endif // _SMPS_CONTROL_H

/* EOF */


