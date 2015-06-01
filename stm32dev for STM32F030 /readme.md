#Full overview of the STM32F030 examples#
The purpose of this directory is to make simple examples that don't rely on the STM32FxDiscovery boards.

These examples are based on:
* __ALT0062 - STM32F030 breakout__

##RCC##
* ECS0017 - STM32F0 - HSI example STM32F030F4 _stdlib
* ECS0017 - STM32F0 - HSI example STM32F030F4 _nolib
  * HSI example, where internal oscillator of 8MHz -> PLL -> 48MHz

##GPIO##
* ECS0014 - STM32F0 - Blinky STM32F030F4 _stdlib
* ECS0014 - STM32F0 - Blinky STM32F030F4 _nolib
  * GPIO example of toggling LEDs

##TIM##
* ECS0015 - STM32F0 - VF blinky STM32F030F4 _stdlib
  * Variable Frequency example fading LEDs 
  
* ECS0016 - STM32F0 - PWM blinky STM32F030F4 _stdlib
  * PWM example fading LEDs

##UART##
* xx
  * UART example full duplex
