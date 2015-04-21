################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/fatfs/src/diskio.c \
../libs/fatfs/src/ff.c 

OBJS += \
./libs/fatfs/src/diskio.o \
./libs/fatfs/src/ff.o 

C_DEPS += \
./libs/fatfs/src/diskio.d \
./libs/fatfs/src/ff.d 


# Each subdirectory must supply rules for building sources it contributes
libs/fatfs/src/%.o: ../libs/fatfs/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DDEBUG -I"../include" -I"../libs/cmsis/include" -I"../libs/StdPeriph/include" -I"../libs/misc/include" -I../libs/fatfs/inc -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


