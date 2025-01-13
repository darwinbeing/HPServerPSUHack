MPLAB PowerSmart™ SDK for Microchip dsPIC33® Digital Signal Controllers
Digital Control Library Designer (PS-DCLD)
=======================================================================

Version 0.9.14.678 Release Notes:
---------------------------------

This additional release was necessary after having moved the website location of PowerSmart™ DCLD to the new Github organiztation Microchip PIC & AVR Tools,
updating links to websites and user's guide. No further changes have been made.


Version 0.9.14.676 Release Notes:
---------------------------------

New Features:

1) Anti-Windup Minimum Auto-Zero Feature Improvement

The Anti-Windup Limiter Auto-Zero feature clamps the output value to zero when the control output is less than the declared minimum threshold.
This feature can improve no-load regulation in asynchronous topologies. In this version the control output history is still updated with the most recent control output value while the zero'ed value is written to the declared target memory address (e.g. PWM duty cycle register)
For improved cycle-skipping mode, this feature can be combined with the Output Saturation feature to improve transitions into and from cycle-skipping to constant frequency output operation.

2) Mirrored Control Output with Offset

The multi-output support got extended by the Mirror Output With Offset option. The Mirrored Output option allows to write the most recent control output to two different target variables/registers simultaneously. The extended option in this version adds an user-specified offset value to the most recent control output before writing it to the alternate target address.
This feature becomes useful in multiphase control systems, where one loop is controlling one primary phase, and PWM steering is used for current balancing across the remaining phases.


Optimizations:

1) Anti-Windup Option Combination

When enabling multiple Anti-Windup Limiter features simultaneously, several conditions need to be combined in source code. The conditional bit- and value-test instruction sequence has been re-organized to reduce the overall number of cycles of minimum and/or maximum value clamping overrides.

2) P-Term Control Nominal Output Setting for Phase-Shifted Converters

When setting up a P-Term control loop for plant measurements of phase-shifted converters, the phase shift value was previously set by specifying the amount of overlap in [%]. This seems to have been a non-intuitive approach. In this version the specification was changed to use Degrees of Phase Angle between overlapping PWM waveforms to be more intuitive.


Bugfixes:

1) Input/Alternate Input Port Swapping

Rating: Medium
Status: Fixed

In floating point and dual bit-shifting scaling mode, input port swapping failed when used in combination with output port mirroring. 

2) Reviewed comments

Rating: Low
Status: Fixed

Comments in generated code has been reviewed and some obsolete/misleading descriptions have been corrected.

3) Input gain calculation of current feedback circuits

Rating: Low
Status: Fixed

Recalculating the maximum sense value got accidentally bypassed causing the user input sanity-check validation to fail when the new feedback gain changed to a degree where the previously calculated maximum sense value exceeded the ADC voltage reference.



Important Change Notes - (Impact on Existing Projects):

1)  API Changes

a) Renamed Extension Function Hook Names

    Some extension function hook names seemed to create confusion among users. Therefore we took the opportunity to get this straight before the use of this new feature is spreading to wide, affecting too many firmware projects.

    These extension function hooks have been renamed:

    - `ptrExtHookTargetFunction` has been renamed to `ptrExtHookPreTargetWriteFunction`
    - `ExtHookTargetFunctionParam` has been renamed to `ExtHookPreTargetWriteFunctionParam`

    - `ptrExtHookStopFunction` has been renamed to `ptrExtHookEndOfLoopFunction`
    - `ExtHookStopFunctionParam` has been renamed to `ExtHookEndOfLoopFunctionParam`

    - `ptrExtHookEndFunction` has been renamed to `ptrExtHookExitFunction`
    - `ExtHookEndFunctionParam` has been renamed to `ExtHookExitFunctionParam`


    Compiling code using any of the previous declarations will result in compiler error: 

    `[path]/[file name]:[line number:column] error: 'volatile struct NPNZ_EXTENSION_HOOKS_s' has no member named 'ExtHookExitctionParam'`


2)  Execution of Generated Code / Timing
(none)



============================================
(c) 2021, Microchip Technology Inc.
