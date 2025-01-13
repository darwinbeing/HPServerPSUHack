@echo off
set SAVE=
set SAVE=%XC16_OMF%
:askagain
echo.
echo Press C for COFF library
echo Press E for ELF library
echo Press Q to quit
echo.
set choice=
set /p choice=Which OMF do you use?
if '%choice%' == 'C' goto COFF
if '%choice%' == 'c' goto COFF
if '%choice%' == 'E' goto ELF
if '%choice%' == 'e' goto ELF
if '%choice%' == 'Q' goto EXIT
if '%choice%' == 'q' goto EXIT
echo.
goto askagain
:EXIT
  echo Not building a library.
  goto end
:COFF
  set XC16_OMF=coff
  goto start
:ELF
  set XC16_OMF=elf
:start
@echo on

xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/flt2frct.s ./src/flt2frct.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/flt2frct.list -o ./obj/flt2frct.deo ./obj/flt2frct.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/frct2flt.s ./src/frct2flt.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/frct2flt.list -o ./obj/frct2flt.deo ./obj/frct2flt.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initbart.s ./src/initbart.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/initbart.list -o ./obj/initbart.deo ./obj/initbart.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initblck.s ./src/initblck.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/initblck.list -o ./obj/initblck.deo ./obj/initblck.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/inithamm.s ./src/inithamm.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/inithamm.list -o ./obj/inithamm.deo ./obj/inithamm.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/inithann.s ./src/inithann.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/inithann.list -o ./obj/inithann.deo ./obj/inithann.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initkais.s ./src/initkais.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/initkais.list -o ./obj/initkais.deo ./obj/initkais.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/inittwid.s ./src/inittwid.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/inittwid.list -o ./obj/inittwid.deo ./obj/inittwid.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initcosf.s ./src/initcosf.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/initcosf.list -o ./obj/initcosf.deo ./obj/initcosf.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/minv.s ./src/minv.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/minv.list -o ./obj/minv.deo ./obj/minv.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/fft32.s ./src/fft32.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/fft32.list -o ./obj/fft32.deo ./obj/fft32.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/ifft32.s ./src/ifft32.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/ifft32.list -o ./obj/ifft32.deo ./obj/ifft32.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/fft32oop.s ./src/fft32oop.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/fft32oop.list -o ./obj/fft32oop.deo ./obj/fft32oop.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/ifft32oop.s ./src/ifft32oop.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/ifft32oop.list -o ./obj/ifft32oop.deo ./obj/ifft32oop.s

xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vmax.lst -o ./obj/vmax.deo ./asm/vmax.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vcopy.lst -o ./obj/vcopy.deo ./asm/vcopy.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vzpad.lst -o ./obj/vzpad.deo ./asm/vzpad.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vmin.lst -o ./obj/vmin.deo ./asm/vmin.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vneg.lst -o ./obj/vneg.deo ./asm/vneg.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vscl.lst -o ./obj/vscl.deo ./asm/vscl.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vadd.lst -o ./obj/vadd.deo ./asm/vadd.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vsub.lst -o ./obj/vsub.deo ./asm/vsub.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vmul.lst -o ./obj/vmul.deo ./asm/vmul.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vdot.lst -o ./obj/vdot.deo ./asm/vdot.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vpow.lst -o ./obj/vpow.deo ./asm/vpow.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vcon.lst -o ./obj/vcon.deo ./asm/vcon.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/vcor.lst -o ./obj/vcor.deo ./asm/vcor.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/dowindow.lst -o ./obj/dowindow.deo ./asm/dowindow.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/mscl.lst -o ./obj/mscl.deo ./asm/mscl.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/mtrp.lst -o ./obj/mtrp.deo ./asm/mtrp.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/madd.lst -o ./obj/madd.deo ./asm/madd.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/msub.lst -o ./obj/msub.deo ./asm/msub.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/mmul.lst -o ./obj/mmul.deo ./asm/mmul.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/dct.lst -o ./obj/dct.deo ./asm/dct.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/dctoop.lst -o ./obj/dctoop.deo ./asm/dctoop.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/fft.lst -o ./obj/fft.deo ./asm/fft.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/fftoop.lst -o ./obj/fftoop.deo ./asm/fftoop.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/ifft.lst -o ./obj/ifft.deo ./asm/ifft.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/ifftoop.lst -o ./obj/ifftoop.deo ./asm/ifftoop.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/bitrev.lst -o ./obj/bitrev.deo ./asm/bitrev.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/tonegen32b.lst -o ./obj/tonegen32b.deo ./asm/tonegen32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/rbrev32b.lst -o ./obj/rbrev32b.deo ./asm/rbrev32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/cplxFft32b.lst -o ./obj/cplxFft32b.deo ./asm/cplxFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/realFft32b.lst -o ./obj/realFft32b.deo ./asm/realFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/cplxiFft32b.lst -o ./obj/cplxiFft32b.deo ./asm/cplxiFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/realiFft32b.lst -o ./obj/realiFft32b.deo ./asm/realiFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/cplxmag32b.lst -o ./obj/cplxmag32b.deo ./asm/cplxmag32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/firinit.lst -o ./obj/firinit.deo ./asm/firinit.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/firdelay.lst -o ./obj/firdelay.deo ./asm/firdelay.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/firintdl.lst -o ./obj/firintdl.deo ./asm/firintdl.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/firdecim.lst -o ./obj/firdecim.deo ./asm/firdecim.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/firinter.lst -o ./obj/firinter.deo ./asm/firinter.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/firlms.lst -o ./obj/firlms.deo ./asm/firlms.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/firlmsn.lst -o ./obj/firlmsn.deo ./asm/firlmsn.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/firlatt.lst -o ./obj/firlatt.deo ./asm/firlatt.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/iirlatt.lst -o ./obj/iirlatt.deo ./asm/iirlatt.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/fir.lst -o ./obj/fir.deo ./asm/fir.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/iircan.lst -o ./obj/iircan.deo ./asm/iircan.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/iirtrans.lst -o ./obj/iirtrans.deo ./asm/iirtrans.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/cplxsqrmag.lst -o ./obj/cplxsqrmag.deo ./asm/cplxsqrmag.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/pid.lst -o ./obj/pid.deo ./asm/pid.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33F=1 -p generic-16dsp -I ./inc -a=./obj/stackguard.lst -o ./obj/stackguard.deo ./asm/stackguard.s

xc16-ar -q lib/libdsp-%XC16_OMF%.a ./obj/flt2frct.deo ./obj/frct2flt.deo ./obj/initbart.deo ./obj/initblck.deo ./obj/inithamm.deo ./obj/inithann.deo ./obj/initkais.deo ./obj/inittwid.deo ./obj/initcosf.deo ./obj/minv.deo ./obj/fft32.deo ./obj/ifft32.deo ./obj/fft32oop.deo ./obj/ifft32oop.deo ./obj/vmax.deo ./obj/vcopy.deo ./obj/vzpad.deo ./obj/vmin.deo ./obj/vneg.deo ./obj/vscl.deo ./obj/vadd.deo ./obj/vsub.deo ./obj/vmul.deo ./obj/vdot.deo ./obj/vpow.deo ./obj/vcon.deo ./obj/vcor.deo ./obj/dowindow.deo ./obj/mscl.deo ./obj/mtrp.deo ./obj/madd.deo ./obj/msub.deo ./obj/mmul.deo ./obj/dct.deo ./obj/dctoop.deo ./obj/fft.deo ./obj/fftoop.deo ./obj/ifft.deo ./obj/ifftoop.deo ./obj/bitrev.deo ./obj/firinit.deo ./obj/firdelay.deo ./obj/firintdl.deo ./obj/firdecim.deo ./obj/firinter.deo ./obj/firlms.deo ./obj/firlmsn.deo ./obj/firlatt.deo ./obj/iirlatt.deo ./obj/fir.deo ./obj/iircan.deo ./obj/iirtrans.deo ./obj/cplxsqrmag.deo ./obj/pid.deo ./obj/tonegen32b.deo ./obj/rbrev32b.deo ./obj/cplxFft32b.deo ./obj/realFft32b.deo ./obj/cplxiFft32b.deo ./obj/realiFft32b.deo ./obj/cplxmag32b.deo ./obj/stackguard.deo

xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/flt2frct.s ./src/flt2frct.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/flt2frct.list -o ./obj/flt2frct.depeo ./obj/flt2frct.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/frct2flt.s ./src/frct2flt.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/frct2flt.list -o ./obj/frct2flt.depeo ./obj/frct2flt.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initbart.s ./src/initbart.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/initbart.list -o ./obj/initbart.depeo ./obj/initbart.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initblck.s ./src/initblck.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/initblck.list -o ./obj/initblck.depeo ./obj/initblck.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/inithamm.s ./src/inithamm.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/inithamm.list -o ./obj/inithamm.depeo ./obj/inithamm.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/inithann.s ./src/inithann.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/inithann.list -o ./obj/inithann.depeo ./obj/inithann.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initkais.s ./src/initkais.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/initkais.list -o ./obj/initkais.depeo ./obj/initkais.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/inittwid.s ./src/inittwid.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/inittwid.list -o ./obj/inittwid.depeo ./obj/inittwid.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initcosf.s ./src/initcosf.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/initcosf.list -o ./obj/initcosf.depeo ./obj/initcosf.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/minv.s ./src/minv.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/minv.list -o ./obj/minv.depeo ./obj/minv.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/fft32.s ./src/fft32.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/fft32.list -o ./obj/fft32.depeo ./obj/fft32.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/ifft32.s ./src/ifft32.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/ifft32.list -o ./obj/ifft32.depeo ./obj/ifft32.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/fft32oop.s ./src/fft32oop.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/fft32oop.list -o ./obj/fft32oop.depeo ./obj/fft32oop.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/ifft32oop.s ./src/ifft32oop.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/ifft32oop.list -o ./obj/ifft32oop.depeo ./obj/ifft32oop.s

xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vmax.lst -o ./obj/vmax.depeo ./asm/vmax.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vcopy.lst -o ./obj/vcopy.depeo ./asm/vcopy.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vzpad.lst -o ./obj/vzpad.depeo ./asm/vzpad.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vmin.lst -o ./obj/vmin.depeo ./asm/vmin.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vneg.lst -o ./obj/vneg.depeo ./asm/vneg.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vscl.lst -o ./obj/vscl.depeo ./asm/vscl.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vadd.lst -o ./obj/vadd.depeo ./asm/vadd.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vsub.lst -o ./obj/vsub.depeo ./asm/vsub.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vmul.lst -o ./obj/vmul.depeo ./asm/vmul.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vdot.lst -o ./obj/vdot.depeo ./asm/vdot.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vpow.lst -o ./obj/vpow.depeo ./asm/vpow.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vcon.lst -o ./obj/vcon.depeo ./asm/vcon.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/vcor.lst -o ./obj/vcor.depeo ./asm/vcor.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/dowindow.lst -o ./obj/dowindow.depeo ./asm/dowindow.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/mscl.lst -o ./obj/mscl.depeo ./asm/mscl.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/mtrp.lst -o ./obj/mtrp.depeo ./asm/mtrp.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/madd.lst -o ./obj/madd.depeo ./asm/madd.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/msub.lst -o ./obj/msub.depeo ./asm/msub.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/mmul.lst -o ./obj/mmul.depeo ./asm/mmul.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/dct.lst -o ./obj/dct.depeo ./asm/dct.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/dctoop.lst -o ./obj/dctoop.depeo ./asm/dctoop.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/fft.lst -o ./obj/fft.depeo ./asm/fft.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/fftoop.lst -o ./obj/fftoop.depeo ./asm/fftoop.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/ifft.lst -o ./obj/ifft.depeo ./asm/ifft.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/ifftoop.lst -o ./obj/ifftoop.depeo ./asm/ifftoop.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/bitrev.lst -o ./obj/bitrev.depeo ./asm/bitrev.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/tonegen32b.lst -o ./obj/tonegen32b.depeo ./asm/tonegen32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/rbrev32b.lst -o ./obj/rbrev32b.depeo ./asm/rbrev32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/cplxFft32b.lst -o ./obj/cplxFft32b.depeo ./asm/cplxFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/realFft32b.lst -o ./obj/realFft32b.depeo ./asm/realFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/cplxiFft32b.lst -o ./obj/cplxiFft32b.depeo ./asm/cplxiFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/realiFft32b.lst -o ./obj/realiFft32b.depeo ./asm/realiFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/cplxmag32b.lst -o ./obj/cplxmag32b.depeo ./asm/cplxmag32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/firinit.lst -o ./obj/firinit.depeo ./asm/firinit.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/firdelay.lst -o ./obj/firdelay.depeo ./asm/firdelay.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/firintdl.lst -o ./obj/firintdl.depeo ./asm/firintdl.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/firdecim.lst -o ./obj/firdecim.depeo ./asm/firdecim.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/firinter.lst -o ./obj/firinter.depeo ./asm/firinter.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/firlms.lst -o ./obj/firlms.depeo ./asm/firlms.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/firlmsn.lst -o ./obj/firlmsn.depeo ./asm/firlmsn.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/firlatt.lst -o ./obj/firlatt.depeo ./asm/firlatt.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/iirlatt.lst -o ./obj/iirlatt.depeo ./asm/iirlatt.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/fir.lst -o ./obj/fir.depeo ./asm/fir.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/iircan.lst -o ./obj/iircan.depeo ./asm/iircan.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/iirtrans.lst -o ./obj/iirtrans.depeo ./asm/iirtrans.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/cplxsqrmag.lst -o ./obj/cplxsqrmag.depeo ./asm/cplxsqrmag.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/pid.lst -o ./obj/pid.depeo ./asm/pid.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33E=1 -p generic-16dsp-ep -I ./inc -a=./obj/stackguard.lst -o ./obj/stackguard.depeo ./asm/stackguard.s

xc16-ar -q lib/libdsp-%XC16_OMF%.a ./obj/flt2frct.depeo ./obj/frct2flt.depeo ./obj/initbart.depeo ./obj/initblck.depeo ./obj/inithamm.depeo ./obj/inithann.depeo ./obj/initkais.depeo ./obj/inittwid.depeo ./obj/initcosf.depeo ./obj/minv.depeo ./obj/fft32.depeo ./obj/ifft32.depeo ./obj/fft32oop.depeo ./obj/ifft32oop.depeo ./obj/vmax.depeo ./obj/vcopy.depeo ./obj/vzpad.depeo ./obj/vmin.depeo ./obj/vneg.depeo ./obj/vscl.depeo ./obj/vadd.depeo ./obj/vsub.depeo ./obj/vmul.depeo ./obj/vdot.depeo ./obj/vpow.depeo ./obj/vcon.depeo ./obj/vcor.depeo ./obj/dowindow.depeo ./obj/mscl.depeo ./obj/mtrp.depeo ./obj/madd.depeo ./obj/msub.depeo ./obj/mmul.depeo ./obj/dct.depeo ./obj/dctoop.depeo ./obj/fft.depeo ./obj/fftoop.depeo ./obj/ifft.depeo ./obj/ifftoop.depeo ./obj/bitrev.depeo ./obj/firinit.depeo ./obj/firdelay.depeo ./obj/firintdl.depeo ./obj/firdecim.depeo ./obj/firinter.depeo ./obj/firlms.depeo ./obj/firlmsn.depeo ./obj/firlatt.depeo ./obj/iirlatt.depeo ./obj/fir.depeo ./obj/iircan.depeo ./obj/iirtrans.depeo ./obj/cplxsqrmag.depeo ./obj/pid.depeo ./obj/tonegen32b.depeo ./obj/rbrev32b.depeo ./obj/cplxFft32b.depeo ./obj/realFft32b.depeo ./obj/cplxiFft32b.depeo ./obj/realiFft32b.depeo ./obj/cplxmag32b.depeo ./obj/stackguard.depeo


xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/flt2frct.s ./src/flt2frct.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/flt2frct.list -o ./obj/flt2frct.dcheo ./obj/flt2frct.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/frct2flt.s ./src/frct2flt.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/frct2flt.list -o ./obj/frct2flt.dcheo ./obj/frct2flt.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initbart.s ./src/initbart.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/initbart.list -o ./obj/initbart.dcheo ./obj/initbart.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initblck.s ./src/initblck.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/initblck.list -o ./obj/initblck.dcheo ./obj/initblck.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/inithamm.s ./src/inithamm.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/inithamm.list -o ./obj/inithamm.dcheo ./obj/inithamm.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/inithann.s ./src/inithann.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/inithann.list -o ./obj/inithann.dcheo ./obj/inithann.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initkais.s ./src/initkais.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/initkais.list -o ./obj/initkais.dcheo ./obj/initkais.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/inittwid.s ./src/inittwid.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/inittwid.list -o ./obj/inittwid.dcheo ./obj/inittwid.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/initcosf.s ./src/initcosf.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/initcosf.list -o ./obj/initcosf.dcheo ./obj/initcosf.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/minv.s ./src/minv.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/minv.list -o ./obj/minv.dcheo ./obj/minv.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/fft32.s ./src/fft32.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/fft32.list -o ./obj/fft32.dcheo ./obj/fft32.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/ifft32.s ./src/ifft32.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/ifft32.list -o ./obj/ifft32.dcheo ./obj/ifft32.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/fft32oop.s ./src/fft32oop.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/fft32oop.list -o ./obj/fft32oop.dcheo ./obj/fft32oop.s
xc16-gcc -S  -I./include -I./data  -Wall -O3 -g -mconst-in-code -mtext=.libdsp -DDATA_TYPE=1 -DIN_SPACE=0 -DIN_X_SPACE=0 -DIN_P_SPACE=1 -DSIM_VAL=0 -DMPLAB_VAL=1 -DVALIDATION=0 -o ./obj/ifft32oop.s ./src/ifft32oop.c
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/ifft32oop.list -o ./obj/ifft32oop.dcheo ./obj/ifft32oop.s

xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vmax.lst -o ./obj/vmax.dcheo ./asm/vmax.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vcopy.lst -o ./obj/vcopy.dcheo ./asm/vcopy.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vzpad.lst -o ./obj/vzpad.dcheo ./asm/vzpad.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vmin.lst -o ./obj/vmin.dcheo ./asm/vmin.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vneg.lst -o ./obj/vneg.dcheo ./asm/vneg.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vscl.lst -o ./obj/vscl.dcheo ./asm/vscl.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vadd.lst -o ./obj/vadd.dcheo ./asm/vadd.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vsub.lst -o ./obj/vsub.dcheo ./asm/vsub.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vmul.lst -o ./obj/vmul.dcheo ./asm/vmul.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vdot.lst -o ./obj/vdot.dcheo ./asm/vdot.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vpow.lst -o ./obj/vpow.dcheo ./asm/vpow.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vcon.lst -o ./obj/vcon.dcheo ./asm/vcon.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/vcor.lst -o ./obj/vcor.dcheo ./asm/vcor.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/dowindow.lst -o ./obj/dowindow.dcheo ./asm/dowindow.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/mscl.lst -o ./obj/mscl.dcheo ./asm/mscl.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/mtrp.lst -o ./obj/mtrp.dcheo ./asm/mtrp.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/madd.lst -o ./obj/madd.dcheo ./asm/madd.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/msub.lst -o ./obj/msub.dcheo ./asm/msub.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/mmul.lst -o ./obj/mmul.dcheo ./asm/mmul.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/dct.lst -o ./obj/dct.dcheo ./asm/dct.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/dctoop.lst -o ./obj/dctoop.dcheo ./asm/dctoop.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/fft.lst -o ./obj/fft.dcheo ./asm/fft.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/fftoop.lst -o ./obj/fftoop.dcheo ./asm/fftoop.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/ifft.lst -o ./obj/ifft.dcheo ./asm/ifft.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/ifftoop.lst -o ./obj/ifftoop.dcheo ./asm/ifftoop.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/bitrev.lst -o ./obj/bitrev.dcheo ./asm/bitrev.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/tonegen32b.lst -o ./obj/tonegen32b.dcheo ./asm/tonegen32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/rbrev32b.lst -o ./obj/rbrev32b.dcheo ./asm/rbrev32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/cplxFft32b.lst -o ./obj/cplxFft32b.dcheo ./asm/cplxFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/realFft32b.lst -o ./obj/realFft32b.dcheo ./asm/realFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/cplxiFft32b.lst -o ./obj/cplxiFft32b.dcheo ./asm/cplxiFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/realiFft32b.lst -o ./obj/realiFft32b.dcheo ./asm/realiFft32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/cplxmag32b.lst -o ./obj/cplxmag32b.dcheo ./asm/cplxmag32b.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/firinit.lst -o ./obj/firinit.dcheo ./asm/firinit.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/firdelay.lst -o ./obj/firdelay.dcheo ./asm/firdelay.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/firintdl.lst -o ./obj/firintdl.dcheo ./asm/firintdl.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/firdecim.lst -o ./obj/firdecim.dcheo ./asm/firdecim.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/firinter.lst -o ./obj/firinter.dcheo ./asm/firinter.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/firlms.lst -o ./obj/firlms.dcheo ./asm/firlms.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/firlmsn.lst -o ./obj/firlmsn.dcheo ./asm/firlmsn.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/firlatt.lst -o ./obj/firlatt.dcheo ./asm/firlatt.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/iirlatt.lst -o ./obj/iirlatt.dcheo ./asm/iirlatt.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/fir.lst -o ./obj/fir.dcheo ./asm/fir.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/iircan.lst -o ./obj/iircan.dcheo ./asm/iircan.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/iirtrans.lst -o ./obj/iirtrans.dcheo ./asm/iirtrans.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/cplxsqrmag.lst -o ./obj/cplxsqrmag.dcheo ./asm/cplxsqrmag.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/pid.lst -o ./obj/pid.dcheo ./asm/pid.s
xc16-as  --defsym YMEM_ERRATA=1 --defsym PSV_ERRATA=1 --defsym __dsPIC33C=1 -p generic-16dsp-ch -I ./inc -a=./obj/stackguard.lst -o ./obj/stackguard.dcheo ./asm/stackguard.s

xc16-ar -q lib/libdsp-%XC16_OMF%.a ./obj/flt2frct.dcheo ./obj/frct2flt.dcheo ./obj/initbart.dcheo ./obj/initblck.dcheo ./obj/inithamm.dcheo ./obj/inithann.dcheo ./obj/initkais.dcheo ./obj/inittwid.dcheo ./obj/initcosf.dcheo ./obj/minv.dcheo ./obj/fft32.dcheo ./obj/ifft32.dcheo ./obj/fft32oop.dcheo ./obj/ifft32oop.dcheo ./obj/vmax.dcheo ./obj/vcopy.dcheo ./obj/vzpad.dcheo ./obj/vmin.dcheo ./obj/vneg.dcheo ./obj/vscl.dcheo ./obj/vadd.dcheo ./obj/vsub.dcheo ./obj/vmul.dcheo ./obj/vdot.dcheo ./obj/vpow.dcheo ./obj/vcon.dcheo ./obj/vcor.dcheo ./obj/dowindow.dcheo ./obj/mscl.dcheo ./obj/mtrp.dcheo ./obj/madd.dcheo ./obj/msub.dcheo ./obj/mmul.dcheo ./obj/dct.dcheo ./obj/dctoop.dcheo ./obj/fft.dcheo ./obj/fftoop.dcheo ./obj/ifft.dcheo ./obj/ifftoop.dcheo ./obj/bitrev.dcheo ./obj/firinit.dcheo ./obj/firdelay.dcheo ./obj/firintdl.dcheo ./obj/firdecim.dcheo ./obj/firinter.dcheo ./obj/firlms.dcheo ./obj/firlmsn.dcheo ./obj/firlatt.dcheo ./obj/iirlatt.dcheo ./obj/fir.dcheo ./obj/iircan.dcheo ./obj/iirtrans.dcheo ./obj/cplxsqrmag.dcheo ./obj/pid.dcheo ./obj/tonegen32b.dcheo ./obj/rbrev32b.dcheo ./obj/cplxFft32b.dcheo ./obj/realFft32b.dcheo ./obj/cplxiFft32b.dcheo ./obj/realiFft32b.dcheo ./obj/cplxmag32b.dcheo ./obj/stackguard.dcheo

set XC16_OMF=
set XC16_OMF=%SAVE%
:end


