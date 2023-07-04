#!/bin/sh
src/openocd -s tcl -f interface/cmsis-dap.cfg -f target/stm32f1x.cfg -c init -c "reset halt" -c "flash read_bank 0 STM32F100RBT6B.bin 0 0x20000" -c "reset" -c shutdown
