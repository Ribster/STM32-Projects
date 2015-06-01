#Full overview of the STM32F030 examples#
The purpose of this directory is to make simple examples that don't rely on the STM32FxDiscovery boards.

These examples are based on __ALT0062 - STM32F030 breakout__

##RCC##
* xx
  * HSI example, where internal oscillator of 8MHz -> PLL -> 48MHz
* xx
  * HSI example, where internal oscillator of 8MHz -> PLL -> 48MHz - __no stdlib dependency__

##GPIO##
* ECS0014 - STM32F0 - Blinky STM32F030F4
  * GPIO example of toggling LEDs
* xx
  * GPIO example of toggling LEDs - __no stdlib dependency__

##TIM##
* ECS0015 - STM32F0 - VF blinky STM32F030F4
  * Variable Frequency example fading LEDs 
*  xx
  * Variable Frequency example fading LEDs - __no stdlib dependency__
* ECS0016 - STM32F0 - PWM blinky STM32F030F4
  * PWM example fading LEDs 
*  xx
  * PWM example fading LEDs - __no stdlib dependency__

##UART##
* xx
  * UART example full duplex
* xx
  * UART example full duplex - __no stdlib dependency__

##COMBINED
* ECS00xx - STM32F0 - 
  * Example of full duplex UART with LEDs 
  * __GPIO, UART__
* ECS00xx - STM32F0 - 
  * Example of full duplex UART with LEDs - __no stdlib dependency__
  * __GPIO, UART__
