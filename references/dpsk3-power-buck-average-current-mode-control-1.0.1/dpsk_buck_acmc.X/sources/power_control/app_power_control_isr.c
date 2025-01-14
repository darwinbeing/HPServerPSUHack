/*
 * File:   app_power_control_isr.c
 * Author: M91406
 *
 * Created on May 26, 2020, 7:32 PM
 */


#include "config/hal.h"
#include "app_power_control.h"
#include "drivers/v_loop.h"

/*********************************************************************************
 * @fn      void _BUCK_VLOOP_Interrupt(void)
 * @ingroup app-layer-power-control-events
 * @brief   Main Control Interrupt
 * @param   void
 * @return  void
 *   
 * @details
 * The control interrupt is calling the control loop. The point in time where
 * this interrupt is thrown is determined by selecting the BUCK_VOUT_TRIGGER_MODE
 * option. 
 * 
 *********************************************************************************/

void __attribute__((__interrupt__, auto_psv, context))_BUCK_VLOOP_Interrupt(void)
{
    #if (DBGPIN2_ENABLE)
    DBGPIN2_Set();
    #endif
    
    // Set flag bit indication ADC interrupt activity
    buck.status.bits.adc_active = true;
    
    // Call feedback loop
    #if (PLANT_MEASUREMENT == false)
    buck.v_loop.ctrl_Update(buck.v_loop.controller);
    #else
    v_loop_PTermUpdate(&v_loop);
    #endif

    // Copy most recent control output to DAC output for debugging
    #if (DBGDAC_ENABLE)
    DACOUT_Set(buck.data.control_output);
    #endif
    
    // Clear the interrupt flag bit allowing the next interrupt to trip
    _BUCK_VLOOP_ISR_IF = 0;

    #if (DBGPIN2_ENABLE)
    DBGPIN2_Clear();
    #endif

}

// end of file
