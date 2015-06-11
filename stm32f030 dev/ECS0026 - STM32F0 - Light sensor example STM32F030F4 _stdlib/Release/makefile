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
ECS0026\ -\ STM32F0\ -\ Light\ sensor\ example\ STM32F030F4\ _stdlib.hex \

SECONDARY_SIZE += \
ECS0026\ -\ STM32F0\ -\ Light\ sensor\ example\ STM32F030F4\ _stdlib.siz \


# All Target
all: ECS0026\ -\ STM32F0\ -\ Light\ sensor\ example\ STM32F030F4\ _stdlib.elf secondary-outputs

# Tool invocations
ECS0026\ -\ STM32F0\ -\ Light\ sensor\ example\ STM32F030F4\ _stdlib.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"ECS0026 - STM32F0 - Light sensor example STM32F030F4 _stdlib.map" -o "ECS0026 - STM32F0 - Light sensor example STM32F030F4 _stdlib.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ECS0026\ -\ STM32F0\ -\ Light\ sensor\ example\ STM32F030F4\ _stdlib.hex: ECS0026\ -\ STM32F0\ -\ Light\ sensor\ example\ STM32F030F4\ _stdlib.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "ECS0026 - STM32F0 - Light sensor example STM32F030F4 _stdlib.elf"  "ECS0026 - STM32F0 - Light sensor example STM32F030F4 _stdlib.hex"
	@echo 'Finished building: $@'
	@echo ' '

ECS0026\ -\ STM32F0\ -\ Light\ sensor\ example\ STM32F030F4\ _stdlib.siz: ECS0026\ -\ STM32F0\ -\ Light\ sensor\ example\ STM32F030F4\ _stdlib.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "ECS0026 - STM32F0 - Light sensor example STM32F030F4 _stdlib.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) "ECS0026 - STM32F0 - Light sensor example STM32F030F4 _stdlib.elf"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
