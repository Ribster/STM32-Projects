################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/cmsis/src/startup_cm.c \
../libs/cmsis/src/startup_stm32f4xx.c \
../libs/cmsis/src/system_stm32f4xx.c \
../libs/cmsis/src/vectors_stm32f4xx.c 

OBJS += \
./libs/cmsis/src/startup_cm.o \
./libs/cmsis/src/startup_stm32f4xx.o \
./libs/cmsis/src/system_stm32f4xx.o \
./libs/cmsis/src/vectors_stm32f4xx.o 

C_DEPS += \
./libs/cmsis/src/startup_cm.d \
./libs/cmsis/src/startup_stm32f4xx.d \
./libs/cmsis/src/system_stm32f4xx.d \
./libs/cmsis/src/vectors_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
libs/cmsis/src/%.o: ../libs/cmsis/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DDEBUG -I"../include" -I"../libs/cmsis/include" -I"../libs/StdPeriph/include" -I"../libs/misc/include" -I../libs/fatfs/inc -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


