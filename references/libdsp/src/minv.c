/****************************************************************************
*
* $Source: /cvs/c30_dsp/src/minv.c,v $
* $Revision: 1.1.1.1 $
*
* Copyright 2002, Microchip, Inc.  All rights reserved.
*
* Matrix Inversion implementation.
*
* $Log: minv.c,v $
* Revision 1.1.1.1  2003/06/20 22:23:00  wilkiec
* DSP library, src and verified build
*
****************************************************************************/

/* Local headers. */
#include "dsp.h"				/* DSP Library interface */

/*...........................................................................*/


/* Matrix Inversion operation. */

/*...........................................................................*/

float* MatrixInvert (			/* Matrix inverse */
					/* dstM = srcM^(-1) */
   int numRowsCols,			/* number rows and columns in matrix */
   					/* matrix MUST be square */
   float* dstM,				/* ptr to destination matrix */
   float* srcM,				/* ptr to source matrix */
   float* pivotFlag,			/* internal use; size numRowsCols */
   int* swappedRows,			/* internal use; size numRowsCols */
   int* swappedCols			/* internal use; size numRowsCols */
   					/* last three vectors required from */
					/* user, so that function is not */
					/* responsible for memory management */
					/* dstM returned (or NULL on error */
					/* if source matrix is singular) */
) {

   /* Local declarations. */
   float* retM = dstM;
   float absVal = 0;
   float maxVal = 0;
   int cntr = 0;
   int r = 0;
   int c = 0;
   int ir = 0;
   int ic = 0;

   /* Initialized local arrays to zero. */
   for (r = 0; r < numRowsCols; r++) {
      pivotFlag[r] = 0.0;
      swappedRows[r] = 0;
      swappedCols[r] = 0;
   }

   /* Since the Gauss-Jordan algorithm operates in place... */
   if (srcM != dstM) {
      for (r = 0; r < numRowsCols; r++) {
         for (c = 0; c < numRowsCols; c++) {
            *(dstM++) = *(srcM++);
         }
      }
      dstM = retM;						/* rewind */
   }

   /* Now, apply algorithm to dstM. */
   for (cntr = 0; cntr < numRowsCols; cntr++) {		/* pivoting iterates */

      /* Find pivot element. */
      maxVal = 0;
      for (r = 0; r < numRowsCols; r++) {
	 if (!pivotFlag[r]) {				/* unused pivot */
	    for (c = 0; c < numRowsCols; c++) {
	       if (!pivotFlag[c]) {			/* unused pivot */
	          absVal = fabs (dstM[r*numRowsCols+c]);
		  if (absVal >= maxVal) {
		     /* Update. */
		     maxVal = absVal;
		     ir = r;
		     ic = c;
		  }
	       }
	    }
	 }
      }
      pivotFlag[ic]++;					/* mark pivot used */

      /* Swap rows to make this diagonal the largest absolute pivot. */
      if (ir != ic) {
         for (c = 0; c < numRowsCols; c++) {
	    absVal = dstM[ir*numRowsCols+c];		/* reusing absVal */
	    dstM[ir*numRowsCols+c] = dstM[ic*numRowsCols+c];
	    dstM[ic*numRowsCols+c] = absVal;
	 }
      }

      /* Update swapping status. */
      swappedRows[cntr] = ir;
      swappedCols[cntr] = ic;

      /* Bail out if matrix is singular. */
      if (dstM[ic*numRowsCols+ic] == 0.0 ) {
         return ((float*) NULL);
      }

      /* Divide the row by the pivot. */
      absVal = 1.0/dstM[ic*numRowsCols+ic];		/* reusing absVal */
      dstM[ic*numRowsCols+ic] = 1.0;			/* avoid round off */
      for (c = 0; c < numRowsCols; c++) {
         dstM[ic*numRowsCols+c] *= absVal;
      }

      /* Fix other rows by subtraction. */
      for (r = 0; r < numRowsCols; r++) {
         if (r != ic) {
	    absVal = dstM[r*numRowsCols+ic];		/* reusing absVal */
	    dstM[r*numRowsCols+ic] = 0.0;
	    for (c = 0; c < numRowsCols; c++) {
	       dstM[r*numRowsCols+c] -= (dstM[ic*numRowsCols+c]*absVal);
	    }
	 }
      }
   }

   /* Reorganized swaps prior to returning. */
   for (c = numRowsCols-1; c >= 0; c--) {
      if (swappedRows[c] != swappedCols[c]) {
         for (r = 0; r < numRowsCols; r++) {
	    absVal = dstM[r*numRowsCols+swappedRows[c]];/* reusing absVal */
	    dstM[r*numRowsCols+swappedRows[c]] = dstM[r*numRowsCols+swappedCols[c]];
	    dstM[r*numRowsCols+swappedCols[c]] = absVal;
	 }
      }
   }

   /* Return destination vector pointer. */
   return (retM);

} /* end of MatrixInvert */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
