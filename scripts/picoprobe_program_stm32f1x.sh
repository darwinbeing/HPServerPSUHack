#!/bin/sh
src/openocd -s tcl -f interface/cmsis-dap.cfg -f target/stm32f1x.cfg -c "program /Users/Thomas/Developer/HP-HSTNS-PLXX/HPServerPSUHack/firmware/stm32f100/STM32F100RBT6B.bin verify reset exit 0x08000000"
