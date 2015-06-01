#include "stm32f0xx.h"
#include <stdio.h>

#define PROJECTNAME "ECS0016 _stdlib"
#define VERSION "V0.0.1"
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

	// timer
	volatile uint16_t PWM_Counter = 0;
	volatile uint16_t PWM_PeriodVal = 1000;
	volatile uint16_t PWM_CC1Val = 0;
	volatile uint16_t PWM_CC2Val = 0;


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

	// timer
	void
	TIM3_Int_Init(uint16_t arr,uint32_t frequency);
	void
	TIM3_NVIC_Init(void);

	// interrupts
	void
	SysTick_Handler(void);
	void
	TIM3_IRQHandler(void);

int
main(void)
{
	// set HSI clock
	hsi_setPLLClock(RCC_PLLMul_12);

	// set timer
	TIM3_Int_Init(1, 1000000);

	// set systick clock
	timer_initMilliseconds();

	// set GPIO
	gpio_init();

	while(1){
		// delay 1 ms
		for (int varloop = 0; varloop < 10; ++varloop) {
			for (int var = 0; var < 100; var++) {
				PWM_CC1Val+= 10;
				timer_delay(1);
			}
			for (int var = 0; var < 100; var++) {
				PWM_CC2Val+= 10;
				timer_delay(1);
			}
			for (int var = 0; var < 100; var++) {
				PWM_CC1Val-= 10;
				timer_delay(1);
			}
			for (int var = 0; var < 100; var++) {
				PWM_CC2Val-= 10;
				timer_delay(1);
			}
		}
		// delay 10 ms
		for (int var = 0; var < 100; var++) {
			PWM_CC1Val+= 10;
			timer_delay(10);
		}
		for (int var = 0; var < 100; var++) {
			PWM_CC2Val+= 10;
			timer_delay(10);
		}
		for (int var = 0; var < 100; var++) {
			PWM_CC1Val-= 10;
			timer_delay(10);
		}
		for (int var = 0; var < 100; var++) {
			PWM_CC2Val-= 10;
			timer_delay(10);
		}
		// delay 100 ms
		for (int var = 0; var < 100; var++) {
			PWM_CC1Val+= 10;
			timer_delay(100);
		}
		for (int var = 0; var < 100; var++) {
			PWM_CC2Val+= 10;
			timer_delay(100);
		}
		for (int var = 0; var < 100; var++) {
			PWM_CC1Val-= 10;
			timer_delay(100);
		}
		for (int var = 0; var < 100; var++) {
			PWM_CC2Val-= 10;
			timer_delay(100);
		}
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

	GPIO_ResetBits(GPIOF, GPIO_Pin_0);
	GPIO_ResetBits(GPIOF, GPIO_Pin_1);
}

// timer
	/**
	 * General purpose timer 3 interrupt initialization
	 * This clock is 2 times of that of APB1, while APB1 is 36M
	 * arr: Automatic reload value.
	 * psc: The frequency of the clock Prescaler
	 * The calculation formula is as follows:
	 * Tout= ((arr+1)*(psc+1))/Tclk,
	 * Among them:
	 * Tclk: The input clock frequency TIM3 (unit: Mhz).
	 * Tout: TIM3 overflow time (unit: US).
	 */
	void TIM3_Int_Init(uint16_t arr,uint32_t frequency)
	{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); /* Clock enable */

		/*TIM3 timer initialization*/
		TIM_TimeBaseStructure.TIM_Period = arr; /*Cycle value*/
		TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / frequency) - 1; /* Pre frequency value */
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /*Set the clock division: TDTS = Tck_tim*/
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* TIM up counting mode*/
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /*According to the specified parameter initialization of TIMx time base unit*/

		TIM_ARRPreloadConfig(TIM3, ENABLE);

		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); /*The specified TIM3 interrupt, interrupt is allowed to update*/
		TIM3_NVIC_Init();


		TIM_Cmd(TIM3,ENABLE);/*Enable TIMx*/
	}

	/**
	 * Interrupt priority set 3 NVIC
	 */
	void TIM3_NVIC_Init(void)
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; /*TIM3 interrupt*/
		NVIC_InitStructure.NVIC_IRQChannelPriority = 3; /*From the priority level 3*/
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /*The IRQ channel is enabled*/
		NVIC_Init(&NVIC_InitStructure);
	}

// interrupts
void
SysTick_Handler(void)
{
  timer_interruptHandler();
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET ) /*Check the TIM3 update interrupt occurs or not*/
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /*Remove TIMx update interrupt flag */
        if(PWM_Counter != PWM_PeriodVal){
        	PWM_Counter++;
        } else {
        	PWM_Counter = 0;
        	if((PWM_CC1Val != 0x00)){
        		GPIO_SetBits(GPIOF, GPIO_Pin_0);
        	}
			if((PWM_CC2Val != 0x00)){
				GPIO_SetBits(GPIOF, GPIO_Pin_1);
			}
        }

        if(PWM_Counter == PWM_CC1Val && (PWM_CC1Val != 0x00)){
        	GPIO_ResetBits(GPIOF, GPIO_Pin_0);
        }
        if(PWM_Counter == PWM_CC2Val && (PWM_CC2Val != 0x00)){
        	GPIO_ResetBits(GPIOF, GPIO_Pin_1);
        }
    }
}
