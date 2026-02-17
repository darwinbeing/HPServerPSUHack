/******************************************************************************* ******************************************************************************
MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any derivatives created by 
any person or entity by or on your behalf, exclusively with Microchip’s products.  Microchip and its licensors
 retain all ownership and intellectual property rights in the accompanying software and in all derivatives hereto.  
This software and any accompanying information is for suggestion only.  It does not modify Microchip’s standard warranty for its products.  
You agree that you are solely responsible for testing the software and determining its suitability.  Microchip has no obligation to modify, test, certify, 
or support the software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, 
BUT NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE 
APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP’S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN 
ANY APPLICATION. 

IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY),
STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS,
DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED 
OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON 
ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP 
FOR THIS SOFTWARE.
MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.

*************************************************************************************************************************************************************/

#include "define.h"

#ifdef __dsPIC30F__

        #include "p30fxxxx.h"

#endif

#ifdef __dsPIC33F__

        #include "p33Fxxxx.h"

#endif

#ifdef __PIC24F__

        #include "p24Fxxxx.h"

#endif

#ifdef __PIC24H__

        #include "p24Hxxxx.h"

#endif

/* ****************************************************************
* Standard Exception Vector handlers if ALTIVT (INTCON2<15>) = 0  *
*                                                                 *
* Not required for labs but good to always include                *
******************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _OscillatorFail(void)
{

        INTCON1bits.OSCFAIL = 0;
        PTCONbits.PTEN = 0;

        while(1);
}

void __attribute__((__interrupt__, no_auto_psv))  _AddressError(void)
{

        INTCON1bits.ADDRERR = 0;
        PTCONbits.PTEN = 0;
        while(1);
}

void __attribute__((__interrupt__, no_auto_psv))  _StackError(void)
{

        INTCON1bits.STKERR = 0;
        PTCONbits.PTEN = 0;
        while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _ISR _MathError(void)
{

        INTCON1bits.MATHERR = 0;
        PTCONbits.PTEN = 0;
        while(1);
}




/* ****************************************************************
* Alternate Exception Vector handlers if ALTIVT (INTCON2<15>) = 1 *
*                                                                 *
******************************************************************/
void __attribute__((__interrupt__, no_auto_psv))  _AltOscillatorFail(void)
{

        INTCON1bits.OSCFAIL = 0;
        while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _AltAddressError(void)
{

        INTCON1bits.ADDRERR = 0;
        while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _AltStackError(void)
{

        INTCON1bits.STKERR = 0;
        while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _AltMathError(void)
{

        INTCON1bits.MATHERR = 0;
        while(1);
}



