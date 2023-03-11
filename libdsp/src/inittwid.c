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


extern fractcomplex* TwidFactorInit (	/* Initialize twiddle factors */
					/* computed in floating point */
					/* converted to fractionals */
   int log2N,				/* log2(N), N is number of factors */
   					/* NOTE: only half of twiddle factors */
					/* are used for I/FFT computation */
   fractcomplex* twidFactors,		/* ptr to twiddle factors */
   int conjFlag				/* indicates whether to generate */
					/* complex conjugates of twiddles */
					/* 0 : no conjugates (default) */
					/* 1 : conjugates */
					/* twidFactors returned */
					/* only the first half: */
					/* WN(0)...WN(N/2-1) */
					/* (or their conjugates) */
) {

   /* Local declarations. */
   fractcomplex* retFactors = twidFactors;
   /* NOTE: instead of wReal, wImag, wRecReal and wRecImag declared	*/
   /* as double, it would have been cleaner to declare w and wRec as	*/
   /* fractcomplex. In this case, the values w.real, w.imag wRec.real	*/
   /* and wRec.imag would have replaced the instances of wReal, wImag,	*/
   /* wRecReal and wRecImag, respectively. However, if declared as	*/
   /* fractcomplex structures, the PIC30 simulator fails to compute	*/
   /* the twiddle factors even though the results are correct when	*/
   /* compiling solely under CYGWIN!!! Hence, the variables have	*/
   /* been declared individual doubles.					*/
   double wReal = 0.0;
   double wImag = 0.0;
   double wRecReal = 0.0;
   double wRecImag = 0.0;
   double wTmp = 0.0;
   double arg = PI;				/* sin/cos argument */
   						/* (default PI) */
   int numFactors = (1<<log2N)/2;		/* number twiddle factors */
   int cntr = 0;

   /* Trivial first factor value. */
   twidFactors->real = Float2Fract (1.0);
   twidFactors++->imag = 0.0;

   /* Set up twiddle factor computation. */
   arg /= (double) (numFactors);
   wRecReal = wReal = cos(arg);
   /* Twiddle factor conjugates? */
   if (conjFlag) {
      /* Yes. */
      wRecImag = wImag = sin(arg);
   } else {
      /* No. */
      wRecImag = wImag =-sin(arg);
   }

   /* Compute twiddle factors recursively. */
   for (cntr = numFactors-2; cntr >= 0; cntr--) {
      twidFactors->real = Float2Fract (wRecReal);
      (twidFactors++)->imag = Float2Fract (wRecImag);
      wTmp = wRecReal*wReal - wRecImag*wImag;
      wRecImag = wRecReal*wImag + wRecImag*wReal;
      wRecReal = wTmp;
   }

   /* Return destination vector pointer. */
   return (retFactors);

} /* end of TwidFactorInit */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
