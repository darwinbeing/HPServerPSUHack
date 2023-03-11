/*********************************************************************
*                                                                    *
*                       Software License Agreement                   *
*                                                                    *
*   The software supplied herewith by Microchip Technology           *
*   Incorporated (the "Company") for its dsPIC controller            *
*   is intended and supplied to you, the Company's customer,         *
*   for use solely and exclusively on Microchip dsPIC                *
*   products. The software is owned by the Company and/or its        *
*   supplier, and is protected under applicable copyright laws. All  *
*   rights are reserved. Any use in violation of the foregoing       *
*   restrictions may subject the user to criminal sanctions under    *
*   applicable laws, as well as to civil liability for the breach of *
*   the terms and conditions of this license.                        *
*                                                                    *
*   THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION.  NO           *
*   WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING,    *
*   BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND    *
*   FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE     *
*   COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,  *
*   INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.  *
*                                                                    *
*   (c) Copyright 2003 Microchip Technology, All rights reserved.    *
*********************************************************************/

/* Local headers. */
#include "dsp.h"				/* DSP Library interface */

/*...........................................................................*/

/* Local defines. */
#define	SCALE		1L<<15			/* 2^15 */
#define	RANGE		1L<<16			/* 2^16 */

/*...........................................................................*/

/* Float2Fract implementation. */

#if	DATA_TYPE==FRACTIONAL		/* [ */

fractional Float2Fract (		/* Converts float into fractional */
   float aVal				/* float value in range [-1, 1) */
) {

   /* Local declarations. */
   long int scale = SCALE;
   long int fractVal = 0.0;
   double decimalVal = 0.0;
   double dummy = 0.0;
   int isOdd = 0;

   /* Convert with convergent rounding and saturation. */
   decimalVal = aVal*scale;
   if (aVal >= 0) {
      fractVal = floor (decimalVal);
      dummy = fractVal/2.0;
      isOdd = (int) ((dummy - floor (dummy))*2.0);
      dummy = decimalVal -fractVal;
      if ((dummy > 0.5) || ((dummy == 0.5) && isOdd)) {
         fractVal += 1.0;
      }
      if (fractVal >= scale) {
         fractVal = scale - 1.0;
      }
   } else {	/* aVal < 0 */
      fractVal = ceil (decimalVal);
      if (fractVal != decimalVal) {
         fractVal -= 1.0;
      }
      dummy = fractVal/2.0;
      isOdd = (int) ((dummy - floor (dummy))*2.0);
      dummy = decimalVal -fractVal;
      if ((dummy > 0.5) || ((dummy == 0.5) && isOdd)) {
         fractVal += 1.0;
      }
      if (fractVal < -scale) {
         fractVal = -scale;
      }
   }

   /* Return fractional value. */
   return ((fractional) fractVal);

} /* end of MatrixInverse */

#endif	/* ] */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
