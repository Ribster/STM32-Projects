################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/project/initialization.c \
../src/project/leds.c \
../src/project/pushbuttons.c \
../src/project/terminal.c 

OBJS += \
./src/project/initialization.o \
./src/project/leds.o \
./src/project/pushbuttons.o \
./src/project/terminal.o 

C_DEPS += \
./src/project/initialization.d \
./src/project/leds.d \
./src/project/pushbuttons.d \
./src/project/terminal.d 


# Each subdirectory must supply rules for building sources it contributes
src/project/%.o: ../src/project/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DDEBUG -I"../include" -I"../libs/cmsis/include" -I"../libs/StdPeriph/include" -I"../libs/misc/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


