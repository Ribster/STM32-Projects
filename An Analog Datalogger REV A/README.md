# School Project: An Analog Datalogger REV A

Eclipse project for an STM32F407VGT6.
Made on custom board ALT0061

The files are split up in multiple .c && .h files.
The configuration is done in the projectconfig.h header.
Normally everything is derived from these settings, although not all changes will go through correctly.
This is because of the STM32F4 that has specific hardware linked to each specific pin.
So changing timers, dma requests need a little more care. Clocks are unaffected.
