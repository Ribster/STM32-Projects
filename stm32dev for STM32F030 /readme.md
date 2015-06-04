#Full overview of the STM32F030 examples#
The purpose of this directory is to make simple examples that don't rely on the STM32FxDiscovery boards.
I'm using every resource that i can to make this possible.
Made an upgrade to the STM32F0xx SPL v1.5.0, so it's up to the lastest lib.
Resources used:
 * STM32F0xx_Snippets_Package_V1.1.1
 * STM32F0xx_StdPeriph_Lib_V1.5.0
 * http://www.eevblog.com/forum/microcontrollers/stm32-ghetto-style/
  * Mostly the HSI routine and the concept idea
 * https://github.com/LonelyWolf/stm32
  * BMP180 structure used 

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
* ECS0018 - STM32F0 - UART reflection example STM32F030F4 _stdlib
  * UART example full duplex reflecting everything that comes in. After \r is received, buffer is transmitted
