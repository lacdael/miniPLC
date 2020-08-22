#!/bin/bash

#try to recompile ezbl_comm again. Keep commenting out where it breakes comment on line says, it only breaks because of the i2c code.
#in wine com5 is mapped to /tty/USB0

#Usage:
#    ezbl_comm -com=path [-baud=bps] [-i2c_addr=target] [-timeout=ms] [-log=log.txt] "firmware.bl2"
#
#    Parameters:
#        -artifact   Source .bl2 file containing the firmware image for programming.
#	            The '-artifact=' prefix can be ommitted if the filename is provided last.
#	-com        System dependent target communications file path.
#	            For Windows MCP2221 I2C use, specify "I2C", "I2C1", "I2C2", etc. The numerical
#	            suffix denotes an instance number where no suffix is synonymous with "I2C1".
#	-baud       Communications baud/physical bit rate (UART default 115200, I2C default 400000)
#	-i2c_addr   Target I2C slave address (default 0x60, ignored for non-I2C)
#	-timeout    Communications timeout in milliseconds (default 1100ms)
#	-log        File to write human readable (ascii hex) TX/RX communications data to (default disabled)
#	            If this option is specified without a filename, data is written to stdout
#    Examples:
#        ezbl_comm.exe -com=COM21 -baud=460800 -timeout=8000 -artifact="ex_app_led_blink.production.bl2"
#	ezbl_comm.exe -com=I2C1 -i2c_addr=0x60 -baud=400000 -log=i2c_log.txt ex_app_led_blink.production.bl2
#	./ezbl_comm -com=/dev/ttyS20 -baud=230400 -timeout=750 -log=log.txt -artifact=app_v2.bl2
#	./ezbl_comm -com=/dev/ACM0 "ex_app_led_blink.production.bl2"
#	./ezbl_comm -com=/dev/I2C-0 -baud=400000 -i2c_addr=0x60 -artifact="ex_app_led_blink.production.bl2" 
														


ls -alF /home/christian/.wine/dosdevices

wine ./ezbl_integration/ezbl_comm.exe -com=com5 -baud=115200 -timeout=1100 -artifact="dist/uart/production/ex_app_led_blink_pic32mm.production.bl2"

