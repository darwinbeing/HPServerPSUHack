////////////////////////////////////////////////////////////////////////////////
// � 2015 Microchip Technology Inc.
//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
// derivatives created by any person or entity by or on your behalf, exclusively
// with Microchip?s products.  Microchip and its licensors retain all ownership
// and intellectual property rights in the accompanying software and in all
// derivatives here to.
//
// This software and any accompanying information is for suggestion only.  It
// does not modify Microchip?s standard warranty for its products.  You agree
// that you are solely responsible for testing the software and determining its
// suitability.  Microchip has no obligation to modify, test, certify, or
// support the software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
// EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
// PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP?S PRODUCTS,
// COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
//
// IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
// (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
// INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
// EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
// ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
// MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
// TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
// CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
// FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//
// MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
// TERMS.
////////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_DEFINES_H
#define	BOOST_DEFINES_H

#ifdef	__cplusplus
extern "C" {
#endif

//Boost Compensator
    
#define BOOST_COMP_3P3Z_COEFF_A1    2886  // Coefficient A1 will be multiplied with controller output u(n-1)
#define BOOST_COMP_3P3Z_COEFF_A2    -859  // Coefficient A2 will be multiplied with controller output u(n-2)
#define BOOST_COMP_3P3Z_COEFF_A3    -319  // Coefficient A3 will be multiplied with controller output u(n-3)

#define BOOST_COMP_3P3Z_COEFF_B0   32766  // Coefficient B0 will be multiplied with error e(n)
#define BOOST_COMP_3P3Z_COEFF_B1  -29256  // Coefficient B1 will be multiplied with error e(n-1)
#define BOOST_COMP_3P3Z_COEFF_B2  -32673  // Coefficient B2 will be multiplied with error e(n-2)
#define BOOST_COMP_3P3Z_COEFF_B3   29349  // Coefficient B3 will be multiplied with error e(n-3)

#define BOOST_COMP_3P3Z_POSTSCALER 19642  // Coefficient Normalization  constant
#define BOOST_COMP_3P3Z_POSTSHIFT     -5  // Normalization shift for control loop results to peripheral
#define BOOST_COMP_3P3Z_PRESHIFT       0  // Normalization shift for control loop results to peripheral

#define BOOST_COMP_3P3Z_MIN_CLAMP        0
#define BOOST_COMP_3P3Z_MAX_CLAMP     2425

#ifdef	__cplusplus
}
#endif

#endif	/* BOOST_DEFINES_H */

