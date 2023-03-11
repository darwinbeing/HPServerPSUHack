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

extern fractcomplex* CosFactorInit (	/* Initialize cosine factors */
					/* CN(k) = exp(i*k*pi/(2*N)) */
					/* computed in floating point */
					/* converted to fractionals */
   int log2N,				/* log2(N), N real plus imag factors */
   					/* (only N/2 complex factors) */
   					/* NOTE: only half of cosine factors */
					/* are used for DCT computation */
   fractcomplex* cosineFactors		/* ptr to cosine factors */
					/* cosineFactors returned */
					/* only the first half: */
					/* CN(0)...CN(N/2-1) */
) {

   /* Local declarations. */
   fractcomplex* retFactors = cosineFactors;
   /* NOTE: instead of cReal, cImag, cRecReal and cRecImag declared	*/
   /* as double, it would have been cleaner to declare c and cRec as	*/
   /* fractcomplex. In this case, the values c.real, c.imag cRec.real	*/
   /* and cRec.imag would have replaced the instances of cReal, cImag,	*/
   /* cRecReal and cRecImag, respectively. However, if declared as	*/
   /* fractcomplex structures, the PIC30 simulator fails to compute	*/
   /* the cosine factors even though the results are correct when	*/
   /* compiling solely under CYGWIN!!! Hence, the variables have	*/
   /* been declared individual doubles.					*/
   double cReal = 0.0;
   double cImag = 0.0;
   double cRecReal = 0.0;
   double cRecImag = 0.0;
   double cTmp = 0.0;
   double arg = PI;				/* sin/cos argument */
   						/* (default PI) */
   int numFactors = (1<<log2N);			/* cosine plus sine factors */
   int cntr = 0;

   /* Set up cosine-sine factor computation. */
   arg /= (2.0*numFactors);			/* PI/(2*N) */
   cRecReal = cos(arg);
   cRecImag = -sin(arg);
   cReal = 1.0;
   cImag = 0.0;

   /* Compute cosine-sine factors recursively. */
   for (cntr = numFactors/2-1; cntr >= 0; cntr--) {
      cosineFactors->real = Float2Fract (cReal);
      (cosineFactors++)->imag = Float2Fract (cImag);
      cTmp = cReal;
      cReal = cReal*cRecReal + cImag*cRecImag;
      cImag = cImag*cRecReal - cTmp*cRecImag;
   }

   /* Return destination vector pointer. */
   return (retFactors);

} /* end of CosFactorInit */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
