################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/CMSIS/src/startup_cm.c \
../libs/CMSIS/src/startup_stm32f0xx.c \
../libs/CMSIS/src/system_stm32f0xx.c \
../libs/CMSIS/src/vectors_stm32f0xx.c 

OBJS += \
./libs/CMSIS/src/startup_cm.o \
./libs/CMSIS/src/startup_stm32f0xx.o \
./libs/CMSIS/src/system_stm32f0xx.o \
./libs/CMSIS/src/vectors_stm32f0xx.o 

C_DEPS += \
./libs/CMSIS/src/startup_cm.d \
./libs/CMSIS/src/startup_stm32f0xx.d \
./libs/CMSIS/src/system_stm32f0xx.d \
./libs/CMSIS/src/vectors_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
libs/CMSIS/src/%.o: ../libs/CMSIS/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DSTM32F030 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../libs/CMSIS/include" -I"../libs/StdPeriph/include" -I"../libs/misc/include" -I../libs/CPAL/include -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


