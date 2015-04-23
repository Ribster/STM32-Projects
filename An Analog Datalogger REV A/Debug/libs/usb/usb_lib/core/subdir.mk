################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/usb/usb_lib/core/usbd_core.c \
../libs/usb/usb_lib/core/usbd_ioreq.c \
../libs/usb/usb_lib/core/usbd_req.c 

OBJS += \
./libs/usb/usb_lib/core/usbd_core.o \
./libs/usb/usb_lib/core/usbd_ioreq.o \
./libs/usb/usb_lib/core/usbd_req.o 

C_DEPS += \
./libs/usb/usb_lib/core/usbd_core.d \
./libs/usb/usb_lib/core/usbd_ioreq.d \
./libs/usb/usb_lib/core/usbd_req.d 


# Each subdirectory must supply rules for building sources it contributes
libs/usb/usb_lib/core/%.o: ../libs/usb/usb_lib/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DDEBUG -I"../include" -I"../libs/cmsis/include" -I"../libs/StdPeriph/include" -I"../libs/misc/include" -I../libs/fatfs/inc -I../libs/usb/usb_conf -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


