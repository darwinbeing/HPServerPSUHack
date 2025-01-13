////////////////////////////////////////////////////////////////////////////////
// (C) 2015 Microchip Technology Inc.
//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
// derivatives created by any person or entity by or on your behalf, exclusively
// with Microchip's products.  Microchip and its licensors retain all ownership
// and intellectual property rights in the accompanying software and in all
// derivatives here to.
//
// This software and any accompanying information is for suggestion only.  It
// does not modify Microchip's standard warranty for its products.  You agree
// that you are solely responsible for testing the software and determining its
// suitability.  Microchip has no obligation to modify, test, certify, or
// support the software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
// EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
// PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
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
#ifndef __MAIN_H
#define __MAIN_H

#include <xc.h>
#include "Define.h"
#include <libpic30.h>
#include <stdint.h>
#include "init.h"
#include "lcd.h"
#include "load_config.h"
#include "buck_Defines.h"
#include "boost_Defines.h"

typedef union{

    struct{
        uint16_t inputVoltage   : 1;
        uint16_t buckSoftStart  : 1;
        uint16_t boostSoftStart : 1;
        uint16_t tempWarning    : 1;
        uint16_t overTemp       : 1;
        uint16_t                : 10;
       };

       uint16_t systemFaults;

} DPSK_FAULTS;


 typedef union{

    struct{
        uint16_t buckSSActive   : 1;
        uint16_t boostSSActive  : 1;
        uint16_t buckDynLoad    : 1;
        uint16_t boostDynLoad   : 1;
        uint16_t                : 1;
        uint16_t                :10;
    };

    uint16_t systemFlags;

} DPSK_FLAGS;


// Coefficients used to calculate LCD Measurements
// Converts ADC reading back to voltage and scaled by 100 for 3 digits for LCD (Q15)
#define VBUCK_LCD_FACTOR (int) ((100/(ADCRESOLUTION * VBUCKFBGAIN)) * 32767)

#define VBOOST_LCD_FACTOR (unsigned int) ((102/(ADCRESOLUTION * VBOOSTFBGAIN)) * 32767)

//Will need to post multiply by two as number exceeds uint range
#define VIN_FACTOR (unsigned int) ((100/(ADCRESOLUTION * VINFBGAIN)) * 32767)

#define TIMEFACTOR 29256        // 1/70M = 14.28n, since display is in ns, and to get Q15
                                // 14.286/16 = 0.892875 * 32767 = 29256

#define SW1 _RB8                // Push Button status


void ADCShortToString(int16_t Value, int16_t DotPos, char *Buffer);
void checkButton_SW1();
void LCD_Refresh();
void LCD_DisplayFault();


#endif // #ifndef __MAIN_H