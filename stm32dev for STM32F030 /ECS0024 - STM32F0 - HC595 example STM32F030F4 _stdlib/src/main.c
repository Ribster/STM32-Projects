#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>

#define PROJECTNAME "ECS0024 _stdlib"
#define VERSION "V0.0.1"
#define TESTED "2015/06/09"

/*
SWD:
PA14: SWCLK
PA15: SWDIO

Pinout:
PF0: LED ORANGE
PF1: LED GREEN
PA2: USART1_TX
PA3: USART1_RX
PA0: HC595_CLK
PA1: HC595_DATA
PB1: HC595_LATCH
*/

// globals
	// systick timer
		volatile uint32_t timer_delayCounter;
	// hc595
		uint8_t _hc595_buffer[2];

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

	// hc595
	void
	hc595_init(void);
	inline void
	hc595_setCLK(uint8_t val);
	inline void
	hc595_setDATA(uint8_t val);
	inline void
	hc595_setLATCH(uint8_t val);
	void
	hc595_sendData(void);

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

	// hc595 init
	hc595_init();

		int64_t maincounter = -200;

/*		seg7led_setPoint(0, 0);
		seg7led_setPoint(1, 1);
		seg7led_setPoint(2, 1);
		seg7led_setPoint(3, 0);*/

		while(1){
			// heartbeat
			timer_delay(100);
			GPIO_WriteBit(GPIOF, GPIO_Pin_1, !GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_1));


			uart_sendString("-------------heartbeat ");
			uart_sendDecSigned(maincounter);

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
	PA0: HC595_CLK
	PA1: HC595_DATA
	PB1: HC595_LATCH
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
	PA0: HC595_CLK
	PA1: HC595_DATA
	PB1: HC595_LATCH
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


// hc595
void
hc595_init(void){
	/*
	SWD:
	PA14: SWCLK
	PA15: SWDIO

	Pinout:
	PF0: LED ORANGE
	PF1: LED GREEN
	PA2: USART1_TX
	PA3: USART1_RX
	PA0: HC595_CLK
	PA1: HC595_DATA
	PB1: HC595_LATCH
	*/
	GPIO_InitTypeDef tmp_gpio;
	tmp_gpio.GPIO_Mode = GPIO_Mode_OUT;
	tmp_gpio.GPIO_OType = GPIO_OType_PP;
	tmp_gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	tmp_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	tmp_gpio.GPIO_Speed = GPIO_Speed_Level_3;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &tmp_gpio);

	tmp_gpio.GPIO_Pin = GPIO_Pin_1;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_Init(GPIOB, &tmp_gpio);
}
inline void
hc595_setCLK(uint8_t val){
	if(val){
		GPIOA->BSRR = GPIO_Pin_0;
	} else {
		GPIOA->BRR = GPIO_Pin_0;
	}
}
inline void
hc595_setDATA(uint8_t val){
	if(val){
		GPIOA->ODR |= GPIO_Pin_1;
	} else {
		GPIOA->ODR &= ~GPIO_Pin_1;
	}
}
inline void
hc595_setLATCH(uint8_t val){
	if(val){
		GPIOB->BSRR = GPIO_Pin_1;
	} else {
		GPIOB->BRR = GPIO_Pin_1;
	}
}
void
hc595_sendData(void){
	// init states
		hc595_setLATCH(0x01);
		hc595_setCLK(0x00);
		hc595_setDATA(0x00);
	// loop over buffer
		for (int buffloop = 0; buffloop < sizeof(_hc595_buffer); ++buffloop) {
			for (int i = 8; i > 0; i--) {
				// set data
				hc595_setDATA( (_hc595_buffer[1-buffloop]&(1<<(i-1))) == ((1<<(i-1))) );
				// clock in
				hc595_setCLK(0x00);
				hc595_setCLK(0x01);
			}
		}
	// latch
		hc595_setLATCH(0x00);
		hc595_setLATCH(0x01);
	// close states
		hc595_setCLK(0x00);
		hc595_setDATA(0x00);
}

// interrupts
void
SysTick_Handler(void)
{
  timer_interruptHandler();
  // redefined systick in stm32f0xx_i2c_cpal_conf_template to integrate in this systick

}
