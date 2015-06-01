################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/StdPeriph/src/stm32f0xx_adc.c \
../libs/StdPeriph/src/stm32f0xx_cec.c \
../libs/StdPeriph/src/stm32f0xx_comp.c \
../libs/StdPeriph/src/stm32f0xx_crc.c \
../libs/StdPeriph/src/stm32f0xx_dac.c \
../libs/StdPeriph/src/stm32f0xx_dbgmcu.c \
../libs/StdPeriph/src/stm32f0xx_exti.c \
../libs/StdPeriph/src/stm32f0xx_flash.c \
../libs/StdPeriph/src/stm32f0xx_gpio.c \
../libs/StdPeriph/src/stm32f0xx_i2c.c \
../libs/StdPeriph/src/stm32f0xx_iwdg.c \
../libs/StdPeriph/src/stm32f0xx_misc.c \
../libs/StdPeriph/src/stm32f0xx_pwr.c \
../libs/StdPeriph/src/stm32f0xx_rcc.c \
../libs/StdPeriph/src/stm32f0xx_rtc.c \
../libs/StdPeriph/src/stm32f0xx_spi.c \
../libs/StdPeriph/src/stm32f0xx_syscfg.c \
../libs/StdPeriph/src/stm32f0xx_tim.c \
../libs/StdPeriph/src/stm32f0xx_usart.c \
../libs/StdPeriph/src/stm32f0xx_wwdg.c 

OBJS += \
./libs/StdPeriph/src/stm32f0xx_adc.o \
./libs/StdPeriph/src/stm32f0xx_cec.o \
./libs/StdPeriph/src/stm32f0xx_comp.o \
./libs/StdPeriph/src/stm32f0xx_crc.o \
./libs/StdPeriph/src/stm32f0xx_dac.o \
./libs/StdPeriph/src/stm32f0xx_dbgmcu.o \
./libs/StdPeriph/src/stm32f0xx_exti.o \
./libs/StdPeriph/src/stm32f0xx_flash.o \
./libs/StdPeriph/src/stm32f0xx_gpio.o \
./libs/StdPeriph/src/stm32f0xx_i2c.o \
./libs/StdPeriph/src/stm32f0xx_iwdg.o \
./libs/StdPeriph/src/stm32f0xx_misc.o \
./libs/StdPeriph/src/stm32f0xx_pwr.o \
./libs/StdPeriph/src/stm32f0xx_rcc.o \
./libs/StdPeriph/src/stm32f0xx_rtc.o \
./libs/StdPeriph/src/stm32f0xx_spi.o \
./libs/StdPeriph/src/stm32f0xx_syscfg.o \
./libs/StdPeriph/src/stm32f0xx_tim.o \
./libs/StdPeriph/src/stm32f0xx_usart.o \
./libs/StdPeriph/src/stm32f0xx_wwdg.o 

C_DEPS += \
./libs/StdPeriph/src/stm32f0xx_adc.d \
./libs/StdPeriph/src/stm32f0xx_cec.d \
./libs/StdPeriph/src/stm32f0xx_comp.d \
./libs/StdPeriph/src/stm32f0xx_crc.d \
./libs/StdPeriph/src/stm32f0xx_dac.d \
./libs/StdPeriph/src/stm32f0xx_dbgmcu.d \
./libs/StdPeriph/src/stm32f0xx_exti.d \
./libs/StdPeriph/src/stm32f0xx_flash.d \
./libs/StdPeriph/src/stm32f0xx_gpio.d \
./libs/StdPeriph/src/stm32f0xx_i2c.d \
./libs/StdPeriph/src/stm32f0xx_iwdg.d \
./libs/StdPeriph/src/stm32f0xx_misc.d \
./libs/StdPeriph/src/stm32f0xx_pwr.d \
./libs/StdPeriph/src/stm32f0xx_rcc.d \
./libs/StdPeriph/src/stm32f0xx_rtc.d \
./libs/StdPeriph/src/stm32f0xx_spi.d \
./libs/StdPeriph/src/stm32f0xx_syscfg.d \
./libs/StdPeriph/src/stm32f0xx_tim.d \
./libs/StdPeriph/src/stm32f0xx_usart.d \
./libs/StdPeriph/src/stm32f0xx_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
libs/StdPeriph/src/%.o: ../libs/StdPeriph/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DSTM32F030 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../libs/CMSIS/include" -I"../libs/StdPeriph/include" -I"../libs/misc/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


