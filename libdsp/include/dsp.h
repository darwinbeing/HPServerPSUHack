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

/****************************************************************************
*
* DSP.H
* Interface to the DSP Library for the dsPIC30F.
*
****************************************************************************/

#ifndef __DSP_LIB__     /* [ */
#define __DSP_LIB__

/***************************************************************************/

/* External headers. */

#include        <stdlib.h>              /* malloc, NULL */
#include        <math.h>                /* fabs, sin, cos, atan, sqrt */

/*...........................................................................*/

/* Local defines. */

#define FLOATING        -1                      /* using floating type */
#define FRACTIONAL       1                      /* using fractional type */
#ifndef DATA_TYPE                       /* [ */
#define DATA_TYPE       FRACTIONAL              /* default */
#endif  /* ] */

/* Some constants. */
#ifndef PI                              /* [ */
#define PI 3.1415926535897931159979634685441851615905761718750 /* double */
#endif  /* ] */
#ifndef SIN_PI_Q                                /* [ */
#define SIN_PI_Q 0.7071067811865474617150084668537601828575134277343750
                                                /* sin(PI/4), (double) */
#endif  /* ] */
#ifndef INV_SQRT2                               /* [ */
#define INV_SQRT2 SIN_PI_Q                      /* 1/sqrt(2), (double) */
                                                /* 1/sqrt(2) = sin(PI/4) */
#endif  /* ] */

#define BART_0           2.0                    /* Bartlett 0th factor */

#define HANN_0           0.50                   /* Hanning 0th factor */
#define HANN_1          -0.50                   /* Hanning 1st factor */

#define HAMM_0           0.53836                /* Hamming 0th factor */
#define HAMM_1          -0.46164                /* Hamming 1st factor */

#define BLCK_0           0.42                   /* Blackman 0th factor */
#define BLCK_1          -0.50                   /* Blackman 1st factor */
#define BLCK_2           0.08                   /* Blackman 2nd factor */

#define COEFFS_IN_DATA  0xFF00                  /* page number used for */
                                                /* filter coefficients */
                                                /* when allocated in X */
                                                /* data memory */
/*...........................................................................*/

/* Local types. */

/* Type definitions. */
#ifndef fractional                      /* [ */

#if     DATA_TYPE==FLOATING             /* [ */
typedef double          fractional;
#else   /* ] */
typedef int             fractional;
#endif  /* ] */

#ifndef fractcomplex                    /* [ */
typedef struct {
  fractional real;
  fractional imag;
} fractcomplex;
#endif  /* ] fractcomplex */

#endif  /* ] fractional */

/*...........................................................................*/

/****************************************************************************
*
* Preliminary remarks.
*
* None of the functions with the exception of SetStackGuard provided within 
* this API allocate memory space.
*
****************************************************************************/

/*...........................................................................*/

/****************************************************************************
*
* Interface to generic function prototypes.
*
****************************************************************************/

/* Generic function prototypes. */
#define Q15(X) \
   ((X < 0.0) ? (int)(32768*(X) - 0.5) : (int)(32767*(X) + 0.5)) 

#if     DATA_TYPE==FLOATING             /* [ */
#define Float2Fract(aVal)       (aVal)  /* Identity function */
#define Fract2Float(aVal)       (aVal)  /* Identity function */
#else   /* ] */
extern fractional Float2Fract (         /* Converts float into fractional */
   float aVal                           /* float value in range [-1, 1) */
);
extern float Fract2Float (        /* Converts fractional into float */
   fractional aVal         /* fract value in range {-1, 1-2^-15} */
);
#endif  /* ] */

/*...........................................................................*/

/****************************************************************************
*
* Interface to vector operations.
*
* A vector is a collection of numerical values, the vector elements,
* allocated contiguosly in memory, with the first element at the
* lowest memory address. One word of memory (two bytes) is used to
* store the value of each element, and this quantity must be interpreted
* as a fractional value in Q.15 format.
*
* A pointer addressing the first element of the vector is used as
* a handle which provides access to each of the vector values. The
* one dimensional arrangement of a vector fits with the memory
* storage model, so that the n-th element of an N-element vector
* can be accessed from the vector's base address BA as:
*
*       BA +  (n-1)*2,
*
* Note that because of the byte addressing capabilities of the dsPIC30F,
* the addressing of vector elements uses an increment (or decrement) size
* of 2: INC2 (or DEC2) instruction.
*
* Unary and binary operations are prototyped in this interface. The
* operand vector in a unary operation is called the source vector.
* In a binary operation the first operand is referred to as source
* one vector, and the second as source two vector. Each operation
* applies some computation to one or several elements of the source
* vector(s). Some operations result in a scalar value (also to be
* interpreted as a Q.15 fractional number), others in a vector. When
* the result is also a vector this is referred to as the destination
* vector.
*
* Some operations resulting in a vector allow computation in place;
* i.e., the results of the operations are placed back on the source
* (or source one, if binary) vector. In this case, the destination
* vector is said to (physically) replace the source (one) vector.
* When an operation can be computed in place it is indicated as such
* in the comments provided with its prototype.
*
* For some binary operations, the two operands can be the same (physical)
* source vector: the operation is applied between the source vector
* and itself. If this type of computation is possible for a given
* operation, it is indicated as such in the comments provided with
* its prototype.
*
* Some operations can be self applicable and computed in place.
*
* The operations prototyped in this interface take as an argument the
* cardinality (number of elements) of the operand vector(s). It is
* assumed that this number is in the range {1, 2, ..., (2^14)-1}, and
* that in the case of binary operations both operand vectors have the
* same cardinality. Note that no boundary checking is performed by
* the operations, and that out of range cardinalities as well as the
* use of source vectors of different sizes in binary operations may
* produce unexpected results.
*
* Additional remarks.
*
* A) Operations which return a destination vector can be nested, so that
*    for instance if:
*
*       a = Op1 (b, c), with b = Op2 (d), and c = Op3 (e, f), then
*
*       a = Op1 (Op2 (d), Op3 (e, f))
*
* B) The vector dot product and power operations could lead to saturation
*    if the sum of products is greater than 1-2^(-15) or smaller than -1.
*
* C) All the functions have been designed to operate on vectors allocated
*    in default RAM memory space (X-Data or Y-Data).
*
* D) The sum of sizes of the vector(s) involved in an operation must not
*    exceed the available memory in the target device.
*
****************************************************************************/

/* Vector operation prototypes. */


extern fractional VectorMax (           /* Vector (last) maximum */
                                        /* maxVal = max{srcV[elem]} */
                                        /* if srcV[i] = srcV[j] = maxVal */
                                        /* and i < j, then *(maxIndex) = j */
   int numElems,                        /* number elements in srcV */
   fractional* srcV,                    /* ptr to source vector */
   int* maxIndex                        /* ptr to index for maximum value */

                                        /* maxVal returned */
);

/*...........................................................................*/

extern fractional VectorMin (           /* Vector (last) minimum */
                                        /* minVal = min{srcV[elem]} */
                                        /* if srcV[i] = srcV[j] = minVal */
                                        /* and i < j, then *(minIndex) = j */
   int numElems,                        /* number elements in srcV */
   fractional* srcV,                    /* ptr to source vector */
   int* minIndex                        /* ptr to index for minimum value */

                                        /* minVal returned */
);

/*...........................................................................*/

extern fractional* VectorCopy (         /* Copy elements from source vector */
                                        /* to an (existing) dest vector */
                                        /* (both srcV and dstV MUST have, */
                                        /* at least, numElems elements). */
                                        /* dstV[n] = srcV[n], 0 <= n < N */
                                        /* (in place capable) */
   int numElems,                        /* number elements to copy (N) */
   fractional* dstV,                    /* ptr to destination vector */
   fractional* srcV                     /* ptr to source vector */

                                        /* dstV returned */
);

/*...........................................................................*/

extern fractional* VectorZeroPad (      /* Zero pad tail of a vector */
                                        /* dstV[n] = srcV[n], 0 <= n < N */
                                        /* dstV[n] = 0, N <= n < N+M */
                                        /* (srcV MUST have length >= N) */
                                        /* (dstV MUST have length N+M) */
                                        /* (in place capable) */
   int numElems,                        /* number elements in srcV (N) */
   int numZeros,                        /* number zeros to append (M) */
   fractional* dstV,                    /* ptr to destination vector */
   fractional* srcV                     /* ptr to source vector */

                                        /* dstV returned */
);

/*...........................................................................*/

extern fractional* VectorNegate (       /* Vector negate */
                                        /* dstV[n] = (-1)*srcV[n]+0, 0<=n<N */
                                        /* (in place capable) */
   int numElems,                        /* number elements in srcV (N) */
   fractional* dstV,                    /* ptr to destination vector */
   fractional* srcV                     /* ptr to source vector */

                                        /* dstV returned */
);

/*...........................................................................*/

extern fractional* VectorScale (        /* Vector scale */
                                        /* dstV[elem] = sclVal*srcV[elem] */
                                        /* (in place capable) */
   int numElems,                        /* number elements in srcV (N) */
   fractional* dstV,                    /* ptr to destination vector */
   fractional* srcV,                    /* ptr to source vector */
   fractional sclVal                    /* scale value (Q.15 fractional) */

                                        /* dstV returned */
);

/*...........................................................................*/

extern fractional* VectorAdd (          /* Vector addition */
                                        /* dstV[elem] =                 */
                                        /*    = srcV1[elem] + srcV2[elem] */
                                        /* (in place capable) */
                                        /* (with itself capable) */
   int numElems,                        /* number elements in srcV[1,2] (N) */
   fractional* dstV,                    /* ptr to destination vector */
   fractional* srcV1,                   /* ptr to source vector one */
   fractional* srcV2                    /* ptr to source vector two */

                                        /* dstV returned */
);

/*...........................................................................*/

extern fractional* VectorSubtract (     /* Vector subtraction */
                                        /* dstV[elem] =                 */
                                        /*    = srcV1[elem] - srcV2[elem] */
                                        /* (in place capable) */
                                        /* (with itself capable) */
   int numElems,                        /* number elements in srcV[1,2] (N) */
   fractional* dstV,                    /* ptr to destination vector */
   fractional* srcV1,                   /* ptr to source vector one */
   fractional* srcV2                    /* ptr to source vector two */

                                        /* dstV returned */
);

/*...........................................................................*/

extern fractional* VectorMultiply (     /* Vector elem-to-elem multiply */
                                        /* dstV[elem] =                 */
                                        /*    = srcV1[elem] * srcV2[elem] */
                                        /* (in place capable) */
                                        /* (with itself capable) */
   int numElems,                        /* number elements in srcV[1,2] (N) */
   fractional* dstV,                    /* ptr to destination vector */
   fractional* srcV1,                   /* ptr to source vector one */
   fractional* srcV2                    /* ptr to source vector two */

                                        /* dstV returned */
);

/*...........................................................................*/

extern fractional VectorDotProduct (    /* Vector dot product */
                                        /* dotVal =                     */
                                        /*    = sum(srcV1[elem]*srcV2[elem]) */
                                        /* (with itself capable) */
   int numElems,                        /* number elements in srcV[1,2] (N) */
   fractional* srcV1,                   /* ptr to source vector one */
   fractional* srcV2                    /* ptr to source vector two */

                                        /* dot product value returned */
);

/*...........................................................................*/

extern fractional VectorPower (         /* Vector power */
                                        /* powVal =                     */
                                        /*    = sum(srcV[elem]^2)       */
   int numElems,                        /* number elements in srcV (N) */
   fractional* srcV                     /* ptr to source vector one */

                                        /* power value returned */
);

/*...........................................................................*/

extern fractional* VectorConvolve (     /* Vector Convolution */
                                        /* (with itself capable) */
   int numElems1,                       /* number elements in srcV1 */
   int numElems2,                       /* number elements in srcV2 */
                                        /* numElems2 <= numElems1 */
   fractional* dstV,                    /* ptr to destination vector */
                                        /* with numElems1+numElems2-1 elems */
   fractional* srcV1,                   /* ptr to source vector one */
   fractional* srcV2                    /* ptr to source vector two */

                                        /* dstV returned */
);

/*...........................................................................*/

extern fractional* VectorCorrelate (    /* Vector Correlation */
                                        /* (with itself capable) */
   int numElems1,                       /* number elements in srcV1 */
   int numElems2,                       /* number elements in srcV2 */
                                        /* numElems2 <= numElems1 */
   fractional* dstV,                    /* ptr to destination vector */
                                        /* with numElems2+numElems1-1 elems */
   fractional* srcV1,                   /* ptr to source vector one */
   fractional* srcV2                    /* ptr to source vector two */

                                        /* dstV returned */
);

/*...........................................................................*/

/****************************************************************************
*
* Interface to windowing operations.
*
* A window is a vector with a specific value distribution within its
* domain ( 0 <= n < numElems). The particular value distribution depends
* on the window being generated.
*
* Given a vector, its value distribution may be modified by applying
* a window to it. In these cases, the window must have the same number
* of elements as the vector to modify.
*
* Before a vector can be windowed, the window must be created. Window
* initialization operations are provided which generate the values of
* the window elements. For higher numerical precision, these values are
* computed in floating point arithmetic, and the resulting quantities
* stored as Q.15 fractionals.
*
* To avoid excessive overhead when applying a window operation, and since
* given a window length the values of the window elements are fixed, a
* particular window could be generated once and used many times during
* the execution of the program. Thus, it is advisable to store the window
* returned by any of the initialization operations in a permanent (static)
* vector.
*
* Additional remarks.
*
* A) All the window initialization functions have been designed to generate
*    window vectors allocated in default RAM memory space (X-Data and Y-Data).
*
* B) The windowing function is designed to operate on vectors allocated
*    in default RAM memory space (X-Data and Y-Data).
*
****************************************************************************/

/* Windowing operation prototypes. */


extern fractional* BartlettInit (       /* Initialize a Bartlett window */
                                        /* computed in floating point */
                                        /* converted to fractionals */
   int numElems,                        /* number elements in window */
   fractional* window                   /* ptr to window */

                                        /* window returned */
);

/*...........................................................................*/

extern fractional* BlackmanInit (       /* Initialize a Blackman window */
                                        /* computed in floating point */
                                        /* converted to fractionals */
   int numElems,                        /* number elements in window */
   fractional* window                   /* ptr to window */

                                        /* window returned */
);

/*...........................................................................*/

extern fractional* HammingInit (        /* Initialize a Hamming window */
                                        /* computed in floating point */
                                        /* converted to fractionals */
   int numElems,                        /* number elements in window */
   fractional* window                   /* ptr to window */

                                        /* window returned */
);

/*...........................................................................*/

extern fractional* HanningInit (        /* Initialize a Hanning window */
                                        /* computed in floating point */
                                        /* converted to fractionals */
   int numElems,                        /* number elements in window */
   fractional* window                   /* ptr to window */

                                        /* window returned */
);

/*...........................................................................*/

extern fractional* KaiserInit (         /* Initialize a Kaiser window */
                                        /* computed in floating point */
                                        /* converted to fractionals */
   int numElems,                        /* number elements in window */
   fractional* window,                  /* ptr to window */
   float betaVal                        /* shape parameter */

                                        /* window returned */
);

/*...........................................................................*/

extern fractional* VectorWindow (       /* Apply window to vector */
                                        /* dstV[n] = srcV[n] * window[n], */
                                        /* 0 <= n < numElems */
                                        /* (in place capable) */
   int numElems,                        /* number elements in srcV and window */
   fractional* dstV,                    /* ptr to destination vector */
   fractional* srcV,                    /* ptr to source vector */
   fractional* window                   /* ptr to window */

                                        /* dstV returned */
);

/*...........................................................................*/

/****************************************************************************
*
* Interface to matrix operations.
*
* A matrix is a collection of numerical values, the matrix elements,
* allocated contiguosly in memory, with the first element at the
* lowest memory address. One word of memory (two bytes) is used to
* store the value of each element, and this quantity must be interpreted
* as a fractional value in Q.15 format.
*
* A pointer addressing the first element of the matrix is used as
* a handle which provides access to each of the matrix values. The
* two dimensional arrangement of a matrix is emulated in the memory
* storage area by placing its elements organized in row major order.
* Thus, the first value in memeory is the first element of the first
* row. It is followed by the rest of the elements of the first row.
* Then, the elements of the second row are stored, and so on, until
* all the rows are in memory. This way, the element at row r and
* column c of a matrix with R rows and C columns is located from
* the matrix base address BA at:
*
*       BA + ((r-1)*C + c-1)*2,
*
* Note that because of the byte addressing capabilities of the dsPIC30F,
* the addressing of a matrix element uses an increment (or decrement)
* size of 2: INC2 (or DEC2) instruction.
*
* Unary and binary operations are prototyped in this interface. The
* operand matrix in a unary operation is called the source matrix.
* In a binary operation the first operand is referred to as source
* one matrix, and the second as source two matrix. Each operation
* applies some computation to one or several elements of the source
* matrix(ces). The operations result in a matrix, referred to as the
* destination matrix.
*
* Some operations resulting in a matrix allow computation in place;
* i.e., the results of the computation are placed back on the source
* (or source one, if binary) matrix. In this case, the destination
* matrix is said to (physically) replace the source (one) matrix.
* When an operation can be computed in place it is indicated as such
* in the comments provided with its prototype.
*
* For some binary operations, the two operands can be the same (physical)
* source matrix: the operation is applied between the source matrix
* and itself. If this type of computation is possible for a given
* operation, it is indicated as such in the comments provided with
* its prototype.
*
* Some operations can be self applicable and computed in place.
*
* The operations prototyped in this interface take the number of rows
* and the number of columns in the operand matrix(ces) as arguments.
* The number of rows times that of columns must be within the range
* {1, 2, ..., (2^14)-1}. In the case of binary operations the number
* of rows and columns of the operand matrices must obey the rules of
* matrix algebra; i.e., for matrix addition and subtraction the two
* matrices must have the same number of rows and columns, while for
* matrix multiplication, the number of columns of the first operand
* must be the same as the number of rows of the second operand. The
* source matrix to the inversion operation must be square (the same
* number of rows as of columns), and non-singular (its determinat
* different than zero).
*
* NOTE: no boundary checking is performed by the operations. So forth,
* out of range number of rows or columns as well as the use of source
* matrices not adhering to the previous rules may produce unexpected
* results.
*
* Additional remarks.
*
* A) Operations which return a destination matrix can be nested, so
*    that for instance if:
*
*       a = Op1 (b, c), with b = Op2 (d), and c = Op3 (e, f), then
*
*       a = Op1 (Op2 (d), Op3 (e, f))
*
* B) The computation of the inverse of a matrix takes as input a floating
*    point valued matrix, uses floating point arithmentic, and returns a
*    floating point valued matrix.
*
* C) All the functions have been designed to operate on matrices allocated
*    in default RAM memory space (X-Data and Y-Data).
*
* D) The sum of sizes of the matrix(ces) involved in an operation must not
*    exceed the available memory in the target device.
*
****************************************************************************/

/* Matrix operation prototypes. */


extern fractional* MatrixScale (        /* Matrix scale */
                                        /* dstM[i][j] = sclVal*srcM[i][j] */
                                        /* (in place capable) */
   int numRows,                         /* number rows in srcM (R) */
   int numCols,                         /* number columns in srcM (C) */
   fractional* dstM,                    /* ptr to destination matrix */
   fractional* srcM,                    /* ptr to source matrix */
   fractional sclVal                    /* scale value (Q.15 fractional) */

                                        /* dstM returned */
);

/*...........................................................................*/

extern fractional* MatrixTranspose (    /* Matrix transpose */
                                        /* dstM[i][j] = srcM[j][i] */
                                        /* (in place capable) */
   int numRows,                         /* number rows in srcM (R) */
   int numCols,                         /* number columns in srcM (C) */
   fractional* dstM,                    /* ptr to destination matrix */
   fractional* srcM                     /* ptr to source matrix */

                                        /* dstM returned */
);

/*...........................................................................*/

float* MatrixInvert (                   /* Matrix inverse */
                                        /* dstM = srcM^(-1) */
                                        /* (in place capable) */
   int numRowsCols,                     /* number rows and columns in matrix */
                                        /* matrix MUST be square */
   float* dstM,                         /* ptr to destination matrix */
   float* srcM,                         /* ptr to source matrix */
   float* pivotFlag,                    /* internal use; size numRowsCols */
   int* swappedRows,                    /* internal use; size numRowsCols */
   int* swappedCols                     /* internal use; size numRowsCols */
                                        /* last three vectors required from */
                                        /* user, so that function is not */
                                        /* responsible for memory management */

                                        /* dstM returned (or NULL on error */
                                        /* if source matrix is singular) */
);

/*...........................................................................*/

extern fractional* MatrixAdd (          /* Matrix addition */
                                        /* dstM[i][j] =                 */
                                        /*    srcM1[i][j] + srcM2[i][j] */
                                        /* (in place capable) */
                                        /* (with itself capable) */
   int numRows,                         /* number rows in srcM[1,2] (R) */
   int numCols,                         /* number columns in srcM[1,2] (C) */
   fractional* dstM,                    /* ptr to destination matrix */
   fractional* srcM1,                   /* ptr to source one matrix */
   fractional* srcM2                    /* ptr to source two matrix */

                                        /* dstM returned */
);

/*...........................................................................*/

extern fractional* MatrixSubtract (     /* Matrix subtraction */
                                        /* dstM[i][j] =                 */
                                        /*    srcM1[i][j] - srcM2[i][j] */
                                        /* (in place capable) */
                                        /* (with itself capable) */
   int numRows,                         /* number rows in srcM[1,2] (R) */
   int numCols,                         /* number columns in srcM[1,2] (C) */
   fractional* dstM,                    /* ptr to destination matrix */
   fractional* srcM1,                   /* ptr to source one matrix */
   fractional* srcM2                    /* ptr to source two matrix */

                                        /* dstM returned */
);

/*...........................................................................*/

extern fractional* MatrixMultiply (     /* Matrix multiplication */
                                        /* dstM[i][j] =                 */
                                        /*    sum_k(srcM1[i][k]*srcM2[k][j]) */
                                        /* i in {0, 1, ..., numRows1-1} */
                                        /* j in {0, 1, ..., numCols2-1} */
                                        /* k in {0, 1, ..., numCols1Rows2-1} */
                                        /* (in place capable, only square) */
                                        /* (with itself capable, only square) */
   int numRows1,                        /* number rows in srcM1 */
   int numCols1Rows2,                   /* number columns in srcM1, same as */
                                        /* number rows in srcM2 */
   int numCols2,                        /* number columns srcM2 */
   fractional* dstM,                    /* ptr to destination matrix */
   fractional* srcM1,                   /* ptr to source one matrix */
   fractional* srcM2                    /* ptr to source two matrix */

                                        /* dstM returned */
);

/*...........................................................................*/

/****************************************************************************
*
* Interface to FIR filter operations.
*
* Filtering a data sequence x[n] with an FIR filter of impulse response
* b[m] (0<= m < M) is equivalent to solving the difference equation:
*
*       y[n] = sum_{m = 0:M-1}(b[m]*x[n-m])
*
* In this operation it is important to know and manage the past history
* of the data sequence (x[m], -M+1 <= m < 0) which represent the initial
* condition of the filtering operation. Also, when applying an FIR filter
* to contiguous sections of a data sequence it is necessary to remember
* the final state of the previous filtering operation (x[m], N-M+1 <= m < N-1),
* and take the state into consideration for the calculations of the next
* filtering stage. Accounting for the past history and current state is
* required to perform a correct (glitch-free) filtering operation.
*
* The management of the past history and current state of the filtering
* operation is commonly implemented via an additional sequence, referred
* to as the delay. Prior to a filtering operation the delay describes the
* past history of the data sequence. After performing the FIR filtering
* the delay contains a set of the most recently filtered data samples.
* (For correct operation, it is advisable to initialize the delay values
* to zero by calling the corresponding init function.)
*
* Even though FIR filtering is a difference equation, several properties
* of FIR filters allow for computation of the operation in more effective
* ways than that of a straight difference equation. Consequently, a set
* of such implementations are hereby provided.
*
* Note that of the four main sequences involved in FIR filtering, input
* data, output data, filter coefficients and delay, the last two are
* usually thought of as making up the filter structure. All the functions
* that follow use the same FIR filter structure to manage the filtering
* operation.
*
* In the current design, the input data sequence is referred to as the
* sequence of source samples, while the resulting filtered sequence
* contains the destination samples. The filters are characterized by
* the number of coefficients or taps, and the delay properties. All of
* these data sets are stored in memory as vectors with their elements
* representing Q.15 fractional quantities. Also, the input and output
* sequences to the filtering operation ought to be allocated in default
* RAM memory (X-Data or Y-Data). The coefficients may be allocated either
* in X-Data or program memory, while the delays must be allocated solely
* in Y-Data memory.
*
****************************************************************************/

/* FIR filter operation prototypes. */


typedef struct {
   int numCoeffs;                       /* number of coeffs in filter (M) */
                                        /* (same as filter order if lattice) */
                                        /* (h[m], 0 <= m < M) */
                                        /* (if lattice, k[m], 0 <= m < M) */
   fractional* coeffsBase;              /* base address of filter coeffs */
                                        /* either in X data or program memory */
                                        /* if in X data memory, it points at */
                                        /* h[0] (if lattice, k[0]) */
                                        /* if in program memory, base is the */
                                        /* offset from program page boundary */
                                        /* to address where coeffs located */
                                        /* (inline assembly psvoffset ()) */
                                        /* when indicated, it must be at a */
                                        /* 'zero' power of 2 address (since */
                                        /* in those cases it is implemented */
                                        /* as an increasing circular buffer) */
   fractional* coeffsEnd;               /* end address of filter coeffs */
                                        /* must be an odd number */
                                        /* if in data memory, points at */
                                        /* last byte of coefficients buffer */
                                        /* if in program memory, end is the */
                                        /* offset from program page boundary */
                                        /* to address of last byte of coeffs */
   int coeffsPage;                      /* if in X data memory, set to */
                                        /* defined value COEFFS_IN_DATA */
                                        /* if in program memory, page number */
                                        /* where coeffs are located */
                                        /* (inline assembly psvpage ()) */
   fractional* delayBase;               /* base address of delay buffer, */
                                        /* only in Y data */
                                        /* points at d[0] of d[m], 0 <= m < M */
                                        /* when indicated, it must be at a */
                                        /* 'zero' power of 2 address (since */
                                        /* in those cases it is implemented */
                                        /* as an increasing circular buffer) */
   fractional* delayEnd;                /* end address of delay buffer, */
                                        /* points at last byte of buffer */
   fractional* delay;                   /* current value of delay pointer */
} FIRStruct;


extern void FIRStructInit (             /* Initialize FIR filter structure */
   FIRStruct* FIRFilter,                /* FIR filter structure */
   int numCoeffs,                       /* number of coeffs in filter (M) */
                                        /* (same as filter order if lattice) */
                                        /* (h[m], 0 <= m < M) */
                                        /* (if lattice, k[m], 0 <= m < M) */
   fractional* coeffsBase,              /* base address of filter coeffs */
                                        /* either in X data or program memory */
                                        /* if in X data memory, it points at */
                                        /* h[0] (if lattice, k[0]) */
                                        /* if in program memory, base is the */
                                        /* offset from program page boundary */
                                        /* to address where coeffs located */
                                        /* (inline assembly psvoffset ()) */
                                        /* when indicated, it must be at a */
                                        /* 'zero' power of 2 address (since */
                                        /* in those cases it is implemented */
                                        /* as an increasing circular buffer) */
   int coeffsPage,                      /* if in X data memory, set to */
                                        /* defined value COEFFS_IN_DATA */
                                        /* if in program memory, page number */
                                        /* where coeffs are located */
                                        /* (inline assembly psvpage ()) */
   fractional* delayBase                /* base address of delay buffer, */
                                        /* only in Y data */
                                        /* points at d[0] of d[m], 0 <= m < M */
                                        /* when indicated, it must be at a */
                                        /* 'zero' power of 2 address (since */
                                        /* in those cases it is implemented */
                                        /* as an increasing circular buffer) */
                                        /* upon return, FIR filter structure */
                                        /* is initialized (delay = delayBase) */
);


extern void FIRDelayInit (              /* Zero out dealy in filter structure */
   FIRStruct* filter                    /* filter structure */
                                        /* NOTE: FIR interpolator's delay is */
                                        /* initialized by FIRInterpDelayInit */
);


extern void FIRInterpDelayInit (        /* Zero out dealy in filter structure */
                                        /* for FIR interpolator */
   FIRStruct* filter,                   /* filter structure */
   int rate                             /* rate of interpolation (1 to) R */
);


extern fractional* FIR (                /* FIR filtering */
   int numSamps,                        /* number of input samples (N) */
   fractional* dstSamps,                /* ptr to output samples */
                                        /* (y[n], 0 <= n < N) */
   fractional* srcSamps,                /* ptr to input samples */
                                        /* (x[n], 0 <= n < N) */
   FIRStruct* filter                    /* filter structure: */
                                        /* number of coefficients in filter */
                                        /* same as number of delay elements */
                                        /* (h[m], 0 <= m < M, an increasing */
                                        /*  circular buffer) */
                                        /* (d[m], 0 <= m < M, an increasing */
                                        /*  circular buffer) */

                                        /* returns dstSamps */
);


extern fractional* FIRDecimate (        /* Decimation by R:1 rate */
   int numSamps,                        /* number of _output_ samples (N) */
                                        /* N = R*p (p integer) */
   fractional* dstSamps,                /* ptr to output samples */
                                        /* (y[n], 0 <= n < N) */
   fractional* srcSamps,                /* ptr to input samples */
                                        /* (x[n], 0 <= n < N*R) */
   FIRStruct* filter,                   /* filter structure: */
                                        /* number of coefficients in filter */
                                        /* same as number of delay elements */
                                        /* M = R*q (q integer) */
                                        /* (h[m], 0 <= m < M) */
                                        /* (d[m], 0 <= m < M) */
   int rate                             /* rate of decimation R (to 1) */

                                        /* returns dstSamps */
);


extern fractional* FIRInterpolate (     /* Interpolation by 1:R rate */
   int numSamps,                        /* number of input samples (N) */
                                        /* N = R*p (p integer) */
   fractional* dstSamps,                /* ptr to output samples */
                                        /* (y[n], 0 <= n < N*R) */
   fractional* srcSamps,                /* ptr to input samples */
                                        /* (x[n], 0 <= n < N) */
   FIRStruct* filter,                   /* filter structure: */
                                        /* number of coefficients in filter */
                                        /* M = R*q (q integer) */
                                        /* (h[m], 0 <= m < M) */
                                        /* (d[m], 0 <= m < M/R) */
   int rate                             /* rate of interpolation (1 to) R */

                                        /* returns dstSamps */
);


extern fractional* FIRLattice (         /* FIR Lattice filtering */
   int numSamps,                        /* number of input samples (N) */
   fractional* dstSamps,                /* ptr to output samples */
                                        /* (y[n], 0 <= n < N) */
   fractional* srcSamps,                /* ptr to input samples */
                                        /* (x[n], 0 <= n < N) */
   FIRStruct* filter                    /* filter structure: */
                                        /* number of coefficients in filter */
                                        /* (also known as kappa values) */
                                        /* same as number of delay elements */
                                        /* same as filter order */
                                        /* (k[m], 0 <= m < M) */
                                        /* (d[m], 0 <= m < M) */

                                        /* returns dstSamps */
);

extern fractional* FIRLMS (             /* FIR Least Mean Square filtering */
                                        /* not normalized implementation */
                                        /* y[n] = sum_{m=0:M-1){h[n]*x[n-m]} */
                                        /* 0 <= n < N */
                                        /* h_m[n] = h_m[n-1] + mu*e[n]*x[n-m] */
                                        /* 0 <= n < N, 0 <= m < M */
                                        /* with e[n] = r[n] - y[n] */
                                        /* NOTE: to avoid saturation while */
                                        /* computing error, -1 <= e[n] < 1 */
                                        /* for 0 <= n < N */
   int numSamps,                        /* number of input samples (N) */
   fractional* dstSamps,                /* ptr to output samples */
                                        /* (y[n], 0 <= n < N) */
   fractional* srcSamps,                /* ptr to input samples */
                                        /* (x[n], 0 <= n < N) */
   FIRStruct* filter,                   /* filter structure: */
                                        /* number of coefficients in filter */
                                        /* same as number of delay elements */
                                        /* (h[m], 0 <= m < M, an increasing */
                                        /*  circular buffer) */
                                        /* (d[m], 0 <= m < M, an increasing */
                                        /*  circular buffer) */
   fractional* refSamps,                /* ptr to reference samples */
                                        /* (r[n], 0 <= n < N) */
   fractional muVal                     /* mu value for correction */

                                        /* returns dstSamps */
);

extern fractional* FIRLMSNorm (         /* FIR Least Mean Square filtering*/
                                        /* Normalized implementation */
                                        /* y[n] = sum_{m=0:M-1}(h[n]*x[n-m]) */
                                        /* 0 <= n < N */
                                        /* h_m[n] = h_m[n-1] + nu*e[n]*x[n-m] */
                                        /* 0 <= n < N, 0 <= m < M */
                                        /* with e[n] = r[n] - y[n], and */
                                        /* nu[n] = mu/(mu+E[n]) */
                                        /* E[n]=E[n-1]+(x[n])^2-(x[n-M+1])^2 */
                                        /* is an estimate of input energy */
                                        /* NOTE: to avoid saturation while */
                                        /* computing error, -1 <= e[n] < 1 */
                                        /* for 0 <= n < N */
                                        /* NOTE: to avoid saturation while */
                                        /* computing the estimate, the input */
                                        /* signal values should be bound */
                                        /* so that sum_{m=0:-M+2}(x[n+m]^2}<1 */
   int numSamps,                        /* number of input samples (N) */
   fractional* dstSamps,                /* ptr to output samples */
                                        /* (y[n], 0 <= n < N) */
   fractional* srcSamps,                /* ptr to input samples */
                                        /* (x[n], 0 <= n < N) */
   FIRStruct* filter,                   /* filter structure: */
                                        /* number of coefficients in filter */
                                        /* same as number of delay elements */
                                        /* (h[m], 0 <= m < M, an increasing */
                                        /*  circular buffer) */
                                        /* (d[m], 0 <= m < M, an increasing */
                                        /*  circular buffer) */
   fractional* refSamps,                /* ptr to reference samples */
                                        /* (r[n], 0 <= n < N) */
   fractional muVal,                    /* mu value for correction */
   fractional* energyEstimate           /* energy estimate for input samples */
                                        /* E[-1] = (x[-1])^2+...+(x[-M+1])^2 */
                                        /* on start up... (zero first time) */
                                        /* E[N-1]=(x[N-1])^2+...+(x[N-M+1])^2 */
                                        /* upon return */

                                        /* returns dstSamps */
);

/* ....................................................................... */

/****************************************************************************
*
* Interface to IIR filter operations.
*
* Filtering a data sequence x[n] with an IIR filter of impulse response
* {b[m] (0<= m < M), a[p] (0 <= p < P)} is equivalent to solving the
* difference equation:
*
*       sum_{p = 0:P-1}(a[p]*y[n-p]) = sum_{m = 0:M-1}(b[m]*x[n-m])
*
* In this operation it is important to know and manage the past history
* of the input and output data sequences (x[m], -M+1 <= m < 0, and y[p],
* -P+1 <= p < 0) which represent the initial conditions of the filtering
* operation. Also, when applying an IIR filter to contiguous sections of
* a data sequence it is necessary to remember the final state of the
* last filtering operation (x[m], N-M+1 <= m < N-1, and y[p], N-P+1 <= p < N-1),
* and take the state into consideration for the calculations of the next
* filtering stage. Accounting for the past history and current state is
* required to perform a correct (glitch-free) filtering operation.
*
* The management of the past history and current state of the filtering
* operation is commonly implemented via additional sequences, referred
* to as the delays. Prior to a filtering operation the delays describe the
* past history of the filter. After performing the IIR filtering operation
* the delays contain a set of the most recently filtered data samples, and
* of the most recent output samples. (For correct operation, it is advisable
* to initialize the delay values to zero by calling the corresponding init
* function.)
*
* Even though IIR filtering is a difference equation, several properties
* of IIR filters allow for computation of the operation in more effective
* ways than that of a straight difference equation. Consequently, a set
* of such implementations are hereby provided.
*
* Note that of the six main sequences involved in IIR filtering, input
* data, output data, filter coefficients (a,b) and delays, the last four
* are usually thought of as making up the filter structure. However, since
* different implementations allow for particular arrangaments of the filter
* structure for efficiency gain, the structure has not been standardized,
* but rather taylored to best fit the particular implementations.
*
* In the current design, the input data sequence is referred to as the
* sequence of source samples, while the resulting filtered sequence
* contains the destination samples. The filters are characterized by
* the number of coefficients or taps in the 'a' and 'b' sets, and the
* delay properties. All of these data sets are stored in memory as
* vectors with their elements representing Q.15 fractional quantities.
* Also, except for the 'IIRLattice' and associated 'IIRLatticeStruct'
* implementations, the filters are made up of cascaded second order
* sections. In all cases, the input, output and coefficient vectors
* are allocated in default RAM memory space (X-Data and Y-Data). The
* coefficients may be allocated in either X-Data or program memory,
* while the delays ought to be in Y-Data.
*
****************************************************************************/

/* IIR filter operation prototypes. */


typedef struct {
  int numSectionsLess1;                 /* 1 less than number of cascaded */
                                        /* second order sections */
  fractional* coeffsBase;               /* ptr to filter coefficients */
                                        /* either in X-Data or P-MEM */
                                        /* number of coefficients is */
                                        /* 5*number of second order sections */
                                        /* {a2,a1,b2,b1,b0} per section */
  int coeffsPage;                       /* page number of program memory if */
                                        /* coefficients are in program memory */
                                        /* COEFFS_IN_DATA if not */
  fractional* delayBase;                /* ptr to filter delay */
                                        /* two words for every section */
                                        /* only in Y-Data */
  fractional initialGain;               /* initial gain value */
  int finalShift;                       /* output scaling (shift left) */
                                        /* restores filter gain to 0 dB */
                                        /* shift count may be zero, if not */
                                        /* zero, it is the number of bits */
                                        /* to shift output: negative means */
                                        /* shift to the left, positive is */
                                        /* shift right */
} IIRCanonicStruct;     /* Direct Form II Canonic biquad filter structure */

extern fractional* IIRCanonic (         /* Direct Form II (Canonic) */
                                        /* biquad filtering */
   int numSamps,                        /* number of input samples (N) */
   fractional* dstSamps,                /* ptr to output samples */
                                        /* (y[n], 0 <= n < N) */
   fractional* srcSamps,                /* ptr to input samples */
                                        /* (x[n], 0 <= n < N) */
   IIRCanonicStruct* filter             /* filter structure */

                                        /* returns dstSamps */
);

extern void IIRCanonicInit (            /* Initialize filter structure */
   IIRCanonicStruct* filter             /* Canonic biquad filter structure */
);

/* ....................................................................... */

typedef struct {
  int numSectionsLess1;                 /* 1 less than number of cascaded */
                                        /* second order sections */
  fractional* coeffsBase;               /* ptr to filter coefficients */
                                        /* either in X-Data or P-MEM */
                                        /* number of coefficients is */
                                        /* 5*number of second order sections */
                                        /* {b0,b1,a1,b2,a2} per section */
  int coeffsPage;                       /* page number of program memory if */
                                        /* coefficients are in program memory */
                                        /* COEFFS_IN_DATA if not */
  fractional* delayBase1;               /* ptr to state variable (delay) 1 */
                                        /* one word for every section */
                                        /* only in Y-Data */
  fractional* delayBase2;               /* ptr to state variable (delay) 2 */
                                        /* one word for every section */
                                        /* only in Y-Data */
  int finalShift;                       /* output scaling (shift left) */
                                        /* restores filter gain to 0 dB */
                                        /* shift count may be zero, if not */
                                        /* zero, it is the number of bits */
                                        /* to shift the output to the left */
                                        /* negative value means shift right */
} IIRTransposedStruct;  /* Transposed Direct Form II biquad filter structure */

extern fractional* IIRTransposed (      /* Direct Form II (Transposed) */
                                        /* biquad filtering */
   int numSamps,                        /* number of input samples (N) */
   fractional* dstSamps,                /* ptr to output samples */
                                        /* (y[n], 0 <= n < N) */
   fractional* srcSamps,                /* ptr to input samples */
                                        /* (x[n], 0 <= n < N) */
   IIRTransposedStruct* filter          /* Transposed biquad filter structure */

                                        /* returns dstSamps */
);

extern void IIRTransposedInit (         /* Initialize filter structure */
   IIRTransposedStruct* filter          /* Transposed biquad filter structure */
);

/* ....................................................................... */

typedef struct {
   int order;                           /* filter order (M) */
                                        /* M <= N (see IIRLattice for N) */
   fractional* kappaVals;               /* ptr to lattice coefficients */
                                        /* (k[m], 0 <= m <= M) */
                                        /* either in X-Data or P-MEM */
   fractional* gammaVals;               /* ptr to ladder coeficients */
                                        /* (g[m], 0 <= m <= M) */
                                        /* either in X-Data or P-MEM */
                                        /* NULL for all pole implementation */
   int coeffsPage;                      /* page number of program memory if */
                                        /* coefficients are in program memory */
                                        /* COEFFS_IN_DATA if not */
   fractional* delay;                   /* ptr to delay */
                                        /* (d[m], 0 <= m <= M) */
                                        /* only in Y-Data */
} IIRLatticeStruct;                     /* IIR Lattice filter structure */

extern fractional* IIRLattice (         /* IIR Lattice filtering */
   int numSamps,                        /* number of input samples (N) */
   fractional* dstSamps,                /* ptr to output samples */
                                        /* (y[n], 0 <= n < N) */
   fractional* srcSamps,                /* ptr to input samples */
                                        /* (x[n], 0 <= n < N) */
   IIRLatticeStruct* filter             /* filter structure */

                                        /* returns dstSamps */
);

extern void IIRLatticeInit (            /* Zero out dealy in filter structure */
   IIRLatticeStruct* filter             /* Lattice filter structure */
);

/* ....................................................................... */

/****************************************************************************
*
* Interface to transform operations.
*
* A set of linear discrete signal transformations (and some of the inverse
* transforms) are prototyped below. The first set applies a Discrete Fourier
* transform (or its inverse) to a complex data set. The second set applies
* a Type II Discrete Cosine Transform (DCT) to a real valued sequence.
*
* A complex valued sequence is represented by a vector in which every pair
* of values corresponds with a sequence element. The first value in the pair
* is the real part of the element, and the second its imaginary part (see
* the declaration of the 'fractcomplex' structure at the beginning of this
* file for further details). Both, the real and imaginary parts, are stored
* in memory using one word (two bytes) each, and must be interpreted as Q.15
* fractionals.
*
* The following transforms have been designed to either operate out-of-place,
* or in-place. The former type populates an output sequence with the results
* of the transformation. In the latter, the input sequence is (physically)
* replaced by the transformed sequence. For out-of-place operations, the user
* must provide with enough memory to accept the results of the computation.
* The input and output sequences to the FFT family of transforms must be
* allocated in Y-Data memopry.
*
* The transforms here described make use of transform factors which must be
* supplied to the transforming function during its invokation. These factors,
* which are complex data sets, are computed in floating point arithmetic,
* and then transformed into fractionals for use by the operations. To avoid
* excessive overhead when applying a transformation, and since for a given
* transform size the values of the factors are fixed, a particular set of
* transform factors could be generated once and used many times during the
* execution of the program. Thus, it is advisable to store the factors
* returned by any of the initialization operations in a permanent (static)
* vector. The factors to a transform may be allocated either in X-Data or
* program memory.
*
* Additional remarks.
*
* A) Operations which return a destination vector can be nested, so that
*    for instance if:
*
*       a = Op1 (b, c), with b = Op2 (d), and c = Op3 (e, f), then
*
*       a = Op1 (Op2 (d), Op3 (e, f))
*
****************************************************************************/

/* Transform operation prototypes. */


extern fractcomplex* TwidFactorInit (   /* Initialize twiddle factors */
                                        /* WN(k) = exp(i*2*pi*k/N) */
                                        /* computed in floating point */
                                        /* converted to fractionals */
   int log2N,                           /* log2(N), N complex factors */
                                        /* (although only N/2 are computed */
                                        /* since only half of twiddle factors */
                                        /* are used for I/FFT computation) */
   fractcomplex* twidFactors,           /* ptr to twiddle factors */
   int conjFlag                         /* indicates whether to generate */
                                        /* complex conjugates of twiddles */
                                        /* 0 : no conjugates (default) */
                                        /* 1 : conjugates */

                                        /* twidfact returned */
                                        /* only the first half: */
                                        /* WN(0)...WN(N/2-1) */
                                        /* (or their conjugates) */
);

/*...........................................................................*/

extern fractcomplex* BitReverseComplex (        /* Bit Reverse Ordering */
                                        /* (complex) */
   int log2N,                           /* log2(N), N is vector length */
   fractcomplex* srcCV                  /* ptr to source complex vector */
                                        /* MUST be N modulo aligned */

                                        /* srcCV returned */
);

/*...........................................................................*/

/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern fractcomplex* FFTComplex (       /* Fast Fourier Transform */
                                        /* (complex, out-of-place) */
   int log2N,                           /* log2(N), N-point transform */
   fractcomplex* dstCV,                 /* ptr to destination complex vector */
                                        /* with time samples */
                                        /* in natural order */
                                        /* MUST be N modulo aligned */
   fractcomplex* srcCV,                 /* ptr to source complex vector */
                                        /* with time samples */
                                        /* in natural order */
   fractcomplex* twidFactors,           /* base address of twiddle factors */
                                        /* either in X data or program memory */
                                        /* if in X data memory, it points at */
                                        /* WN(0).real */
                                        /* if in program memory, base is the */
                                        /* offset from program page boundary */
                                        /* to address where factors located */
                                        /* (inline assembly psvoffset ()) */
   int factPage                         /* if in X data memory, set to */
                                        /* defined value COEFFS_IN_DATA */
                                        /* if in program memory, page number */
                                        /* where factors are located */
                                        /* (inline assembly psvpage ()) */

                                        /* dstCV returned */
                                        /* with frequency components */
                                        /* in natural order */
                                        /* and scaled by 1/(1<<log2N) */
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern fractcomplex* FFTComplexIP (     /* Fast Fourier Transform */
                                        /* (complex, in-place) */
   int log2N,                           /* log2(N), N-point transform */
   fractcomplex* srcCV,                 /* ptr to source complex vector */
                                        /* with time samples */
                                        /* in natural order */
   fractcomplex* twidFactors,           /* base address of twiddle factors */
                                        /* either in X data or program memory */
                                        /* if in X data memory, it points at */
                                        /* WN(0).real */
                                        /* if in program memory, base is the */
                                        /* offset from program page boundary */
                                        /* to address where factors located */
                                        /* (inline assembly psvoffset ()) */
   int factPage                         /* if in X data memory, set to */
                                        /* defined value COEFFS_IN_DATA */
                                        /* if in program memory, page number */
                                        /* where factors are located */
                                        /* (inline assembly psvpage ()) */

                                        /* srcCV returned */
                                        /* with frequency components */
                                        /* in bit reverse order */
                                        /* and scaled by 1/(1<<log2N) */
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern fractcomplex* IFFTComplex (      /* Inverse Fast Fourier Transform */
                                        /* (complex, out-of-place) */
   int log2N,                           /* log2(N), N-point transform */
   fractcomplex* dstCV,                 /* ptr to destination complex vector */
   fractcomplex* srcCV,                 /* ptr to source complex vector */
                                        /* with frequency components */
                                        /* in natural order */
                                        /* MUST be N modulo aligned */
   fractcomplex* twidFactors,           /* base address of twiddle factors */
                                        /* either in X data or program memory */
                                        /* if in X data memory, it points at */
                                        /* WN(0).real */
                                        /* if in program memory, base is the */
                                        /* offset from program page boundary */
                                        /* to address where factors located */
                                        /* (inline assembly psvoffset ()) */
   int factPage                         /* if in X data memory, set to */
                                        /* defined value COEFFS_IN_DATA */
                                        /* if in program memory, page number */
                                        /* where factors are located */
                                        /* (inline assembly psvpage ()) */

                                        /* dstCV returned */
                                        /* with time samples */
                                        /* in natural order */
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern fractcomplex* IFFTComplexIP (    /* Inverse Fast Fourier Transform */
                                        /* (complex, in-place) */
   int log2N,                           /* log2(N), N-point transform */
   fractcomplex* srcCV,                 /* ptr to source complex vector */
                                        /* with frequency components */
                                        /* in bit reverse order */
                                        /* MUST be N modulo aligned */
   fractcomplex* twidFactors,           /* base address of twiddle factors */
                                        /* either in X data or program memory */
                                        /* if in X data memory, it points at */
                                        /* WN(0).real */
                                        /* if in program memory, base is the */
                                        /* offset from program page boundary */
                                        /* to address where factors located */
                                        /* (inline assembly psvoffset ()) */
   int factPage                         /* if in X data memory, set to */
                                        /* defined value COEFFS_IN_DATA */
                                        /* if in program memory, page number */
                                        /* where factors are located */
                                        /* (inline assembly psvpage ()) */

                                        /* srcCV returned */
                                        /* with time samples */
                                        /* in natural order */
);

/*...........................................................................*/

extern long toneGen32b(
		int samp, 
		int tone, 
		int phase,
		long *tfBuff
);

/*...........................................................................*/

extern void BitReverseReal32bIP(
		int N, 
		long *ipBuff
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern long* FFTReal32bIP(
			int log2N, 
			int N,
			long *ipBuff, 
			long *tfBuf,
			int factPage
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern long* FFTReal32b(
			int log2N, 
			int N,
			long *opBuff, 
			long *ipBuff, 
			long *tfBuf,
			int factPage
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern void FFTComplex32bIP(
		int log2N, 
		long *ipBuff, 
		long *tfBuff,
		int factPage
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern void realFft32bIP(
		int log2N, 
		long *ipBuff, 
		long *tfBuff,
		int factPage
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern long* IFFTReal32bIP(
			int log2N, 
			int N,
			long *ipBuff, 
			long *tfBuf,
			int factPage
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern long* IFFTReal32b(
			int log2N, 
			int N,
			long *opBuff, 
			long *ipBuff, 
			long *tfBuf,
			int factPage
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern void IFFTComplex32bIP(
		int log2N, 
		long *ipBuff, 
		long *tfBuff,
		int factPage
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern void realiFft32bIP(
		int log2N, 
		long *ipBuff, 
		long *tfBuff,
		int factPage
);

/*...........................................................................*/

extern void MagnitudeCplx32bIP(
		int N, 
		long *ipBuff
);

/*...........................................................................*/

extern fractcomplex* CosFactorInit (    /* Initialize cosine factors */
                                        /* CN(k) = exp(i*k*pi/(2*N)) */
                                        /* computed in floating point */
                                        /* converted to fractionals */
   int log2N,                           /* log2(N), N complex factors */
                                        /* (although only N/2 are computed */
                                        /* since only half of cosine factors */
                                        /* are used for DCT computation) */
   fractcomplex* cosFactors             /* ptr to cosine factors */

                                        /* cosineFactors returned */
                                        /* only the first half: */
                                        /* CN(0)...CN(N/2-1) */
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern fractional* DCT (                /* Type II Discrete Cosine Transform */
                                        /* (out-of-place) */
   int log2N,                           /* log2(N), N is transform length */
   fractional* dstV,                    /* ptr to destination vector (2*N) */
                                        /* (transform in first N samples) */
                                        /* MUST be N modulo aligned */
   fractional* srcV,                    /* ptr to source vector (N) in Y-Data */
                                        /* MUST be zero padded to length 2*N */
   fractcomplex* cosFactors,            /* base address of cosine factors */
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
   fractcomplex* twidFactors,           /* base address of complex conjugate */
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
   int factPage                         /* if in X data memory, set to */
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
                                        /* DCT values scaled by 1/sqrt(2*N) */
);

/*...........................................................................*/
/* For dsPIC33E/dsPIC33C target with factPage pointing to psvpage, the coefficients
   may be copied from PSV to stack depending on stack space availability.
   Conditions for PSV to stack copy are: SP+TABLE_SIZE+STACK_GUARD < SPLIM
   and SP+TABLE_SIZE < __YDATA_BASE where
   a) SP: Stack Pointer
   b) TABLE_SIZE: Size of table of constants in PSV
   c) STACK_GUARD: Buffer space on the stack beyond the table of constants 
                   to be copied from PSV
   d) SPLIM: Stack Pointer Limit
   e) __YDATA_BASE: Base address of Y memory
   
   STACK_GUARD has a default value of 1024 words but can be modified with the 
   SetStackGuard function. Care must be taken when modifying the STACK_GUARD.
   a) Large values imply that more stack space is reserved for interrupts 
      etc … SPLIM will more likely be exceeded and code will run out of PSV. 
	  This will increase the cycle count. If this happens, decrease the
	  STACK_GUARD value.
   b) Smaller values imply that less stack space is reserved for interrupts 
      etc … SPLIM is less likely to be exceeded and code will run out of RAM. 
	  Stack overflows may occur since there is less buffer space. If this
	  happens, increase the STACK_GUARD value.  
*/

extern fractional* DCTIP (              /* Type II Discrete Cosine Transform */
                                        /* (in-place) */
   int log2N,                           /* log2(N), N is transform length */
   fractional* srcV,                    /* ptr to source vector in Y-Data */
                                        /* MUST be zero padded to length 2*N */
                                        /* MUST be N modulo aligned */
   fractcomplex* cosFactors,            /* base address of cosine factors */
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
   fractcomplex* twidFactors,           /* base address of complex conjugate */
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
   int factPage                         /* if in X data memory, set to */
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
                                        /* DCT values scaled by 1/sqrt(2*N) */
);

/*...........................................................................*/

extern fractional* SquareMagnitudeCplx ( /* Squared-Magnitude of complex  */
   int numelems,                        /* source vector, srcV, of length */
   fractcomplex* srcV,                  /* numelems, is stored in the     */
   fractional* dstV                     /* destination vector dstV        */
                                        /* dstV may be located in X or Y  */
                                        /* data space, while srcV is in Y */
                                        /* data space                     */
                                        /* Address of dstV is returned    */
                                        /* Function is typically, called  */
                                        /* after performing the FFTComplex*/
                                        /* operation                      */
);

/*...........................................................................*/

typedef struct {
        fractional* abcCoefficients;    /* Pointer to A, B & C coefficients located in X-space */
                                        /* These coefficients are derived from */
                                        /* the PID gain values - Kp, Ki and Kd */
        fractional* controlHistory;     /* Pointer to 3 delay-line samples located in Y-space */
                                        /* with the first sample being the most recent */
        fractional controlOutput;       /* PID Controller Output  */
        fractional measuredOutput;      /* Measured Output sample */
        fractional controlReference;    /* Reference Input sample */
} tPID;

/*...........................................................................*/

extern void PIDCoeffCalc(               /* Derive A, B and C coefficients using PID gain values-Kp, Ki & Kd*/
        fractional* kCoeffs,            /* pointer to array containing Kp, Ki & Kd in sequence */
        tPID* controller                /* pointer to PID data structure */
);

/*...........................................................................*/

extern void PIDInit (                   /* Clear the PID state variables and output sample*/
        tPID* controller                /* pointer to PID data structure */
);


/*...........................................................................*/

extern fractional* PID (                /* PID Controller Function */
        tPID* controller                /* Pointer to PID controller data structure */
);
/*...........................................................................*/

/****************************************************************************
*
* Interface to stack guard prototype
*
****************************************************************************/

/* Function prototype. */

extern void SetStackGuard   (
   unsigned int stackGuard              /* STACK_GUARD value */
);
/*...........................................................................*/

/***************************************************************************/

#endif  /* ] __DSP_LIB__ */

/***************************************************************************/
/* EOF */

