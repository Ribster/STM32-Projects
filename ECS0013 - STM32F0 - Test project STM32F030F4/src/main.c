#include "stm32f0xx.h"
#include <stdio.h>

static void
Delay(__IO uint32_t nTime);

static void
TimingDelay_Decrement(void);

void
SysTick_Handler(void);

/* ----- SysTick definitions ----------------------------------------------- */

#define SYSTICK_FREQUENCY_HZ       1000

/* ----- LED definitions --------------------------------------------------- */

/* STM32F0DISCOVERY definitions (the GREEN LED) */
/* Adjust them for your own board. */

#define BLINK_PORT      GPIOF
#define BLINK_PIN       0
#define BLINK_RCC_BIT   RCC_AHBPeriph_GPIOF

#define BLINK_TICKS     SYSTICK_FREQUENCY_HZ/2


int
main(void)
{
	// ---- Setup PLL for 48 MHz :) ----
		RCC_DeInit();
		RCC_PLLCmd(DISABLE);
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
		// Flash: 1 WaitState for 24MHz < SysCLK < 48 MHz
		FLASH_SetLatency(FLASH_Latency_0);
		FLASH_PrefetchBufferCmd(ENABLE);
		// Set ADC clock to sync PCLK/4->12MHz
		RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
		// and turn the PLL back on again
		RCC_PLLCmd(ENABLE);
		// set PLL as system clock source
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		// ---- End of Setup PLL for 48 MHz :) ----




	  /* Use SysTick as reference for the timer */
	  SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY_HZ);

	  /* GPIO Periph clock enable */
	  RCC_AHBPeriphClockCmd(BLINK_RCC_BIT, ENABLE);

	  GPIO_InitTypeDef GPIO_InitStructure;

	  /* Configure pin in output push/pull mode */
	  GPIO_InitStructure.GPIO_Pin = (1 << BLINK_PIN);
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(BLINK_PORT, &GPIO_InitStructure);

	  int seconds = 0;

	while(1){
	      /* Assume the LED is active low */

	      /* Turn on led by setting the pin low */
	      GPIO_ResetBits(BLINK_PORT, (1 << BLINK_PIN));

	      Delay(BLINK_TICKS);

	      /* Turn off led by setting the pin high */
	      GPIO_SetBits(BLINK_PORT, (1 << BLINK_PIN));

	      Delay(BLINK_TICKS);

	      ++seconds;


	}

}

static __IO uint32_t uwTimingDelay;

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

