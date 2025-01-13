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
#include "dsp.h"				/* testing interface */

/*...........................................................................*/


float Fract2Float (			/* Converts fractional into float */
   fractional aVal			/* fract value in range {-1, 1-2^-15} */
) {

#if	DATA_TYPE==FRACTIONAL		/* [ */

   /* Local declarations. */
   double scale = pow (2.0, -15.0);		/* 2^(-15) */
   long int fullRange = 1L<<16;			/* 2^(16) */
   long int halfRange = 1L<<15;			/* 2^(15) */
   double decimalVal = 0.0;

   /* Convert. */
   if (aVal >= halfRange) {
      aVal -= fullRange;
   }
   decimalVal = ((double) aVal)*scale;

   /* Return decimal value in floating point. */
   return ((float) decimalVal);

#else	/* ] [ */

   /* Return input value in floating point. */
   return ((float) aVal);

#endif	/* ] */

} /* end of Fract2Float */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
