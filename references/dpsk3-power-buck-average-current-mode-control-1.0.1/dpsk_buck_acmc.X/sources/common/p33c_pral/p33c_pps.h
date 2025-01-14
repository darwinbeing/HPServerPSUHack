/*!Software License Agreement
 * ************************************************************************************************
 *
 * Software License Agreement
 *
 * Copyright © 2018 Microchip Technology Inc.  All rights reserved. Microchip licenses to you the
 * right to use, modify, copy and distribute Software only when embedded on a Microchip 
 * microcontroller or digital signal controller, which is integrated into your product or third 
 * party product (pursuant to the sublicense terms in the accompanying license agreement).
 *
 * You should refer to the license agreement accompanying this Software for additional information 
 * regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR 
 * IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR 
 * OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT  
 * LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS  
 * OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY  
 * THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *
 * ***********************************************************************************************/

/*@@p33c_pps.h
 * ************************************************************************************************
 * Summary:
 * Generic Peripheral Pin Select (PPS) Driver Module (header file)
 *
 * Description:
 * This additional header file contains defines for all required bit-settings of all related 
 * special function registers of a peripheral module and/or instance. 
 * This file is an additional header file on top of the generic device header file.
 * 
 * See Also:
 *	p33c_pps.c
 * ***********************************************************************************************/

#ifndef MCAL_P33C_PPS_ABSTRACTION_DRIVER_H
#define MCAL_P33C_PPS_ABSTRACTION_DRIVER_H

// Device header file
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer types header file
#include <stdbool.h> // include standard boolean types header file

// Default definitions for all de4vices

#define PINMAP_LOCK			(0U)
#define PINMAP_UNLOCK		(1U)

// Generic declarations for pin numbers for all devices

#define PIN_RPI0    (0U)
#define PIN_RPI1    (1U)
#define PIN_RPI2    (2U)
#define PIN_RPI3    (3U)
#define PIN_RPI4    (4U)
#define PIN_RPI5    (5U)
#define PIN_RPI6    (6U)
#define PIN_RPI7    (7U)
#define PIN_RPI8    (8U)
#define PIN_RPI9    (9U)
#define PIN_RPI10   (10U)
#define PIN_RPI11   (11U)
#define PIN_RPI12   (12U)
#define PIN_RPI13   (13U)
#define PIN_RPI14   (14U)
#define PIN_RPI15   (15U)
#define PIN_RPI16   (16U)
#define PIN_RPI17   (17U)
#define PIN_RPI18   (18U)
#define PIN_RPI19   (19U)
#define PIN_RPI20   (20U)
#define PIN_RPI21   (21U)
#define PIN_RPI22   (22U)
#define PIN_RPI23   (23U)
#define PIN_RPI24   (24U)
#define PIN_RPI25   (25U)
#define PIN_RPI26   (26U)
#define PIN_RPI27   (27U)
#define PIN_RPI28   (28U)
#define PIN_RPI29   (29U)
#define PIN_RPI30   (30U)
#define PIN_RPI31   (31U)
#define PIN_RPI32   (32U)
#define PIN_RPI33   (33U)
#define PIN_RPI34   (34U)
#define PIN_RPI35   (35U)
#define PIN_RPI36   (36U)
#define PIN_RPI37   (37U)
#define PIN_RPI38   (38U)
#define PIN_RPI39   (39U)
#define PIN_RPI40   (40U)
#define PIN_RPI41   (41U)
#define PIN_RPI42   (42U)
#define PIN_RPI43   (43U)
#define PIN_RPI44   (44U)
#define PIN_RPI45   (45U)
#define PIN_RPI46   (46U)
#define PIN_RPI47   (47U)
#define PIN_RPI48   (48U)
#define PIN_RPI49   (49U)
#define PIN_RPI50   (50U)
#define PIN_RPI51   (51U)
#define PIN_RPI52   (52U)
#define PIN_RPI53   (53U)
#define PIN_RPI54   (54U)
#define PIN_RPI55   (55U)
#define PIN_RPI56   (56U)
#define PIN_RPI57   (57U)
#define PIN_RPI58   (58U)
#define PIN_RPI59   (59U)
#define PIN_RPI60   (60U)
#define PIN_RPI61   (61U)
#define PIN_RPI62   (62U)
#define PIN_RPI63   (63U)
#define PIN_RPI64   (64U)
#define PIN_RPI65   (65U)
#define PIN_RPI66   (66U)
#define PIN_RPI67   (67U)
#define PIN_RPI68   (68U)
#define PIN_RPI69   (69U)
#define PIN_RPI70   (70U)
#define PIN_RPI71   (71U)
#define PIN_RPI72   (72U)
#define PIN_RPI73   (73U)
#define PIN_RPI74   (74U)
#define PIN_RPI75   (75U)
#define PIN_RPI76   (76U)
#define PIN_RPI77   (77U)
#define PIN_RPI78   (78U)
#define PIN_RPI79   (79U)
#define PIN_RPI80   (80U)
#define PIN_RPI81   (81U)
#define PIN_RPI82   (82U)
#define PIN_RPI83   (83U)
#define PIN_RPI84   (84U)
#define PIN_RPI85   (85U)
#define PIN_RPI86   (86U)
#define PIN_RPI87   (87U)
#define PIN_RPI88   (88U)
#define PIN_RPI89   (89U)
#define PIN_RPI90   (90U)
#define PIN_RPI91   (91U)
#define PIN_RPI92   (92U)
#define PIN_RPI93   (93U)
#define PIN_RPI94   (94U)
#define PIN_RPI95   (95U)
#define PIN_RPI96   (96U)
#define PIN_RPI97   (97U)
#define PIN_RPI98   (98U)
#define PIN_RPI99   (99U)
#define PIN_RPI100  (100U)
#define PIN_RPI101  (101U)
#define PIN_RPI102  (102U)
#define PIN_RPI103  (103U)
#define PIN_RPI104  (104U)
#define PIN_RPI105  (105U)
#define PIN_RPI106  (106U)
#define PIN_RPI107  (107U)
#define PIN_RPI108  (108U)
#define PIN_RPI109  (109U)
#define PIN_RPI110  (110U)
#define PIN_RPI111  (111U)
#define PIN_RPI112  (112U)
#define PIN_RPI113  (113U)
#define PIN_RPI114  (114U)
#define PIN_RPI115  (115U)
#define PIN_RPI116  (116U)
#define PIN_RPI117  (117U)
#define PIN_RPI118  (118U)
#define PIN_RPI119  (119U)
#define PIN_RPI120  (120U)
#define PIN_RPI121  (121U)
#define PIN_RPI122  (122U)
#define PIN_RPI123  (123U)
#define PIN_RPI124  (124U)
#define PIN_RPI125  (125U)
#define PIN_RPI126  (126U)
#define PIN_RPI127  (127U)
#define PIN_RPI128  (128U)
#define PIN_RPI129  (129U)
#define	PIN_RPI130	(130U)
#define	PIN_RPI131	(131U)
#define	PIN_RPI132	(132U)
#define	PIN_RPI133	(133U)
#define	PIN_RPI134	(134U)
#define	PIN_RPI135	(135U)
#define	PIN_RPI136	(136U)
#define	PIN_RPI137	(137U)
#define	PIN_RPI138	(138U)
#define	PIN_RPI139	(139U)
#define	PIN_RPI140	(140U)
#define	PIN_RPI141	(141U)
#define	PIN_RPI142	(142U)
#define	PIN_RPI143	(143U)
#define	PIN_RPI144	(144U)
#define	PIN_RPI145	(145U)
#define	PIN_RPI146	(146U)
#define	PIN_RPI147	(147U)
#define	PIN_RPI148	(148U)
#define	PIN_RPI149	(149U)
#define	PIN_RPI150	(150U)
#define	PIN_RPI151	(151U)
#define	PIN_RPI152	(152U)
#define	PIN_RPI153	(153U)
#define	PIN_RPI154	(154U)
#define	PIN_RPI155	(155U)
#define	PIN_RPI156	(156U)
#define	PIN_RPI157	(157U)
#define	PIN_RPI158	(158U)
#define	PIN_RPI159	(159U)
#define	PIN_RPI160	(160U)
#define	PIN_RPI161	(161U)
#define	PIN_RPI162	(162U)
#define	PIN_RPI163	(163U)
#define	PIN_RPI164	(164U)
#define	PIN_RPI165	(165U)
#define	PIN_RPI166	(166U)
#define	PIN_RPI167	(167U)
#define	PIN_RPI168	(168U)
#define	PIN_RPI169	(169U)
#define	PIN_RPI170	(170U)
#define	PIN_RPI171	(171U)
#define	PIN_RPI172	(172U)
#define	PIN_RPI173	(173U)
#define	PIN_RPI174	(174U)
#define	PIN_RPI175	(175U)
#define	PIN_RPI176	(176U)
#define	PIN_RPI177	(177U)
#define	PIN_RPI178	(178U)
#define	PIN_RPI179	(179U)
#define	PIN_RPI180	(180U)
#define	PIN_RPI181	(181U)
#define	PIN_RPI182	(182U)
#define	PIN_RPI183	(183U)
#define	PIN_RPI184	(184U)
#define	PIN_RPI185	(185U)
#define	PIN_RPI186	(186U)
#define	PIN_RPI187	(187U)
#define	PIN_RPI188	(188U)
#define	PIN_RPI189	(189U)
#define	PIN_RPI190	(190U)
#define	PIN_RPI191	(191U)
#define	PIN_RPI192	(192U)
#define	PIN_RPI193	(193U)
#define	PIN_RPI194	(194U)
#define	PIN_RPI195	(195U)
#define	PIN_RPI196	(196U)
#define	PIN_RPI197	(197U)
#define	PIN_RPI198	(198U)
#define	PIN_RPI199	(199U)
#define	PIN_RPI200	(200U)
#define	PIN_RPI201	(201U)
#define	PIN_RPI202	(202U)
#define	PIN_RPI203	(203U)
#define	PIN_RPI204	(204U)
#define	PIN_RPI205	(205U)
#define	PIN_RPI206	(206U)
#define	PIN_RPI207	(207U)
#define	PIN_RPI208	(208U)
#define	PIN_RPI209	(209U)
#define	PIN_RPI210	(210U)
#define	PIN_RPI211	(211U)
#define	PIN_RPI212	(212U)
#define	PIN_RPI213	(213U)
#define	PIN_RPI214	(214U)
#define	PIN_RPI215	(215U)
#define	PIN_RPI216	(216U)
#define	PIN_RPI217	(217U)
#define	PIN_RPI218	(218U)
#define	PIN_RPI219	(219U)
#define	PIN_RPI220	(220U)
#define	PIN_RPI221	(221U)
#define	PIN_RPI222	(222U)
#define	PIN_RPI223	(223U)
#define	PIN_RPI224	(224U)
#define	PIN_RPI225	(225U)
#define	PIN_RPI226	(226U)
#define	PIN_RPI227	(227U)
#define	PIN_RPI228	(228U)
#define	PIN_RPI229	(229U)
#define	PIN_RPI230	(230U)
#define	PIN_RPI231	(231U)
#define	PIN_RPI232	(232U)
#define	PIN_RPI233	(233U)
#define	PIN_RPI234	(234U)
#define	PIN_RPI235	(235U)
#define	PIN_RPI236	(236U)
#define	PIN_RPI237	(237U)
#define	PIN_RPI238	(238U)
#define	PIN_RPI239	(239U)
#define	PIN_RPI240	(240U)
#define	PIN_RPI241	(241U)
#define	PIN_RPI242	(242U)
#define	PIN_RPI243	(243U)
#define	PIN_RPI244	(244U)
#define	PIN_RPI245	(245U)
#define	PIN_RPI246	(246U)
#define	PIN_RPI247	(247U)
#define	PIN_RPI248	(248U)
#define	PIN_RPI249	(249U)
#define	PIN_RPI250	(250U)
#define	PIN_RPI251	(251U)
#define	PIN_RPI252	(252U)
#define	PIN_RPI253	(253U)
#define	PIN_RPI254	(254U)
#define	PIN_RPI255	(255U)

// driver definitions for maximum pin number supported
#define RP_PINNO_MIN		32
#define RP_PINNO_MAX		181

#define PPSOUT_DEFAULT	0b0000000   // RPn tied to VSS
#define PPSOUT_NULL		0b0000000   // RPn tied to VSS
#define PPSOUT_NONE		0b0000000   // RPn tied to VSS
#define PPSPIN_NULL         (0U)

/* ---------------------------------------------------------------------------
 *  Enumeration for remappable input pins (values for register RPINRx)
 * ---------------------------------------------------------------------------*/

#define PPSI_RB0    (32U)
#define PPSI_RB1    (33U)
#define PPSI_RB2    (34U)
#define PPSI_RB3    (35U)
#define PPSI_RB4    (36U)
#define PPSI_RB5    (37U)
#define PPSI_RB6    (38U)
#define PPSI_RB7    (39U)
#define PPSI_RB8    (40U)
#define PPSI_RB9    (41U)
#define PPSI_RB10   (42U)
#define PPSI_RB11   (43U)
#define PPSI_RB12   (44U)
#define PPSI_RB13   (45U)
#define PPSI_RB14   (46U)
#define PPSI_RB15   (47U)

#define PPSI_RC0    (48U)
#define PPSI_RC1    (49U)
#define PPSI_RC2    (50U)
#define PPSI_RC3    (51U)
#define PPSI_RC4    (52U)
#define PPSI_RC5    (53U)
#define PPSI_RC6    (54U)
#define PPSI_RC7    (55U)
#define PPSI_RC8    (56U)
#define PPSI_RC9    (57U)
#define PPSI_RC10   (58U)
#define PPSI_RC11   (59U)
#define PPSI_RC12   (60U)
#define PPSI_RC13   (61U)
#define PPSI_RC14   (62U)
#define PPSI_RC15   (63U)

#define PPSI_RD0    (64U)
#define PPSI_RD1    (65U)
#define PPSI_RD2    (66U)
#define PPSI_RD3    (67U)
#define PPSI_RD4    (68U)
#define PPSI_RD5    (69U)
#define PPSI_RD6    (70U)
#define PPSI_RD7    (71U)
        
/* ---------------------------------------------------------------------------
 *  // Enumeration for remappable output pins (offset to register RPOR0)
 * ---------------------------------------------------------------------------*/

#define PPSO_RB0    (5U)
#define PPSO_RB1    (6U)
#define PPSO_RB2    (7U)
#define PPSO_RB3    (8U)
#define PPSO_RB4    (9U)
#define PPSO_RB5    (10U)
#define PPSO_RB6    (11U)
#define PPSO_RB7    (12U)
#define PPSO_RB8    (13U)
#define PPSO_RB9    (14U)
#define PPSO_RB10   (15U)
#define PPSO_RB11   (16U)
#define PPSO_RB12   (17U)
#define PPSO_RB13   (18U)
#define PPSO_RB14   (19U)
#define PPSO_RB15   (20U)

#define PPSO_RC0    (21U)
#define PPSO_RC1    (22U)
#define PPSO_RC2    (23U)
#define PPSO_RC3    (24U)
#define PPSO_RC4    (25U)
#define PPSO_RC5    (26U)
#define PPSO_RC6    (27U)
#define PPSO_RC7    (28U)
#define PPSO_RC8    (29U)
#define PPSO_RC9    (30U)
#define PPSO_RC10   (31U)
#define PPSO_RC11   (32U)
#define PPSO_RC12   (33U)
#define PPSO_RC13   (34U)
#define PPSO_RC14   (35U)
#define PPSO_RC15   (36U)

#define PPSO_RD0    (37U)
#define PPSO_RD1    (38U)
#define PPSO_RD2    (39U)
#define PPSO_RD3    (40U)
#define PPSO_RD4    (41U)
#define PPSO_RD5    (42U)
#define PPSO_RD6    (43U)
#define PPSO_RD7    (44U)
	
/* ---------------------------------------------------------------------------
 * 	Defines for register direct addressing of available remappable output pins
 * ---------------------------------------------------------------------------*/

#define PPS_RPO32    (uint8_t*)&RPOR0
#define PPS_RPO33    (uint8_t*)&RPOR0+1
#define PPS_RPO34    (uint8_t*)&RPOR1
#define PPS_RPO35    (uint8_t*)&RPOR1+1
#define PPS_RPO36    (uint8_t*)&RPOR2
#define PPS_RPO37    (uint8_t*)&RPOR2+1
#define PPS_RPO38    (uint8_t*)&RPOR3
#define PPS_RPO39    (uint8_t*)&RPOR3+1
#define PPS_RPO40    (uint8_t*)&RPOR4
#define PPS_RPO41    (uint8_t*)&RPOR4+1
#define PPS_RPO42    (uint8_t*)&RPOR5
#define PPS_RPO43    (uint8_t*)&RPOR5+1
#define PPS_RPO44    (uint8_t*)&RPOR6
#define PPS_RPO45    (uint8_t*)&RPOR6+1
#define PPS_RPO46    (uint8_t*)&RPOR7
#define PPS_RPO47    (uint8_t*)&RPOR7+1
#define PPS_RPO48    (uint8_t*)&RPOR8
#define PPS_RPO49    (uint8_t*)&RPOR8+1
#define PPS_RPO50    (uint8_t*)&RPOR9
#define PPS_RPO51    (uint8_t*)&RPOR9+1
#define PPS_RPO52    (uint8_t*)&RPOR10
#define PPS_RPO53    (uint8_t*)&RPOR10+1
#define PPS_RPO54    (uint8_t*)&RPOR11
#define PPS_RPO55    (uint8_t*)&RPOR11+1
#define PPS_RPO56    (uint8_t*)&RPOR12
#define PPS_RPO57    (uint8_t*)&RPOR12+1
#define PPS_RPO58    (uint8_t*)&RPOR13
#define PPS_RPO59    (uint8_t*)&RPOR13+1
#define PPS_RPO60    (uint8_t*)&RPOR14
#define PPS_RPO61    (uint8_t*)&RPOR14+1
#define PPS_RPO62    (uint8_t*)&RPOR15
#define PPS_RPO63    (uint8_t*)&RPOR15+1
#define PPS_RPO64    (uint8_t*)&RPOR16
#define PPS_RPO65    (uint8_t*)&RPOR16+1
#define PPS_RPO66    (uint8_t*)&RPOR17
#define PPS_RPO67    (uint8_t*)&RPOR17+1
#define PPS_RPO68    (uint8_t*)&RPOR18
#define PPS_RPO69    (uint8_t*)&RPOR18+1
#define PPS_RPO70    (uint8_t*)&RPOR19
#define PPS_RPO71    (uint8_t*)&RPOR19+1
#define PPS_RPO72    (uint8_t*)&RPOR20
#define PPS_RPO73    (uint8_t*)&RPOR20+1
#define PPS_RPO74    (uint8_t*)&RPOR21
#define PPS_RPO75    (uint8_t*)&RPOR21+1
#define PPS_RPO76    (uint8_t*)&RPOR22
#define PPS_RPO78    (uint8_t*)&RPOR22+1
#define PPS_RPO79    (uint8_t*)&RPOR23
#define PPS_RPO80    (uint8_t*)&RPOR23+1

// Virtual pins
#define PPS_VITUAL_RPO176   (uint8_t*)&RPOR24
#define PPS_VITUAL_RPO177   (uint8_t*)&RPOR24+1
#define PPS_VITUAL_RPO178   (uint8_t*)&RPOR25
#define PPS_VITUAL_RPO179   (uint8_t*)&RPOR25+1
#define PPS_VITUAL_RPO180   (uint8_t*)&RPOR26
#define PPS_VITUAL_RPO181   (uint8_t*)&RPOR26+1

/* ---------------------------------------------------------------------------
 * 	defines for register addresses for available output sources
 *  The pin number 0...181 will be the value written to the registers 
 *  declared here
 * 
 * TABLE: OUTPUT SELECTION FOR REMAPPABLE PINS (RPn)
 * ---------------------------------------------------------------------------*/
        
#define PPSOUT_VSS      (uint8_t)0b000000 // RPn tied to VSS

#define PPSOUT_U1TX     (uint8_t)0b000001 // RPn tied to UART1 Transmit
#define PPSOUT_U1RTS    (uint8_t)0b000010 // RPn tied to UART1 Request-to-Send
#define PPSOUT_U2TX     (uint8_t)0b000011 // RPn tied to UART2 Transmit
#define PPSOUT_U2RTS    (uint8_t)0b000100 // RPn tied to UART2 Request-to-Send
#define PPSOUT_U3TX     (uint8_t)0b011011 // RPn tied to UART3 Transmit
#define PPSOUT_U3RTS    (uint8_t)0b011100 // RPn tied to UART3 Request-to-Send

#define PPSOUT_SDO1     (uint8_t)0b000101 // RPn tied to SPI1 Data Output
#define PPSOUT_SCK1     (uint8_t)0b000110 // RPn tied to SPI1 Clock Output
#define PPSOUT_SS1      (uint8_t)0b000111 // RPn tied to SPI1 Slave Select
#define PPSOUT_SDO2     (uint8_t)0b001000 // RPn tied to SPI2 Data Output
#define PPSOUT_SCK2     (uint8_t)0b001001 // RPn tied to SPI2 Clock Output
#define PPSOUT_SS2      (uint8_t)0b001010 // RPn tied to SPI2 Slave Select
#define PPSOUT_SDO3     (uint8_t)0b001011 // RPn tied to SPI3 Data Output
#define PPSOUT_SCK3     (uint8_t)0b001100 // RPn tied to SPI3 Clock Output
#define PPSOUT_SS3      (uint8_t)0b001101 // RPn tied to SPI3 Slave Select            

#define PPSOUT_REFCLKO  (uint8_t)0b001110 // RPn tied to Reference Clock Output
#define PPSOUT_OCM1     (uint8_t)0b001111 // RPn tied to SCCP1 Output
#define PPSOUT_OCM2     (uint8_t)0b010000 // RPn tied to SCCP2 Output
#define PPSOUT_OCM3     (uint8_t)0b010001 // RPn tied to SCCP3 Output
#define PPSOUT_OCM4     (uint8_t)0b010010 // RPn tied to SCCP4 Output
#define PPSOUT_OCM5     (uint8_t)0b010011 // RPn tied to SCCP5 Output
#define PPSOUT_OCM6     (uint8_t)0b010100 // RPn tied to SCCP6 Output
#define PPSOUT_CAN1     (uint8_t)0b010101 // RPn tied to CAN1 Output
#define PPSOUT_CMP1     (uint8_t)0b010111 // RPn tied to Comparator 1 Output
#define PPSOUT_CMP2     (uint8_t)0b011000 // RPn tied to Comparator 2 Output
#define PPSOUT_CMP3     (uint8_t)0b011001 // RPn tied to Comparator 3 Output

#define PPSOUT_PWM4H    (uint8_t)0b100010 // RPn tied to PWM4H Output
#define PPSOUT_PWM4L    (uint8_t)0b100011 // RPn tied to PWM4L Output
#define PPSOUT_PWMEA    (uint8_t)0b100100 // RPn tied to PWM Event A Output
#define PPSOUT_PWMEB    (uint8_t)0b100101 // RPn tied to PWM Event B Output
#define PPSOUT_QEICMP1  (uint8_t)0b100110 // RPn tied to QEI1 Comparator Output
#define PPSOUT_QEICMP2  (uint8_t)0b100111 // RPn tied to QEI2 Comparator Output
#define PPSOUT_CLC1OUT  (uint8_t)0b101000 // RPn tied to CLC1 Output
#define PPSOUT_CLC2OUT  (uint8_t)0b101001 // RPn tied to CLC2 Output
#define PPSOUT_OCM7     (uint8_t)0b101010 // RPn tied to SCCP7 Output
#define PPSOUT_OCM8     (uint8_t)0b101011 // RPn tied to SCCP8 Output

#define PPSOUT_PWMEC    (uint8_t)0b101100 // RPn tied to PWM Event C Output
#define PPSOUT_PWMED    (uint8_t)0b101101 // RPn tied to PWM Event D Output
#define PPSOUT_PTGTRG24 (uint8_t)0b101110 // PTG Trigger Output 24
#define PPSOUT_PTGTRG25 (uint8_t)0b101111 // PTG Trigger Output 25
#define PPSOUT_SENT1OUT (uint8_t)0b110000 // RPn tied to SENT1 Output
#define PPSOUT_SENT2OUT (uint8_t)0b110001 // RPn tied to SENT2 Output
#define PPSOUT_MCCP9A   (uint8_t)0b110010 // RPn tied to MCCP9 Output A
#define PPSOUT_MCCP9B   (uint8_t)0b110011 // RPn tied to MCCP9 Output B
#define PPSOUT_MCCP9C   (uint8_t)0b110100 // RPn tied to MCCP9 Output C
#define PPSOUT_MCCP9D   (uint8_t)0b110101 // RPn tied to MCCP9 Output D
#define PPSOUT_MCCP9E   (uint8_t)0b110110 // RPn tied to MCCP9 Output E
#define PPSOUT_MCCP9F   (uint8_t)0b110111 // RPn tied to MCCP9 Output F        
#define PPSOUT_CLC3OUT  (uint8_t)0b110010 // RPn tied to CLC3 Output
#define PPSOUT_CLC4OUT  (uint8_t)0b110011 // RPn tied to CLC4 Output
  
#define PPSOUT_U1DTR    (uint8_t)0b110100 // Data Terminal Ready Output 1
#define PPSOUT_U2DTR    (uint8_t)0b110101 // Data Terminal Ready Output 2
#define PPSOUT_U3DTR    (uint8_t)0b111111 //RPn tied to UART3 DTR   
		
/* peripheral ID macros */
#if defined (PPSOUT_OCM9)
#define PPSOUT_OCM_INSTANCES (uint8_t[9]){ PPSOUT_OCM1, PPSOUT_OCM2, PPSOUT_OCM3, PPSOUT_OCM4, PPSOUT_OCM5, PPSOUT_OCM6, PPSOUT_OCM7, PPSOUT_OCM8, PPSOUT_OCM9 }
#elif defined (PPSOUT_OCM8)
#define PPSOUT_OCM_INSTANCES (uint8_t[8]){ PPSOUT_OCM1, PPSOUT_OCM2, PPSOUT_OCM3, PPSOUT_OCM4, PPSOUT_OCM5, PPSOUT_OCM6, PPSOUT_OCM7, PPSOUT_OCM8 }
#elif defined (PPSOUT_OCM7)
#define PPSOUT_OCM_INSTANCES (uint8_t[7]){ PPSOUT_OCM1, PPSOUT_OCM2, PPSOUT_OCM3, PPSOUT_OCM4, PPSOUT_OCM5, PPSOUT_OCM6, PPSOUT_OCM7 }
#elif defined (PPSOUT_OCM6)
#define PPSOUT_OCM_INSTANCES (uint8_t[6]){ PPSOUT_OCM1, PPSOUT_OCM2, PPSOUT_OCM3, PPSOUT_OCM4, PPSOUT_OCM5, PPSOUT_OCM6 }
#elif defined (PPSOUT_OCM5)
#define PPSOUT_OCM_INSTANCES (uint8_t[5]){ PPSOUT_OCM1, PPSOUT_OCM2, PPSOUT_OCM3, PPSOUT_OCM4, PPSOUT_OCM5 }
#elif defined (PPSOUT_OCM4)
#define PPSOUT_OCM_INSTANCES (uint8_t[4]){ PPSOUT_OCM1, PPSOUT_OCM2, PPSOUT_OCM3, PPSOUT_OCM4 }
#elif defined (PPSOUT_OCM3)
#define PPSOUT_OCM_INSTANCES (uint8_t[3]){ PPSOUT_OCM1, PPSOUT_OCM2, PPSOUT_OCM3 }
#elif defined (PPSOUT_OCM2)
#define PPSOUT_OCM_INSTANCES (uint8_t[2]){ PPSOUT_OCM1, PPSOUT_OCM2 }
#elif defined (PPSOUT_OCM1)
#define PPSOUT_OCM_INSTANCES (uint8_t[1]){ PPSOUT_OCM1 }
#endif

#if defined (PPSOUT_CMP6)
#define PPSOUT_CMP_INSTANCES (uint8_t[5]){ PPSOUT_CMP1, PPSOUT_CMP2, PPSOUT_CMP3, PPSOUT_CMP4, PPSOUT_CMP5, PPSOUT_CMP6 }
#elif defined (PPSOUT_CMP5)
#define PPSOUT_CMP_INSTANCES (uint8_t[5]){ PPSOUT_CMP1, PPSOUT_CMP2, PPSOUT_CMP3, PPSOUT_CMP4, PPSOUT_CMP5 }
#elif defined (PPSOUT_CMP4)
#define PPSOUT_CMP_INSTANCES (uint8_t[4]){ PPSOUT_CMP1, PPSOUT_CMP2, PPSOUT_CMP3, PPSOUT_CMP4 }
#elif defined (PPSOUT_CMP3)
#define PPSOUT_CMP_INSTANCES (uint8_t[3]){ PPSOUT_CMP1, PPSOUT_CMP2, PPSOUT_CMP3 }
#elif defined (PPSOUT_CMP2)
#define PPSOUT_CMP_INSTANCES (uint8_t[2]){ PPSOUT_CMP1, PPSOUT_CMP2 }
#elif defined (PPSOUT_CMP1)
#define PPSOUT_CMP_INSTANCES (uint8_t[1]){ PPSOUT_CMP1 }
#endif


// Macro to derive Peripehral Pin Select Output number of Output Compare peripheral instance
#define p33c_PPS_GetRPOR_OCM(x)   {(uint8_t)PPSOUT_OCM_INSTANCES[x-1]}
#define p33c_PPS_GetRPOR_CMP(x)   {(uint8_t)PPSOUT_CMP_INSTANCES[x-1]}


/* ---------------------------------------------------------------------------
 * 	defines for register addresses for available input sources
 *  The pin number 0...181 will be the value written to the registers 
 *  declared here
 * 
 * TABLE: SELECTABLE INPUT SOURCES (MAPS INPUT TO FUNCTION)
 * ---------------------------------------------------------------------------*/

#define PPSIN_INT1	   (uint8_t*)&RPINR0+1  // External Interrupt 1
#define PPSIN_INT2	   (uint8_t*)&RPINR1    // External Interrupt 2
#define PPSIN_INT3	   (uint8_t*)&RPINR1+1  // External Interrupt 3
#define PPSIN_T1CK	   (uint8_t*)&RPINR2+1  // Timer1 External Clock
#define PPSIN_TCKI1	   (uint8_t*)&RPINR3    // SCCP Timer1
#define PPSIN_ICM1	   (uint8_t*)&RPINR3+1  // SCCP Capture 1
#define PPSIN_TCKI2	   (uint8_t*)&RPINR4    // SCCP Timer2
#define PPSIN_ICM2	   (uint8_t*)&RPINR4+1  // SCCP Capture 2
#define PPSIN_TCKI3	   (uint8_t*)&RPINR5    // SCCP Timer3
#define PPSIN_ICM3	   (uint8_t*)&RPINR5+1  // SCCP Capture 3
#define PPSIN_TCKI4	   (uint8_t*)&RPINR6    // SCCP Timer4	
#define PPSIN_ICM4	   (uint8_t*)&RPINR6+1  // SCCP Capture 4
#define PPSIN_TCKI5	   (uint8_t*)&RPINR7    // SCCP Timer5	
#define PPSIN_ICM5	   (uint8_t*)&RPINR7+1  // SCCP Capture 5
#define PPSIN_TCKI6	   (uint8_t*)&RPINR8    // SCCP Timer6	
#define PPSIN_ICM6	   (uint8_t*)&RPINR8+1  // SCCP Capture 6
#define PPSIN_TCKI7	   (uint8_t*)&RPINR9    // SCCP Timer7	
#define PPSIN_ICM7	   (uint8_t*)&RPINR9+1  // SCCP Capture 7
#define PPSIN_TCKI8	   (uint8_t*)&RPINR10   // SCCP Timer8	
#define PPSIN_ICM8	   (uint8_t*)&RPINR10+1 // SCCP Capture 8
#define PPSIN_OCFA	   (uint8_t*)&RPINR11   // xCCP Fault A
#define PPSIN_OCFB	   (uint8_t*)&RPINR11+1 // xCCP Fault B
#define PPSIN_PCI8	   (uint8_t*)&RPINR12   // PWM PCI 8
#define PPSIN_PCI9	   (uint8_t*)&RPINR12+1 // PWM PCI 9
#define PPSIN_PCI10	   (uint8_t*)&RPINR13   // PWM PCI 10
#define PPSIN_PCI11	   (uint8_t*)&RPINR13+1 // PWM PCI 11
#define PPSIN_QEIA1	   (uint8_t*)&RPINR14   // QEI1 Input A
#define PPSIN_QEIB1	   (uint8_t*)&RPINR14+1 // QEI1 Input B
#define PPSIN_QEINDX1  (uint8_t*)&RPINR15   // QEI1 Index 1
#define PPSIN_QEIHOM1  (uint8_t*)&RPINR15+1 // QEI1 Home	1
#define PPSIN_QEIA2	   (uint8_t*)&RPINR16   // QEI2 Input A
#define PPSIN_QEIB2	   (uint8_t*)&RPINR16+1 // QEI2 Input B
#define PPSIN_QEINDX2  (uint8_t*)&RPINR17   // QEI2 Index 1
#define PPSIN_QEIHOM2  (uint8_t*)&RPINR17+1 // QEI2 Home 1
#define PPSIN_U1RX	   (uint8_t*)&RPINR18   // UART1 Receive	
#define PPSIN_U1DSR	   (uint8_t*)&RPINR18+1 // UART1 Data-Set Ready	
#define PPSIN_U2RX     (uint8_t*)&RPINR19   // UART2 Receive	
#define PPSIN_U2DSR    (uint8_t*)&RPINR19+1 // UART2 Data-Set-Ready	
#define PPSIN_SDI1	   (uint8_t*)&RPINR20   // SPI1 Data Input
#define PPSIN_SCK1IN   (uint8_t*)&RPINR20+1 // SPI1 Clock Input
#define PPSIN_SS1      (uint8_t*)&RPINR21   // SPI1 Slave Select 
#define PPSIN_REFOI	   (uint8_t*)&RPINR21+1 // Reference Clock Input
#define PPSIN_SDI2     (uint8_t*)&RPINR22   // SPI2 Data Input
#define PPSIN_SCK2IN   (uint8_t*)&RPINR22+1 // SPI2 Clock Input
#define PPSIN_SS2      (uint8_t*)&RPINR23   // SPI2 Slave Select 
#define PPSIN_CAN1RX   (uint8_t*)&RPINR26   // CAN1 Input 
#define PPSIN_U3RX	   (uint8_t*)&RPINR27   // UART3 Receive	
#define PPSIN_U3DSR    (uint8_t*)&RPINR27+1 // UART3 Data-Set-Ready	
#define PPSIN_SDI3     (uint8_t*)&RPINR29   // SPI3 Data Input
#define PPSIN_SCK3IN   (uint8_t*)&RPINR29+1 // SPI3 Clock Input
#define PPSIN_SS3      (uint8_t*)&RPINR30   // SPI3 Slave Select 
#define PPSIN_TCKI9    (uint8_t*)&RPINR32+1 // MCCP Timer9	
#define PPSIN_ICM9     (uint8_t*)&RPINR33   // MCCP Capture 9
#define PPSIN_OCFC     (uint8_t*)&RPINR37   // xCCP Fault C
#define PPSIN_PCI17    (uint8_t*)&RPINR37+1 // PWM Input 17
#define PPSIN_PCI18    (uint8_t*)&RPINR38   // PWM Input 18
#define PPSIN_PCI12    (uint8_t*)&RPINR42   // PWM Input 12
#define PPSIN_PCI13    (uint8_t*)&RPINR42+1 // PWM Input 13
#define PPSIN_PCI14    (uint8_t*)&RPINR43   // PWM Input 14
#define PPSIN_PCI15    (uint8_t*)&RPINR43+1 // PWM Input 15
#define PPSIN_PCI16    (uint8_t*)&RPINR44   // PWM Input 16
#define PPSIN_SENT1    (uint8_t*)&RPINR44+1 // SENT1 Input	
#define PPSIN_SENT2    (uint8_t*)&RPINR45   // SENT2 Input	
#define PPSIN_CLCINA   (uint8_t*)&RPINR45+1 // CLC Input A
#define PPSIN_CLCINB   (uint8_t*)&RPINR46   // CLC Input B
#define PPSIN_CLCINC   (uint8_t*)&RPINR46+1 // CLC Input C
#define PPSIN_CLCIND   (uint8_t*)&RPINR47   // CLC Input D
#define PPSIN_ADCTRG   (uint8_t*)&RPINR47+1 // ADC Trigger Input (ADTRIG31)
#define PPSIN_OCFD	   (uint8_t*)&RPINR48   // xCCP Fault D
#define PPSIN_U1CTS	   (uint8_t*)&RPINR48+1 // UART1 Clear-to-Send	
#define PPSIN_U2CTS	   (uint8_t*)&RPINR49   // UART2 Clear-to-Send	
#define PPSIN_U3CTS	   (uint8_t*)&RPINR49+1 // UART3 Clear-to-Send	

/* Specific declarations for virtual, remappable pins */
// ToDo: Add support of vitual pins, tailored to device families.
//       The code below requires different device family macros

//#if defined (__P33SMPS_CH_SLV__)
//
//	#define PPS_RPV0            (170U)
//	#define PPS_RPV1            (171U)
//	#define PPS_RPV2            (172U)
//	#define PPS_RPV3            (173U)
//	#define PPS_RPV4            (174U)
//	#define PPS_RPV5            (175U)
//
//#elif defined (__P33SMPS_CH_MSTR__)
//
//	#define PPS_RPV0            (176U)
//	#define PPS_RPV1            (177U)
//	#define PPS_RPV2            (178U)
//	#define PPS_RPV3            (179U)
//	#define PPS_RPV4            (180U)
//	#define PPS_RPV5            (181U)
//
//#elif defined (__P33SMPS_CK__)
//    
//
//#else
//
//    #pragma message "error: === p33c_pps: selected device type not supported ==="
//
//#endif

// PROTOTYPES FOR p33c_pps
    
extern volatile uint16_t PPS_RemapOutput(volatile uint8_t pinno, volatile uint8_t peripheral);
extern volatile uint16_t PPS_UnmapOutput(volatile uint8_t pinno);
extern volatile uint16_t PPS_RemapInput(volatile uint8_t pinno, volatile uint8_t *peripheral);
extern volatile uint16_t PPS_UnmapInput(volatile uint8_t *peripheral);
extern volatile uint16_t PPS_LockIO(void);
extern volatile uint16_t PPS_UnlockIO(void);

#endif // end of MCAL_P33C_PPS_ABSTRACTION_DRIVER_H
// End of File 
    