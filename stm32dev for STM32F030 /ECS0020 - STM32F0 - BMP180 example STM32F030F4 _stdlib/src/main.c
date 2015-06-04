#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>

#define PROJECTNAME "ECS0020 _stdlib"
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

	// bmp180
		#define BMP180_ADDR                     0xEE // BMP180 address
		#define BMP180_I2C						I2C1
		#define BMP180_TIMEOUT			 		0x3FFFF
		/* registers */
		#define BMP180_PROM_START_ADDR          0xAA // E2PROM calibration data start register
		#define BMP180_PROM_DATA_LEN            22   // E2PROM length
		#define BMP180_CHIP_ID_REG              0xD0 // Chip ID
		#define BMP180_VERSION_REG              0xD1 // Version
		#define BMP180_CTRL_MEAS_REG            0xF4 // Measurements control (OSS[7.6], SCO[5], CTL[4.0]
		#define BMP180_ADC_OUT_MSB_REG          0xF6 // ADC out MSB  [7:0]
		#define BMP180_ADC_OUT_LSB_REG          0xF7 // ADC out LSB  [7:0]
		#define BMP180_ADC_OUT_XLSB_REG         0xF8 // ADC out XLSB [7:3]
		#define BMP180_SOFT_RESET_REG           0xE0 // Soft reset control
		/* control values */
		#define BMP180_T_MEASURE                0x2E // temperature measurement
		#define BMP180_P0_MEASURE               0x34 // pressure measurement (OSS=0, 4.5ms)
		#define BMP180_P1_MEASURE               0x74 // pressure measurement (OSS=1, 7.5ms)
		#define BMP180_P2_MEASURE               0xB4 // pressure measurement (OSS=2, 13.5ms)
		#define BMP180_P3_MEASURE               0xF4 // pressure measurement (OSS=3, 25.5ms)
		/* Pressure calculation constants */
		#define BMP180_PARAM_MG                 3038
		#define BMP180_PARAM_MH                -7357
		#define BMP180_PARAM_MI                 3791
		/* Calibration parameters structure */
		typedef struct {
			int16_t AC1;
			int16_t AC2;
			int16_t AC3;
			uint16_t AC4;
			uint16_t AC5;
			uint16_t AC6;
			int16_t B1;
			int16_t B2;
			int16_t MB;
			int16_t MC;
			int16_t MD;
			int32_t B5;
		} BMP180_Calibration_TypeDef;
		/* Calibration parameters from E2PROM of BMP180 */
		BMP180_Calibration_TypeDef _bmp180_calibration;

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

	// bmp180
	void
	bmp180_init(void);
	void
	bmp180_testSensor(void);
	void
	bmp180_executeFunction(void);
	void
	bmp180_reset(void);
	void
	bmp180_writeReg(uint8_t reg, uint8_t val);
	uint8_t
	bmp180_readReg(uint8_t reg);
	void
	bmp180_readRegMulti(uint8_t reg, uint8_t* buff, uint8_t count);
	void
	bmp180_writeRegMulti(uint8_t reg, uint8_t* buff, uint8_t count);
	void
	bmp180_readCalibration(void);
	void
	bmp180_printCalibration(void);
	uint16_t
	bmp180_readUT(void);
	uint32_t
	bmp180_readPT(uint8_t oss);
	int16_t
	bmp180_calcRT(uint16_t ut);
	int32_t
	bmp180_calcRP(uint32_t up, uint8_t oss);
	int32_t bmp180_kpaToMmhg(int32_t pa);

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

		bmp180_init();

	while(1){
		// heartbeat
		timer_delay(1000);
		GPIO_WriteBit(GPIOF, GPIO_Pin_1, !GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_1));

		bmp180_executeFunction();

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
// bmp180
void
bmp180_init(void){
	uart_sendString("ChipID: ");
	uint8_t ChipID = bmp180_readReg(BMP180_CHIP_ID_REG);
	uart_sendDec(ChipID);
	uart_sendString("\r\n");

	uart_sendString("Version: ");
	uint8_t Version = bmp180_readReg(BMP180_VERSION_REG);
	uart_sendDec(Version);
	uart_sendString("\r\n");

	bmp180_readCalibration();
	bmp180_printCalibration();
}
void
bmp180_testSensor(void){
	i2c_testSlave(BMP180_I2C, BMP180_ADDR, BMP180_TIMEOUT);
}
void
bmp180_executeFunction(void){
	// read bmp and print
	uint32_t u_temp,u_pres;
	int32_t rt,rp;

	u_temp = bmp180_readUT();
	//u_temp = 27898;
	rt = bmp180_calcRT(u_temp);

	u_pres = bmp180_readPT(0);
	//u_pres = 23843;
	rp = bmp180_calcRP(u_pres,0);

	uart_sendString("Un compensated temperature: "); uart_sendDecSigned(u_temp); uart_sendString("\r\n");
	uart_sendString("Un compensated pressure: "); uart_sendDecSigned(u_pres); uart_sendString("\r\n");

	uart_sendString("Real temperature: ");
	uart_sendDecSigned(rt); uart_sendString(" -> ");
	uart_sendDecSigned(rt / 10); uart_sendString(".");
	uart_sendDecSigned(rt % 10); uart_sendString("C\r\n");

	uart_sendString("Real pressure: ");
	uart_sendDecSigned(rp); uart_sendString(" -> ");
	uart_sendDecSigned(rp / 1000); uart_sendString(".");
	uart_sendDecSigned(rp % 1000); uart_sendString("kPa (");
	uart_sendDecSigned(bmp180_kpaToMmhg(rp)); uart_sendString("mmHg)\r\n");
}
void
bmp180_reset(void){
	bmp180_writeReg(BMP180_SOFT_RESET_REG, 0xB6);	// software reset
}
void
bmp180_writeReg(uint8_t reg, uint8_t val){
	i2c_writeReg(BMP180_I2C, BMP180_ADDR, reg, &val, 1, BMP180_TIMEOUT);
}
uint8_t
bmp180_readReg(uint8_t reg){
	uint8_t value;
	i2c_readReg(BMP180_I2C, BMP180_ADDR, reg, &value, 1, BMP180_TIMEOUT);
	return value;
}

void
bmp180_readRegMulti(uint8_t reg, uint8_t* buff, uint8_t count){
	i2c_readReg(BMP180_I2C, BMP180_ADDR, reg, buff, count, BMP180_TIMEOUT);
}
void
bmp180_writeRegMulti(uint8_t reg, uint8_t* buff, uint8_t count){
	i2c_writeReg(BMP180_I2C, BMP180_ADDR, reg, buff, count, BMP180_TIMEOUT);
}

void
bmp180_readCalibration(void){
	uint8_t buffer[BMP180_PROM_DATA_LEN];
	i2c_readReg(BMP180_I2C, BMP180_ADDR, BMP180_PROM_START_ADDR, buffer, sizeof(buffer), BMP180_TIMEOUT);
	_bmp180_calibration.AC1 = (buffer[0]  << 8) | buffer[1];
	_bmp180_calibration.AC2 = (buffer[2]  << 8) | buffer[3];
	_bmp180_calibration.AC3 = (buffer[4]  << 8) | buffer[5];
	_bmp180_calibration.AC4 = (buffer[6]  << 8) | buffer[7];
	_bmp180_calibration.AC5 = (buffer[8]  << 8) | buffer[9];
	_bmp180_calibration.AC6 = (buffer[10] << 8) | buffer[11];
	_bmp180_calibration.B1  = (buffer[12] << 8) | buffer[13];
	_bmp180_calibration.B2  = (buffer[14] << 8) | buffer[15];
	_bmp180_calibration.MB  = (buffer[16] << 8) | buffer[17];
	_bmp180_calibration.MC  = (buffer[18] << 8) | buffer[19];
	_bmp180_calibration.MD  = (buffer[20] << 8) | buffer[21];
}
void
bmp180_printCalibration(void){
/*  AC1:    9182
    AC2:    -1329
    AC3:    -14514
    AC4:    34261
    AC5:    25396
    AC6:    16294
    B1:     6515
    B2:     58
    MB:     -32768
    MC:     -11786
    MD:     2579
*/
	uart_sendString("E2PROM Calibration values:\r\n");
	uart_sendString("\tAC1:\t"); uart_sendDecSigned(_bmp180_calibration.AC1); uart_sendString("\r\n");
	uart_sendString("\tAC2:\t"); uart_sendDecSigned(_bmp180_calibration.AC2); uart_sendString("\r\n");
	uart_sendString("\tAC3:\t"); uart_sendDecSigned(_bmp180_calibration.AC3); uart_sendString("\r\n");
	uart_sendString("\tAC4:\t"); uart_sendDecSigned(_bmp180_calibration.AC4); uart_sendString("\r\n");
	uart_sendString("\tAC5:\t"); uart_sendDecSigned(_bmp180_calibration.AC5); uart_sendString("\r\n");
	uart_sendString("\tAC6:\t"); uart_sendDecSigned(_bmp180_calibration.AC6); uart_sendString("\r\n");
	uart_sendString("\tB1:\t"); uart_sendDecSigned(_bmp180_calibration.B1);  uart_sendString("\r\n");
	uart_sendString("\tB2:\t"); uart_sendDecSigned(_bmp180_calibration.B2);  uart_sendString("\r\n");
	uart_sendString("\tMB:\t"); uart_sendDecSigned(_bmp180_calibration.MB);  uart_sendString("\r\n");
	uart_sendString("\tMC:\t"); uart_sendDecSigned(_bmp180_calibration.MC);  uart_sendString("\r\n");
	uart_sendString("\tMD:\t"); uart_sendDecSigned(_bmp180_calibration.MD);  uart_sendString("\r\n");
}
uint16_t
bmp180_readUT(void){
	uint8_t buffer[2];
	bmp180_writeReg(BMP180_CTRL_MEAS_REG, BMP180_T_MEASURE);
	timer_delay(6);

	bmp180_readRegMulti(BMP180_ADC_OUT_MSB_REG, buffer, 2);

	return ((buffer[0] << 8) | buffer[1]);
}
uint32_t
bmp180_readPT(uint8_t oss){
	uint32_t PT;
	uint8_t cmd, delay;
	uint8_t buffer[3];

	switch(oss) {
	case 0:
		cmd = BMP180_P0_MEASURE;
		delay   = 6;
		break;
	case 1:
		cmd = BMP180_P1_MEASURE;
		delay   = 9;
		break;
	case 2:
		cmd = BMP180_P2_MEASURE;
		delay   = 15;
		break;
	case 3:
		cmd = BMP180_P3_MEASURE;
		delay   = 27;
		break;
	}

	bmp180_writeReg(BMP180_CTRL_MEAS_REG, cmd);
	timer_delay(delay);

	bmp180_readRegMulti(BMP180_ADC_OUT_MSB_REG, buffer, 3);

	PT = ((buffer[0]<<16)|(buffer[1]<<8)|(buffer[2]));

	return PT>>(8-oss);
}
int16_t
bmp180_calcRT(uint16_t ut){
	_bmp180_calibration.B5  = (((int32_t)ut - (int32_t)_bmp180_calibration.AC6) * (int32_t)_bmp180_calibration.AC5) >> 15;
	_bmp180_calibration.B5 += ((int32_t)_bmp180_calibration.MC << 11) / (_bmp180_calibration.B5 + _bmp180_calibration.MD);

	return (_bmp180_calibration.B5 + 8) >> 4;
}
int32_t
bmp180_calcRP(uint32_t up, uint8_t oss){
	int32_t B3,B6,X3,p;
	uint32_t B4,B7;

	B6 = _bmp180_calibration.B5 - 4000;
	X3 = ((_bmp180_calibration.B2 * ((B6 * B6) >> 12)) >> 11) + ((_bmp180_calibration.AC2 * B6) >> 11);
	B3 = (((((int32_t)_bmp180_calibration.AC1) * 4 + X3) << oss) + 2) >> 2;
	X3 = (((_bmp180_calibration.AC3 * B6) >> 13) + ((_bmp180_calibration.B1 * ((B6 * B6) >> 12)) >> 16) + 2) >> 2;
	B4 = (_bmp180_calibration.AC4 * (uint32_t)(X3 + 32768)) >> 15;
	B7 = ((uint32_t)up - B3) * (50000 >> oss);
	if (B7 < 0x80000000) p = (B7 << 1) / B4; else p = (B7 / B4) << 1;
	p += ((((p >> 8) * (p >> 8) * BMP180_PARAM_MG) >> 16) + ((BMP180_PARAM_MH * p) >> 16) + BMP180_PARAM_MI) >> 4;

	return p;
}
int32_t bmp180_kpaToMmhg(int32_t pa){
	return (pa * 75) / 10000;
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
