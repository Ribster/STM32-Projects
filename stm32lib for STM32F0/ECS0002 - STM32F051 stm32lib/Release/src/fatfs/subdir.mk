################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/fatfs/diskio.c \
../src/fatfs/ff.c \
../src/fatfs/stm320518_eval.c \
../src/fatfs/stm320518_eval_spi_sd.c 

OBJS += \
./src/fatfs/diskio.o \
./src/fatfs/ff.o \
./src/fatfs/stm320518_eval.o \
./src/fatfs/stm320518_eval_spi_sd.o 

C_DEPS += \
./src/fatfs/diskio.d \
./src/fatfs/ff.d \
./src/fatfs/stm320518_eval.d \
./src/fatfs/stm320518_eval_spi_sd.d 


# Each subdirectory must supply rules for building sources it contributes
src/fatfs/%.o: ../src/fatfs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"/Users/Robbe/Documents/PROJECTS/ECLIPSE STM32 RVA_STM32Lib/ECR0002 - RVA_STM32Lib REV B/RVA_STM32Lib_Final/RVA_STM32Lib_include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


