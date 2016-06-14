################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c \
../src/main.c \
../src/stm32f0xx_it.c \
../src/stm32f0xx_nucleo.c 

OBJS += \
./src/_write.o \
./src/main.o \
./src/stm32f0xx_it.o \
./src/stm32f0xx_nucleo.o 

C_DEPS += \
./src/_write.d \
./src/main.d \
./src/stm32f0xx_it.d \
./src/stm32f0xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DUSE_HAL_LIBRARY -DDEBUG -DSTM32F030x6 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


