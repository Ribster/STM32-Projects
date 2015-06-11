#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>

#define PROJECTNAME "ECS0019 _stdlib"
#define VERSION "V0.0.1"
#define TESTED "2015/06/04"

/*
SWD:
PA14: SWCLK
PA15: SWDIO

Pinout:
PF0: LED ORANGE
PF1: LED GREEN
PA2: USART1_TX
PA3: USART1_RX
PA10: I2C1_SDA
PA9: I2C1_SCL
*/

// globals
	// systick timer
		volatile uint32_t timer_delayCounter;
	// i2c

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

	// i2c
	uint8_t
	i2c_init(void);
	inline void
	i2c_readReg(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* buff, uint8_t count, uint32_t timeout);
	inline void
	i2c_writeReg(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* buff, uint8_t count, uint32_t timeout);
	inline void
	i2c_testSlave(I2C_TypeDef* I2Cx, uint8_t address, uint32_t timeout);

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

	// set i2c
		if(!i2c_init()){
			uart_sendString("I2C ready \r\n");
		} else {
			uart_sendString("I2C init failed \r\n");
		}

		uint64_t maincounter;

	while(1){
		// heartbeat
		timer_delay(1000);
		GPIO_WriteBit(GPIOF, GPIO_Pin_1, !GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_1));


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
	PA10: I2C1_SDA
	PA9: I2C1_SCL
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
	PA10: I2C1_SDA
	PA9: I2C1_SCL
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

// i2c
uint8_t
i2c_init(void){
	/*
	SWD:
	PA14: SWCLK
	PA15: SWDIO

	Pinout:
	PF0: LED ORANGE
	PF1: LED GREEN
	PA2: USART1_TX
	PA3: USART1_RX
	PA10: I2C1_SDA
	PA9: I2C1_SCL
	*/


	GPIO_InitTypeDef tmp_gpio;
	tmp_gpio.GPIO_Mode = GPIO_Mode_AF;
	tmp_gpio.GPIO_OType = GPIO_OType_OD;
	tmp_gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	tmp_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	tmp_gpio.GPIO_Speed = GPIO_Speed_Level_3;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &tmp_gpio);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_4);

	/* Configure the I2C1 Clock Source */
	RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	I2C_InitTypeDef tmp_i2c;
	I2C_DeInit(I2C1);
	// configure I2C1
	tmp_i2c.I2C_Mode = I2C_Mode_I2C;			// I2C mode
	tmp_i2c.I2C_OwnAddress1 = 0x00;			// own address, not relevant in master mode
	tmp_i2c.I2C_Ack = I2C_Ack_Disable;		// disable acknowledge when reading (can be changed later on)
	tmp_i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
	// page 623 of RM0091
	/*uint32_t timingConfig = 0x50000000 | 	// prescaler 0x5
							0x9 |			// SCLL 0x9
							0x300 |			// SCLH 0x3
							0x30000 |			// SDADEL 0x3
							0x300000 ;			// SCLDEL 0x3
	*/
	tmp_i2c.I2C_Timing = 0x50330309;	// 400kHz at 48Mhz
	//tmp_i2c.I2C_Timing = 0x1045061D;	//

	tmp_i2c.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	tmp_i2c.I2C_DigitalFilter = 0;
	I2C_Init(I2C1, &tmp_i2c);				// init I2C1
	I2C_Cmd(I2C1, ENABLE);



	NVIC_InitTypeDef tmp_nvic;
	/* Reconfigure and enable I2C1 error interrupt to have the higher priority */
	tmp_nvic.NVIC_IRQChannel = I2C1_IRQn;
	tmp_nvic.NVIC_IRQChannelPriority = 0;
	tmp_nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&tmp_nvic);


	while (I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)); // Wait until I2C free


	return (0);
}

inline void
i2c_readReg(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* buff, uint8_t count, uint32_t timeout){
	uint32_t I2C_TimeOut;
	// empty the buffer
	memset(buff, 0, count);

	/* Test on BUSY Flag */
	I2C_TimeOut = timeout;
	while((I2C_GetFlagStatus(I2Cx, I2C_ISR_BUSY) != RESET) && (I2C_TimeOut-- != 0));
	if((I2C_TimeOut == 0)){
		uart_sendString("1...timeout error...");
		return ;
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(I2Cx, address, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	/* Wait until TXIS flag is set */
	I2C_TimeOut = timeout;
	while((I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET) && (I2C_TimeOut-- != 0))
	if((I2C_TimeOut == 0)){
		uart_sendString("2...timeout error...");
		return ;
	}

	/* Send Register address */
	I2C_SendData(I2Cx, (uint8_t)reg);

	/* Wait until TC flag is set */
	I2C_TimeOut = timeout;
	while((I2C_GetFlagStatus(I2Cx, I2C_ISR_TC) == RESET) && (I2C_TimeOut-- != 0))
	if((I2C_TimeOut == 0)){
		uart_sendString("3...timeout error...");
		return ;
	}


	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(I2Cx, address, count, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

	for (int counter = 0; counter < count; ++counter) {
		/* Wait until RXNE flag is set */
		I2C_TimeOut = timeout;
		while((I2C_GetFlagStatus(I2Cx, I2C_ISR_RXNE) == RESET) && (I2C_TimeOut-- != 0))
		if((I2C_TimeOut == 0)){
			uart_sendString("4...timeout error...");
			return ;
		}

		/* Read data from RXDR */
		buff[counter] = I2C_ReceiveData(I2Cx);
	}


	/* Wait until STOPF flag is set */
	I2C_TimeOut = timeout;
	while((I2C_GetFlagStatus(I2Cx, I2C_ISR_STOPF) == RESET) && (I2C_TimeOut-- != 0))
	if((I2C_TimeOut == 0)){
		uart_sendString("5...timeout error...");
		return ;
	}

	/* Clear STOPF flag */
	I2C_ClearFlag(I2Cx, I2C_ICR_STOPCF);
}
inline void
i2c_writeReg(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* buff, uint8_t count, uint32_t timeout){
	uint32_t I2C_TimeOut;
	/* Test on BUSY Flag */
	I2C_TimeOut = timeout;
	while((I2C_GetFlagStatus(I2Cx, I2C_ISR_BUSY) != RESET) && (I2C_TimeOut-- != 0))
	if((I2C_TimeOut == 0)){
		uart_sendString("1...timeout error...");
		return ;
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(I2Cx, address, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

	/* Wait until TXIS flag is set */
	I2C_TimeOut = timeout;
	while((I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET) && (I2C_TimeOut-- != 0))
	if((I2C_TimeOut == 0)){
		uart_sendString("2...timeout error...");
		return ;
	}

	/* Send Register address */
	I2C_SendData(I2Cx, reg);

	/* Wait until TCR flag is set */
	I2C_TimeOut = timeout;
	while((I2C_GetFlagStatus(I2Cx, I2C_ISR_TCR) == RESET) && (I2C_TimeOut-- != 0))
	if((I2C_TimeOut == 0)){
		uart_sendString("3...timeout error...");
		return ;
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(I2Cx, address, count, I2C_AutoEnd_Mode, I2C_No_StartStop);


		for (int counter = 0; counter < count; ++counter) {
			/* Wait until TXIS flag is set */
			I2C_TimeOut = timeout;
			while((I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET) && (I2C_TimeOut-- != 0))
			if((I2C_TimeOut == 0)){
				uart_sendString("4...timeout error...");
				return ;
			}

			/* Write data to TXDR */
			I2C_SendData(I2Cx, buff[counter]);
		}


	/* Wait until STOPF flag is set */
	I2C_TimeOut = timeout;
	while((I2C_GetFlagStatus(I2Cx, I2C_ISR_STOPF) == RESET) && (I2C_TimeOut-- != 0))
	if((I2C_TimeOut == 0)){
		uart_sendString("5...timeout error...");
		return ;
	}

	/* Clear STOPF flag */
	I2C_ClearFlag(I2Cx, I2C_ICR_STOPCF);
}
inline void
i2c_testSlave(I2C_TypeDef* I2Cx, uint8_t address, uint32_t timeout){
	uint32_t I2C_TimeOut = timeout;

	 /*Configure slave address, nbytes, reload, end mode and start or stop generation*/
	I2C_TransferHandling(I2Cx, address, 0, I2C_AutoEnd_Mode, I2C_No_StartStop);

	 /*Clear NACKF and STOPF*/
	I2C_ClearFlag(I2Cx, I2C_ICR_NACKCF | I2C_ICR_STOPCF);

	 /*Generate start*/
	I2C_GenerateSTART(I2Cx, ENABLE);

	/* Wait until timeout elapsed*/
	while ((I2C_GetFlagStatus(I2Cx, I2C_ISR_STOPF) == RESET) && (I2C_TimeOut-- != 0));

	 /*Check if Temp sensor is ready for use*/
	if ((I2C_GetFlagStatus(I2Cx, I2C_ISR_NACKF) != RESET) || (I2C_TimeOut == 0))
	{
		 /*Clear NACKF and STOPF*/
		I2C_ClearFlag(I2Cx, I2C_ICR_NACKCF | I2C_ICR_STOPCF);

		uart_sendString("Sensor error...");
	}
	else
	{
		 /*Clear STOPF*/
		I2C_ClearFlag(I2Cx, I2C_ICR_STOPCF);

		uart_sendString("Sensor working...");
	}
}

// interrupts
void
SysTick_Handler(void)
{
  timer_interruptHandler();
  // redefined systick in stm32f0xx_i2c_cpal_conf_template to integrate in this systick

}
void I2C1_IRQHandler(void)
{
  /* Check on I2C1 SMBALERT flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_ALERT))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_ALERT);
  }
  /* Check on I2C1 Time out flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_TIMEOUT);
  }
  /* Check on I2C1 Arbitration Lost flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_ARLO))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_ARLO);
  }
  /* Check on I2C1 PEC error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_PECERR))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_PECERR);
  }
  /* Check on I2C1 Overrun/Underrun error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_OVR))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_OVR);
  }
  /* Check on I2C1 Acknowledge failure error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_NACKF))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_NACKF);
  }
  /* Check on I2C1 Bus error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_BERR))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
  }
}
