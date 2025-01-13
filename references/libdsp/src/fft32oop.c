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
*   (c) Copyright 2007 Microchip Technology, All rights reserved.    *
*********************************************************************/

/* Local headers. */
#include "dsp.h"				/* DSP Library interface */

/*...........................................................................*/



extern long* FFTReal32b(
			int log2N, 
			int N,
			long *opBuff, 
			long *ipBuff, 
			long *tfBuf,
			int factPage			
){
	
	int i;
	for(i=0; i<N; i++)
		opBuff[i] = ipBuff[i];

	// Bit reverse the input samples
	BitReverseReal32bIP(N,opBuff);

	// Perform N/2 Point Complex FFT
  	FFTComplex32bIP(log2N, opBuff, tfBuf, factPage);  

	// Real FFT split function  
	realFft32bIP(log2N, opBuff, tfBuf, factPage);

	return(opBuff);

} /* end of FFTReal32b */

/*...........................................................................*/

/***************************************************************************/
/* EOF */
