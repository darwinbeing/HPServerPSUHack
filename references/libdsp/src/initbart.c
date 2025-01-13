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


extern fractional* BartlettInit (	/* Initialize a Bartlett window */
					/* computed in floating point */
					/* converted to fractionals */
   int numElems,			/* number elements in window */
   fractional* window			/* ptr to window */
					/* window returned */
) {

   /* Local declarations. */
   fractional* retWindow = window;
   double arg = 2.0/((double) (numElems-1));
   int cntr = 0;

   /* Compute window factors. */
   for (cntr = 0; cntr <= (numElems-1)/2; cntr++) {
      *(window++) = Float2Fract (arg*cntr);
   }
   for ( ; cntr < numElems; cntr++) {
      *(window++) = Float2Fract (BART_0 - arg*cntr);
   }

   /* Return window pointer. */
   return (retWindow);

} /* end of BartlettInit */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
