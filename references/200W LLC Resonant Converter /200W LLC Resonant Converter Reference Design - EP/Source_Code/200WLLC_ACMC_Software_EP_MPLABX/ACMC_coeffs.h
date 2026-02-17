////////////////////////////////////////////////////////////////////////////////
// © 2019 Microchip Technology Inc. and its subsidiaries.
//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: Subject to your compliance with 
// these terms, you may use Microchip software and any derivatives exclusively 
// with Microchip products. It is your responsibility to comply with third party
// license terms applicable to your use of third party software (including open 
// source software) that may accompany Microchip software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
// EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
// IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
// FOR A PARTICULAR PURPOSE.
//
// IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
// INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
// WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
// HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
// THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
// CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
// OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

//-----ACMC Inner Current Control Coefficient for 5kHz cross-over and 200kHz sampling rate
//              0.13037 (s+5e004) (s+2.805e004)
//Gci(s)   =    -------------------------------
//                      s (s+2.437e004)
//              0.2464 z^2 - 0.4616 z + 0.221
//Gci(z)        -----------------------------
//                 z^2 - 1.885 z + 0.8851
#define CON_2P2Z_COEFF_B0_I      0x10BB               //0.2464/1.885 in Q1.15
#define CON_2P2Z_COEFF_B1_I      0xE0A8               //-0.4616/1.885 in Q1.15
#define CON_2P2Z_COEFF_B2_I      0x0F01               //0.221/1.885 in Q1.15
#define CON_2P2Z_COEFF_A1_I      0x7FFF               //-1.885/1.885 convert to -ve sign in Q1.15
#define CON_2P2Z_COEFF_A2_I      0xC3E7               //0.8851/1.885 convert to -ve sign in Q1.15

#define CON_2P2Z_POSTSCALER_I    0x78A2               //1.885/2 in Q1.15 
#define CON_2P2Z_POSTSHIFT_I     0xFFFF               //Multiplied with 2 --> shift by 1 bits 
#define CON_2P2Z_MINOUTPUT_I     0xF800               //-1 in Q4.12
#define CON_2P2Z_MAXOUTPUT_I     0x07FF               //+1 in Q4.12

//-----ACMC Outer Voltage Control Coefficient for 1kHz cross-over and 50kHz sampling rate
//               4.9075 (s+2500)
//Gcv(s)   =    ----------------
//                       s  
//              (4.045 z - 3.847)       
//Gcv(z)   =    -----------------       
//                   (z - 1)            
#define CON_2P2Z_COEFF_B0_V      0x7FFF       //4.045/4.045 
#define CON_2P2Z_COEFF_B1_V      0x8645      //-3.847/4.045 
#define CON_2P2Z_COEFF_B2_V      0x0000       //0 in Q1.15
#define CON_2P2Z_COEFF_A1_V      0x1FA4       //-1/4.045 convert to -ve sign in Q1.15
#define CON_2P2Z_COEFF_A2_V      0x0000       //0 in Q1.15

#define CON_2P2Z_POSTSCALER_V    0x40B7       //4.045/8 in Q1.15 
#define CON_2P2Z_POSTSHIFT_V     0xFFFD       //Multiplied by 8 --> Right shift by 3 bits
#define CON_2P2Z_MINOUTPUT_V     0xF800       //-1 in Q4.12
#define CON_2P2Z_MAXOUTPUT_V     0x07FF       //+1 in Q4.12
