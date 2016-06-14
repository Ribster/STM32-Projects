################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c \
../src/main.c \
../src/stm32746g_discovery.c \
../src/stm32f7xx_it.c 

OBJS += \
./src/_write.o \
./src/main.o \
./src/stm32746g_discovery.o \
./src/stm32f7xx_it.o 

C_DEPS += \
./src/_write.d \
./src/main.d \
./src/stm32746g_discovery.d \
./src/stm32f7xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DSTM32F746xx -DUSE_HAL_LIBRARY -DDEBUG -DTRACE -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f7xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


