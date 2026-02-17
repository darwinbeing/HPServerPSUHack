/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
*******************************************************************************/
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



