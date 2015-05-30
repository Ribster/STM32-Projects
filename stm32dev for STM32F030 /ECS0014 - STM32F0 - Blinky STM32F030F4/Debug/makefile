################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include libs/misc/src/subdir.mk
-include libs/StdPeriph/src/subdir.mk
-include libs/CMSIS/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ECS0013\ -\ STM32F0\ -\ Test\ project\ STM32F030F4.hex \

SECONDARY_SIZE += \
ECS0013\ -\ STM32F0\ -\ Test\ project\ STM32F030F4.siz \


# All Target
all: ECS0013\ -\ STM32F0\ -\ Test\ project\ STM32F030F4.elf secondary-outputs

# Tool invocations
ECS0013\ -\ STM32F0\ -\ Test\ project\ STM32F030F4.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g1 -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"ECS0013 - STM32F0 - Test project STM32F030F4.map" -o "ECS0013 - STM32F0 - Test project STM32F030F4.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ECS0013\ -\ STM32F0\ -\ Test\ project\ STM32F030F4.hex: ECS0013\ -\ STM32F0\ -\ Test\ project\ STM32F030F4.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "ECS0013 - STM32F0 - Test project STM32F030F4.elf"  "ECS0013 - STM32F0 - Test project STM32F030F4.hex"
	@echo 'Finished building: $@'
	@echo ' '

ECS0013\ -\ STM32F0\ -\ Test\ project\ STM32F030F4.siz: ECS0013\ -\ STM32F0\ -\ Test\ project\ STM32F030F4.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "ECS0013 - STM32F0 - Test project STM32F030F4.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) "ECS0013 - STM32F0 - Test project STM32F030F4.elf"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
