/****************************************************************************
*
* $Source: /cvs/c30_dsp/src/iirmodel.c,v $
* $Revision: 1.1.1.1 $
*
* Copyright 2002, Microchip, Inc.  All rights reserved.
*
* Implementation of IIR filters.
*
* $Log: iirmodel.c,v $
* Revision 1.1.1.1  2003/06/20 22:22:59  wilkiec
* DSP library, src and verified build
*
****************************************************************************/

/* Local headers. */
#include "dsp.h"				/* DSP Library interface */

/*...........................................................................*/

extern fractional* IIRCanonic (		/* Direct Form II (Canonic) */
					/* biquad filtering */
   int numSamps,			/* number of input samples (N) */
   fractional* dstSamps,		/* ptr to output samples */
   					/* (y[n], 0 <= n < N) */
   fractional* srcSamps,		/* ptr to input samples */
   					/* (x[n], 0 <= n < N) */
   IIRCanonicStruct* filter		/* filter structure */

   					/* returns dstSamps */
) {

   /* NOTE: Because the filter coefficients are generated for a fractional */
   /* implementation, this function cannot use those coefficients and just */
   /* implement the difference equation, which is what would have given the */
   /* test robustness (comparing output of difference equation with cascade */
   /* of fractional biquads). Hence, a pseudo-fractional implementation is */
   /* being used in this model. */

   /* Local declarations. */
   fractional* y = dstSamps;
   fractional* x = srcSamps;
   fractional* k = filter->coeffsBase;			/* {a2,a1,b2,b1,b0} */
   							/* per section */
   fractional* del = filter->delayBase;			/* {w[-2], w[-1]} */
   							/* per section */
   fractional gain = filter->initialGain;
   int shift = filter->finalShift;
   fractional W2 = 0;					/* holder for w[-2] */
   fractional W1 = 0;					/* holder for w[-1] */
   fractional W = 0;
   fractional Y = 0;
   int N = numSamps;
   int M = filter->numSectionsLess1+1;
   int n = 0;
   int m = 0;

   /* Filter the N input samples. */
   for (n = 0; n < N; n++) {

      /* Init section. */
      Y = gain*(*(x++));				/* get input sample */
      k = filter->coeffsBase;				/* {a2,a1,b2,b1,b0} */
      del = filter->delayBase;				/* {w[-2], w[-1]} */

      /* Apply each second order section. */
      for (m = 0; m < M; m++) {

         /* Init temp holders. */
	 W  = Y;					/* W  = Y (before) */
	 W2 = *(del++);					/* W2 = w[-2] */
	 						/* del->w[-1] */
	 W1 = *(del--);					/* W2 = w[-1] */
	 						/* del->w[-2] */

	 /* Perform second order section computation. */
	 W += *(k++)*W2;
	 W += *(k++)*W1;
	 W *= 2;					/* like fractional */
	 Y  = *(k++)*W2;
	 Y += *(k++)*W1;
	 Y += *(k++)*W;

	 /* Update section delays. */
	 *(del++) = W1;					/* w[-2] = W1 */
	 						/* del->w[-1] */
	 *(del++) = W;					/* w[-1] = W  */
	 						/* del->w[-2] (next) */

      }

      /* Perform final shift. */
      if (shift < 0) {
         for (m = 0; m > shift; m--) {
	    Y /= 2.0;
	 }
      } else {
         for (m = 0; m < shift; m++) {
	    Y *= 2.0;
	 }
      }

      /* Save output. */
      *(y++) = Y*2;					/* like fractional */

   }

   /* Return destination samples pointer. */
   return (dstSamps);

} /* end of IIRCanonic */

/* ....................................................................... */

extern void IIRCanonicInit (  		/* Initialize filter structure */
   IIRCanonicStruct* filter		/* Canonic biquad filter structure */
) {

   /* Local declarations. */
   fractional* aPtr = filter->delayBase;
   int cntr = 0;

   for (cntr = 0; cntr <= filter->numSectionsLess1; cntr++) {
      *(aPtr++) = 0;				/* two delays per section */
      *(aPtr++) = 0;
   }

} /* end of IIRCanonicInit */

/* ....................................................................... */

extern fractional* IIRTransposed (	/* Direct Form II (Transposed) */
					/* biquad filtering */
   int numSamps,			/* number of input samples (N) */
   fractional* dstSamps,		/* ptr to output samples */
   					/* (y[n], 0 <= n < N) */
   fractional* srcSamps,		/* ptr to input samples */
   					/* (x[n], 0 <= n < N) */
   IIRTransposedStruct* filter		/* Transposed biquad filter structure */

   					/* returns dstSamps */
) {

   /* NOTE: Because the filter coefficients are generated for a fractional */
   /* implementation, this function cannot use those coefficients and just */
   /* implement the difference equation, which is what would have given the */
   /* test robustness (comparing output of difference equation with cascade */
   /* of fractional biquads). Hence, a pseudo-fractional implementation is */
   /* being used in this model. */

   /* Local declarations. */
   fractional* y = dstSamps;
   fractional* x = srcSamps;
   fractional* k = filter->coeffsBase;			/* {b0,b1,a1,b2,a2} */
   							/* per section */
   fractional* del1 = filter->delayBase1;		/* xm[n-2] (mth sect) */
   fractional* del2 = filter->delayBase2;		/* ym[n-2] (mth sect) */
   int shift = filter->finalShift;
   fractional A = 0;
   fractional B = 0;
   fractional W6 = 0;
   fractional W7 = 0;
   int N = numSamps;
   int M = filter->numSectionsLess1+1;
   int n = 0;
   int m = 0;

   /* Filter the N input samples. */
   for (n = 0; n < N; n++) {

      /* Init section. */
      k = filter->coeffsBase;				/* {a2,a1,b2,b1,b0} */
      W6 = *(x++);					/* get input sample */
      del1 = filter->delayBase1;
      del2 = filter->delayBase2;
      A = *(del1)/2;					/* A = W1[0] */
		
      /* Apply each second order section. */
      for (m = 0; m < M; m++) {

	 /* Perform second order section computation. */
	 A += (*(k++))*W6;				/* A = W1[m] + */
	 						/*   + b0[m]*X */
							/* k-> b1[m] */
	 B  = *(del2)/2;				/* B = W2[m] */
	 						/* like fractional */
	 W7  = A*2;					/* Y = y[n] */
	 						/* like fractional */
	 B += (*(k++))*W6;				/* B = W2[m] + */
	 						/*   + b1[m]*X */
							/* k-> a1[m] */
	 B += (*(k++))*W7;				/* B = W2[m] + */
	 						/*   + b1[m]*X */
	 						/*   + a1[m]*Y */
							/* k-> b2[m] */
	 *(del1++) = B*2;				/* W1[m] updated */
	 						/* like fractional */
							/* del1-> W1[m+1] */
	 B  = (*(k++))*W6;				/* B = b2[m]*X */
							/* k-> a2[m] */
	 W6  = A*2;					/* update X */
	 						/* like fractional */
	 A  = *(del1)/2;				/* A = W1[m+1] */
	 						/* like fractional */
	 B += (*(k++))*W7;				/* B = b2[m]*X + */
	 						/*   + a2[m]*Y */
							/* k-> b0[m+1] */
	 *(del2++) = B*2;				/* W2[m] updated */
	 						/* like fractional */
							/* del2-> W2[m+1] */
      }

      /* Perform final shift. */
      if (shift < 0) {
         for (m = 0; m > shift; m--) {
	    W6 /= 2.0;
	 }
      } else {
         for (m = 0; m < shift; m++) {
	    W6 *= 2.0;
	 }
      }

      /* Save output. */
      *(y++) = W6;

   }

   /* Return destination samples pointer. */
   return (dstSamps);

} /* end of IIRTransposed */

/* ....................................................................... */

extern void IIRTransposedInit (		/* Initialize filter structure */
   IIRTransposedStruct* filter		/* Transposed biquad filter structure */
) {

   /* Local declarations. */
   fractional* aPtr1 = filter->delayBase1;
   fractional* aPtr2 = filter->delayBase2;
   int cntr = 0;

   for (cntr = 0; cntr <= filter->numSectionsLess1; cntr++) {
      *(aPtr1++) = 0;				/* two delays per section */
      *(aPtr2++) = 0;
   }

} /* end of IIRTransposedInit */

/* ....................................................................... */

extern fractional* IIRLattice (		/* IIR Lattice filtering */
   int numSamps,			/* number of input samples (N) */
   					/* M <= N (see IIRLatticeStruct for M)*/
   fractional* dstSamps,		/* ptr to output samples (0 <= n < N) */
   fractional* srcSamps,		/* ptr to input samples (0 <= n < N) */
   IIRLatticeStruct* filter		/* filter structure */
   					/* returns dstSamps */
) {

   /* Local declarations. */
   fractional* retSamps = dstSamps;
   fractional* y = dstSamps;				/* points at y[0] */
   fractional* x = srcSamps;				/* points at x[0] */
   fractional* k = filter->kappaVals;			/* points at k[0] */
   fractional* g = filter->gammaVals;			/* points at g[0] */
   fractional* del = filter->delay;			/* points at d[0] */
   fractional current = 0;				/* current values */
   fractional after = 0;				/* after values */
   int N = numSamps;
   int M = filter->order;
   int n = 0;
   int m = 0;

   /* Filter the N input samples. */
   for (n = 0; n < N; n++) {

      /* Get new sample. */
      current = *(x++);

      /* Lattice structure. */
      for (m = 0; m < M; m++) {

         after = current - k[M-1-m]*del[m+1];
	 del[m] = del[m+1] + k[M-1-m]*after;
	 current = after;

      }
      del[M] = after;

      if (g != NULL) {
         /* Ladder structure. */
	 after = 0;					/* reuse after */
	 for (m = 0; m <= M; m++) {
	    after += g[M-m]*del[m];
	 }
      }

      /* Write output. */
      *(y++) = after;

   }

   /* Return destination samples pointer. */
   return (retSamps);

} /* end of IIRLattice */

/* ....................................................................... */

extern fractional* IIRLatticeAP (	/* IIR Lattice filtering (all-pole) */
   int numSamps,			/* number of input samples (N) */
   					/* M <= N (see IIRLatticeStruct for M)*/
   fractional* dstSamps,		/* ptr to output samples (0 <= n < N) */
   fractional* srcSamps,		/* ptr to input samples (0 <= n < N) */
   IIRLatticeStruct* filter		/* filter structure */
   					/* returns dstSamps */
) {

   /* Local declarations. */
   fractional* retSamps = dstSamps;
   fractional* y = dstSamps;
   fractional* x = srcSamps;
   fractional* k = filter->kappaVals+(filter->order-1);	/* k[M-1] */
   fractional* del = filter->delay+(filter->order-1);	/* del[M-1] */
   fractional* ep = NULL;				/* e'_(m)[n], 0<=m<M */
   							/* becomes delay... */
   fractional e = 0;					/* e_(m)[n], 0<=m<=M */
   fractional ep_M = 0;					/* e'_(M)[n] */
   int N = numSamps;
   int M = filter->order;
   int n = 0;
   int m = 0;

   /* Note that at this point, we have x-> x[0], and del[m] = e'_(m)[n-1] */
   /* Equations: */
   /*		e_(M)[n]   =  x[n], */
   /*		e_(m-1)[n] =  e_(m)[n] + k_(m-1)*e'_(m-1)[n-1], */
   /*		e'_(m)[n]  = -k_(m-1)*e_(m-1)[n] + e'_(m-1)[n-1], */
   /* 		e'_(0)[n] = e_(0)[n]; */
   /* and	y[n] = e_(0)[n], */
   /* with	0 <= n < N, and M >= m >= 1. */

   /* Filter the N input samples. */
   for (n = 0; n < N; n++) {

      /* For m = M (set up recursion). */
      e = *(x++);				/* e_(M)[n] = x[n] */
      						/* x-> x[n+1] */
      e += *(k)*(*(del));
      		/* e_(M-1)[n] = e_(M)[n] + k_(M-1)*e'_(M-1)[n-1] */
      ep_M = -(*(k--))*e +(*(del));
      		/* e'_(M)[n] = -k_(m-1)*e_(M-1)[n] + e'_(M-1)[n-1] */
		/* k -> k[M-2] */
      ep = (del--);
      		/* ep -> e'_(M-1)[n] */
		/* del -> e'_(M-2)[n-1] */

      /* For M-1 >= m >= 1. */
      for (m = M-1; m >= 1; m--) {

	 /* Compute recursive terms. */
	 e += *(k)*(*(del));
      		/* e_(m-1)[n] = e_(m)[n] + k_(m-1)*e'_(m-1)[n-1] */
	 *(ep--) = -(*(k--))*e + (*(del--));
      		/* e'_(m)[n] = -k_(m-1)*e_(m-1)[n] + e'_(m-1)[n-1] */
		/* k -> k[m-2] */
		/* del -> e'_(m-2)[n-1] */

      }
      /* Update first delay. */
      *(ep) = e;
      		/* e'_(0)[n] = e_(0)[n] */

      /* At last, generate output. */
      *(y++) = e;
      		/* y[n] = e'_(0)[n]) */
		/* y -> y[n+1] */

      /* Rewind pointers. */
      k = filter->kappaVals+(filter->order-1);
      del = filter->delay+(filter->order-1);

   }

   /* Return destination samples pointer. */
   return (retSamps);

} /* end of IIRLatticeAP */

/* ....................................................................... */

extern void IIRLatticeInit (		/* Initialize filter structure */
   IIRLatticeStruct* filter		/* filter structure */
){

   /* Local declarations. */
   fractional* aPtr = filter->delay;
   int cntr = 0;

   for (cntr = 0; cntr < filter->order; cntr++) {
      *(aPtr++) = 0;
   }

} /* end of IIRLatticeInit */

/* ....................................................................... */

/***************************************************************************/
/* EOF */
