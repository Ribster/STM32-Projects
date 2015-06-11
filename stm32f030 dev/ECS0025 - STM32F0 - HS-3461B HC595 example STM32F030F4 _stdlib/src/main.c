#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>

#define PROJECTNAME "ECS0025 _stdlib"
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
	// seg7led
		char _seg7led_buffer[4];
		char _seg7led_pointbuffer[4];
		uint8_t _seg7led_digitLoop = 0;

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

	// HS-3461B
	void
	seg7led_init(void);
	void
	seg7led_setSegmentValue(uint8_t val);
	void
	seg7led_setSegment(uint8_t val);
	void
	seg7led_setPoint(uint8_t segment, uint8_t val);
	void
	seg7led_setValue(int32_t val);

	// timer
	void
	TIM3_Int_Init(uint16_t arr,uint32_t frequency);
	void
	TIM3_NVIC_Init(void);

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

	// 7 seg init
	seg7led_init();

		int64_t maincounter = -200;

/*		seg7led_setPoint(0, 0);
		seg7led_setPoint(1, 1);
		seg7led_setPoint(2, 1);
		seg7led_setPoint(3, 0);*/

		while(1){
			// heartbeat
			timer_delay(100);
			GPIO_WriteBit(GPIOF, GPIO_Pin_1, !GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_1));

			if(maincounter <10000){
				seg7led_setValue(++maincounter);
			} else {
				maincounter = 0;
			}

			uart_sendString("-------------heartbeat ");
			uart_sendDecSigned(maincounter);

			uart_sendString("--------------\r\n");
		}

	while(1){
		// heartbeat
		timer_delay(1000);
		GPIO_WriteBit(GPIOF, GPIO_Pin_1, !GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_1));

		timer_delay(250);
		seg7led_setSegmentValue('0');
		seg7led_setSegment(1);
		hc595_sendData();
		uart_sendString("\r\n");

		timer_delay(250);
		seg7led_setSegmentValue('1');
		seg7led_setSegment(2);
		hc595_sendData();
		uart_sendString("\r\n");

		timer_delay(250);
		seg7led_setSegmentValue('2');
		seg7led_setSegment(3);
		hc595_sendData();
		uart_sendString("\r\n");

		timer_delay(250);
		seg7led_setSegmentValue('3');
		seg7led_setSegment(4);
		hc595_sendData();
		uart_sendString("\r\n");

		timer_delay(250);
		seg7led_setSegmentValue('4');
		seg7led_setSegment(1);
		hc595_sendData();
		uart_sendString("\r\n");

		timer_delay(250);
		seg7led_setSegmentValue('5');
		seg7led_setSegment(2);
		hc595_sendData();
		uart_sendString("\r\n");

		timer_delay(250);
		seg7led_setSegmentValue('6');
		seg7led_setSegment(3);
		hc595_sendData();
		uart_sendString("\r\n");

		timer_delay(250);
		seg7led_setSegmentValue('7');
		seg7led_setSegment(4);
		hc595_sendData();
		uart_sendString("\r\n");

		timer_delay(250);
		seg7led_setSegmentValue('8');
		seg7led_setSegment(1);
		hc595_sendData();
		uart_sendString("\r\n");

		timer_delay(250);
		seg7led_setSegmentValue('9');
		seg7led_setSegment(2);
		hc595_sendData();
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

// HS-3461B
void
seg7led_init(void){
	// init timer and interrupt
	TIM3_Int_Init(100, 50000);
	TIM3_NVIC_Init();
	memset(_seg7led_pointbuffer, 0, 4);

	// bitmapping of the HC595
		// buffer 0
			// bit 0: DIG3
			// bit 1: b
			// bit 2: DIG4
			// bit 3: g
			// bit 4: c
			// bit 5: dp
			// bit 6: d
			// bit 7: e

		// buffer 1
			// bit 0:
			// bit 1:
			// bit 2:
			// bit 3:
			// bit 4: DIG1
			// bit 5: a
			// bit 6: f
			// bit 7: DIG2
}
void
seg7led_setSegmentValue(uint8_t val){
	// bitmapping of the HC595
		// buffer 0
			// bit 0: DIG3
			// bit 1: b
			// bit 2: DIG4
			// bit 3: g
			// bit 4: c
			// bit 5: dp
			// bit 6: d
			// bit 7: e

		// buffer 1
			// bit 0:
			// bit 1:
			// bit 2:
			// bit 3:
			// bit 4: DIG1
			// bit 5: a
			// bit 6: f
			// bit 7: DIG2

	// clear bits
	_hc595_buffer[0] |= (0xDA);
	_hc595_buffer[1] |= (0x60);

/*
	_hc595_buffer[1] &= ~(1<<5);	// a
	_hc595_buffer[0] &= ~(1<<1);	// b
	_hc595_buffer[0] &= ~(1<<4);	// c
	_hc595_buffer[0] &= ~(1<<6);	// d
	_hc595_buffer[0] &= ~(1<<7);	// e
	_hc595_buffer[1] &= ~(1<<6);	// f
	_hc595_buffer[0] &= ~(1<<3);	// g
*/
	switch(val){
		case '0':
			// a, b, c, d, e, f
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[1] &= ~(1<<6);	// f
			break;
		case '1':
			// b, c
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			break;
		case '2':
			// a, b, d, e, g
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case '3':
			// a, b, c, d, g
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case '4':
			// b, c, f, g
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[1] &= ~(1<<6);	// f
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case '5':
			// a, c, d, f, g
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[1] &= ~(1<<6);	// f
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case '6':
			// a, c, d, e, f, g
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[1] &= ~(1<<6);	// f
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case '7':
			// a, b, c
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			break;
		case '8':
			// a, b, c, d, e, f, g
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[1] &= ~(1<<6);	// f
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case '9':
			// a, b, c, d, f, g
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[1] &= ~(1<<6);	// f
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case 'A':
			// a, b, c, e, f, g
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[1] &= ~(1<<6);	// f
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case 'B':
			// c, d, e, f, g
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[1] &= ~(1<<6);	// f
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case 'C':
			// a, d, e, f
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[1] &= ~(1<<6);	// f
			break;
		case 'D':
			// b, c, d, e, g
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case 'E':
			// a, d, e, f, g
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[1] &= ~(1<<6);	// f
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case 'F':
			// a, e, f, g
			_hc595_buffer[1] &= ~(1<<5);	// a
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[1] &= ~(1<<6);	// f
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case '-':
			// g
			_hc595_buffer[0] &= ~(1<<3);	// g
			break;
		case 'U':
			// b, c, d, e, f
			_hc595_buffer[0] &= ~(1<<1);	// b
			_hc595_buffer[0] &= ~(1<<4);	// c
			_hc595_buffer[0] &= ~(1<<6);	// d
			_hc595_buffer[0] &= ~(1<<7);	// e
			_hc595_buffer[1] &= ~(1<<6);	// f
			break;
	}
}
void
seg7led_setSegment(uint8_t val){
	// bitmapping of the HC595
		// buffer 0
			// bit 0: DIG3
			// bit 1: b
			// bit 2: DIG4
			// bit 3: g
			// bit 4: c
			// bit 5: dp
			// bit 6: d
			// bit 7: e

		// buffer 1
			// bit 0:
			// bit 1:
			// bit 2:
			// bit 3:
			// bit 4: DIG1
			// bit 5: a
			// bit 6: f
			// bit 7: DIG2

	_hc595_buffer[0] &= ~( 0x05 );	// clear DIG3 && DIG4
	_hc595_buffer[1] &= ~( 0x90 );	// clear DIG2 && DIG1
	switch(val){
		case 0:
			_hc595_buffer[1] |= ( 0x10 );	// set DIG 1
			break;
		case 1:
			_hc595_buffer[1] |= ( 0x80 );	// set DIG 2
			break;
		case 2:
			_hc595_buffer[0] |= ( 0x01 );	// set DIG3
			break;
		case 3:
			_hc595_buffer[0] |= ( 0x04 );	// set DIG4
			break;
	}
	if(val < 4){
		if(_seg7led_pointbuffer[val] == 0x00){
			_hc595_buffer[0] |= ( 0x20 );
		} else {
			_hc595_buffer[0] &= ~( 0x20 );
		}
	}

}

void
seg7led_setPoint(uint8_t segment, uint8_t val){
	if(segment<4){
		_seg7led_pointbuffer[segment] = val;
	}
}

void
seg7led_setValue(int32_t val){
	char* asciiptr = uart_getDecSigned(val);
	memset(_seg7led_buffer, 0, 4);
	if( (*(asciiptr+1)) == 0 ){
		memcpy(_seg7led_buffer+3, asciiptr, 1);
	} else if( (*(asciiptr+2)) == 0 ){
		memcpy(_seg7led_buffer+2, asciiptr, 2);
	} else if( (*(asciiptr+3)) == 0 ){
		memcpy(_seg7led_buffer+1, asciiptr, 3);
	} else if( (*(asciiptr+4)) == 0 ){
		memcpy(_seg7led_buffer, asciiptr, 4);
	}
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
  // redefined systick in stm32f0xx_i2c_cpal_conf_template to integrate in this systick

}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET ) /*Check the TIM3 update interrupt occurs or not*/
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /*Remove TIMx update interrupt flag */
		switch (_seg7led_digitLoop) {
			case 0:
				seg7led_setSegmentValue(_seg7led_buffer[_seg7led_digitLoop]);
				seg7led_setSegment(_seg7led_digitLoop);
				hc595_sendData();
				_seg7led_digitLoop++;
				break;
			case 1:
				seg7led_setSegmentValue(_seg7led_buffer[_seg7led_digitLoop]);
				seg7led_setSegment(_seg7led_digitLoop);
				hc595_sendData();
				_seg7led_digitLoop++;
				break;
			case 2:
				seg7led_setSegmentValue(_seg7led_buffer[_seg7led_digitLoop]);
				seg7led_setSegment(_seg7led_digitLoop);
				hc595_sendData();
				_seg7led_digitLoop++;
				break;
			case 3:
				seg7led_setSegmentValue(_seg7led_buffer[_seg7led_digitLoop]);
				seg7led_setSegment(_seg7led_digitLoop);
				hc595_sendData();
				_seg7led_digitLoop = 0;
				break;
		}
    }
}
