################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../system/include/cmsis/startup_stm32f051x8.S 

OBJS += \
./system/include/cmsis/startup_stm32f051x8.o 

S_UPPER_DEPS += \
./system/include/cmsis/startup_stm32f051x8.d 


# Each subdirectory must supply rules for building sources it contributes
system/include/cmsis/%.o: ../system/include/cmsis/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -x assembler-with-cpp -DSTM32F051x8 -DUSE_HAL_LIBRARY -DDEBUG -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0xx" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


