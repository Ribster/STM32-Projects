################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/usb/usb_conf/src/usb_bsp.c \
../libs/usb/usb_conf/src/usbd_desc.c \
../libs/usb/usb_conf/src/usbd_usr.c 

OBJS += \
./libs/usb/usb_conf/src/usb_bsp.o \
./libs/usb/usb_conf/src/usbd_desc.o \
./libs/usb/usb_conf/src/usbd_usr.o 

C_DEPS += \
./libs/usb/usb_conf/src/usb_bsp.d \
./libs/usb/usb_conf/src/usbd_desc.d \
./libs/usb/usb_conf/src/usbd_usr.d 


# Each subdirectory must supply rules for building sources it contributes
libs/usb/usb_conf/src/%.o: ../libs/usb/usb_conf/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DDEBUG -I"../include" -I"../libs/cmsis/include" -I"../libs/StdPeriph/include" -I"../libs/misc/include" -I../libs/fatfs/inc -I../libs/usb/usb_conf/inc -I../libs/usb/usb_lib/cdc/inc -I../libs/usb/usb_lib/core/inc -I../libs/usb/usb_lib/otg/inc -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


