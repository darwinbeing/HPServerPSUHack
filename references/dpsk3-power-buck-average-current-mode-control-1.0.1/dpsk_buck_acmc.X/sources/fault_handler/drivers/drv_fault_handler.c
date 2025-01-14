/*
 * File:   fault_handler.c
 * Author: M91406
 *
 * Created on December 27, 2019, 12:21 PM
 */
/**
 *  (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 *  Subject to your compliance with these terms, you may use Microchip software
 *  and any derivatives exclusively with Microchip products. You're responsible
 *  for complying with 3rd party license terms applicable to your use of 3rd
 *  party software (including open source software) that may accompany Microchip
 *  software.
 *
 *  SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 *  APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 *  MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 *  INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 *  WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
 *  HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
 *  THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
 *  CLAIMS RELATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF FEES, IF ANY,
 *  YOU PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 *  @file    drv_fault_handler.c
 *  @brief   This file contains APIs to check current fault status of a user-defined fault object
 *
 *  @note
 *  Microchip Technology Inc. has followed development methods required by
 *  IEC-60730 and performed extensive validation and static testing to ensure
 *  that the code operates as intended. Any modification to the code can
 *  invalidate the results of Microchip's validation and testing.
 *
 */

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stdlib.h> // include standard library data types and macros

#include "drv_fault_handler.h" // 


/*******************************************************************************
 * @var	FAULT_OBJECT_s fltObjectClear
 * @ingroup lib-layer-fault-properties-private-variables
 * @brief Clears the fault objects
 * @details
 *  The fltObjectClear object is used during pre-initialization of user-defined
 *  fault pbjects, clearing all settings and setting the FaultActive and
 *  FaultStatus fault object status flag bts. 
*****************************************************************************/
volatile struct FAULT_OBJECT_s fltObjectClear = 
{
        .Status.bits.CompareType = FLTCMP_NONE,     ///< Clear fault object comparison type
        .Status.bits.Enabled = false,               ///< Clear fault object enable bit
        .Status.bits.FaultActive = true,            ///< Set fault object ACTIVE bit (must be cleared by fault check)
        .Status.bits.FaultStatus = true,            ///< Set fault object STATUS bit (must be cleared by fault check)
        .Counter = 0,                               ///< Clear internal fault counter
        .SourceObject.ptrObject = NULL,             ///< Clear source object pointer
        .SourceObject.bitMask = 0xFFFF,             ///< Reset bit-filter mask to compare all bits
        .ReferenceObject.ptrObject = NULL,          ///< Clear reference object pointer
        .ReferenceObject.bitMask = 0xFFFF,          ///< Reset bit-filter mask to compare all bits
        .TripResponse.compareThreshold = 0,         ///< Clear fault trip signal level
        .TripResponse.eventThreshold = 0,           ///< Clear fault trip counter threshold
        .TripResponse.ptrResponseFunction = NULL,   ///< Clear fault trip response function pointer
        .RecoveryResponse.compareThreshold = 0,     ///< Clear fault recovery signal level
        .RecoveryResponse.eventThreshold = 0,       ///< Clear fault recovery counter threshold
        .RecoveryResponse.ptrResponseFunction = NULL,  ///< Clear fault recovery response function pointer
    };

/*******************************************************************************
 * @fn	uint16_t drv_FaultHandler_CheckObject(volatile struct FAULT_OBJECT_s* fltObject)
 * @ingroup lib-layer-fault-functions-public
 * @param	fltObject Pointer to a Fault Monitoring Object of type struct FAULT_OBJECT_s
 * @return  unsigned integer (0=failure, 1=success)
 *
 * @brief Check current fault status of a user-defined fault object
 *
 * @details
 * This routine compares the most recent value of a global variable or SFR
 * (SOURCE) against user-defined thresholds. If the value violates/exceeds
 * one or more of the user defined conditions, a fault counter is incremented. 
 * 
 * When this fault counter exceeds the user-defined maximum, a fault condition 
 * is tripped and the fault_status flag bit is set.
 * 
 * When a fault condition has been tripped and the fault status is set (=true), 
 * the fault check monitors for the source value triggering a recovery event. 
 * The recovery process works like the fault trip event by comparing the most 
 * recent source value against the user-defined RECOVERY_LEVEL. Once the value
 * has been RSTCNT_MAX times below the RECOVERY_LEVEL threshold, the fault 
 * will automatically be cleared.
 * 
 * a) Comparison Types
 * 
 * The fault handler offers the following different comparison methods:
 *      - Greater Than:
 *          - performs comparison SOURCE > TRIP_LEVEL
 * 
 *          - TRIP_LEVEL is greater than RECOVERY_LEVEL. The difference between 
 *          TRIP_LEVEL and RECOVERY_LEVEL is the hysteresis of the defined
 *          threshold.
 * 
 *      - Less Than:
 *          - performs comparison SOURCE < TRIP_LEVEL
 * 
 *          - TRIP_LEVEL is less than RECOVERY_LEVEL. The difference between 
 *          TRIP_LEVEL and RECOVERY_LEVEL is the hysteresis of the defined
 *          threshold.
 *
 *      - Is Equal:
 *          - performs comparison SOURCE == TRIP_LEVEL
 *
 *          - RECOVERY_LEVEL is ignored.
 *
 *      - Is Not Equal:
 *          - performs comparison SOURCE != TRIP_LEVEL
 *
 *          - RECOVERY_LEVEL is ignored.
 *
 *      - Between:
 *          - performs comparison RECOVERY_LEVEL < SOURCE < TRIP_LEVEL
 *
 *          - min/max of the FAULT range is defined by the range between
 *          RECOVERY_LEVEL (min) and TRIP_LEVEL (max)
 *
 *      - Outside:
 *          - performs comparison (SOURCE < RECOVERY_LEVEL) or (TRIP_LEVEL < SOURCE)
 *
 *          - min/max of the allowed operating range is defined by the range 
 *          between RECOVERY_LEVEL (min) and TRIP_LEVEL (max)
 * 
 * b) Value Filtering
 * 
 *    A bit-mask filter can be used to monitor individual register bits or filter
 *    on specific bits within a 16-bit wide value. The bit-mask is always applied 
 *    the source value. If no bit-mask is required, set bitmask = 0xFFFF; 
 * 
 * c) Comparison of two dynamic values
 * 
 *    When dynamic values of two variables need to be compared, the fault object
 *    offers a second pointer to a so-called reference object "ref_obj". When this
 *    pointer is != NULL, the absolute value of the difference between SOURCE and 
 *    REFERNCE is compared against the user-defined TRIP and RECOVERY thresholds.
 *    When no second  value is used, set ref_obj = NULL (NULL-pointer). This 
 *    will force the fault check to ignore this value.
 * 
 * d) User defined responses
 *    
 *    Function pointers "trip_response" and "reset_response" can be used 
 *    to assign user-defined functions which should be called when a fault
 *    condition has been tripped or reset.
 *    I no user-defined function should be called, these pointers can be 
 *    set = NULL (NULL-pointer)
 * 
 * @note If the value is within normal operating conditions, the fault 
 *      counter will be cleared. Thus fault events must occur successively 
 *      incrementing the fault event counter high enough to eventually 
 *      trip a fault event. Thus the fault event counter can be used to 
 *      adjust the sensitivity of the fault response.
 * 
 *****************************************************************************/

volatile uint16_t drv_FaultHandler_CheckObject(volatile struct FAULT_OBJECT_s* fltObject) {

    volatile uint16_t retval=1;
    volatile uint16_t source=0;
    
    // If the fault object is not initialized, exit here with error
    if (fltObject == NULL)
        return(0);

    // If FAULT CHECK is disabled, exit here
    if (!fltObject->Status.bits.Enabled) {
        fltObject->Counter = 0;                        // Clear Counter
        fltObject->Status.bits.FaultActive = false;   // Clear immediate fault flag
        fltObject->Status.bits.FaultStatus = false;   // Clear fault status flag
        return(1);  // Return success
    }
    
    // If the source object is not initialized, exit here with error
    if (fltObject->SourceObject.ptrObject == NULL)
        return(0);
    
    // Read most recent fault object value with bit-mask
    source = (*fltObject->SourceObject.ptrObject & fltObject->SourceObject.bitMask);
    
    // If a reference object has been defined, read reference object value and override source with 
    // absolute value of difference between source and reference object values
    if(fltObject->ReferenceObject.ptrObject != NULL) {  
        
        uint16_t reference = (*fltObject->ReferenceObject.ptrObject & fltObject->ReferenceObject.bitMask);
        source = (volatile uint16_t)
            abs((volatile int32_t)source - (volatile int32_t)reference); // Load most recent value
    }

    // Check fault condition

    switch(fltObject->Status.bits.CompareType) {

        case FLTCMP_GREATER_THAN:  // Check if SOURCE > TRIP_LEVEL
            
            if (source > fltObject->TripResponse.compareThreshold)  // Check if SOURCE > TRIP_LEVEL
                fltObject->Status.bits.FaultActive = true; // Set FAULT_ACTIVE status flag bit
            else if (source < fltObject->RecoveryResponse.compareThreshold) // Check if SOURCE < RECOVERY_LEVEL
                fltObject->Status.bits.FaultActive = false; // Clear FAULT_ACTIVE status flag bit
            break;

        case FLTCMP_LESS_THAN:
            if(source < fltObject->TripResponse.compareThreshold)  // Check if SOURCE < TRIP_LEVEL
                fltObject->Status.bits.FaultActive = true; // Set FAULT_ACTIVE status flag bit
            else if(source > fltObject->RecoveryResponse.compareThreshold) // Check if SOURCE > RECOVERY_LEVEL
                fltObject->Status.bits.FaultActive = false; // Clear FAULT_ACTIVE status flag bit
            break;

        case FLTCMP_IS_EQUAL:
            if(source == fltObject->TripResponse.compareThreshold)  // Check if SOURCE == TRIP_LEVEL
                fltObject->Status.bits.FaultActive = true; // Set FAULT_ACTIVE status flag bit
            else if(source != fltObject->TripResponse.compareThreshold)  // Check if SOURCE != TRIP_LEVEL
                fltObject->Status.bits.FaultActive = false; // Clear FAULT_ACTIVE status flag bit
            break;

        case FLTCMP_IS_NOT_EQUAL:
            if(source != fltObject->TripResponse.compareThreshold)  // Check if SOURCE != TRIP_LEVEL
                fltObject->Status.bits.FaultActive = true; // Set FAULT_ACTIVE status flag bit
            else if(source == fltObject->TripResponse.compareThreshold)  // Check if SOURCE == TRIP_LEVEL
                fltObject->Status.bits.FaultActive = false; // Clear FAULT_ACTIVE status flag bit
            break;

        case FLTCMP_BETWEEN:
            // Check if SOURCE is between "RECOVERY_LEVEL ti TRIP_LEVEL"
            if((fltObject->RecoveryResponse.compareThreshold < source) && (source < fltObject->TripResponse.compareThreshold))  
                fltObject->Status.bits.FaultActive = true; // Set FAULT_ACTIVE status flag bit
            else 
                fltObject->Status.bits.FaultActive = false; // Clear FAULT_ACTIVE status flag bit
            break;

        case FLTCMP_OUTSIDE:
            // Check if SOURCE is outside "RECOVERY_LEVEL to TRIP_LEVEL"
            if((source < fltObject->RecoveryResponse.compareThreshold) || (fltObject->TripResponse.compareThreshold < source))  
                fltObject->Status.bits.FaultActive = true; // Set FAULT_ACTIVE status flag bit
            else 
                fltObject->Status.bits.FaultActive = false; // Clear FAULT_ACTIVE status flag bit
            break;

        default:
            return(0);  // Return=>Error (Ignore fault check if compare type is not defined)
            break;
    
    }
            
    // If a fault condition has been detected while no FAULT has been tripped....
    if ((fltObject->Status.bits.FaultActive) && (!fltObject->Status.bits.FaultStatus)) {
    
        fltObject->Counter++;  // Increment fault event counter
        
        // Trigger on FAULT conditions
        if (fltObject->Counter >= fltObject->TripResponse.eventThreshold)
        {
            fltObject->Status.bits.FaultStatus = true;    // Set FAULT STATUS FLAG BIT
            fltObject->Counter = fltObject->TripResponse.eventThreshold; // Set fault event counter to threshold level
            if (fltObject->TripResponse.ptrResponseFunction != NULL)    // If a user function has been defined,
                retval = fltObject->TripResponse.ptrResponseFunction();   // => call this function and capture return value
        }

    }
    // If a FAULT has been tripped but no fault condition has been detected anymore....
    else if ((fltObject->Status.bits.FaultStatus) && (!fltObject->Status.bits.FaultActive)) {
    
        fltObject->Counter++;  // Increment fault event counter

        // Trigger on RECOVERY conditions
        if (fltObject->Counter >= fltObject->RecoveryResponse.eventThreshold)
        {
            fltObject->Status.bits.FaultStatus = false;   // Clear FAULT STATUS FLAG BIT
            fltObject->Counter = fltObject->RecoveryResponse.eventThreshold; // Set fault event counter to threshold level
            if (fltObject->RecoveryResponse.ptrResponseFunction != NULL)   // If a user function has been defined,
                retval = fltObject->RecoveryResponse.ptrResponseFunction();  // => call this function and capture return value
        }
    
    }
    // If everything is OK, reset counter
    else
    {
        fltObject->Counter = 0;  // clear fault event counter
    }

    
    return (retval); // Fault handler executed successfully
}


// end of file
