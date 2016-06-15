################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/system_stm32f1xx.c 

S_UPPER_SRCS += \
../system/src/cmsis/startup_stm32f105xc.S 

OBJS += \
./system/src/cmsis/startup_stm32f105xc.o \
./system/src/cmsis/system_stm32f1xx.o 

S_UPPER_DEPS += \
./system/src/cmsis/startup_stm32f105xc.d 

C_DEPS += \
./system/src/cmsis/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/%.o: ../system/src/cmsis/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -x assembler-with-cpp -DUSE_HAL_LIBRARY -DDEBUG -DTRACE -DSTM32F105xC -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1xx" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/cmsis/%.o: ../system/src/cmsis/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DUSE_HAL_LIBRARY -DDEBUG -DTRACE -DSTM32F105xC -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


