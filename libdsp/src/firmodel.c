/****************************************************************************
*
* $Source: /cvs/c30_dsp/src/firmodel.c,v $
* $Revision: 1.1.1.1 $
*
* Copyright 2002, Microchip, Inc.  All rights reserved.
*
* Implementation of FIR filters.
*
* $Log: firmodel.c,v $
* Revision 1.1.1.1  2003/06/20 22:22:59  wilkiec
* DSP library, src and verified build
*
****************************************************************************/

/* Local headers. */
#include "dsp.h"				/* DSP Library interface */

/*...........................................................................*/

extern void FIRStructInit (		/* Initialize FIR filter structure */
   FIRStruct* FIRFilter,		/* FIR filter structure */
   int numCoeffs,			/* number of coeffs in filter (M) */
					/* same as filter order if lattice */
   					/* (h[m], 0 <= m < M) */
   					/* (if lattice, k[m], 0 <= m < M) */
   fractional* coeffsBase,		/* base address of filter coeffs */
					/* must be an even number */
					/* either in X data or program memory */
					/* if in X data memory, it points at */
   					/* h[0] (if lattice, k[0]) */
					/* if in program memory, base is the */
					/* offset from program page boundary */
					/* to address where coeffs located */
					/* (inline assembly psvoffset ()) */
   int coeffsPage,			/* if in X data memory, set to */
   					/* defined value COEFFS_IN_DATA */
					/* if in program memory, page number */
					/* where coeffs are located */
					/* (inline assembly psvpage ()) */
   fractional* delayBase		/* base address of delay buffer, */
					/* only in Y data */
   					/* points at d[0] of d[m], 0 <= m < M */
					/* returns initialized structure */
					/* (with delay = delayBase) */
) {

   /* Initialize structure elements. */
   FIRFilter->numCoeffs = numCoeffs;
   FIRFilter->coeffsBase = coeffsBase;
   FIRFilter->coeffsEnd = coeffsBase+numCoeffs-1;	/* not used, anyway */
   FIRFilter->coeffsPage = coeffsPage;
   FIRFilter->delayBase = delayBase;
   FIRFilter->delayEnd = delayBase+numCoeffs-1;		/* not used, anyway */
   FIRFilter->delay = delayBase;

} /* end of FIRStructInit */

/*...........................................................................*/

extern void FIRDelayInit (
   FIRStruct* FIRFilter			/* FIR filter structure */
) {

   /* Local declarations. */
   fractional* aPtr = FIRFilter->delayBase;
   int numDelays = FIRFilter->numCoeffs;
   int cntr = 0;

   for (cntr = 0; cntr < numDelays; cntr++) {
      *(aPtr++) = 0;
   }

} /* end of FIRDelayInit */

/* ....................................................................... */

extern void FIRInterpDelayInit (
   FIRStruct* FIRFilter,		/* FIR filter structure */
   int rate				/* rate of interpolation (1 to) R */
) {

   /* Local declarations. */
   fractional* aPtr = FIRFilter->delayBase;
   int numDelays = (FIRFilter->numCoeffs)/rate;
   int cntr = 0;

   for (cntr = 0; cntr < numDelays; cntr++) {
      *(aPtr++) = 0;
   }

} /* end of FIRInterpDelayInit */

/* ....................................................................... */

extern fractional* FIR (		/* FIR filtering */
					/* y[n] = sum_{k=0:M-1){h[n]*x[n-k]} */
					/* 0 <= n < N */
   int numSamps,			/* number of input samples (N) */
   fractional* dstSamps,		/* ptr to output samples */
   					/* (y[n], 0 <= n < N) */
   fractional* srcSamps,		/* ptr to input samples */
   					/* (x[n], 0 <= n < N) */
   FIRStruct* filter			/* filter structure */
   					/* returns dstSamps */
) {

   /* Local declarations. */
   fractional* retSamps = dstSamps;
   fractional* y = dstSamps;
   fractional* x = srcSamps;
   fractional* h = filter->coeffsBase;
   fractional* d = filter->delayBase;
   int N = numSamps;
   int M = filter->numCoeffs;
   int n = 0;
   int k = 0;

   /* Filter the samples. */
   /* First stage: 0 <= n < M-1, */
   /* y[n] = sum_(k=0:n){h[k]*x[n-k]} + sum(k=n+1:M-1){h[k]*d[M-1+n-k]}. */
   for (n = 0; n < M-1; n++) {
      *(y+n) = 0;
      for (k = 0; k <= n; k++) {	/* first sum (no delay) */
         *(y+n) += *(h+k)*(*(x+n-k));
      }
      for ( ; k < M; k++) {		/* second sum (only delay) */
         *(y+n) += *(h+k)*(*(d+M-1+n-k));
      }
   }

   /* Second stage: M-1 <= n < N, */
   /* y[n] = sum_(k=0:M-1){h[k]*x[n-k]}. */
   for ( ; n < N; n++) {
      *(y+n) = 0;
      for (k = 0; k < M; k++) {
         *(y+n) += *(h+k)*(*(x+n-k));
      }
   }

   /* Update delay. */
   for (k = 0, n = N-M; n < N; k++, n++) {
      *(d+k) = *(x+n);
   }

   /* Return pointer to destination samples. */
   return (retSamps);

} /* end of FIR */

/*...........................................................................*/

extern fractional* FIRLMS (		/* FIR Least Mean Square filtering */
					/* Not normalized implementation */
					/* y[n] = sum_{m=0:M-1){h[n]*x[n-m]} */
					/* 0 <= n < N */
					/* h_m[n] = h_m[n-1] + mu*e[n]*x[n-m] */
					/* 0 <= n < N, 0 <= m < M */
					/* with e[n] = r[n] - y[n] */
   int numSamps,			/* number of input samples (N) */
   fractional* dstSamps,		/* ptr to output samples */
   					/* (y[n], 0 <= n < N) */
   fractional* srcSamps,		/* ptr to input samples */
   					/* (x[n], 0 <= n < N) */
   FIRStruct* filter,			/* filter structure: */
   					/* number of coefficients in filter */
   					/* same as number of delay elements */
   					/* (h[m], 0 <= m < M) */
   					/* (d[m], 0 <= m < M) */
   fractional* refSamps,		/* ptr to reference samples */
   					/* (r[n], 0 <= n < N) */
   fractional muVal			/* mu value for correction */
   					/* returns dstSamps */
) {

   /* Local declarations. */
   fractional* retSamps = dstSamps;
   fractional* y = dstSamps;
   fractional* x = srcSamps;
   fractional* r = refSamps;
   fractional* h = filter->coeffsBase;
   fractional* d = filter->delayBase;
   fractional error = 0;
   int N = numSamps;
   int M = filter->numCoeffs;
   int n = 0;
   int k = 0;

   /* Filter the samples. */
   /* First stage: 0 <= n < M-1, */
   /* y[n] = sum_(k=0:n){h[k]*x[n-k]} + sum(k=n+1:M-1){h[k]*d[M-1+n-k]}. */
   for (n = 0; n < M-1; n++) {
      *(y+n) = 0;
      for (k = 0; k <= n; k++) {	/* first sum (no delay) */
         *(y+n) += *(h+k)*(*(x+n-k));
      }
      for ( ; k < M; k++) {		/* second sum (only delay) */
         *(y+n) += *(h+k)*(*(d+M-1+n-k));
      }
      
      /* With the new output, and the corresponding reference sample, */
      /* update the filter coefficients. */
      error = *(r+n) - *(y+n);			/* current error */
      error *= muVal;				/* attenuated error */
      /* Adaptation. */
      for (k = 0; k <= n; k++) {	/* first block (no delay) */
         *(h+k) += error*(*(x+n-k));
      }
      for ( ; k < M; k++) {		/* second block (only delay) */
         *(h+k) += error*(*(d+M-1+n-k));
      }
   }

   /* Second stage: M-1 <= n < N, */
   /* y[n] = sum_(k=0:M-1){h[k]*x[n-k]}. */
   for ( ; n < N; n++) {
      *(y+n) = 0;
      for (k = 0; k < M; k++) {
         *(y+n) += *(h+k)*(*(x+n-k));
      }

      /* With the new output, and the corresponding reference sample, */
      /* update the filter coefficients. */
      error = *(r+n) - *(y+n);			/* current error */
      error *= muVal;				/* attenuated error */
      /* Adaptation. */
      for (k = 0; k < M; k++) {
         *(h+k) += error*(*(x+n-k));
      }
   }

   /* Update delay. */
   for (k = 0, n = N-M; n < N; k++, n++) {
      *(d+k) = *(x+n);
   }

   /* Return pointer to destination samples. */
   return (retSamps);

} /* end of FIRLMS */

/*...........................................................................*/

extern fractional* FIRLMSNorm (		/* FIR Least Mean Square filtering*/
					/* Normalized implementation */
					/* y[n] = sum_{m=0:M-1}(h[n]*x[n-m]) */
					/* 0 <= n < N */
					/* h_m[n] = h_m[n-1] + nu*e[n]*x[n-m] */
					/* 0 <= n < N, 0 <= m < M */
					/* with e[n] = r[n] - y[n], and */
					/* nu[n] = mu/(mu+E[n]) */
					/* E[n]=E[n-1]+(x[n])^2-(x[n-M+1])^2 */
					/* is an estimate of input power */
					/* NOTE: to avoid saturation while */
					/* computing the estimate, the input */
					/* signal values should be bound */
					/* so that (x[n])^2 < M */
   int numSamps,			/* number of input samples (N) */
   fractional* dstSamps,		/* ptr to output samples */
   					/* (y[n], 0 <= n < N) */
   fractional* srcSamps,		/* ptr to input samples */
   					/* (x[n], 0 <= n < N) */
   FIRStruct* filter,			/* filter structure: */
   					/* number of coefficients in filter */
   					/* same as number of delay elements */
   					/* (h[m], 0 <= m < M) */
   					/* (d[m], 0 <= m < M) */
   fractional* refSamps,		/* ptr to reference samples */
   					/* (r[n], 0 <= n < N) */
   fractional muVal,			/* mu value for correction */
   fractional* energyEstimate		/* energy estimate for input samples */
					/* E[-1] = (x[-1])^2+...+(x[-M+1])^2 */
					/* on start up... (zero first time) */
					/* E[N-1]=(x[N-1])^2+...+(x[N-M+1])^2 */
   					/* upon return */
   					/* returns dstSamps */
) {

   /* Local declarations. */
   fractional* retSamps = dstSamps;
   fractional* y = dstSamps;
   fractional* x = srcSamps;
   fractional* r = refSamps;
   fractional* h = filter->coeffsBase;
   fractional* d = filter->delayBase;
   fractional error = 0;
   fractional nu = 0;
   int N = numSamps;
   int M = filter->numCoeffs;
   int n = 0;
   int k = 0;
   fractional estimate = *energyEstimate;			/* E[-1] */

   /* Filter the samples. */
   /* First stage: 0 <= n < M-1, */
   /* y[n] = sum_(k=0:n){h[k]*x[n-k]} + sum(k=n+1:M-1){h[k]*d[M-1+n-k]}. */
   for (n = 0; n < M-1; n++) {
      *(y+n) = 0;
      for (k = 0; k <= n; k++) {	/* first sum (no delay) */
         *(y+n) += *(h+k)*(*(x+n-k));
      }
      for ( ; k < M; k++) {		/* second sum (only delay) */
         *(y+n) += *(h+k)*(*(d+M-1+n-k));
      }
      
      /* With the new output, and the corresponding reference sample, */
      /* update the filter coefficients. */
      estimate += x[n]*x[n];			/* E[n-1] += (x[n])^2 */
      estimate -= d[n]*d[n];			/*        += (x[n-M+1])^2 */
      						/*         = E[n] */
      nu = muVal/(muVal + estimate);		/* nu[n] */
      error = *(r+n) - *(y+n);			/* current error */
      error *= nu;				/* attenuated error */
      /* Adaptation. */
      for (k = 0; k <= n; k++) {	/* first block (no delay) */
         *(h+k) += error*(*(x+n-k));
      }
      for ( ; k < M; k++) {		/* second block (only delay) */
         *(h+k) += error*(*(d+M-1+n-k));
      }
   }

   /* Second stage: M-1 <= n < N, */
   /* y[n] = sum_(k=0:M-1){h[k]*x[n-k]}. */
   for ( ; n < N; n++) {
      *(y+n) = 0;
      for (k = 0; k < M; k++) {
         *(y+n) += *(h+k)*(*(x+n-k));
      }

      /* With the new output, and the corresponding reference sample, */
      /* update the filter coefficients. */
      estimate += x[n]*x[n];			/* E[n-1] += (x[n])^2 */
      estimate -= x[n-M+1]*x[n-M+1];		/*        += (x[n-M+1])^2 */
      						/*         = E[n] */
      nu = muVal/(muVal + estimate);		/* nu[n] */
      error = *(r+n) - *(y+n);			/* current error */
      error *= nu;				/* attenuated error */
      /* Adaptation. */
      for (k = 0; k < M; k++) {
         *(h+k) += error*(*(x+n-k));
      }
   }

   /* Update delay. */
   for (k = 0, n = N-M; n < N; k++, n++) {
      *(d+k) = *(x+n);
   }
   /* Update power estimate. */
   *(energyEstimate) = estimate;

   /* Return pointer to destination samples. */
   return (retSamps);

} /* end of FIRLMSNorm */

/*...........................................................................*/

extern fractional* FIRDecimate (	/* Decimation by R:1 rate */
   int numSamps,			/* number of output samples (N) */
   					/* N = R*p (p integer) */
   fractional* dstSamps,		/* ptr output samples (0 <= n < N) */
   fractional* srcSamps,		/* ptr input samples (0 <= n < N*R) */
   FIRStruct* filter,			/* filter structure: */
   					/* number of coefficients in filter */
   					/* same as number of delay elements */
   					/* M = R*q (q integer) */
   					/* (h[m], 0 <= m < M) */
   					/* (d[m], 0 <= m < M) */
   int rate				/* rate of decimation R (to 1) */
   					/* returns dstSamps */
) {

   /* Local declarations. */
   fractional* retSamps = dstSamps;
   fractional* y = dstSamps;
   fractional* x = srcSamps;
   fractional* h = filter->coeffsBase;
   fractional* d = filter->delayBase;
   int N = numSamps;
   int M = filter->numCoeffs;
   int R = rate;
   int n = 0;
   int k = 0;
   int j = 0;
   int i = 0;

   /* Filter the N*R input samples. */
   for (j = 0, n = 0; n < N*R; j++, n += R) {
      /* Make room in delay for next R input samples. */
      for (k = 0; k <= M-1-R; k++) {
         *(d+k) = *(d+R+k);
      }
      /* Place next R input samples in dealy. */
      for (k = 0, i = n; k <= R-1; k++, i++) {
         *(d+M-R+k) = *(x+i);
      }
      /* Generate next output. */
      *(y+j) = 0;
      for (k = 0; k < M; k++) {
         *(y+j) += *(h+k)*(*(d+M-1-k));
      }
   }

   /* Return destination samples pointer. */
   return (retSamps);

} /* end of FIRDecimate */

/*...........................................................................*/

extern fractional* FIRInterpolate (	/* Interpolation by 1:R rate */
   int numSamps,			/* number of input samples (N) */
   					/* N = R*p (p integer) */
   fractional* dstSamps,		/* ptr output samples (0 <= n < N*R) */
   fractional* srcSamps,		/* ptr input samples (0 <= n < N) */
   FIRStruct* filter,			/* filter structure: */
   					/* number of coefficients in filter */
   					/* M = R*q (q integer) */
   					/* (h[m], 0 <= m < M) */
   					/* (d[m], 0 <= m < M/R) */
   int rate				/* rate of decimation R (to 1) */
   					/* returns dstSamps */
) {

   /* Local declarations. */
   fractional* retSamps = dstSamps;
   fractional* y = dstSamps;
   fractional* x = srcSamps;
   fractional* h = filter->coeffsBase;
   fractional* d = filter->delayBase;
   fractional tmpVal = 0;
   int N = numSamps;
   int M = filter->numCoeffs;
   int R = rate;
   int n = 0;
   int k = 0;
   int j = 0;
   int i = 0;

   /* Filter the N input samples. */
   for (n = 0; n < N; n++) {
      /* Make room in delay for next input sample. */
      for (k = 0; k <= M/R-2; k++) {
         *(d+k) = *(d+k+1);
      }
      /* Place next input sample in dealy. */
      *(d+M/R-1) = *(x+n);
      /* Generate next R outputs. */
      for (j = 0; j < R; j++) {
	 tmpVal = 0;
         for (i = 0, k = j; k < M; i++, k += R) {
            tmpVal += *(h+k)*(*(d+M/R-1-i));
         }
         *(y++) = tmpVal;
      }
   }

   /* Return destination samples pointer. */
   return (retSamps);

} /* end of FIRInterpolate */

/*...........................................................................*/

extern fractional* FIRLattice (		/* FIR Lattice filtering */
   int numSamps,			/* number of input samples (N) */
   fractional* dstSamps,		/* ptr to output samples (0 <= n < N) */
   fractional* srcSamps,		/* ptr to input samples (0 <= n < N) */
   FIRStruct* filter			/* filter structure: */
   					/* number of coefficients in filter */
   					/* (also known as kappa values) */
   					/* same as number of delay elements */
   					/* same as filter order */
   					/* (k[m], 0 <= m < M) */
   					/* (d[m], 0 <= m < M) */
   					/* returns dstSamps */
) {

   /* Local declarations. */
   fractional* retSamps = dstSamps;
   fractional* y = dstSamps;
   fractional* x = srcSamps;
   fractional* k = filter->coeffsBase;
   fractional* del = filter->delayBase;
   fractional dirNow = 0;			/* e_(m)[n] */
   fractional tmp = 0;				/* e_(m-1)[n] */
   fractional delBefore = 0;			/* e'_(m-1)[n-1] */
   int N = numSamps;
   int M = filter->numCoeffs;
   int n = 0;
   int m = 0;

   /* Note that at this point, we have x-> x[0], and del[m] = e'_(m)[n-1] */
   /* Equations: */
   /*		e_(0)[n] = e'_(0)[n] = x[n], */
   /*		e_(m)[n] =  e_(m-1)[n] - k_(m-1)*e'_(m-1)[n-1], */
   /*	       e'_(m)[n] = -k_(m-1)*e_(m-1)[n] + e'_(m-1)[n-1], */
   /* and	y[n] = e_(M)[n]; */
   /* with	0 <= n < N, and 0 <= m < M. */

   /* Filter the N input samples. */
   for (n = 0; n < N; n++) {

      /* For m = 0. */
      dirNow = *(x++);				/* e_(0)[n] = x[n] */
      						/* x-> x[n+1] */
      tmp = dirNow;				/* tmp = e_(0)[n] */
      delBefore = *(del);			/* delBefore = e'_(0)[n-1] */
      *(del++) = dirNow;			/* e'_(0)[n] = x[n] */
      						/* del-> e'_(1)[n-1] */

      /* For 1 <= m < M. */
      for (m = 1; m < M; m++) {

	 /* Compute recursive terms. */
	 dirNow -= *(k)*delBefore;
 		/* dirNow = e_(m)[n] = e_(m-1)[n] - k_(m-1)*e'_(m-1)[n-1] */
	 tmp = -(*(k++))*tmp + delBefore;
 		/* tmp = e'_(m)[n] = -k_(m-1)*e_(m-1)[n] + e'_(m-1)[n-1] */
      						/* k-> k_(m) */

	 /* Update. */
	 delBefore = *(del);			/* delBefore = e'_(m)[n-1] */
	 *(del++) = tmp;			/* e'_(m)[n] = tmp */
      						/* del-> e'_(m+1)[n-1] */
	 tmp = dirNow;				/* tmp = e_(m)[n] */
      }

      /* For M, generate output. */
      dirNow -= *(k)*delBefore;
 		/* dirNow = e_(M)[n] = e_(M-1)[n] - k_(M-1)*e'_(M-1)[n-1] */
      *(y++) = dirNow;				/* y[n] = e_(M)[n] */

      /* Rewind pointers. */
      k = filter->coeffsBase;
      del = filter->delayBase;

   }

   /* Return destination samples pointer. */
   return (retSamps);

} /* end of FIRLattice */

/* ....................................................................... */

/***************************************************************************/
/* EOF */
