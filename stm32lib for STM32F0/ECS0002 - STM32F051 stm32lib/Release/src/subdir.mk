################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/newlib_stubs.c 

CPP_SRCS += \
../src/DELAY.cpp \
../src/HC595.cpp \
../src/IO.cpp \
../src/NEC.cpp \
../src/OSCILLATOR.cpp \
../src/RCCConfig.cpp \
../src/SPI.cpp \
../src/UART.cpp \
../src/WS2812B.cpp \
../src/nRF24L01p.cpp \
../src/nRF24L01p_Old.cpp 

OBJS += \
./src/DELAY.o \
./src/HC595.o \
./src/IO.o \
./src/NEC.o \
./src/OSCILLATOR.o \
./src/RCCConfig.o \
./src/SPI.o \
./src/UART.o \
./src/WS2812B.o \
./src/nRF24L01p.o \
./src/nRF24L01p_Old.o \
./src/newlib_stubs.o 

C_DEPS += \
./src/newlib_stubs.d 

CPP_DEPS += \
./src/DELAY.d \
./src/HC595.d \
./src/IO.d \
./src/NEC.d \
./src/OSCILLATOR.d \
./src/RCCConfig.d \
./src/SPI.d \
./src/UART.d \
./src/WS2812B.d \
./src/nRF24L01p.d \
./src/nRF24L01p_Old.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"/Users/Robbe/Documents/PROJECTS/ECLIPSE STM32 RVA_STM32Lib/ECR0002 - RVA_STM32Lib REV B/RVA_STM32Lib_Final/RVA_STM32Lib_include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0-stdperiph" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"/Users/Robbe/Documents/PROJECTS/ECLIPSE STM32 RVA_STM32Lib/ECR0002 - RVA_STM32Lib REV B/RVA_STM32Lib_Final/RVA_STM32Lib_include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


