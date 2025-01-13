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

/* Local prototypes. */
double ModifiedBesselI0 (double x);

/*...........................................................................*/


extern fractional* KaiserInit (		/* Initialize a Kaiser window */
					/* computed in floating point */
					/* converted to fractionals */
   int numElems,			/* number elements in window */
   fractional* window,			/* ptr to window */
   float betaVal			/* shape parameter */
					/* window returned */
) {

   /* Local declarations. */
   fractional* retWindow = window;
   double alpha = (double) (numElems-1)/2.0;
   double num, den;
   int cntr = 0;

   /* Compute window factors. */
   den = ModifiedBesselI0 (betaVal);
   for (cntr = 0; cntr < numElems; cntr++) {
      num = ((double) cntr - alpha)/alpha;
      num *= num;
      num = betaVal * sqrt (1.0 - num);
      num = ModifiedBesselI0 (num);
      *(window++) = Float2Fract (num/den);
   }

   /* Return window pointer. */
   return (retWindow);

} /* end of KaiserInit */

/*...........................................................................*/

double ModifiedBesselI0 (
   double x
) {

       int k; double xk, kfact, xq, bessel;
       x *=  0.5; kfact = 1.0; xk = xq = x; 
       bessel = 1.0 + xq*xq;
       for (k = 2; 1.0e-8 < xq; k++) {
             kfact *= k; xk *= x; xq = xk / kfact; 
             bessel += xq*xq;
       }
       return bessel;

} /* end of ModifiedBesselI0 */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
