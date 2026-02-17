G04*
G04  File:            05-07149_R0.2.TSP, Tue Feb 16 17:35:11 2010*
G04  Source:          P-CAD 2006 PCB, Version 19.02.958, (C:\cvs\HW\R_Boards\Board_Index\16-07149\PCB Files\Rev 0.2\PCB\05-07149_R0.2.pcb)*
G04  Format:          Gerber Format (RS-274-D), ASCII*
G04*
G04  Format Options:  Absolute Positioning*
G04                   Leading-Zero Suppression*
G04                   Scale Factor 1:1*
G04                   NO Circular Interpolation*
G04                   Inch Units*
G04                   Numeric Format: 4.4 (XXXX.XXXX)*
G04                   G54 NOT Used for Aperture Change*
G04                   Apertures Embedded*
G04*
G04  File Options:    Offset = (0.0mil,0.0mil)*
G04                   Drill Symbol Size = 80.0mil*
G04                   No Pad/Via Holes*
G04*
G04  File Contents:   Pads*
G04                   No Vias*
G04                   No Designators*
G04                   No Types*
G04                   No Values*
G04                   No Drill Symbols*
G04                   Top Paste*
G04                   Crop Marks*
G04*
%IN05-07149_R0.2.TSP*%
%ICAS*%
%MOIN*%
G04*
G04  Aperture MACROs for general use --- invoked via D-code assignment *
G04*
G04  General MACRO for flashed round with rotation and/or offset hole *
%AMROTOFFROUND*
1,1,$1,0.0000,0.0000*
1,0,$2,$3,$4*%
G04*
G04  General MACRO for flashed oval (obround) with rotation and/or offset hole *
%AMROTOFFOVAL*
21,1,$1,$2,0.0000,0.0000,$3*
1,1,$4,$5,$6*
1,1,$4,0-$5,0-$6*
1,0,$7,$8,$9*%
G04*
G04  General MACRO for flashed oval (obround) with rotation and no hole *
%AMROTOVALNOHOLE*
21,1,$1,$2,0.0000,0.0000,$3*
1,1,$4,$5,$6*
1,1,$4,0-$5,0-$6*%
G04*
G04  General MACRO for flashed rectangle with rotation and/or offset hole *
%AMROTOFFRECT*
21,1,$1,$2,0.0000,0.0000,$3*
1,0,$4,$5,$6*%
G04*
G04  General MACRO for flashed rectangle with rotation and no hole *
%AMROTRECTNOHOLE*
21,1,$1,$2,0.0000,0.0000,$3*%
G04*
G04  General MACRO for flashed rounded-rectangle *
%AMROUNDRECT*
21,1,$1,$2-$4,0.0000,0.0000,$3*
21,1,$1-$4,$2,0.0000,0.0000,$3*
1,1,$4,$5,$6*
1,1,$4,$7,$8*
1,1,$4,0-$5,0-$6*
1,1,$4,0-$7,0-$8*
1,0,$9,$10,$11*%
G04*
G04  General MACRO for flashed rounded-rectangle with rotation and no hole *
%AMROUNDRECTNOHOLE*
21,1,$1,$2-$4,0.0000,0.0000,$3*
21,1,$1-$4,$2,0.0000,0.0000,$3*
1,1,$4,$5,$6*
1,1,$4,$7,$8*
1,1,$4,0-$5,0-$6*
1,1,$4,0-$7,0-$8*%
G04*
G04  General MACRO for flashed regular polygon *
%AMREGPOLY*
5,1,$1,0.0000,0.0000,$2,$3+$4*
1,0,$5,$6,$7*%
G04*
G04  General MACRO for flashed regular polygon with no hole *
%AMREGPOLYNOHOLE*
5,1,$1,0.0000,0.0000,$2,$3+$4*%
G04*
G04  General MACRO for target *
%AMTARGET*
6,0,0,$1,$2,$3,4,$4,$5,$6*%
G04*
G04  General MACRO for mounting hole *
%AMMTHOLE*
1,1,$1,0,0*
1,0,$2,0,0*
$1=$1-$2*
$1=$1/2*
21,1,$2+$1,$3,0,0,$4*
21,1,$3,$2+$1,0,0,$4*%
G04*
G04*
G04  D10 : "Ellipse X5.0mil Y5.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0050*
%ADD10C, 0.0050*%
G04  D11 : "Ellipse X1.0mil Y1.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0010*
%ADD11C, 0.0010*%
G04  D12 : "Ellipse X10.0mil Y10.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0100*
%ADD12C, 0.0100*%
G04  D13 : "Ellipse X100.0mil Y100.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.1000*
%ADD13C, 0.1000*%
G04  D14 : "Ellipse X12.0mil Y12.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0120*
%ADD14C, 0.0120*%
G04  D15 : "Ellipse X15.0mil Y15.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0150*
%ADD15C, 0.0150*%
G04  D16 : "Ellipse X20.0mil Y20.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0200*
%ADD16C, 0.0200*%
G04  D17 : "Ellipse X25.0mil Y25.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0250*
%ADD17C, 0.0250*%
G04  D18 : "Ellipse X3.0mil Y3.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0030*
%ADD18C, 0.0030*%
G04  D19 : "Ellipse X30.0mil Y30.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0300*
%ADD19C, 0.0300*%
G04  D20 : "Ellipse X40.0mil Y40.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0400*
%ADD20C, 0.0400*%
G04  D21 : "Ellipse X50.0mil Y50.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0500*
%ADD21C, 0.0500*%
G04  D22 : "Ellipse X6.0mil Y6.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0060*
%ADD22C, 0.0060*%
G04  D23 : "Ellipse X6.5mil Y6.5mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0065*
%ADD23C, 0.0065*%
G04  D24 : "Ellipse X60.0mil Y60.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0600*
%ADD24C, 0.0600*%
G04  D25 : "Ellipse X7.0mil Y7.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0070*
%ADD25C, 0.0070*%
G04  D26 : "Ellipse X7.1mil Y7.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0071*
%ADD26C, 0.0071*%
G04  D27 : "Ellipse X8.0mil Y8.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0080*
%ADD27C, 0.0080*%
G04  D28 : "Ellipse X80.0mil Y80.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0800*
%ADD28C, 0.0800*%
G04  D29 : "Ellipse X9.0mil Y9.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Draw"*
G04  Disc: OuterDia=0.0090*
%ADD29C, 0.0090*%
G04  D30 : "Ellipse X100.0mil Y100.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1000*
%ADD30C, 0.1000*%
G04  D31 : "Ellipse X103.0mil Y103.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1030*
%ADD31C, 0.1030*%
G04  D32 : "Ellipse X105.0mil Y105.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1050*
%ADD32C, 0.1050*%
G04  D33 : "Ellipse X106.4mil Y106.4mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1064*
%ADD33C, 0.1064*%
G04  D34 : "Ellipse X108.0mil Y108.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1080*
%ADD34C, 0.1080*%
G04  D35 : "Ellipse X115.0mil Y115.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1150*
%ADD35C, 0.1150*%
G04  D36 : "Ellipse X118.7mil Y118.7mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1187*
%ADD36C, 0.1187*%
G04  D37 : "Ellipse X125.0mil Y125.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1250*
%ADD37C, 0.1250*%
G04  D38 : "Ellipse X133.0mil Y133.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1330*
%ADD38C, 0.1330*%
G04  D39 : "Ellipse X138.4mil Y138.4mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1384*
%ADD39C, 0.1384*%
G04  D40 : "Ellipse X150.0mil Y150.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1500*
%ADD40C, 0.1500*%
G04  D41 : "Ellipse X158.0mil Y158.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1580*
%ADD41C, 0.1580*%
G04  D42 : "Ellipse X165.0mil Y165.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1650*
%ADD42C, 0.1650*%
G04  D43 : "Ellipse X190.0mil Y190.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1900*
%ADD43C, 0.1900*%
G04  D44 : "Ellipse X196.9mil Y196.9mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1969*
%ADD44C, 0.1969*%
G04  D45 : "Ellipse X204.9mil Y204.9mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.2049*
%ADD45C, 0.2049*%
G04  D46 : "Ellipse X236.9mil Y236.9mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.2369*
%ADD46C, 0.2369*%
G04  D47 : "Ellipse X275.6mil Y275.6mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.2756*
%ADD47C, 0.2756*%
G04  D48 : "Ellipse X283.6mil Y283.6mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.2836*
%ADD48C, 0.2836*%
G04  D49 : "Ellipse X47.2mil Y47.2mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0472*
%ADD49C, 0.0472*%
G04  D50 : "Ellipse X55.2mil Y55.2mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0552*
%ADD50C, 0.0552*%
G04  D51 : "Ellipse X59.1mil Y59.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0591*
%ADD51C, 0.0591*%
G04  D52 : "Ellipse X60.0mil Y60.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0600*
%ADD52C, 0.0600*%
G04  D53 : "Ellipse X66.0mil Y66.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0660*
%ADD53C, 0.0660*%
G04  D54 : "Ellipse X67.1mil Y67.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0671*
%ADD54C, 0.0671*%
G04  D55 : "Ellipse X68.0mil Y68.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0680*
%ADD55C, 0.0680*%
G04  D56 : "Ellipse X74.0mil Y74.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0740*
%ADD56C, 0.0740*%
G04  D57 : "Ellipse X75.0mil Y75.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0750*
%ADD57C, 0.0750*%
G04  D58 : "Ellipse X78.0mil Y78.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0780*
%ADD58C, 0.0780*%
G04  D59 : "Ellipse X78.7mil Y78.7mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0787*
%ADD59C, 0.0787*%
G04  D60 : "Ellipse X79.4mil Y79.4mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0794*
%ADD60C, 0.0794*%
G04  D61 : "Ellipse X82.0mil Y82.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0820*
%ADD61C, 0.0820*%
G04  D62 : "Ellipse X86.6mil Y86.6mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0866*
%ADD62C, 0.0866*%
G04  D63 : "Ellipse X86.7mil Y86.7mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0867*
%ADD63C, 0.0867*%
G04  D64 : "Ellipse X87.2mil Y87.2mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0872*
%ADD64C, 0.0872*%
G04  D65 : "Ellipse X91.2mil Y91.2mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0912*
%ADD65C, 0.0912*%
G04  D66 : "Ellipse X94.6mil Y94.6mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0946*
%ADD66C, 0.0946*%
G04  D67 : "Ellipse X98.4mil Y98.4mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0984*
%ADD67C, 0.0984*%
G04  D68 : "Ellipse X99.1mil Y99.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0991*
%ADD68C, 0.0991*%
G04  D69 : "Mounting Hole X125.0mil Y125.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Mounting Hole: Diameter=0.1250, Rotation=0.0, LineWidth=0.0050 *
%ADD69MTHOLE, 0.1250 X0.1050 X0.0050 X0.0*%
G04  D70 : "Ellipse X118.1mil Y118.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1181*
%ADD70C, 0.1181*%
G04  D71 : "Ellipse X126.1mil Y126.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.1261*
%ADD71C, 0.1261*%
G04  D72 : "Oval X63.0mil Y12.5mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Obround: DimX=0.0630, DimY=0.0125, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD72O, 0.0630 X0.0125*%
G04  D73 : "Oval X12.5mil Y63.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Obround: DimX=0.0125, DimY=0.0630, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD73O, 0.0125 X0.0630*%
G04  D74 : "Oval X71.0mil Y20.5mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Obround: DimX=0.0710, DimY=0.0205, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD74O, 0.0710 X0.0205*%
G04  D75 : "Oval X20.5mil Y71.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Obround: DimX=0.0205, DimY=0.0710, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD75O, 0.0205 X0.0710*%
G04  D76 : "Rectangle X118.1mil Y100.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.1181, DimY=0.1000, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD76R, 0.1181 X0.1000*%
G04  D77 : "Rectangle X106.4mil Y106.4mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.1064, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD77R, 0.1064 X0.1064*%
G04  D78 : "Rectangle X106.4mil Y144.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.1064, DimY=0.1440, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD78R, 0.1064 X0.1440*%
G04  D79 : "Rectangle X126.1mil Y108.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.1261, DimY=0.1080, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD79R, 0.1261 X0.1080*%
G04  D80 : "Rectangle X118.1mil Y118.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.1181, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD80R, 0.1181 X0.1181*%
G04  D81 : "Rectangle X59.1mil Y118.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0591, DimY=0.1181, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD81R, 0.0591 X0.1181*%
G04  D82 : "Rectangle X125.0mil Y125.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.1250, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD82R, 0.1250 X0.1250*%
G04  D83 : "Rectangle X126.1mil Y126.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.1261, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD83R, 0.1261 X0.1261*%
G04  D84 : "Rectangle X67.1mil Y126.1mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0671, DimY=0.1261, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD84R, 0.0671 X0.1261*%
G04  D85 : "Rectangle X133.0mil Y133.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.1330, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD85R, 0.1330 X0.1330*%
G04  D86 : "Rectangle X86.0mil Y140.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0860, DimY=0.1400, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD86R, 0.0860 X0.1400*%
G04  D87 : "Rectangle X145.7mil Y145.7mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.1457, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD87R, 0.1457 X0.1457*%
G04  D88 : "Rectangle X94.0mil Y148.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0940, DimY=0.1480, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD88R, 0.0940 X0.1480*%
G04  D89 : "Rectangle X153.7mil Y153.7mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.1537, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD89R, 0.1537 X0.1537*%
G04  D90 : "Rectangle X118.1mil Y177.2mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.1181, DimY=0.1772, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD90R, 0.1181 X0.1772*%
G04  D91 : "Rectangle X126.1mil Y185.2mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.1261, DimY=0.1852, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD91R, 0.1261 X0.1852*%
G04  D92 : "Rectangle X32.0mil Y23.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0320, DimY=0.0230, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD92R, 0.0320 X0.0230*%
G04  D93 : "Rectangle X255.9mil Y232.3mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.2559, DimY=0.2323, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD93R, 0.2559 X0.2323*%
G04  D94 : "Rectangle X263.9mil Y240.3mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.2639, DimY=0.2403, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD94R, 0.2639 X0.2403*%
G04  D95 : "Rectangle X220.0mil Y250.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.2200, DimY=0.2500, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD95R, 0.2200 X0.2500*%
G04  D96 : "Rectangle X228.0mil Y258.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.2280, DimY=0.2580, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD96R, 0.2280 X0.2580*%
G04  D97 : "Rectangle X40.0mil Y31.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0400, DimY=0.0310, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD97R, 0.0400 X0.0310*%
G04  D98 : "Rectangle X32.0mil Y32.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0320, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD98R, 0.0320 X0.0320*%
G04  D99 : "Rectangle X32.0mil Y35.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0320, DimY=0.0350, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD99R, 0.0320 X0.0350*%
G04  D100 : "Rectangle X35.0mil Y32.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0350, DimY=0.0320, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD100R, 0.0350 X0.0320*%
G04  D101 : "Rectangle X36.0mil Y48.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0360, DimY=0.0480, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD101R, 0.0360 X0.0480*%
G04  D102 : "Rectangle X48.0mil Y36.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0480, DimY=0.0360, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD102R, 0.0480 X0.0360*%
G04  D103 : "Rectangle X40.0mil Y40.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0400, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD103R, 0.0400 X0.0400*%
G04  D104 : "Rectangle X40.0mil Y43.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0400, DimY=0.0430, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD104R, 0.0400 X0.0430*%
G04  D105 : "Rectangle X43.0mil Y40.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0430, DimY=0.0400, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD105R, 0.0430 X0.0400*%
G04  D106 : "Rectangle X44.0mil Y56.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0440, DimY=0.0560, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD106R, 0.0440 X0.0560*%
G04  D107 : "Rectangle X56.0mil Y44.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0560, DimY=0.0440, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD107R, 0.0560 X0.0440*%
G04  D108 : "Rectangle X45.0mil Y110.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0450, DimY=0.1100, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD108R, 0.0450 X0.1100*%
G04  D109 : "Rectangle X110.0mil Y45.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.1100, DimY=0.0450, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD109R, 0.1100 X0.0450*%
G04  D110 : "Rectangle X86.0mil Y47.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0860, DimY=0.0470, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD110R, 0.0860 X0.0470*%
G04  D111 : "Rectangle X90.6mil Y51.2mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0906, DimY=0.0512, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD111R, 0.0906 X0.0512*%
G04  D112 : "Rectangle X53.0mil Y118.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0530, DimY=0.1180, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD112R, 0.0530 X0.1180*%
G04  D113 : "Rectangle X118.0mil Y53.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.1180, DimY=0.0530, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD113R, 0.1180 X0.0530*%
G04  D114 : "Rectangle X94.0mil Y55.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0940, DimY=0.0550, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD114R, 0.0940 X0.0550*%
G04  D115 : "Rectangle X550.0mil Y420.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.5500, DimY=0.4200, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD115R, 0.5500 X0.4200*%
G04  D116 : "Rectangle X558.0mil Y428.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.5580, DimY=0.4280, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD116R, 0.5580 X0.4280*%
G04  D117 : "Rectangle X98.6mil Y59.2mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0986, DimY=0.0592, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD117R, 0.0986 X0.0592*%
G04  D118 : "Rectangle X60.0mil Y120.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0600, DimY=0.1200, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD118R, 0.0600 X0.1200*%
G04  D119 : "Rectangle X120.0mil Y60.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.1200, DimY=0.0600, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD119R, 0.1200 X0.0600*%
G04  D120 : "Rectangle X60.0mil Y13.5mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0600, DimY=0.0135, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD120R, 0.0600 X0.0135*%
G04  D121 : "Rectangle X60.0mil Y56.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0600, DimY=0.0560, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD121R, 0.0600 X0.0560*%
G04  D122 : "Rectangle X56.0mil Y60.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0560, DimY=0.0600, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD122R, 0.0560 X0.0600*%
G04  D123 : "Rectangle X60.0mil Y60.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0600, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD123R, 0.0600 X0.0600*%
G04  D124 : "Rectangle X64.0mil Y64.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0640, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD124R, 0.0640 X0.0640*%
G04  D125 : "Rectangle X66.0mil Y66.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0660, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD125R, 0.0660 X0.0660*%
G04  D126 : "Rectangle X68.0mil Y128.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0680, DimY=0.1280, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD126R, 0.0680 X0.1280*%
G04  D127 : "Rectangle X128.0mil Y68.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.1280, DimY=0.0680, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD127R, 0.1280 X0.0680*%
G04  D128 : "Rectangle X68.0mil Y21.5mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0680, DimY=0.0215, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD128R, 0.0680 X0.0215*%
G04  D129 : "Rectangle X68.0mil Y64.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0680, DimY=0.0640, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD129R, 0.0680 X0.0640*%
G04  D130 : "Rectangle X64.0mil Y68.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0640, DimY=0.0680, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD130R, 0.0640 X0.0680*%
G04  D131 : "Rectangle X68.0mil Y68.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0680, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD131R, 0.0680 X0.0680*%
G04  D132 : "Rectangle X70.0mil Y170.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0700, DimY=0.1700, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD132R, 0.0700 X0.1700*%
G04  D133 : "Rectangle X72.0mil Y72.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0720, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD133R, 0.0720 X0.0720*%
G04  D134 : "Rectangle X74.0mil Y74.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0740, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD134R, 0.0740 X0.0740*%
G04  D135 : "Rectangle X78.0mil Y178.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0780, DimY=0.1780, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD135R, 0.0780 X0.1780*%
G04  D136 : "Rectangle X78.7mil Y78.7mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0787, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD136R, 0.0787 X0.0787*%
G04  D137 : "Rectangle X80.0mil Y26.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0800, DimY=0.0260, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD137R, 0.0800 X0.0260*%
G04  D138 : "Rectangle X85.0mil Y70.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0850, DimY=0.0700, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD138R, 0.0850 X0.0700*%
G04  D139 : "Rectangle X86.7mil Y86.7mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0867, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD139R, 0.0867 X0.0867*%
G04  D140 : "Rectangle X87.0mil Y55.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0870, DimY=0.0550, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD140R, 0.0870 X0.0550*%
G04  D141 : "Rectangle X88.0mil Y34.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0880, DimY=0.0340, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD141R, 0.0880 X0.0340*%
G04  D142 : "Rectangle X93.0mil Y78.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0930, DimY=0.0780, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD142R, 0.0930 X0.0780*%
G04  D143 : "Rectangle X95.0mil Y63.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0950, DimY=0.0630, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD143R, 0.0950 X0.0630*%
G04  D144 : "Rectangle X98.4mil Y136.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Rectangular: DimX=0.0984, DimY=0.1360, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000 *
%ADD144R, 0.0984 X0.1360*%
G04  D145 : "Rectangle X98.4mil Y98.4mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Square: Side=0.0984, Rotation=0.0, OffsetX=0.0000, OffsetY=0.0000, HoleDia=0.0000*
%ADD145R, 0.0984 X0.0984*%
G04  D146 : "Thermal 4 Spoke/45 O130.0mil I97.5mil G21.7mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Thermal Relief: OuterDia=0.1300, InnerDia=0.0975, GapWidth=0.0217, Rotation=45.0000 *
%AMTHERMAL4D146*
1,1,0.1300,0,0*
1,0,0.0975,0,0*
21,0,0.1300,0.0217,0,0,45.0000*
21,0,0.0217,0.1300,0,0,45.0000*%
%ADD146THERMAL4D146*%
G04  D147 : "Thermal 4 Spoke/45 O76.0mil I57.0mil G12.7mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Thermal Relief: OuterDia=0.0760, InnerDia=0.0570, GapWidth=0.0127, Rotation=45.0000 *
%AMTHERMAL4D147*
1,1,0.0760,0,0*
1,0,0.0570,0,0*
21,0,0.0760,0.0127,0,0,45.0000*
21,0,0.0127,0.0760,0,0,45.0000*%
%ADD147THERMAL4D147*%
G04  D148 : "Thermal 4 Spoke/45 O78.7mil I59.1mil G13.1mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Thermal Relief: OuterDia=0.0787, InnerDia=0.0591, GapWidth=0.0131, Rotation=45.0000 *
%AMTHERMAL4D148*
1,1,0.0787,0,0*
1,0,0.0591,0,0*
21,0,0.0787,0.0131,0,0,45.0000*
21,0,0.0131,0.0787,0,0,45.0000*%
%ADD148THERMAL4D148*%
G04  D149 : "Thermal 4 Spoke/45 O84.0mil I60.0mil G15.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Thermal Relief: OuterDia=0.0840, InnerDia=0.0600, GapWidth=0.0150, Rotation=45.0000 *
%AMTHERMAL4D149*
1,1,0.0840,0,0*
1,0,0.0600,0,0*
21,0,0.0840,0.0150,0,0,45.0000*
21,0,0.0150,0.0840,0,0,45.0000*%
%ADD149THERMAL4D149*%
G04  D150 : "Thermal 4 Spoke/45 O84.0mil I63.0mil G14.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Thermal Relief: OuterDia=0.0840, InnerDia=0.0630, GapWidth=0.0140, Rotation=45.0000 *
%AMTHERMAL4D150*
1,1,0.0840,0,0*
1,0,0.0630,0,0*
21,0,0.0840,0.0140,0,0,45.0000*
21,0,0.0140,0.0840,0,0,45.0000*%
%ADD150THERMAL4D150*%
G04  D151 : "Ellipse X20.0mil Y20.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0200*
%ADD151C, 0.0200*%
G04  D152 : "Ellipse X28.0mil Y28.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0280*
%ADD152C, 0.0280*%
G04  D153 : "Ellipse X33.0mil Y33.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0330*
%ADD153C, 0.0330*%
G04  D154 : "Ellipse X40.0mil Y40.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0400*
%ADD154C, 0.0400*%
G04  D155 : "Ellipse X41.0mil Y41.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0410*
%ADD155C, 0.0410*%
G04  D156 : "Ellipse X48.0mil Y48.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0480*
%ADD156C, 0.0480*%
G04  D157 : "Ellipse X50.0mil Y50.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0500*
%ADD157C, 0.0500*%
G04  D158 : "Ellipse X58.0mil Y58.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0580*
%ADD158C, 0.0580*%
G04  D159 : "Ellipse X65.0mil Y65.0mil H0.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Disc: OuterDia=0.0650*
%ADD159C, 0.0650*%
G04  D160 : "Thermal 4 Spoke/45 O36.0mil I27.0mil G6.0mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Thermal Relief: OuterDia=0.0360, InnerDia=0.0270, GapWidth=0.0060, Rotation=45.0000 *
%AMTHERMAL4D160*
1,1,0.0360,0,0*
1,0,0.0270,0,0*
21,0,0.0360,0.0060,0,0,45.0000*
21,0,0.0060,0.0360,0,0,45.0000*%
%ADD160THERMAL4D160*%
G04  D161 : "Thermal 4 Spoke/45 O50.0mil I37.5mil G8.3mil 0.0deg (0.0mil,0.0mil) Flash"*
G04  Thermal Relief: OuterDia=0.0500, InnerDia=0.0375, GapWidth=0.0083, Rotation=45.0000 *
%AMTHERMAL4D161*
1,1,0.0500,0,0*
1,0,0.0375,0,0*
21,0,0.0500,0.0083,0,0,45.0000*
21,0,0.0083,0.0500,0,0,45.0000*%
%ADD161THERMAL4D161*%
G04*
%FSLAX44Y44*%
%SFA1B1*%
%OFA0.0000B0.0000*%
G04*
G70*
G90*
G01*
D2*
%LNTop Paste*%
D121*
X50500Y91850D3*
X49600D3*
D122*
X53360Y124730D3*
Y123830D3*
X52500Y124730D3*
Y123830D3*
D98*
X58200Y94470D3*
X57800Y93600D3*
X58600D3*
D100*
X61985Y92300D3*
X61315D3*
D99*
X63950Y93365D3*
Y94035D3*
X67950Y90765D3*
Y91435D3*
X73700Y87965D3*
Y88635D3*
D100*
X72415Y90600D3*
X73085D3*
D99*
X77400Y79230D3*
Y78560D3*
X77300Y93920D3*
Y94590D3*
D121*
X50500Y92750D3*
X49600D3*
D122*
X52300Y95750D3*
Y94850D3*
Y93950D3*
Y93050D3*
X53000Y86500D3*
Y87400D3*
X55900Y88200D3*
Y87300D3*
X54900D3*
Y88200D3*
D98*
X59050Y116620D3*
X58650Y115750D3*
X59450D3*
D92*
X61150Y93225D3*
Y92850D3*
X62190D3*
X61150Y93600D3*
X62190D3*
D98*
X65250Y95670D3*
X64850Y94800D3*
X65650D3*
X63950Y92870D3*
X63550Y92000D3*
X64350D3*
D102*
X64000Y116000D3*
Y117300D3*
X63950Y115200D3*
Y113900D3*
D99*
X67650Y95670D3*
Y96340D3*
X67150Y95665D3*
Y96335D3*
X66650Y96330D3*
Y95660D3*
D121*
X67400Y113800D3*
X66500D3*
D99*
X69700Y78280D3*
Y77610D3*
X70700Y78280D3*
Y77610D3*
D122*
X69700Y76350D3*
Y75450D3*
X70700Y76350D3*
Y75450D3*
D99*
X78600Y80370D3*
Y81040D3*
D122*
X49000Y96000D3*
Y96900D3*
D136*
X59000Y87200D3*
Y85600D3*
D122*
X58000Y85300D3*
Y86200D3*
D124*
X61900Y98990D3*
Y97650D3*
D122*
X64200Y86700D3*
Y85800D3*
D121*
X30700Y79500D3*
X29800D3*
X32650D3*
X31750D3*
D124*
X33660Y92400D3*
X35000D3*
D121*
X33800Y93500D3*
X34700D3*
D124*
X33860Y101200D3*
X35200D3*
D121*
X45150Y75800D3*
X44250D3*
X45000Y79350D3*
X44100D3*
D101*
X47750D3*
X46450D3*
D121*
X47950Y94150D3*
X47050D3*
D124*
X46540Y124200D3*
X45200D3*
X48940D3*
X47600D3*
D120*
X57000Y86200D3*
Y86460D3*
Y86720D3*
Y84900D3*
Y85160D3*
Y85420D3*
Y85680D3*
X54900Y86200D3*
Y86460D3*
Y86720D3*
Y85160D3*
Y85420D3*
Y84900D3*
Y85680D3*
X57000Y85940D3*
X54900D3*
D121*
X53900Y88200D3*
X53000D3*
D100*
X59785Y92300D3*
X59115D3*
D98*
X59450Y94470D3*
X59050Y93600D3*
X59850D3*
D99*
X58050Y115865D3*
Y116535D3*
D124*
X51340Y124200D3*
X50000D3*
D98*
X54750Y124700D3*
X54350Y123830D3*
X55150D3*
D122*
X71700Y76350D3*
Y75450D3*
D81*
X62972Y85719D3*
X62070D3*
D100*
X69665Y88500D3*
X70335D3*
X70635Y87900D3*
X69965D3*
X71365D3*
X72035D3*
X70870Y88500D3*
X71540D3*
D99*
X62750Y93370D3*
Y94040D3*
X63350Y94030D3*
Y93360D3*
D121*
X68100Y106600D3*
X69000D3*
D122*
X62100Y111400D3*
Y112300D3*
D121*
X69000Y115200D3*
X69900D3*
X69000Y116300D3*
X69900D3*
D108*
X29800Y78200D3*
X30890D3*
X32700D3*
X33790D3*
D102*
X35900Y92300D3*
Y93600D3*
D122*
X35700Y96300D3*
Y95400D3*
D111*
X34000Y94500D3*
Y96271D3*
D93*
X31440Y95385D3*
D122*
X35800Y105000D3*
Y104100D3*
D102*
X36000Y101100D3*
Y102400D3*
D121*
X33900Y102300D3*
X34800D3*
D111*
X34250Y103450D3*
Y105221D3*
D93*
X31690Y104335D3*
D121*
X44100Y78450D3*
X45000D3*
D138*
X47210Y89070D3*
Y87470D3*
D121*
X54900Y84200D3*
X55800D3*
D144*
X57250Y80000D3*
X54650D3*
D119*
X54750Y77050D3*
X57300D3*
D90*
X54950Y82550D3*
X57050D3*
D99*
X60450Y92170D3*
Y92840D3*
D122*
X53900Y87400D3*
Y86500D3*
X56900Y88200D3*
Y87300D3*
D100*
X59120Y92900D3*
X59790D3*
D76*
X57600Y112811D3*
Y111000D3*
D115*
X53700Y111900D3*
D99*
X71700Y78280D3*
Y77610D3*
D72*
X71427Y90974D3*
Y90206D3*
Y90462D3*
Y90718D3*
Y91229D3*
Y91741D3*
Y91485D3*
X68900Y90977D3*
Y90466D3*
Y90722D3*
Y91233D3*
Y90210D3*
Y91745D3*
Y91489D3*
D87*
X70167Y90977D3*
D73*
X70935Y89714D3*
X70679D3*
X70167D3*
X69400D3*
X69655D3*
X69911D3*
X70935Y92237D3*
X70679D3*
X70167D3*
X69655D3*
X69400D3*
X69911D3*
X70423Y89714D3*
Y92237D3*
D100*
X71985Y92800D3*
X71315D3*
Y93350D3*
X71985D3*
D99*
X62750Y92170D3*
Y92840D3*
D121*
X68100Y107400D3*
X69000D3*
X68100Y108200D3*
X69000D3*
D122*
X65700Y112900D3*
Y113800D3*
D137*
X68000Y116200D3*
Y115200D3*
Y114700D3*
Y115700D3*
X65800Y116200D3*
Y115200D3*
Y114700D3*
Y115700D3*
D121*
X61900Y117300D3*
X62800D3*
D122*
X62300Y119900D3*
Y119000D3*
D98*
X78500Y79270D3*
X78100Y78400D3*
X78900D3*
X77600Y80380D3*
X78000Y81250D3*
X77200D3*
D99*
X78050Y94580D3*
Y93910D3*
Y95115D3*
Y95785D3*
D132*
X47200Y85050D3*
Y81550D3*
D109*
X60615Y85784D3*
Y86874D3*
D132*
X51450Y85050D3*
Y81550D3*
D76*
X57600Y120411D3*
Y118600D3*
D115*
X53700Y119500D3*
D99*
X66050Y96330D3*
Y95660D3*
D118*
X66250Y84550D3*
Y87100D3*
D140*
X69850Y85800D3*
Y86800D3*
X72350D3*
Y85800D3*
D124*
X37440Y75800D3*
X36100D3*
X61760Y116300D3*
X63100D3*
X61760Y115100D3*
X63100D3*
D80*
X49150Y77200D3*
X53201D3*
D110*
X35450Y78450D3*
Y79350D3*
D86*
X37900Y79350D3*
D110*
X35450Y80250D3*
D137*
X51200Y95150D3*
Y94150D3*
Y93650D3*
Y94650D3*
X49000Y95150D3*
Y94150D3*
Y93650D3*
Y94650D3*
D80*
X59200Y82350D3*
X63251D3*
D124*
X63240Y87000D3*
X61900D3*
D121*
X62050Y114100D3*
X62950D3*
D144*
X72700Y96400D3*
X70100D3*
D95*
X70650Y105850D3*
Y101850D3*
X79650Y103850D3*
%LNCrop Marks*%
D02M02*
