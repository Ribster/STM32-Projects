#include "stm32f0xx.h"
#include <stdio.h>

#define PROJECTNAME "ECS0014 _stdlib"
#define VERSION "V0.0.2"
#define TESTED "2015/06/01"

/*
SWD:
PA14: SWCLK
PA15: SWDIO

Pinout:
PF0: LED ORANGE
PF1: LED GREEN
*/

// globals
	// systick timer
	volatile uint32_t timer_delayCounter;

// prototypes
	// systick timer
	void
	timer_delay(volatile uint32_t time);
	void
	timer_initMilliseconds(void);
	void
	timer_interruptHandler(void);

	// HSI for PLL
	void
	hsi_setPLLClock(uint32_t RCC_PLLMul);

	// gpio
	void
	gpio_init(void);

	// interrupts
	void
	SysTick_Handler(void);

int
main(void)
{
	// set HSI clock
	hsi_setPLLClock(RCC_PLLMul_12);

	// set systick clock
	timer_initMilliseconds();

	// set GPIO
	gpio_init();

	while(1){
		GPIO_SetBits(GPIOF, GPIO_Pin_0);
		GPIO_ResetBits(GPIOF, GPIO_Pin_1);

		timer_delay(1000);

		GPIO_ResetBits(GPIOF, GPIO_Pin_0);
		GPIO_SetBits(GPIOF, GPIO_Pin_1);

		timer_delay(1000);
	}

}




// systick timer
volatile uint32_t timer_delayCounter;
void
timer_delay(volatile uint32_t time){
	timer_delayCounter = time;
	while(timer_delayCounter!=0x00);
}
void
timer_initMilliseconds(void){
	SysTick_Config(SystemCoreClock / 1000);
}
void
timer_interruptHandler(void){
	if(timer_delayCounter!=0x00){(timer_delayCounter--);}
}

// HSI for PLL
void
hsi_setPLLClock(uint32_t RCC_PLLMul){
	RCC_DeInit();
	RCC_PLLCmd(DISABLE);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul);
	// Flash: 1 WaitState for 24MHz < SysCLK < 48 MHz
	FLASH_SetLatency(FLASH_Latency_0);
	FLASH_PrefetchBufferCmd(ENABLE);
	// Set ADC clock to sync PCLK/4->12MHz
	RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
	// and turn the PLL back on again
	RCC_PLLCmd(ENABLE);
	// set PLL as system clock source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}

//gpio
void
gpio_init(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	GPIO_InitTypeDef tmp;
	GPIO_StructInit(&tmp);
	tmp.GPIO_Mode = GPIO_Mode_OUT;
	tmp.GPIO_OType = GPIO_OType_PP;
	tmp.GPIO_PuPd = GPIO_PuPd_NOPULL;
	tmp.GPIO_Speed = GPIO_Speed_Level_3;
	tmp.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOF, &tmp);
}

// interrupts
void
SysTick_Handler(void)
{
  timer_interruptHandler();
}

