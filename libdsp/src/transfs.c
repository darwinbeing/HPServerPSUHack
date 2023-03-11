/****************************************************************************
*
* $Source: /cvs/c30_dsp/src/transfs.c,v $
* $Revision: 1.1.1.1 $
*
* Copyright 2002, Microchip, Inc.  All rights reserved.
*
* Transforms implementation.
*
* $Log: transfs.c,v $
* Revision 1.1.1.1  2003/06/20 22:23:01  wilkiec
* DSP library, src and verified build
*
****************************************************************************/

/* Local headers. */
#include "dsp.h"				/* DSP Library interface */

/*...........................................................................*/


/* Transforms operations. */


extern fractcomplex* BitReverseComplex (	/* Bit Reverse Ordering */
					/* (complex) */
   int log2N,				/* log2(N), N is vector length */
   fractcomplex* srcCV			/* ptr to source complex vector */
					/* srcCV returned */
) {

   /* Local declarations. */
   fractcomplex* retCV = srcCV;
   fractcomplex* reverse = NULL;		/* ptr to reverse ordered */
   fractcomplex* direct = NULL;			/* ptr to direct ordered */
   fractcomplex	temp;				/* temp structure */
   int numElems = 1<<log2N;			/* number elements in vector */
   int revIndex = 0;
   int dirIndex = 0;
   int cntr = 0;

   /* Perform all N/2 reorderings. */
   for (revIndex = 0; revIndex < numElems; revIndex++) {
      dirIndex = 0;
      for (cntr = 0; cntr < log2N; cntr++) {
         dirIndex = (dirIndex << 1) | (1 & (revIndex >> cntr));
      }
      if (dirIndex > revIndex) {
      reverse = srcCV + revIndex;
      direct = srcCV + dirIndex;
      temp = *direct;
      *direct = *reverse;
      *reverse = temp;
      }
   }

   /* Return destination vector pointer. */
   return (retCV);

} /* end of BitReverseComplex */

/*...........................................................................*/

extern fractcomplex* FFTComplex (	/* (DIT) Fast Fourier Transform */
					/* (complex) */
   int log2N,				/* log2(N), N is transform length */
   fractcomplex* dstCV,			/* ptr to destination complex vector */
   fractcomplex* srcCV,			/* ptr to source complex vector */
   					/* with time samples */
					/* in natural order */
   fractcomplex* twidFactors,		/* base address of twiddle factors */
					/* either in X data or program memory */
					/* if in X data memory, it points at */
   					/* WN(0).real */
					/* if in program memory, base is the */
					/* offset from program page boundary */
					/* to address where factors located */
					/* (inline assembly psvoffset ()) */
   int factPage				/* if in X data memory, set to */
   					/* defined value COEFFS_IN_DATA */
					/* if in program memory, page number */
					/* where factors are located */
					/* (inline assembly psvpage ()) */
					/* dstCV returned */
   					/* with frequency components */
					/* in natural order */
					/* and scaled by 1/(1<<log2N) */
) {

   /* Local declarations. */
   int numElems = 1<<log2N;			/* number elements in vector */

   /* Copy source vector into destination. */
   dstCV = (fractcomplex*) VectorCopy (numElems*2, (fractional*) dstCV, (fractional*) srcCV);

   /* Apply in-place operation to destination vector. */
   dstCV = FFTComplexIP (log2N, dstCV, twidFactors, factPage);

   /* Bit reverse results, so that frequencies are in natural order. */
   dstCV = BitReverseComplex (log2N, dstCV);

   /* Finally, return pointer to destination vector. */
   return (dstCV);

} /* end of FFTComplex */

/*...........................................................................*/

extern fractcomplex* FFTComplexIP (	/* (DIT) Fast Fourier Transform */
					/* (complex in-place) */
   int log2N,				/* log2(N), N is transform length */
   fractcomplex* srcCV,			/* ptr to source complex vector */
   					/* with time samples */
					/* in natural order */
   fractcomplex* twidFactors,		/* base address of twiddle factors */
					/* either in X data or program memory */
					/* if in X data memory, it points at */
   					/* WN(0).real */
					/* if in program memory, base is the */
					/* offset from program page boundary */
					/* to address where factors located */
					/* (inline assembly psvoffset ()) */
   int factPage				/* if in X data memory, set to */
   					/* defined value COEFFS_IN_DATA */
					/* if in program memory, page number */
					/* where factors are located */
					/* (inline assembly psvpage ()) */

					/* srcCV returned */
   					/* with frequency components */
					/* in bit reverse order */
					/* and scaled by 1/(1<<log2N) */
) {

   /* Local declarations. */
   fractcomplex* upper = NULL;			/* ptr to upper leg of butt */
   fractcomplex* lower = NULL;			/* ptr to lower leg of butt */
   fractcomplex* theTwids = twidFactors+1;	/* ptr to non trivial twids */
   fractcomplex* newTwid = NULL;		/* ptr to twids per group */
   fractcomplex	twid;				/* holder for twid factor */
   fractcomplex	temp, tmp;			/* temp structures */
   int numFactors = 1<<log2N;			/* number twiddle factors */
   int numElems = 1<<log2N;			/* number elements in vector */
   int numGroups = numFactors;			/* groups for same twid fact */
   int offFactors = 1;				/* offset between twid fact */
   int passCntr = 0;
   int groupCntr = 0;
   int buttCntr = 0;

   /* Perform all log2N passes. */
   for (passCntr = 0; passCntr < log2N; passCntr++) {

      numGroups /= 2;				/* decrement number of groups */

      /* Perform each group of same twiddle factor. */

	 /* NOTE that multiplications are not necessary */
	 /* for first (trivial) twiddle factor: 1 + j*0, */
         /* Perform each butterfly. */
         for (buttCntr = 0; buttCntr < numFactors; buttCntr += 2*numGroups) {
	 
	    /* Butterfly proper. */
	    upper = srcCV + buttCntr;
	    lower = upper + numGroups;
	    temp.real = upper->real + lower->real;
	    temp.imag = upper->imag + lower->imag;
	    lower->real = upper->real - lower->real;
	    lower->imag = upper->imag - lower->imag;
	    *upper = temp;

	 }

      newTwid = theTwids + offFactors - 1;	/* update twids pointer */
      for (groupCntr = 1; groupCntr < numGroups; groupCntr++) {

	 twid = *newTwid;			/* twiddle factor for group */

         /* Perform each butterfly. */
         for (buttCntr = groupCntr; buttCntr < numFactors; buttCntr += 2*numGroups) {
	 
	    /* Butterfly proper. */
	    upper = srcCV + buttCntr;
	    lower = upper + numGroups;
	    temp.real = upper->real + lower->real;
	    temp.imag = upper->imag + lower->imag;
	    tmp.real = upper->real - lower->real;
	    tmp.imag = upper->imag - lower->imag;
	    lower->real = tmp.real*twid.real - tmp.imag*twid.imag;
	    lower->imag = tmp.real*twid.imag + tmp.imag*twid.real;
	    *upper = temp;

	 }

	 /* Update twiddle pointer. */
	 newTwid += offFactors;

      }

      /* Update offset to factors. */
      offFactors *= 2;
   }

   /* Finally, to simulate implicit scaling implemented in */
   /* assembly code, scale results by 1/numElems. */
   /* Note that vector scale applies to simple (real valued) vectors. */
   return ((fractcomplex*) VectorScale (numElems*2, (fractional*) srcCV, (fractional*) srcCV, (fractional) 1./numElems));

} /* end of FFTComplexIP */

/*...........................................................................*/

extern fractcomplex* IFFTComplex (	/* Inverse Fast Fourier Transform */
					/* (complex) */
   int log2N,				/* log2(N), N is transform length */
   fractcomplex* dstCV,			/* ptr to destination complex vector */
   fractcomplex* srcCV,			/* ptr to source complex vector */
   					/* with frequency components */
					/* in natural order */
   fractcomplex* twidFactors,		/* base address of twiddle factors */
					/* either in X data or program memory */
					/* if in X data memory, it points at */
   					/* WN(0).real */
					/* if in program memory, base is the */
					/* offset from program page boundary */
					/* to address where factors located */
					/* (inline assembly psvoffset ()) */
   int factPage				/* if in X data memory, set to */
   					/* defined value COEFFS_IN_DATA */
					/* if in program memory, page number */
					/* where factors are located */
					/* (inline assembly psvpage ()) */
					/* dstCV returned */
   					/* with time samples */
					/* in natural order */
) {

   /* Local declarations. */
   int numElems = 1<<log2N;			/* number elements in vector */

   /* Copy source vector into destination. */
   dstCV = (fractcomplex*) VectorCopy (numElems*2, (fractional*) dstCV, (fractional*) srcCV);

   /* Apply in-place operation to destination vector. */
   dstCV = IFFTComplexIP (log2N, dstCV, twidFactors, factPage);

   /* Finally, return pointer to destination vector. */
   return (dstCV);

} /* end of IFFTComplex */

/*...........................................................................*/

extern fractcomplex* IFFTComplexIP (	/* Inverse Fast Fourier Transform */
					/* (complex) */
   int log2N,				/* log2(N), N is transform length */
   fractcomplex* srcCV,			/* ptr to source complex vector */
   					/* with frequency components */
					/* in bit reverse order */
   fractcomplex* twidFactors,		/* base address of twiddle factors */
					/* either in X data or program memory */
					/* if in X data memory, it points at */
   					/* WN(0).real */
					/* if in program memory, base is the */
					/* offset from program page boundary */
					/* to address where factors located */
					/* (inline assembly psvoffset ()) */
   int factPage				/* if in X data memory, set to */
   					/* defined value COEFFS_IN_DATA */
					/* if in program memory, page number */
					/* where factors are located */
					/* (inline assembly psvpage ()) */
					/* srcCV returned */
   					/* with time samples */
					/* in natural order */
) {

   /* Compute IFFT using DIF FFT algorithm. */
   FFTComplexIP (log2N, srcCV, twidFactors, factPage);

   /* Finally, unscramble results back to natural order. */
   return (BitReverseComplex (log2N, srcCV));

} /* end of IFFTComplexIP */

/*...........................................................................*/

extern fractional* CosineInit	 (	/* Initialize cosine factors */
					/* computed in floating point */
					/* converted to fractionals */
   int log2N,				/* log2(N), N is number of factors */
   fractional* cosFactors		/* ptr to cosine factors */
					/* cosfact returned */
) {

   /* Not implemented yet. */
   return ((fractional*) NULL);

} /* end of CosineInit */

/*...........................................................................*/

extern fractional* DCT (		/* Discrete Cosine Transform */
   int log2N,				/* log2(N), N is transform length */
   fractional* dstV,			/* ptr to destination vector (2*N) */
   					/* (transform in first N samples) */
   fractional* srcV,			/* ptr to source vector (N) in Y-Data */
   fractcomplex* cosFactors,		/* base address of cosine factors */
					/* either in X data or program memory */
					/* if in X data memory, it points at */
   					/* CN(0).real */
					/* if in program memory, base is the */
					/* offset from program page boundary */
					/* to address where factors located */
					/* (inline assembly psvoffset ()) */
					/* a total of N/2 complex values: */
					/* CN(k) = exp(i*k*pi/(2*N)) */
					/* CN(0)...CN(N/2-1) */
   fractcomplex* twidFactors,		/* base address of complex conjugate */
   					/* twiddle factors */
					/* either in X data or program memory */
					/* if in X data memory, it points at */
   					/* WN(0).real */
					/* if in program memory, base is the */
					/* offset from program page boundary */
					/* to address where factors located */
					/* (inline assembly psvoffset ()) */
					/* a total of N/2 complex values: */
					/* WN(k) = exp(-i*2*pi*k/N) */
					/* WN(0)...WN(N/2-1) */
   int factPage				/* if in X data memory, set to */
   					/* defined value COEFFS_IN_DATA */
					/* if in program memory, page number */
					/* where factors are located */
					/* (inline assembly psvpage ()) */

					/* Both, cosine and twiddle factors, */
					/* MUST be allocated in the same */
					/* memory space: both in X-Data, */
					/* or both in program memory */
					/* dstV returned */
					/* Only first N elements represent */
					/* DCT values scaled by sqrt(2*N) */
) {

   /* Local declarations. */
   int numElems = 1<<log2N;			/* number elements in vector */

   /* Zero pad source vector into destination. */
   dstV = VectorZeroPad (numElems, numElems, dstV, srcV);

   /* Apply in-place operation to destination vector. */
   dstV = DCTIP (log2N, dstV, cosFactors, twidFactors, factPage);

   /* Finally, return pointer to destination vector. */
   return (dstV);

} /* end of DCT */

/*...........................................................................*/

extern fractional* DCTIP (		/* Discrete Cosine Transform */
   int log2N,				/* log2(N), N is transform length */
   fractional* srcV,			/* ptr to source vector in Y-Data */
   					/* MUST BE zero padded to length 2*N */
   fractcomplex* cosFactors,		/* base address of cosine factors */
					/* either in X data or program memory */
					/* if in X data memory, it points at */
   					/* CN(0).real */
					/* if in program memory, base is the */
					/* offset from program page boundary */
					/* to address where factors located */
					/* (inline assembly psvoffset ()) */
					/* a total of N/2 complex values: */
					/* CN(k) = exp(i*k*pi/(2*N)) */
					/* CN(0)...CN(N/2-1) */
   fractcomplex* twidFactors,		/* base address of complex conjugate */
   					/* twiddle factors */
					/* either in X data or program memory */
					/* if in X data memory, it points at */
   					/* WN(0).real */
					/* if in program memory, base is the */
					/* offset from program page boundary */
					/* to address where factors located */
					/* (inline assembly psvoffset ()) */
					/* a total of N/2 complex values: */
					/* WN(k) = exp(-i*2*pi*k/N) */
					/* WN(0)...WN(N/2-1) */
   int factPage				/* if in X data memory, set to */
   					/* defined value COEFFS_IN_DATA */
					/* if in program memory, page number */
					/* where factors are located */
					/* (inline assembly psvpage ()) */

					/* Both, cosine and twiddle factors, */
					/* MUST be allocated in the same */
					/* memory space: both in X-Data, */
					/* or both in program memory */

					/* srcV returned */
					/* Only first N elements represent */
					/* DCT values scaled by sqrt(2*N) */
) {

   /* Local declarations. */
   fractional* retV = srcV;
   int N = 1<<log2N;				/* number elements in vector */
   fractional* x = srcV+1;
   fractional* y = srcV+(2*N-1);		/* end of padded area */
   fractcomplex* c = cosFactors+1;		/* c->{cos(theta),sin(theta)} */
   fractional tmp = 0;
   int cntr = 0;

   /* Arrange even/odd elements of x in complex y as follows:		*/
   /* y[n] = x[2*n], y[2*N-2-2*n] = x[2*n+1], for 0 <= n < N/2-1, and	*/
   /* y[2*n+1] = i*0, for 0 <= n < N-1.					*/
   /* result in place using zero padded area...				*/
   /* In other words, push all odd elements to zero padded area, and	*/
   /* zero out imaginary parts						*/
   for (cntr = N/2-1; cntr >= 0; cntr--) {	/* do N/2 times */
      tmp = *(x);				/* tmp = x[2*n+1] */
      *(x++) = 0;				/* x[2*n+1] = 0 (imag) */
      						/* x-> x[2*n+2] (real) */
      x++;					/* leave even value alone */
      						/* x-> x[2*n+3] (imag) */
      *(y--) = 0;				/* y[2*N-1-2*n] = 0 (imag) */
      						/* y->y[2*N-2-2*n] (real) */
      *(y--) = tmp;				/* y[2*N-2-2*n] = tmp (real) */
      						/* y->y[2*N-3-2*n] (imag) */
   }

   /* Apply complex IFFT to reorganized vector. */
   srcV = (fractional*) IFFTComplexIP (log2N, (fractcomplex*) srcV, twidFactors, (int) factPage);

   /* Modulate: Let cVect be the complex output of the IFFT; i.e.,	*/
   /*	cVect[n] = srcV[2*n] + i*srcV[2*n+1], 0 <= n < N.		*/
   /* Then, modulation consists of:					*/
   /*	cVect[n] = exp(i*n*theta)*cVect[n], 0 <= n < N/2, theta = PI/(2*N). */
   /* Since								*/
   /*	srcV[0] = real(cVect[0]) = cVect[0],				*/
   /*	srcV[N] = real(cVect[N/2]) = cVect[N/2], and for 1 <= n < N/2	*/
   /* 	srcV[2*n] = real(cVect[n]) and srcV[2*n+1] = imag(cVect[n]),	*/
   /* then modulation is equivalently applied as follows:		*/
   /*	srcV[0] = cos(0*theta)*srcV[0] = srcV[0], (trivial; do nothing) */
   /*	srcV[N] = cos((N/2)*theta)*srcV[N] + i*sin((N/2)*theta)*srcV[N],*/
   /* (but only imaginary part needed!),				*/
   /* and for 1 <= n < N/2						*/
   /*	srcV[2*n] = cos(n*theta)*srcV[2*n] - sin(n*theta)*srcV[2*n+1],	*/
   /* srcV[2*n+1] = cos(n*theta)*srcV[2*n+1] + sin(n*theta)*srcV[2*n].	*/
   /* NOTE that								*/
   /*	cosFactors[n] = cos(n*theta), and cosFactors[n+1] = sin(n*theta). */
   /* Do modulation: */
   x = srcV+2;					/* x-> srcV[2] */
   y = x+1;					/* y-> srcV[3] */
   for (cntr = N/2-2; cntr >= 0; cntr--) {	/* do N/2-1 times */
      tmp = *(x);				/* tmp = srcV[2*n] */
      *(x++) = (c->real)*tmp - (c->imag)*(*(y));
      						/* srcV[2*n] = real part */
						/* x-> srcV[2*n+1] */
      *(y++) = (c->real)*(*(x++)) + (c->imag)*tmp;
      						/* srcV[2*n+1] = imag part */
						/* x-> srcV[2*n+2] */
      c++;					/* next cosine-sine factors */
      y++;					/* next imag part */
   }
   						/* now x->srcV[N] */
   /* Now for N+1 (only imaginary part needed). */
   *(x) = SIN_PI_Q*(*(x));

   /* Reorganize into transform. */
   /* First, imaginary parts to upper half of vector (zero padd extension). */
   x = srcV+N-1;				/* x-> srcV[N-1] */
   y = x+2;					/* y-> srcV[N+1] */
   for (cntr = N/2-2; cntr >= 0; cntr--) {	/* do N/2-1 times */
      *(y++) = *(x--);				/* srcV[N+1+n]=srcV[N-1-2*n] */
      						/* y-> srcV[N+1+n+1] */
      						/* x-> srcV[N-1-2*n-1] */
      x--;					/* x-> srcV[N-1-2*n-2] */
   }

   /* Second, compact real parts. */
   x = srcV+2;					/* x = srcV[2] */
   y = x-1;					/* y = srcV[1] */
   for (cntr = N/2-2; cntr >= 0; cntr--) {	/* do N/2-1 times */
      *(y++) = *(x++);				/* srcV[1+n]=srcV[2+2*n] */
      						/* y-> srcV[1+n+1] */
      						/* x-> srcV[2+2*n+1] */
      x++;					/* x-> srcV[2+2*n+2] */
   }

   /* Third, bring imaginary parts back to lower half of vector. */
   x = srcV+N;					/* x = srcV[N] */
   y = x-N/2;					/* y = srcV[N/2] */
   for (cntr = N/2-1; cntr >= 0; cntr--) {	/* do N/2 times */
      *(y++) = *(x++);				/* srcV[N/2+n]=srcV[N+n] */
      						/* y-> srcV[N/2+n+1] */
      						/* x-> srcV[N+n+1] */
   }

   /* To avoid computation of square root, elements are not sacled. */
   /* The elements should be scaled by the factor sqrt(2*N). */
   /* First element must be scaled also by 1/sqrt(2) constant. */
   *(srcV) *= INV_SQRT2;			/* srcV[0] = srcV[0]/sqrt(2) */
   						/* no computation of sqrt(*) */

   /* Return pointer to (in-place) resulting vector. */
   return (retV);

} /* end of DCTIP */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
