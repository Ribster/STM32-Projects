################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/fonts/subdir.mk
-include src/subdir.mk
-include libs/misc/src/subdir.mk
-include libs/fatfs/src/subdir.mk
-include libs/cmsis/src/subdir.mk
-include libs/StdPeriph/src/subdir.mk
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
ECW4011\ -\ STM32F4\ -\ Analog\ Datalogger\ REV\ B\ on\ ALT0061.hex \

SECONDARY_SIZE += \
ECW4011\ -\ STM32F4\ -\ Analog\ Datalogger\ REV\ B\ on\ ALT0061.siz \


# All Target
all: ECW4011\ -\ STM32F4\ -\ Analog\ Datalogger\ REV\ B\ on\ ALT0061.elf secondary-outputs

# Tool invocations
ECW4011\ -\ STM32F4\ -\ Analog\ Datalogger\ REV\ B\ on\ ALT0061.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -T libs.ld -T mem.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"ECW4011 - STM32F4 - Analog Datalogger REV B on ALT0061.map" -o "ECW4011 - STM32F4 - Analog Datalogger REV B on ALT0061.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ECW4011\ -\ STM32F4\ -\ Analog\ Datalogger\ REV\ B\ on\ ALT0061.hex: ECW4011\ -\ STM32F4\ -\ Analog\ Datalogger\ REV\ B\ on\ ALT0061.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "ECW4011 - STM32F4 - Analog Datalogger REV B on ALT0061.elf"  "ECW4011 - STM32F4 - Analog Datalogger REV B on ALT0061.hex"
	@echo 'Finished building: $@'
	@echo ' '

ECW4011\ -\ STM32F4\ -\ Analog\ Datalogger\ REV\ B\ on\ ALT0061.siz: ECW4011\ -\ STM32F4\ -\ Analog\ Datalogger\ REV\ B\ on\ ALT0061.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "ECW4011 - STM32F4 - Analog Datalogger REV B on ALT0061.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) "ECW4011 - STM32F4 - Analog Datalogger REV B on ALT0061.elf"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
