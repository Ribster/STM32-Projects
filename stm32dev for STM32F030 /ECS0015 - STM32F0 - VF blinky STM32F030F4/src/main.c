#include "stm32f0xx.h"
#include <stdio.h>

static void
Delay(volatile uint32_t nTime);

static void
TimingDelay_Decrement(void);

void
SysTick_Handler(void);
void
TIM3_Int_Init(uint16_t arr,uint16_t psc);
void
TIM3_NVIC_Init(void);
void
TIM3_IRQHandler(void);

/* ----- SysTick definitions ----------------------------------------------- */

#define SYSTICK_FREQUENCY_HZ       1000

#define BLINK_TICKS     SYSTICK_FREQUENCY_HZ/2


__IO uint16_t CCR1_Val = 40961;
__IO uint16_t CCR2_Val = 27309;
__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 20;
uint16_t capture = 0;
uint16_t PrescalerValue = 0;

int
main(void)
{
	// ---- Setup PLL for 48 MHz :) ----
		RCC_DeInit();
		RCC_PLLCmd(DISABLE);
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
		// Flash: 1 WaitState for 24MHz < SysCLK < 48 MHz
		FLASH_SetLatency(FLASH_Latency_1);
		FLASH_PrefetchBufferCmd(ENABLE);
		// Set ADC clock to sync PCLK/4->12MHz
		RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
		// and turn the PLL back on again
		RCC_PLLCmd(ENABLE);
		// set PLL as system clock source
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		// ---- End of Setup PLL for 48 MHz :) ----


		  RCC->AHBENR |= RCC_AHBENR_GPIOFEN; /* (1) */
		  GPIOF->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODER0|GPIO_MODER_MODER1)) \
		                | (GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0);

		  GPIOF->ODR &= ~(1<<0);
		  GPIOF->ODR &= ~(1<<1);


		  /* Compute the prescaler value */
		  PrescalerValue = (uint16_t) (SystemCoreClock / 6000) - 1;
		  TIM3_Int_Init(500, PrescalerValue);






	  /* Use SysTick as reference for the timer */
	  SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY_HZ);

	while(1){

/*		for(uint16_t i=0; i<20000; i++){
			CCR3_Val = i;
			Delay(1);
		}
		for(uint16_t i=20000; i>0; i--){
			CCR3_Val = i;
			Delay(1);
		}*/

		for(uint32_t ii=1; ii<50; ii++){

			TIM3->ARR = ii;
			GPIOF->ODR ^= (1<<1);
			Delay(100);
		}
		for(uint32_t ii=500; ii>2; ii--){

			TIM3->ARR = ii;
			GPIOF->ODR ^= (1<<1);
			Delay(10);
		}
		for(uint32_t ii=5; ii>2; ii--){

			TIM3->ARR = ii;
			GPIOF->ODR ^= (1<<1);
			Delay(1000);
		}

	}

}



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
void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); /* Clock enable */

    /*TIM3 timer initialization*/
    TIM_TimeBaseStructure.TIM_Period = arr; /*Cycle value*/
    TIM_TimeBaseStructure.TIM_Prescaler = psc; /* Pre frequency value */
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




volatile uint32_t uwTimingDelay;

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in SysTick ticks.
 * @retval None
 */
void
Delay(__IO uint32_t nTime)
{
  uwTimingDelay = nTime;

  while (uwTimingDelay != 0)
    ;
}

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void
TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
    {
      uwTimingDelay--;
    }
}

// ----------------------------------------------------------------------------

/**
 * @brief  This function is the SysTick Handler.
 * @param  None
 * @retval None
 */
void
SysTick_Handler(void)
{
  TimingDelay_Decrement();


}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET ) /*Check the TIM3 update interrupt occurs or not*/
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /*Remove TIMx update interrupt flag */
        GPIOF->ODR ^= (1<<0);
    }

}
