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

//-----VMC control parameters for 10khz cross-over and 200kHz sampling rate
//                    27.12 z^2 - 49.26 z + 22.53
//     Gcv(z)   =     ---------------------------
//                    z^2 - 1.338 z + 0.3377 
#define CON_2P2Z_COEFF_B0      0x4677       //27.12     --> scaled to 27.12/49.26 in Q1.15
#define CON_2P2Z_COEFF_B1      0x8000       //-49.26    --> scaled to -49.26/49.26 in Q1.15
#define CON_2P2Z_COEFF_B2      0x3A8A       //22.53     --> scaled to 22.53/49.26 in Q1.15
#define CON_2P2Z_COEFF_A1      0x037A       //-1.338    --> scaled to -1.338/49.26 and convert to -ve sign in Q1.15
#define CON_2P2Z_COEFF_A2      0xFF20       //0.3377    --> scaled to 0.3377/49.26 and convert to -ve sign in Q1.15

#define CON_2P2Z_POSTSCALER    0x6284       //49.26/64 = 0.7697 in Q1.15 
#define CON_2P2Z_POSTSHIFT     0xFFFA       //Multiplied with 64 --> Right shift by 6 bits 
#define CON_2P2Z_PRESHIFT      0x0000       // 
#define CON_2P2Z_MINOUTPUT     0xF800       //-1 in Q4.12
#define CON_2P2Z_MAXOUTPUT     0x07FF       //+1 in Q4.12
