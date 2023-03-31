# `devtools`

This folder contains bits of code that are useful for development. 

This file contains brief descriptions of each of the sub-folders.

## `commandSpammer`
This is a PlatformIO project made to run on an Arduino Leonardo. When pin 2 is high,
the Leonardo will repeatedly send a string over UART. The string is set by sending a 
string to the Leonardo over the USB serial connection terminated with a linefeed 
character (`\n`).


## `QuickNDirtyPowerToggle`
This is an Arduino project which can be used to toggle power to the power supply by 
sending commands like `ON`, `OFF`, or `ESTOP`. 

## `QuickNDirtySerialEcho`
This is an Arduino project which echos any text received over the serial port back. 
