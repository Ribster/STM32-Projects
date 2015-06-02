#include "stm32f0xx.h"
#include <stdio.h>

#define PROJECTNAME "ECS0018 _stdlib"
#define VERSION "V0.0.1"
#define TESTED "2015/06/02"

/*
SWD:
PA14: SWCLK
PA15: SWDIO

Pinout:
PF0: LED ORANGE
PF1: LED GREEN
PA2: USART1_TX
PA3: USART1_RX
*/

// globals
	// systick timer
	volatile uint32_t timer_delayCounter;
	// uart
	volatile char uart_receiveBuffer[1000];
	volatile uint8_t uart_counter = 0;


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

	// set uart
	uart_init();

	while(1){
		timer_delay(1000);
		GPIO_WriteBit(GPIOF, GPIO_Pin_1, !GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_1));
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
	*/
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

	/* NVIC configuration */
	/* Enable the USARTx Interrupt */
	NVIC_InitTypeDef tmp_nvic;
	tmp_nvic.NVIC_IRQChannel = USART1_IRQn;
	tmp_nvic.NVIC_IRQChannelPriority = 0;
	tmp_nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&tmp_nvic);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* USART enable */
	USART_Cmd(USART1, ENABLE);

	uart_sendString("Testing uart 1!! \r\n");
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

// interrupts
void
SysTick_Handler(void)
{
  timer_interruptHandler();
}

void USART1_IRQHandler(void)
{
  /* USART in mode Receiver --------------------------------------------------*/
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
	  char tmp_buffer = USART_ReceiveData(USART1);
	  if(tmp_buffer != '\r' && uart_counter < (sizeof(uart_receiveBuffer)-1)){
		  uart_receiveBuffer[uart_counter++] = tmp_buffer;
	  } else {
		  if(uart_counter < (sizeof(uart_receiveBuffer)-1)){
			  uart_receiveBuffer[uart_counter] = '\0';
		  } else {
			  uart_receiveBuffer[sizeof(uart_receiveBuffer)-1] = '\0';
		  }

		  uart_counter = 0;
		  uart_sendString((char*)uart_receiveBuffer);
		  uart_sendString("\r\n");
	  }
  }
}
