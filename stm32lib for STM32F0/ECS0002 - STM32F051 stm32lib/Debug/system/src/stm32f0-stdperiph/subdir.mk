################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/stm32f0-stdperiph/stm32f0xx_adc.c \
../system/src/stm32f0-stdperiph/stm32f0xx_can.c \
../system/src/stm32f0-stdperiph/stm32f0xx_cec.c \
../system/src/stm32f0-stdperiph/stm32f0xx_comp.c \
../system/src/stm32f0-stdperiph/stm32f0xx_crc.c \
../system/src/stm32f0-stdperiph/stm32f0xx_crs.c \
../system/src/stm32f0-stdperiph/stm32f0xx_dac.c \
../system/src/stm32f0-stdperiph/stm32f0xx_dbgmcu.c \
../system/src/stm32f0-stdperiph/stm32f0xx_dma.c \
../system/src/stm32f0-stdperiph/stm32f0xx_exti.c \
../system/src/stm32f0-stdperiph/stm32f0xx_flash.c \
../system/src/stm32f0-stdperiph/stm32f0xx_gpio.c \
../system/src/stm32f0-stdperiph/stm32f0xx_i2c.c \
../system/src/stm32f0-stdperiph/stm32f0xx_iwdg.c \
../system/src/stm32f0-stdperiph/stm32f0xx_misc.c \
../system/src/stm32f0-stdperiph/stm32f0xx_pwr.c \
../system/src/stm32f0-stdperiph/stm32f0xx_rcc.c \
../system/src/stm32f0-stdperiph/stm32f0xx_rtc.c \
../system/src/stm32f0-stdperiph/stm32f0xx_spi.c \
../system/src/stm32f0-stdperiph/stm32f0xx_syscfg.c \
../system/src/stm32f0-stdperiph/stm32f0xx_tim.c \
../system/src/stm32f0-stdperiph/stm32f0xx_usart.c \
../system/src/stm32f0-stdperiph/stm32f0xx_wwdg.c 

OBJS += \
./system/src/stm32f0-stdperiph/stm32f0xx_adc.o \
./system/src/stm32f0-stdperiph/stm32f0xx_can.o \
./system/src/stm32f0-stdperiph/stm32f0xx_cec.o \
./system/src/stm32f0-stdperiph/stm32f0xx_comp.o \
./system/src/stm32f0-stdperiph/stm32f0xx_crc.o \
./system/src/stm32f0-stdperiph/stm32f0xx_crs.o \
./system/src/stm32f0-stdperiph/stm32f0xx_dac.o \
./system/src/stm32f0-stdperiph/stm32f0xx_dbgmcu.o \
./system/src/stm32f0-stdperiph/stm32f0xx_dma.o \
./system/src/stm32f0-stdperiph/stm32f0xx_exti.o \
./system/src/stm32f0-stdperiph/stm32f0xx_flash.o \
./system/src/stm32f0-stdperiph/stm32f0xx_gpio.o \
./system/src/stm32f0-stdperiph/stm32f0xx_i2c.o \
./system/src/stm32f0-stdperiph/stm32f0xx_iwdg.o \
./system/src/stm32f0-stdperiph/stm32f0xx_misc.o \
./system/src/stm32f0-stdperiph/stm32f0xx_pwr.o \
./system/src/stm32f0-stdperiph/stm32f0xx_rcc.o \
./system/src/stm32f0-stdperiph/stm32f0xx_rtc.o \
./system/src/stm32f0-stdperiph/stm32f0xx_spi.o \
./system/src/stm32f0-stdperiph/stm32f0xx_syscfg.o \
./system/src/stm32f0-stdperiph/stm32f0xx_tim.o \
./system/src/stm32f0-stdperiph/stm32f0xx_usart.o \
./system/src/stm32f0-stdperiph/stm32f0xx_wwdg.o 

C_DEPS += \
./system/src/stm32f0-stdperiph/stm32f0xx_adc.d \
./system/src/stm32f0-stdperiph/stm32f0xx_can.d \
./system/src/stm32f0-stdperiph/stm32f0xx_cec.d \
./system/src/stm32f0-stdperiph/stm32f0xx_comp.d \
./system/src/stm32f0-stdperiph/stm32f0xx_crc.d \
./system/src/stm32f0-stdperiph/stm32f0xx_crs.d \
./system/src/stm32f0-stdperiph/stm32f0xx_dac.d \
./system/src/stm32f0-stdperiph/stm32f0xx_dbgmcu.d \
./system/src/stm32f0-stdperiph/stm32f0xx_dma.d \
./system/src/stm32f0-stdperiph/stm32f0xx_exti.d \
./system/src/stm32f0-stdperiph/stm32f0xx_flash.d \
./system/src/stm32f0-stdperiph/stm32f0xx_gpio.d \
./system/src/stm32f0-stdperiph/stm32f0xx_i2c.d \
./system/src/stm32f0-stdperiph/stm32f0xx_iwdg.d \
./system/src/stm32f0-stdperiph/stm32f0xx_misc.d \
./system/src/stm32f0-stdperiph/stm32f0xx_pwr.d \
./system/src/stm32f0-stdperiph/stm32f0xx_rcc.d \
./system/src/stm32f0-stdperiph/stm32f0xx_rtc.d \
./system/src/stm32f0-stdperiph/stm32f0xx_spi.d \
./system/src/stm32f0-stdperiph/stm32f0xx_syscfg.d \
./system/src/stm32f0-stdperiph/stm32f0xx_tim.d \
./system/src/stm32f0-stdperiph/stm32f0xx_usart.d \
./system/src/stm32f0-stdperiph/stm32f0xx_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/stm32f0-stdperiph/%.o: ../system/src/stm32f0-stdperiph/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


