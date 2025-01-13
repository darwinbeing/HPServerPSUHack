/****************************************************************************
*
* $Source: /cvs/c30_dsp/src/matrix.c,v $
* $Revision: 1.1.1.1 $
*
* Copyright 2002, Microchip, Inc.  All rights reserved.
*
* Matrix implementation.
*
* $Log: matrix.c,v $
* Revision 1.1.1.1  2003/06/20 22:23:00  wilkiec
* DSP library, src and verified build
*
****************************************************************************/

/* Local headers. */
#include "dsp.h"				/* DSP Library interface */

/*...........................................................................*/


/* Matrix operations. */


fractional* MatrixScale (		/* Matrix scale */
					/* dstM[i][j] = sclVal*srcM[i][j] */
   int numRows,				/* number rows in matrix(s) */
   int numCols,				/* number columns in matrix(s) */
   fractional* dstM,			/* ptr to destination matrix */
   fractional* srcM,			/* ptr to source matrix */
   fractional sclVal			/* scale value */
					/* dstM returned */
) {

   /* Local declarations. */
   fractional* retM = dstM;
   int r = 0;				/* instead of loop numRows*numCols */
   int c = 0;				/* double loop numRows and numCols */
   					/* since dimmension in {1,...,2^14-1} */
   					/* hence, numRows*numCols >> 2^16-1 */

   for (r = numRows; r > 0; r--) {
      for (c = numCols; c > 0; c--) {
         *(dstM++) = sclVal * (*(srcM++));
      }
   }

   /* Return destination vector pointer. */
   return (retM);

} /* end of MatrixScale */

/*...........................................................................*/

fractional* MatrixTranspose (		/* Matrix transpose */
					/* dstM[i][j] = srcM[j][i] */
   int numRows,				/* number rows in matrix(s) */
   int numCols,				/* number columns in matrix(s) */
   fractional* dstM,			/* ptr to destination matrix */
   fractional* srcM			/* ptr to source matrix */
					/* dstM returned */
) {

   /* Local declarations. */
   fractional* retM = dstM;
   fractional* tmp = srcM++;
   int r = 0;				/* instead of loop numRows*numCols */
   int c = 0;				/* double loop numRows and numCols */
   					/* since dimmension in {1,...,2^14-1} */
   					/* hence, numRows*numCols >> 2^16-1 */

   for (c = numCols; c > 0; c--) {
      for (r = numRows; r > 0; r--) {
         *(dstM++) = *tmp;
	 tmp += numCols;
      }
      tmp = srcM++;
   }

   /* Return destination vector pointer. */
   return (retM);

} /* end of MatrixTranspose */

/*...........................................................................*/

fractional* MatrixAdd (			/* Matrix addition */
					/* dstM[i][j] =			*/
					/*    srcM1[i][j] + srcM2[i][j] */
   int numRows,				/* number rows in matrix(s) */
   int numCols,				/* number columns in matrix(s) */
   fractional* dstM,			/* ptr to destination matrix */
   fractional* srcM1,			/* ptr to source one matrix */
   fractional* srcM2			/* ptr to source two matrix */
					/* dstM returned */
) {

   /* Local declarations. */
   fractional* retM = dstM;
   int r = 0;				/* instead of loop numRows*numCols */
   int c = 0;				/* double loop numRows and numCols */
   					/* since dimmension in {1,...,2^14-1} */
   					/* hence, numRows*numCols >> 2^16-1 */

   for (r = numRows; r > 0; r--) {
      for (c = numCols; c > 0; c--) {
         *(dstM++) = *(srcM1++) + (*(srcM2++));
      }
   }

   /* Return destination vector pointer. */
   return (retM);

} /* end of MatrixAdd */

/*...........................................................................*/

fractional* MatrixSubtract (		/* Matrix subtraction */
					/* dstM[i][j] =			*/
					/*    srcM1[i][j] - srcM2[i][j] */
   int numRows,				/* number rows in matrix(s) */
   int numCols,				/* number columns in matrix(s) */
   fractional* dstM,			/* ptr to destination matrix */
   fractional* srcM1,			/* ptr to source one matrix */
   fractional* srcM2			/* ptr to source two matrix */
					/* dstM returned */
) {

   /* Local declarations. */
   fractional* retM = dstM;
   int r = 0;				/* instead of loop numRows*numCols */
   int c = 0;				/* double loop numRows and numCols */
   					/* since dimmension in {1,...,2^14-1} */
   					/* hence, numRows*numCols >> 2^16-1 */

   for (r = numRows; r > 0; r--) {
      for (c = numCols; c > 0; c--) {
         *(dstM++) = *(srcM1++) - (*(srcM2++));
      }
   }

   /* Return destination vector pointer. */
   return (retM);

} /* end of MatrixSubtract */

/*...........................................................................*/

fractional* MatrixMultiply (		/* Matrix multiplication */
					/* dstM[i][j] = 		*/
					/*    sum_k(srcM1[i][k]*srcM2[k][j]) */
   int numRows1,			/* number rows source one matrix */
   int numCols1Rows2,			/* number columns source one matrix */
   					/* same as rows source two matrix */
   int numCols2,			/* number columns source two matrix */
   fractional* dstM,			/* ptr to destination matrix */
   fractional* srcM1,			/* ptr to source one matrix */
   fractional* srcM2			/* ptr to source two matrix */
					/* dstM returned */
) {

   /* Local declarations. */
   fractional* retM = dstM;
   fractional* ptr1M = NULL;
   fractional* ptr2M = NULL;
   fractional* ptrTmp = NULL;
   fractional accumulator = (fractional) 0;
   int n = 0;
   int m = 0;
   int p = 0;

   for (n = numRows1-1; n >= 0; n--) {
      ptr2M = srcM2;
      for (p = numCols2-1; p >= 0; p--) {
	 accumulator = 0;
	 ptr1M = srcM1;
	 ptrTmp = ptr2M;
         for (m = numCols1Rows2-1; m >= 0; m--) {
	    accumulator += *(ptr1M++) * (*(ptrTmp));
	    ptrTmp += numCols2;
	 }
	 *(dstM++) = accumulator;
	 ptr2M++;
      }
      srcM1 += numCols1Rows2;
   }

   /* Return destination vector pointer. */
   return (retM);

} /* end of MatrixMultiply */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
