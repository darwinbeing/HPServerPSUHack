/****************************************************************************
*
* $Source: /cvs/c30_dsp/src/vector.c,v $
* $Revision: 1.1.1.1 $
*
* Copyright 2002, Microchip, Inc.  All rights reserved.
*
* Vector implementation.
*
* $Log: vector.c,v $
* Revision 1.1.1.1  2003/06/20 22:23:01  wilkiec
* DSP library, src and verified build
*
****************************************************************************/

/* Local headers. */
#include "dsp.h"				/* DSP Library interface */

/*...........................................................................*/


/* Vector operations. */

fractional VectorMax (			/* Vector (last) maximum */
					/* maxVal = max{srcV[elem]} */
					/* if srcV[i] = srcV[j] = maxVal */
					/* and i < j, then *(maxIndex) = j */
   int numElems,			/* number elements in vector(s) */
   fractional* srcV,			/* ptr to source vector */
   int* maxIndex			/* ptr to index for maximum value */
					/* maxVal returned */
) {

   /* Local declarations. */
   fractional maxVal = (fractional) 0;
   fractional aVal = (fractional) 0;
   int cntr = 0;

   /* Find maximum value of vector */
   maxVal = *(srcV++);					/* get first */
   *(maxIndex) = cntr;
   for (cntr = numElems - 1; cntr > 0; cntr--) {
      aVal = *(srcV++);					/* get next */
      if (maxVal <= aVal) {
	 /* update if le */
	 maxVal = aVal;
	 *(maxIndex) = numElems - cntr;
      }
   }

   /* Return maximum vector value. */
   return (maxVal);

} /* end of VectorMax */

/*...........................................................................*/

fractional VectorMin (			/* Vector (last) minimum */
					/* minVal = min{srcV[elem]} */
					/* if srcV[i] = srcV[j] = minVal */
					/* and i < j, then *(minIndex) = j */
   int numElems,			/* number elements in vector(s) */
   fractional* srcV,			/* ptr to source vector */
   int* minIndex			/* ptr to index for minimum value */
					/* minVal returned */
) {

   /* Local declarations. */
   fractional minVal = (fractional) 0;
   fractional aVal = (fractional) 0;
   int cntr = 0;

   /* Find minimum value of vector */
   minVal = *(srcV++);					/* get first */
   *(minIndex) = cntr;
   for (cntr = numElems - 1; cntr > 0; cntr--) {
      aVal = *(srcV++);					/* get next */
      if (minVal >= aVal) {				/* compare */
	 /* update if ge */
         minVal = aVal;
	 *(minIndex) = numElems - cntr;
      }
   }

   /* Return minimum vector value. */
   return (minVal);

} /* end of VectorMin */

/*...........................................................................*/

fractional* VectorNegate (		/* Vector negate */
					/* dstV[elem] = -srcV[elem] */
   int numElems,			/* number elements in vector(s) */
   fractional* dstV,			/* ptr to destination vector */
   fractional* srcV			/* ptr to source vector */
					/* dstV returned */
) {

   /* Local declarations. */
   fractional* retV = dstV;
   int cntr = 0;

   /* Negate vector elements */
   for (cntr = numElems; cntr > 0; cntr--) {
      *(dstV++) = -(*srcV++);
   }

   /* Return destination vector pointer. */
   return (retV);

} /* end of VectorNegate */

/*...........................................................................*/

extern fractional* VectorScale (	/* Vector scale */
					/* dstV[elem] = sclVal*srcV[elem] */
					/* (in place capable) */
   int numElems,			/* number elements in vector(s) */
   fractional* dstV,			/* ptr to destination vector */
   fractional* srcV,			/* ptr to source vector */
   fractional sclVal			/* scale value */
					/* dstV returned */
) {

   /* Local declarations. */
   fractional* retV = dstV;
   int cntr = 0;

   /* Scale vector elements */
   for (cntr = numElems; cntr > 0; cntr--) {
      *(dstV++) = sclVal*(*srcV++);
   }

   /* Return destination vector pointer. */
   return (retV);

} /* end of VectorScale */

/*...........................................................................*/

fractional* VectorAdd (			/* Vector addition */
					/* dstV[elem] =			*/
					/*    = srcV1[elem] + srcV2[elem] */
   int numElems,			/* number elements in vector(s) */
   fractional* dstV,			/* ptr to destination vector */
   fractional* srcV1,			/* ptr to source vector one */
   fractional* srcV2			/* ptr to source vector two */
					/* dstV returned */
) {

   /* Local declarations. */
   fractional* retV = dstV;
   int cntr = 0;

   /* Add vector elements */
   for (cntr = numElems; cntr > 0; cntr--) {
      *(dstV++) = *(srcV1++) + *(srcV2++);
   }

   /* Return destination vector pointer. */
   return (retV);

} /* end of VectorAdd */

/*...........................................................................*/

fractional* VectorSubtract (		/* Vector subtraction */
					/* dstV[elem] =			*/
					/*    = srcV1[elem] - srcV2[elem] */
   int numElems,			/* number elements in vector(s) */
   fractional* dstV,			/* ptr to destination vector */
   fractional* srcV1,			/* ptr to source vector one */
   fractional* srcV2			/* ptr to source vector two */
					/* dstV returned */
) {

   /* Local declarations. */
   fractional* retV = dstV;
   int cntr = 0;

   /* Subtract vector elements */
   for (cntr = numElems; cntr > 0; cntr--) {
      *(dstV++) = *(srcV1++) - *(srcV2++);
   }

   /* Return destination vector pointer. */
   return (retV);

} /* end of VectorSubtract */

/*...........................................................................*/

fractional* VectorMultiply (		/* Vector elem-to-elem multiply */
					/* dstV[elem] =			*/
					/*    = srcV1[elem] * srcV2[elem] */
   int numElems,			/* number elements in vector(s) */
   fractional* dstV,			/* ptr to destination vector */
   fractional* srcV1,			/* ptr to source vector one */
   fractional* srcV2			/* ptr to source vector two */
					/* dstV returned */
) {

   /* Local declarations. */
   fractional* retV = dstV;
   int cntr = 0;

   /* Multiply vector elements */
   for (cntr = numElems; cntr > 0; cntr--) {
      *(dstV++) = *(srcV1++) * (*(srcV2++));
   }

   /* Return destination vector pointer. */
   return (retV);

} /* end of VectorMultiply */

/*...........................................................................*/

fractional VectorDotProduct (		/* Vector dot product */
					/* dotVal =			*/
					/*    sum(srcV1[elem]*srcV2[elem]) */
   int numElems,			/* number elements in vector(s) */
   fractional* srcV1,			/* ptr to source vector one */
   fractional* srcV2			/* ptr to source vector two */
					/* dotVal returned */
) {

   /* Local declarations. */
   fractional dotVal = (fractional) 0;
   int cntr = 0;

   /* Multiply/add vector elements */
   for (cntr = numElems; cntr > 0; cntr--) {
      dotVal += *(srcV1++) * (*(srcV2++));
   }

   /* Return dot product result. */
   return (dotVal);

} /* end of VectorDotProduct */

/*...........................................................................*/

fractional VectorPower (		/* Vector power */
					/* powVal =			*/
					/*    = sum(srcV[elem]^2)	*/
   int numElems,			/* number elements in vector(s) */
   fractional* srcV			/* ptr to source vector one */
					/* powVal returned */
) {

   /* Local declarations. */
   fractional powVal = (fractional) 0;
   int cntr = 0;

   /* Multiply/add vector elements */
   for (cntr = numElems; cntr > 0; cntr--) {
      powVal += *(srcV) * (*(srcV));
      srcV++;
   }

   /* Return power result. */
   return (powVal);

} /* end of VectorPower */

/*...........................................................................*/

extern fractional* VectorConvolve (	/* Vector Convolution */
   int numElems1,			/* number elements in vector one */
   int numElems2,			/* number elements in vector two */
   					/* numElems2 <= numElems1 */
   fractional* dstV,			/* ptr to destination vector */
   					/* with numElems1+numElems2-1 elems */
   fractional* srcV1,			/* ptr to source vector one */
   fractional* srcV2			/* ptr to source vector two */
					/* dstV returned */
) {

   /* Local declarations. */
   fractional* retV = dstV;
   fractional* y = dstV;
   fractional* x = srcV1;
   fractional* h = srcV2;
   int N = numElems1;
   int M = numElems2;
   int n = 0;
   int k = 0;

   /* First stage: y[n] = sum_(k=0:n){x[k]*h[n-k]}, 0 <= n < M. */
   for (n = 0; n < M; n++) {
      *(y+n) = 0;
      for (k = 0; k <= n; k++) {
         *(y+n) += *(x+k)*(*(h+n-k));
      }
   }

   /* Second stage: y[n] = sum_(k=n-M+1:n){x[k]*h[n-k]}, M <= n < N. */
   for (n = M; n < N; n++) {
      *(y+n) = 0;
      for (k = n-M+1; k <= n; k++) {
         *(y+n) += *(x+k)*(*(h+n-k));
      }
   }

   /* Third stage: y[n] = sum_(k=n-M+1:N-1){x[k]*h[n-k]}, N <= n < N+M-1. */
   for (n = N; n < N+M-1; n++) {
      *(y+n) = 0;
      for (k = n-M+1; k < N; k++) {
         *(y+n) += *(x+k)*(*(h+n-k));
      }
   }

   /* Return destination vector pointer. */
   return (retV);

} /* end of VectorConvolve */

/*...........................................................................*/

extern fractional* VectorCorrelate (	/* Vector Correlation */
   int numElems1,			/* number elements in vector one */
   int numElems2,			/* number elements in vector two */
   					/* numElems2 <= numElems1 */
   fractional* dstV,			/* ptr to destination vector */
   					/* with numElems2+numElems1-1 elems */
   fractional* srcV1,			/* ptr to source vector one */
   fractional* srcV2			/* ptr to source vector two */
					/* dstV returned */
) {

   /* Local declarations. */
   fractional* tmp = srcV2 + (numElems2-1);
   fractional tmpVal = *(tmp);
   int m = 0;

   /* Compute correlation (as a convolution). */
   /* First, revert srcV2... */
   for (m = 0; m < numElems2/2; m++) {
      *(tmp--) = *(srcV2);
      *(srcV2++) = tmpVal;
      tmpVal = *tmp;
   }
   /* Rewind... */
   srcV2 -= numElems2/2;

   /* Then, return destination vector pointer (as result of convolution). */
   return (VectorConvolve (numElems1, numElems2, dstV, srcV1, srcV2));

} /* end of VectorCorrelate */

/*...........................................................................*/

extern fractional* VectorWindow (	/* Apply window to vector */
   int numElems,			/* number elements in vector/window */
   fractional* dstV,			/* ptr to destination vector */
   fractional* srcV,			/* ptr to source vector */
   fractional* window			/* ptr to window */
					/* dstV returned */
) {

   /* Return windowed vector. */
   return (VectorMultiply (numElems, dstV, srcV, window));

} /* end of VectorWindow */

/*...........................................................................*/

extern fractional* VectorCopy (		/* Copy elements of a vector into */
					/* an already existing vector */
					/* (dstV MUST have the same or more */
					/* elements as srcV). */
					/* dstV[n] = srcV[n], 0 <= n < N */
   int numElems,			/* number elements in vector (N) */
   fractional* dstV,			/* ptr to destination vector */
   fractional* srcV			/* ptr to source vector */
					/* dstV returned */
) {

   /* Local declarations. */
   fractional* retV = dstV;
   int cntr = 0;

   /* Copy N elements from srcV to dstV. */
   for (cntr = 0; cntr < numElems; cntr++) {
      *(dstV++) = *(srcV++);
   }

   /* Return zero padded vector. */
   return (retV);

} /* end of VectorCopy */

/*...........................................................................*/

extern fractional* VectorZeroPad (	/* Zero pad tail of a vector */
					/* dstV[n] = srcV[n], 0 <= n < N */
					/* dstV[n] = 0, N <= n < N+M */
   int numElems,			/* number elements in vector (N) */
   int numZeros,			/* number zeros to append (M) */
   fractional* dstV,			/* ptr to destination vector */
   fractional* srcV			/* ptr to source vector */
					/* dstV returned */
) {

   /* Local declarations. */
   fractional* retV = dstV;
   int cntr = 0;

   /* Copy N elements from srcV to dstV. */
   dstV = VectorCopy (numElems, dstV, srcV);

   /* Set to zero the remaining M elements in dstV. */
   dstV += numElems;				/* advance to tail */
   for (cntr = 0; cntr < numZeros; cntr++) {
      *(dstV++) = 0;
   }

   /* Return zero padded vector. */
   return (retV);

} /* end of VectorZeroPad */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
