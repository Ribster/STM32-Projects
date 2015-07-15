#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>

#define PROJECTNAME "ECS0022 _stdlib"
#define VERSION "V0.0.1"
#define TESTED "2015/06/05"

/*
SWD:
PA14: SWCLK
PA15: SWDIO

Pinout:
PF0: LED ORANGE
PF1: LED GREEN
PA2: USART1_TX
PA3: USART1_RX
PA0: ADC_IN0
PA1: ADC_IN1
PB1: GPIO_EXTI1
*/

// globals
	// systick timer
		volatile uint32_t timer_delayCounter;
	// adc
		#define SAMPLES 2
		volatile uint16_t RegularConvData[SAMPLES];

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

	// uart
	void
	uart_init(void);
	void
	uart_sendString(char* str);
	char*
	uart_getDec(uint64_t dec);
	void
	uart_sendDec(uint64_t dec);
	char*
	uart_getDecSigned(int64_t dec);
	void
	uart_sendDecSigned(int64_t dec);

	// adc
	void
	adc_init(void);

	// exti
	void
	exti_init(void);

int
main(void)
{
	// set HSI clock
	hsi_setPLLClock(RCC_PLLMul_12);

	// set ADC
	adc_init();

	// set systick clock
	timer_initMilliseconds();

	// set GPIO
	gpio_init();

	// set uart
	uart_init();

	// set exti
	exti_init();



		uint64_t maincounter;

	while(1){
		// heartbeat
		timer_delay(100);
		GPIO_WriteBit(GPIOF, GPIO_Pin_1, !GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_1));

		  uart_sendString("x: ");
		  uart_sendDec(RegularConvData[0]);
		  uart_sendString("\t\t");

		  uart_sendString("y: ");
		  uart_sendDec(RegularConvData[1]);
		  uart_sendString("\r\n");

		uart_sendString("-------------heartbeat ");
		uart_sendDec(++maincounter);

		uart_sendString("--------------\r\n");
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
	/*
	SWD:
	PA14: SWCLK
	PA15: SWDIO

	Pinout:
	PF0: LED ORANGE
	PF1: LED GREEN
	PA2: USART1_TX
	PA3: USART1_RX
	PA0: ADC_IN0
	PA1: ADC_IN1
	PB1: GPIO_EXTI1
	*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	GPIO_InitTypeDef tmp;
	GPIO_StructInit(&tmp);
	tmp.GPIO_Mode = GPIO_Mode_OUT;
	tmp.GPIO_OType = GPIO_OType_PP;
	tmp.GPIO_PuPd = GPIO_PuPd_UP;
	tmp.GPIO_Speed = GPIO_Speed_Level_3;
	tmp.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOF, &tmp);

	GPIO_ResetBits(GPIOF, GPIO_Pin_0);
	GPIO_ResetBits(GPIOF, GPIO_Pin_1);
}

// uart
void
uart_init(void){
	/*
	SWD:
	PA14: SWCLK
	PA15: SWDIO

	Pinout:
	PF0: LED ORANGE
	PF1: LED GREEN
	PA2: USART1_TX
	PA3: USART1_RX
	PA0: ADC_IN0
	PA1: ADC_IN1
	PB1: GPIO_EXTI1
	*/
	GPIO_InitTypeDef tmp_gpio;
	tmp_gpio.GPIO_Mode = GPIO_Mode_AF;
	tmp_gpio.GPIO_OType = GPIO_OType_PP;
	tmp_gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	tmp_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	tmp_gpio.GPIO_Speed = GPIO_Speed_Level_3;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &tmp_gpio);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);


	USART_InitTypeDef tmp_uart;
	tmp_uart.USART_BaudRate = 230400;
	tmp_uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	tmp_uart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	tmp_uart.USART_Parity = USART_Parity_No;
	tmp_uart.USART_StopBits = USART_StopBits_1;
	tmp_uart.USART_WordLength = USART_WordLength_8b;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_Init(USART1, &tmp_uart);

	/* USART enable */
	USART_Cmd(USART1, ENABLE);

	uart_sendString("Testing uart!!\r\n");

	// configured for rx && tx, with no interrupts.
	// this is done to make terminal debugging possible
}
void
uart_sendString(char* str){
	GPIO_SetBits(GPIOF, GPIO_Pin_0);
	while(*str != '\0'){
		USART_SendData(USART1, *str);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		str++;
	}
	GPIO_ResetBits(GPIOF, GPIO_Pin_0);
}
// convert base 2 to base 10 ASCII
char*
uart_getDec(uint64_t dec){
	  static char str[16];
	  char *s = str + sizeof(str);

	  *--s = 0;

	  do
	  {
	    *--s = '0' + (char)(dec % 10);
	    dec /= 10;
	  }
	  while(dec);

	  return(s);
}
void
uart_sendDec(uint64_t dec){
	  uart_sendString(uart_getDec(dec));
}
char*
uart_getDecSigned(int64_t dec){
	  static char str[16];
	  char *s = str + sizeof(str);
	  // convert 2's complement to unsigned
	  uint64_t b;
	  if(dec <= -1){
		  b = (~dec)+1;
	  } else {
		  b = dec;
	  }


	  *--s = 0;

	  do
	  {
	    *--s = '0' + (char)(b % 10);
	    b /= 10;
	  }
	  while(b);


	  if(dec <= -1){
		  // negative
		  *--s = '-';
	  }

	  return(s);
}
void
uart_sendDecSigned(int64_t dec){
	uart_sendString(uart_getDecSigned(dec));
}

// adc
void
adc_init(void){
	/*
	SWD:
	PA14: SWCLK
	PA15: SWDIO

	Pinout:
	PF0: LED ORANGE
	PF1: LED GREEN
	PA2: USART1_TX
	PA3: USART1_RX
	PA0: ADC_IN0
	PA1: ADC_IN1
	PB1: GPIO_EXTI1
	*/
	  TIM_TimeBaseInitTypeDef temp_tim_tb;
	  TIM_OCInitTypeDef tmp_tim_oc;
	  ADC_InitTypeDef tmp_adc;
	  DMA_InitTypeDef tmp_dma;
	  GPIO_InitTypeDef tmp_gpio;

	  /* GPIOA Periph clock enable */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	  /* Configure ADC Channels as analog input - might conflict */
	  tmp_gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	  tmp_gpio.GPIO_Mode = GPIO_Mode_AN;
	  tmp_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOA, &tmp_gpio);


	  /* ADC1 DeInit */
	  ADC_DeInit(ADC1);

	  /* ADC1 Periph clock enable */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_TIM1, ENABLE);

	  /* DMA1 clock enable */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	  /* TIM2 Configuration */
	  TIM_DeInit(TIM1);

	  /* Time base configuration */
	  TIM_TimeBaseStructInit(&temp_tim_tb);
	  temp_tim_tb.TIM_Prescaler = (SystemCoreClock / 1000000) - 1; // 1 MHz, from 48 MHz
	  temp_tim_tb.TIM_Period = 1000 - 1; // 1 KHz
	  temp_tim_tb.TIM_CounterMode = TIM_CounterMode_Up;
	  temp_tim_tb.TIM_ClockDivision = 0;
	  temp_tim_tb.TIM_RepetitionCounter = 0;
	  TIM_TimeBaseInit(TIM1, &temp_tim_tb);

	  /* Output Compare PWM Mode configuration */
	  TIM_OCStructInit(&tmp_tim_oc);
	  tmp_tim_oc.TIM_OCMode = TIM_OCMode_PWM1; /* low edge by default */
	  tmp_tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	  tmp_tim_oc.TIM_Pulse = 1;
	  TIM_OC4Init(TIM1, &tmp_tim_oc);

	  /* TIM1 enable counter */
	  TIM_Cmd(TIM1, ENABLE);

	  /* Main Output Enable */
	  TIM_CtrlPWMOutputs(TIM1, ENABLE);

	  /* DMA1 Channel1 Config */
	  DMA_DeInit(DMA1_Channel1);
	  tmp_dma.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	  tmp_dma.DMA_MemoryBaseAddr = (uint32_t)&RegularConvData[0];
	  tmp_dma.DMA_DIR = DMA_DIR_PeripheralSRC;
	  tmp_dma.DMA_BufferSize = SAMPLES;
	  tmp_dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  tmp_dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  tmp_dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	  tmp_dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	  tmp_dma.DMA_Mode = DMA_Mode_Circular;
	  tmp_dma.DMA_Priority = DMA_Priority_High;
	  tmp_dma.DMA_M2M = DMA_M2M_Disable;
	  DMA_Init(DMA1_Channel1, &tmp_dma);

	  /* DMA1 Channel1 enable */
	  DMA_Cmd(DMA1_Channel1, ENABLE);

	  /* ADC DMA request in circular mode */
	  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

	  /* Enable ADC_DMA */
	  ADC_DMACmd(ADC1, ENABLE);

	  /* Initialize ADC structure */
	  ADC_StructInit(&tmp_adc);

	  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
	  tmp_adc.ADC_Resolution = ADC_Resolution_12b;
	  tmp_adc.ADC_ContinuousConvMode = DISABLE;
	  tmp_adc.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	  tmp_adc.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC4;
	  tmp_adc.ADC_DataAlign = ADC_DataAlign_Right;
	  tmp_adc.ADC_ScanDirection = ADC_ScanDirection_Upward;
	  ADC_Init(ADC1, &tmp_adc);

	  /* Convert the  ADC_Channnel_0  with 7.5 Cycles as sampling time */
	  ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_7_5Cycles);

	  /* Convert the  ADC_Channnel_1  with 7.5 Cycles as sampling time */
	  ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_7_5Cycles);

	  /* ADC Calibration */
	  ADC_GetCalibrationFactor(ADC1);

	  /* Enable ADC1 */
	  ADC_Cmd(ADC1, ENABLE);

	  /* Wait the ADCEN flag */
	  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));

	  /* ADC1 regular Software Start Conv */
	  ADC_StartOfConversion(ADC1);

}

// exti
void
exti_init(void){
	/*
	SWD:
	PA14: SWCLK
	PA15: SWDIO

	Pinout:
	PF0: LED ORANGE
	PF1: LED GREEN
	PA2: USART1_TX
	PA3: USART1_RX
	PA0: ADC_IN0
	PA1: ADC_IN1
	PB1: GPIO_EXTI1
	*/
	  /* Enable GPIOA clock */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	  GPIO_InitTypeDef tmp_gpio;

	  /* Configure PA0 pin as input floating */
	  tmp_gpio.GPIO_Pin = GPIO_Pin_1;
	  tmp_gpio.GPIO_Mode = GPIO_Mode_IN;
	  tmp_gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
	  GPIO_Init(GPIOB, &tmp_gpio);

	  /* Enable SYSCFG clock */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	  /* Connect EXTI0 Line to PA0 pin */
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);

	  EXTI_InitTypeDef tmp_exti;
	  /* Configure EXTI0 line */
	  tmp_exti.EXTI_Line = EXTI_Line1;
	  tmp_exti.EXTI_Mode = EXTI_Mode_Interrupt;
	  tmp_exti.EXTI_Trigger = EXTI_Trigger_Falling;
	  tmp_exti.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&tmp_exti);

	  NVIC_InitTypeDef tmp_nvic;
	  /* Enable and set EXTI0 Interrupt */
	  tmp_nvic.NVIC_IRQChannel = EXTI0_1_IRQn;
	  tmp_nvic.NVIC_IRQChannelPriority = 0x01;
	  tmp_nvic.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&tmp_nvic);
}

// interrupts
void
SysTick_Handler(void)
{
  timer_interruptHandler();
  // redefined systick in stm32f0xx_i2c_cpal_conf_template to integrate in this systick

}
void EXTI0_1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    /* Toggle LED2 */
    uart_sendString("pushbutton pressed!!\r\n");

    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}
