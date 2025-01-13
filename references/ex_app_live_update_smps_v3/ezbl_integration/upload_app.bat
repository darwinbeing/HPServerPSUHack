:: Set com=I2C or I2C1/I2C2/etc. for MCP2221A I2C transfers. i2c_addr is ignored for UART transfers.
@setlocal
@set com=COM21
@set baud=230400
@set timeout=1100
@set i2c_addr=0x60
@set artifact=%1

:: See if we have an .elf and need to convert to a .bl2 file instead
@if /I not "%~x1"==".elf" goto SendBL2
java -jar %~dp0ezbl_tools.jar --blobber -artifact=%artifact%
@set artifact=%~dp1%~n1.bl2
@goto SendBL2

:SendBL2
%~dp0ezbl_comm -com=%com% -baud=%baud% -timeout=%timeout% -i2c_addr=%i2c_addr% -log="%TEMP%\ezbl_comm_log.txt" -artifact=%artifact%
@goto End


:End
@pause

