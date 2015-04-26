################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/afe.c \
../src/delay.c \
../src/dma.c \
../src/encoders.c \
../src/gpio.c \
../src/initialization.c \
../src/interrupts.c \
../src/leds.c \
../src/main.c \
../src/menustructure.c \
../src/nvic.c \
../src/pushbuttons.c \
../src/rcc.c \
../src/rng.c \
../src/rtc.c \
../src/sdio.c \
../src/spi.c \
../src/ssd1306.c \
../src/uart.c \
../src/usb.c 

OBJS += \
./src/afe.o \
./src/delay.o \
./src/dma.o \
./src/encoders.o \
./src/gpio.o \
./src/initialization.o \
./src/interrupts.o \
./src/leds.o \
./src/main.o \
./src/menustructure.o \
./src/nvic.o \
./src/pushbuttons.o \
./src/rcc.o \
./src/rng.o \
./src/rtc.o \
./src/sdio.o \
./src/spi.o \
./src/ssd1306.o \
./src/uart.o \
./src/usb.o 

C_DEPS += \
./src/afe.d \
./src/delay.d \
./src/dma.d \
./src/encoders.d \
./src/gpio.d \
./src/initialization.d \
./src/interrupts.d \
./src/leds.d \
./src/main.d \
./src/menustructure.d \
./src/nvic.d \
./src/pushbuttons.d \
./src/rcc.d \
./src/rng.d \
./src/rtc.d \
./src/sdio.d \
./src/spi.d \
./src/ssd1306.d \
./src/uart.d \
./src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DDEBUG -I"../include" -I"../libs/cmsis/include" -I"../libs/StdPeriph/include" -I"../libs/misc/include" -I../libs/fatfs/inc -I../libs/usb/usb_conf/inc -I../libs/usb/usb_lib/cdc/inc -I../libs/usb/usb_lib/core/inc -I../libs/usb/usb_lib/otg/inc -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


