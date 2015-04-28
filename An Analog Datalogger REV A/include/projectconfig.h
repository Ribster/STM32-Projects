/*
 * projectconfig.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef PROJECTCONFIG_PROJECT_H_
#define PROJECTCONFIG_PROJECT_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

// general macro's


// Project version
#define PROJVER "0.0.005"

#define DBG
//#define DBGIO


#define FEATURE_TERMINAL
#define FEATURE_LEDS
#define FEATURE_PUSHBUTTONS
#define FEATURE_ENCODERS
#define FEATURE_SYSTICK
#define FEATURE_RTC
#define FEATURE_AFE
#define FEATURE_RNG
#define FEATURE_SD
#define FEATURE_SSD1306
#define FEATURE_USB
//#define FEATURE_nRF24L01p



// LEDS
/*
1	PE2	Output	GPIO_Output	LED0
2	PE3	Output	GPIO_Output	LED1
3	PE4	Output	GPIO_Output	LED2
4	PE5	Output	GPIO_Output	LED3
5	PE6	Output	GPIO_Output	LED4
*/

	enum ledList{
		ledList_White1,
		ledList_Blue,
		ledList_Orange,
		ledList_Green,
		ledList_White2
	};
	// White 1
	#define LED_PORT_0 GPIOE
	#define LED_PIN_0 2
	#define LED_MODE_0 GPIO_Mode_OUT
	#define LED_PULL_0 GPIO_PuPd_NOPULL
	#define LED_OTYPE_0 GPIO_OType_PP
	#define LED_SPEED_0 GPIO_Speed_100MHz
	#define LED_INITSTATE_0 Bit_RESET

	// Blue
	#define LED_PORT_1 GPIOE
	#define LED_PIN_1 3
	#define LED_MODE_1 GPIO_Mode_OUT
	#define LED_PULL_1 GPIO_PuPd_NOPULL
	#define LED_OTYPE_1 GPIO_OType_PP
	#define LED_SPEED_1 GPIO_Speed_100MHz
	#define LED_INITSTATE_1 Bit_RESET

	// Orange
	#define LED_PORT_2 GPIOE
	#define LED_PIN_2 4
	#define LED_MODE_2 GPIO_Mode_OUT
	#define LED_PULL_2 GPIO_PuPd_NOPULL
	#define LED_OTYPE_2 GPIO_OType_PP
	#define LED_SPEED_2 GPIO_Speed_100MHz
	#define LED_INITSTATE_2 Bit_RESET

	// Green
	#define LED_PORT_3 GPIOE
	#define LED_PIN_3 5
	#define LED_MODE_3 GPIO_Mode_OUT
	#define LED_PULL_3 GPIO_PuPd_NOPULL
	#define LED_OTYPE_3 GPIO_OType_PP
	#define LED_SPEED_3 GPIO_Speed_100MHz
	#define LED_INITSTATE_3 Bit_RESET

	// White 2
	#define LED_PORT_4 GPIOE
	#define LED_PIN_4 6
	#define LED_MODE_4 GPIO_Mode_OUT
	#define LED_PULL_4 GPIO_PuPd_NOPULL
	#define LED_OTYPE_4 GPIO_OType_PP
	#define LED_SPEED_4 GPIO_Speed_100MHz
	#define LED_INITSTATE_4 Bit_RESET


// PUSHBUTTONS
/*
 * 	62	PD15	I/O	GPIO_EXTI15	INPUT0
	61	PD14	I/O	GPIO_EXTI14	INPUT1
	60	PD13	I/O	GPIO_EXTI13	INPUT2
	59	PD12	I/O	GPIO_EXTI12	INPUT3
	58	PD11	I/O	GPIO_EXTI11	INPUT4
	57	PD10	I/O	GPIO_EXTI10	INPUT5
	*/
	#define PB_DEBOUNCE_DELAY_MS 50

	#define PB_IT_IRQChannelPreemptionPriority 0x0F
	#define PB_IT_IRQChannelSubPriority 0x0F
	// UP
	#define PB_PORT_0 GPIOD
	#define PB_PIN_0 15
	#define PB_MODE_0 GPIO_Mode_IN
	#define PB_PULL_0 GPIO_PuPd_NOPULL
	#define PB_OTYPE_0 GPIO_OType_PP
	#define PB_SPEED_0 GPIO_Speed_100MHz
	#define PB_EXTI_Line_0 (1<<PB_PIN_0)

	// LEFT
	#define PB_PORT_1 GPIOD
	#define PB_PIN_1 14
	#define PB_MODE_1 GPIO_Mode_IN
	#define PB_PULL_1 GPIO_PuPd_NOPULL
	#define PB_OTYPE_1 GPIO_OType_PP
	#define PB_SPEED_1 GPIO_Speed_100MHz
	#define PB_EXTI_Line_1 (1<<PB_PIN_1)

	// MIDDLE
	#define PB_PORT_2 GPIOD
	#define PB_PIN_2 13
	#define PB_MODE_2 GPIO_Mode_IN
	#define PB_PULL_2 GPIO_PuPd_NOPULL
	#define PB_OTYPE_2 GPIO_OType_PP
	#define PB_SPEED_2 GPIO_Speed_100MHz
	#define PB_EXTI_Line_2 (1<<PB_PIN_2)

	// RIGHT
	#define PB_PORT_3 GPIOD
	#define PB_PIN_3 12
	#define PB_MODE_3 GPIO_Mode_IN
	#define PB_PULL_3 GPIO_PuPd_NOPULL
	#define PB_OTYPE_3 GPIO_OType_PP
	#define PB_SPEED_3 GPIO_Speed_100MHz
	#define PB_EXTI_Line_3 (1<<PB_PIN_3)

	// DOWN
	#define PB_PORT_4 GPIOD
	#define PB_PIN_4 11
	#define PB_MODE_4 GPIO_Mode_IN
	#define PB_PULL_4 GPIO_PuPd_NOPULL
	#define PB_OTYPE_4 GPIO_OType_PP
	#define PB_SPEED_4 GPIO_Speed_100MHz
	#define PB_EXTI_Line_4 (1<<PB_PIN_4)

	// ENTER
	#define PB_PORT_5 GPIOD
	#define PB_PIN_5 10
	#define PB_MODE_5 GPIO_Mode_IN
	#define PB_PULL_5 GPIO_PuPd_NOPULL
	#define PB_OTYPE_5 GPIO_OType_PP
	#define PB_SPEED_5 GPIO_Speed_100MHz
	#define PB_EXTI_Line_5 (1<<PB_PIN_5)



// ENCODERS
/*
63	PC6			I/O	TIM8_CH1	ENC1_A
64	PC7			I/O	TIM8_CH2	ENC1_B
23	PA0-WKUP	I/O	TIM2_CH1	ENC2_A
24	PA1			I/O	TIM2_CH2	ENC2_B
	*/
#define ENC_1_TIMER TIM8
	// CHAN A
	#define ENC_1_A_PORT GPIOC
	#define ENC_1_A_PIN 6
	#define ENC_1_A_MODE GPIO_Mode_AF
	#define ENC_1_A_PULL GPIO_PuPd_NOPULL
	#define ENC_1_A_OTYPE GPIO_OType_PP
	#define ENC_1_A_SPEED GPIO_Speed_100MHz
	#define ENC_1_A_AF GPIO_AF_TIM8 //AF3
	// CHAN B
	#define ENC_1_B_PORT GPIOC
	#define ENC_1_B_PIN 7
	#define ENC_1_B_MODE GPIO_Mode_AF
	#define ENC_1_B_PULL GPIO_PuPd_NOPULL
	#define ENC_1_B_OTYPE GPIO_OType_PP
	#define ENC_1_B_SPEED GPIO_Speed_100MHz
	#define ENC_1_B_AF GPIO_AF_TIM8 //AF3

#define ENC_2_TIMER TIM5
	// CHAN A
	#define ENC_2_A_PORT GPIOA
	#define ENC_2_A_PIN 0
	#define ENC_2_A_MODE GPIO_Mode_AF
	#define ENC_2_A_PULL GPIO_PuPd_NOPULL
	#define ENC_2_A_OTYPE GPIO_OType_PP
	#define ENC_2_A_SPEED GPIO_Speed_100MHz
	#define ENC_2_A_AF GPIO_AF_TIM5 //AF2
	// CHAN B
	#define ENC_2_B_PORT GPIOA
	#define ENC_2_B_PIN 1
	#define ENC_2_B_MODE GPIO_Mode_AF
	#define ENC_2_B_PULL GPIO_PuPd_NOPULL
	#define ENC_2_B_OTYPE GPIO_OType_PP
	#define ENC_2_B_SPEED GPIO_Speed_100MHz
	#define ENC_2_B_AF GPIO_AF_TIM5 //AF2

// BATTERY MANAGEMENT
/*
25	PA2	I/O	ADC1_IN2	BATVOLTAGE
*/
#define BATVOLTAGE_PORT GPIOA
#define BATVOLTAGE_PIN 2
#define BATVOLTAGE_MODE GPIO_Mode_AN
#define BATVOLTAGE_PULL GPIO_PuPd_NOPULL
#define BATVOLTAGE_OTYPE GPIO_OType_PP
#define BATVOLTAGE_SPEED GPIO_Speed_100MHz

// AFE
	/*
29	PA4	I/O		SPI1_NSS	AFE_CS
33	PC4	Output	GPIO_Output	AFE_INT
31	PA6	I/O		SPI1_MISO	AFE_MISO
32	PA7	I/O		SPI1_MOSI	AFE_MOSI
30	PA5	I/O		SPI1_SCK	AFE_SCK
34	PC5	I/O		GPIO_EXTI5	AFE_SYNC1
35	PB0	I/O		GPIO_EXTI0	AFE_SYNC2
36	PB1	I/O		GPIO_EXTI1	AFE_SYNC3
38	PE7	I/O		GPIO_EXTI7	EXT_SYNC
	 */
#define AFE_DUMMY_SMALL
//#define AFE_DUMMY_BIG

#ifdef AFE_DUMMY_SMALL
	#define AFE_DMA_BufferSize 512
#elif defined(AFE_DUMMY_BIG)
#define AFE_DMA_BufferSize 5120
#endif

#define AFE_SPI SPI1
#define AFE_SPI_IRQn SPI1_IRQn



	// SPI Config
		#define AFE_SPI_Direction SPI_Direction_2Lines_FullDuplex
		#define AFE_SPI_Mode SPI_Mode_Slave
		#define AFE_SPI_DataSize SPI_DataSize_8b
		#define AFE_SPI_CPOL SPI_CPOL_Low
		#define AFE_SPI_CPHA SPI_CPHA_1Edge
		#define AFE_SPI_NSS SPI_NSS_Soft
		#define AFE_SPI_BaudRatePrescaler SPI_BaudRatePrescaler_16
		#define AFE_SPI_FirstBit SPI_FirstBit_MSB
		#define AFE_SPI_CRCPolynomial 7
	// DMA SPI from Memory 2 Peripheral
		// TX STREAM
		#define AFE_DMA_TX_DMAStream DMA2_Stream3
		#define AFE_DMA_TX_DMAIRQ DMA2_Stream3_IRQn
		#define AFE_DMA_TX_DMAChannel DMA_Channel_3
		#define AFE_DMA_TX_TransferCompleteFlag DMA_FLAG_TCIF3
		#define AFE_DMA_TX_TransferHalfCompleteFlag DMA_FLAG_HTIF3
		#define AFE_DMA_TX_DMARequest SPI_I2S_DMAReq_Tx
		#define AFE_DMA_TX_DIR DMA_DIR_MemoryToPeripheral

		// RX STREAM
		#define AFE_DMA_RX_DMAStream DMA2_Stream2
		#define AFE_DMA_RX_DMAIRQ DMA2_Stream2_IRQn
		#define AFE_DMA_RX_DMAChannel DMA_Channel_3
		#define AFE_DMA_RX_TransferCompleteFlag DMA_FLAG_TCIF2
		#define AFE_DMA_RX_TransferHalfCompleteFlag DMA_FLAG_HTIF2
		#define AFE_DMA_RX_DMARequest SPI_I2S_DMAReq_Rx
		#define AFE_DMA_RX_DIR DMA_DIR_PeripheralToMemory

		#define AFE_DMA_PeripheralBaseAddr (uint32_t)(&(AFE_SPI->DR))
		//#define AFE_DMA_Memory0BaseAddr //heap pointer
		#define AFE_DMA_PeripheralInc DMA_PeripheralInc_Disable
		#define AFE_DMA_MemoryInc DMA_MemoryInc_Enable
		#define AFE_DMA_PeripheralDataSize DMA_PeripheralDataSize_Byte
		#define AFE_DMA_MemoryDataSize DMA_MemoryDataSize_Byte
		#define AFE_DMA_Mode DMA_Mode_Normal
		#define AFE_DMA_Priority DMA_Priority_High
		#define AFE_DMA_FIFOMode DMA_FIFOMode_Disable
		#define AFE_DMA_FIFOThreshold DMA_FIFOThreshold_Full
		#define AFE_DMA_MemoryBurst DMA_MemoryBurst_Single
		#define AFE_DMA_PeripheralBurst DMA_PeripheralBurst_Single

	// INTERRUPT PIN
	#define AFE_INT_PORT GPIOC
	#define AFE_INT_PIN 4
	#define AFE_INT_MODE GPIO_Mode_OUT
	#define AFE_INT_PULL GPIO_PuPd_UP
	#define AFE_INT_OTYPE GPIO_OType_PP
	#define AFE_INT_SPEED GPIO_Speed_100MHz
	#define AFE_INT_INITSTATE Bit_SET
	// CHIPSELECT PIN
	#define AFE_CS_PORT GPIOA
	#define AFE_CS_PIN 4
	#define AFE_CS_MODE GPIO_Mode_IN
	#define AFE_CS_PULL GPIO_PuPd_UP
	#define AFE_CS_OTYPE GPIO_OType_PP
	#define AFE_CS_SPEED GPIO_Speed_100MHz
	// MISO PIN
	#define AFE_MISO_PORT GPIOA
	#define AFE_MISO_PIN 6
	#define AFE_MISO_MODE GPIO_Mode_AF
	#define AFE_MISO_PULL GPIO_PuPd_DOWN
	#define AFE_MISO_OTYPE GPIO_OType_PP
	#define AFE_MISO_SPEED GPIO_Speed_100MHz
	#define AFE_MISO_AF GPIO_AF_SPI1 //AF5
	// MOSI PIN
	#define AFE_MOSI_PORT GPIOA
	#define AFE_MOSI_PIN 7
	#define AFE_MOSI_MODE GPIO_Mode_AF
	#define AFE_MOSI_PULL GPIO_PuPd_DOWN
	#define AFE_MOSI_OTYPE GPIO_OType_PP
	#define AFE_MOSI_SPEED GPIO_Speed_100MHz
	#define AFE_MOSI_AF GPIO_AF_SPI1 //AF5
	// SCK PIN
	#define AFE_SCK_PORT GPIOA
	#define AFE_SCK_PIN 5
	#define AFE_SCK_MODE GPIO_Mode_AF
	#define AFE_SCK_PULL GPIO_PuPd_DOWN
	#define AFE_SCK_OTYPE GPIO_OType_PP
	#define AFE_SCK_SPEED GPIO_Speed_100MHz
	#define AFE_SCK_AF GPIO_AF_SPI1 //AF5
	// SYNC1 PIN
	#define AFE_SYNC1_PORT GPIOC
	#define AFE_SYNC1_PIN 5
	#define AFE_SYNC1_MODE GPIO_Mode_OUT
	#define AFE_SYNC1_PULL GPIO_PuPd_NOPULL
	#define AFE_SYNC1_OTYPE GPIO_OType_PP
	#define AFE_SYNC1_SPEED GPIO_Speed_100MHz
	#define AFE_SYNC1_INITSTATE Bit_RESET
	// SYNC2 PIN
	#define AFE_SYNC2_PORT GPIOB
	#define AFE_SYNC2_PIN 0
	#define AFE_SYNC2_MODE GPIO_Mode_IN
	#define AFE_SYNC2_PULL GPIO_PuPd_NOPULL
	#define AFE_SYNC2_OTYPE GPIO_OType_PP
	#define AFE_SYNC2_SPEED GPIO_Speed_100MHz
	#define AFE_SYNC2_EXTI_Line (1<<AFE_SYNC2_PIN)
	// SYNC3 PIN
	#define AFE_SYNC3_PORT GPIOB
	#define AFE_SYNC3_PIN 1
	#define AFE_SYNC3_MODE GPIO_Mode_IN
	#define AFE_SYNC3_PULL GPIO_PuPd_NOPULL
	#define AFE_SYNC3_OTYPE GPIO_OType_PP
	#define AFE_SYNC3_SPEED GPIO_Speed_100MHz
	#define AFE_SYNC3_EXTI_Line (1<<AFE_SYNC3_PIN)
	// EXTERNAL SYNC PIN
	#define AFE_EXT_SYNC_PORT GPIOE
	#define AFE_EXT_SYNC_PIN 7
	#define AFE_EXT_SYNC_MODE GPIO_Mode_IN
	#define AFE_EXT_SYNC_PULL GPIO_PuPd_NOPULL
	#define AFE_EXT_SYNC_OTYPE GPIO_OType_PP
	#define AFE_EXT_SYNC_SPEED GPIO_Speed_100MHz
	#define AFE_EXT_SYNC_EXTI_Line (1<<AFE_EXT_SYNC_PIN)

// nRF
/*
56	PD9	Output	GPIO_Output	nRF_CE
55	PD8		I/O	GPIO_EXTI8	nRF_IRQ
53	PB14	I/O	SPI2_MISO	nRF_MISO
54	PB15	I/O	SPI2_MOSI	nRF_MOSI
52	PB13	I/O	SPI2_SCK	nRF_SCK
51	PB12	I/O	SPI2_NSS	nRF_SS
	*/
#define nRF_SPI SPI2

// SPI Config
	#define nRF_SPI_Direction SPI_Direction_2Lines_FullDuplex
	#define nRF_SPI_Mode SPI_Mode_Master
	#define nRF_SPI_DataSize SPI_DataSize_8b
	#define nRF_SPI_CPOL SPI_CPOL_Low
	#define nRF_SPI_CPHA SPI_CPHA_1Edge
	#define nRF_SPI_NSS SPI_NSS_Soft
	#define nRF_SPI_BaudRatePrescaler SPI_BaudRatePrescaler_4
	#define nRF_SPI_FirstBit SPI_FirstBit_MSB
	#define nRF_SPI_CRCPolynomial 7

	#define nRF_CE_PORT GPIOD
	#define nRF_CE_PIN 9
	#define nRF_CE_MODE GPIO_Mode_OUT
	#define nRF_CE_PULL GPIO_PuPd_NOPULL
	#define nRF_CE_OTYPE GPIO_OType_PP
	#define nRF_CE_SPEED GPIO_Speed_100MHz
	#define nRF_CE_INITSTATE Bit_RESET

	#define nRF_IRQ_PORT GPIOD
	#define nRF_IRQ_PIN 8
	#define nRF_IRQ_MODE GPIO_Mode_IN
	#define nRF_IRQ_PULL GPIO_PuPd_NOPULL
	#define nRF_IRQ_OTYPE GPIO_OType_PP
	#define nRF_IRQ_SPEED GPIO_Speed_100MHz
	#define nRF_IRQ_EXTI_Line (1<<nRF_IRQ_PIN)

	#define nRF_MISO_PORT GPIOB
	#define nRF_MISO_PIN 14
	#define nRF_MISO_MODE GPIO_Mode_AF
	#define nRF_MISO_PULL GPIO_PuPd_NOPULL
	#define nRF_MISO_OTYPE GPIO_OType_PP
	#define nRF_MISO_SPEED GPIO_Speed_100MHz
	#define nRF_MISO_AF GPIO_AF_SPI2 //AF5

	#define nRF_MOSI_PORT GPIOB
	#define nRF_MOSI_PIN 15
	#define nRF_MOSI_MODE GPIO_Mode_AF
	#define nRF_MOSI_PULL GPIO_PuPd_NOPULL
	#define nRF_MOSI_OTYPE GPIO_OType_PP
	#define nRF_MOSI_SPEED GPIO_Speed_100MHz
	#define nRF_MOSI_AF GPIO_AF_SPI2 //AF5

	#define nRF_SCK_PORT GPIOB
	#define nRF_SCK_PIN 13
	#define nRF_SCK_MODE GPIO_Mode_AF
	#define nRF_SCK_PULL GPIO_PuPd_NOPULL
	#define nRF_SCK_OTYPE GPIO_OType_PP
	#define nRF_SCK_SPEED GPIO_Speed_100MHz
	#define nRF_SCK_AF GPIO_AF_SPI2 //AF5

	#define nRF_SS_PORT GPIOB
	#define nRF_SS_PIN 12
	#define nRF_SS_MODE GPIO_Mode_OUT
	#define nRF_SS_PULL GPIO_PuPd_NOPULL
	#define nRF_SS_OTYPE GPIO_OType_PP
	#define nRF_SS_SPEED GPIO_Speed_100MHz
	#define nRF_SS_INITSTATE Bit_RESET

// OLED
/*
92	PB6	Output	GPIO_Output	OLED_MOSI
91	PB5	I/O		SPI3_MOSI	OLED_MOSI
93	PB7	Output	GPIO_Output	OLED_RST
89	PB3	I/O		SPI3_SCK	OLED_SCK
95	PB8	Output	GPIO_Output	OLED_SS
	*/
#define OLED_SPI SPI3
// Perspective of sizes is landscape orientation
	#define OLED_HEIGHT 64
	#define OLED_WIDTH 128
	#define OLED_BufferSize (OLED_HEIGHT*OLED_WIDTH/8)
	#define OLED_EXTERNALVCC Bit_RESET
	#define OLED_ORIENTATION SSD1306_ORIENTATION_LANDSCAPE_UpsideDown
	//#define OLED_ORIENTATION SSD1306_ORIENTATION_PORTRAIT_CCW
	#define OLED_VIEWMODE SSD1306_VIEWMODE_NORMAL
// SPI Config
	#define OLED_SPI_Direction SPI_Direction_1Line_Tx
	#define OLED_SPI_Mode SPI_Mode_Master
	#define OLED_SPI_DataSize SPI_DataSize_8b
	#define OLED_SPI_CPOL SPI_CPOL_Low
	#define OLED_SPI_CPHA SPI_CPHA_1Edge
	#define OLED_SPI_NSS SPI_NSS_Soft
	#define OLED_SPI_BaudRatePrescaler SPI_BaudRatePrescaler_2
	#define OLED_SPI_FirstBit SPI_FirstBit_MSB
	#define OLED_SPI_CRCPolynomial 7
// DMA SPI from Memory 2 Peripheral
	#define OLED_DMA_TX_DMAStream DMA1_Stream7
	#define OLED_DMA_TX_DMAChannel DMA_Channel_0
	#define OLED_DMA_TX_TransferCompleteFlag DMA_FLAG_TCIF7
	#define OLED_DMA_TX_DMARequest SPI_I2S_DMAReq_Tx
	#define OLED_DMA_PeripheralBaseAddr (uint32_t)&(OLED_SPI->DR)
//#define OLED_DMA_Memory0BaseAddr //heap pointer
	#define OLED_DMA_TX_DIR DMA_DIR_MemoryToPeripheral
	#define OLED_DMA_PeripheralInc DMA_PeripheralInc_Disable
	#define OLED_DMA_MemoryInc DMA_MemoryInc_Enable
	#define OLED_DMA_PeripheralDataSize DMA_PeripheralDataSize_Byte
	#define OLED_DMA_MemoryDataSize DMA_MemoryDataSize_Byte
	#define OLED_DMA_Mode DMA_Mode_Normal
	#define OLED_DMA_Priority DMA_Priority_High
	#define OLED_DMA_FIFOMode DMA_FIFOMode_Disable
	#define OLED_DMA_FIFOThreshold DMA_FIFOThreshold_1QuarterFull
	#define OLED_DMA_MemoryBurst DMA_MemoryBurst_Single
	#define OLED_DMA_PeripheralBurst DMA_PeripheralBurst_Single

// Refresh timer
	#define OLED_TIMER TIM13 /* Timer used for auto refresh interrupt */
	#define OLED_TIMER_ITFlag TIM_IT_Update
	#define OLED_TIMER_IRQn TIM8_UP_TIM13_IRQn
	// DC PIN
	#define OLED_DC_PORT GPIOB
	#define OLED_DC_PIN 6
	#define OLED_DC_MODE GPIO_Mode_OUT
	#define OLED_DC_PULL GPIO_PuPd_NOPULL
	#define OLED_DC_OTYPE GPIO_OType_PP
	#define OLED_DC_SPEED GPIO_Speed_100MHz
	#define OLED_DC_INITSTATE Bit_RESET
	// MOSI PIN
	#define OLED_MOSI_PORT GPIOB
	#define OLED_MOSI_PIN 5
	#define OLED_MOSI_MODE GPIO_Mode_AF
	#define OLED_MOSI_PULL GPIO_PuPd_DOWN
	#define OLED_MOSI_OTYPE GPIO_OType_PP
	#define OLED_MOSI_SPEED GPIO_Speed_100MHz
	#define OLED_MOSI_AF GPIO_AF_SPI3 //AF6
	// RST PIN
	#define OLED_RST_PORT GPIOB
	#define OLED_RST_PIN 7
	#define OLED_RST_MODE GPIO_Mode_OUT
	#define OLED_RST_PULL GPIO_PuPd_NOPULL
	#define OLED_RST_OTYPE GPIO_OType_PP
	#define OLED_RST_SPEED GPIO_Speed_100MHz
	#define OLED_RST_INITSTATE Bit_RESET
	// SCK PIN
	#define OLED_SCK_PORT GPIOB
	#define OLED_SCK_PIN 3
	#define OLED_SCK_MODE GPIO_Mode_AF
	#define OLED_SCK_PULL GPIO_PuPd_DOWN
	#define OLED_SCK_OTYPE GPIO_OType_PP
	#define OLED_SCK_SPEED GPIO_Speed_100MHz
	#define OLED_SCK_AF GPIO_AF_SPI3 //AF6
	// SS PIN
	#define OLED_SS_PORT GPIOB
	#define OLED_SS_PIN 8
	#define OLED_SS_MODE GPIO_Mode_OUT
	#define OLED_SS_PULL GPIO_PuPd_NOPULL
	#define OLED_SS_OTYPE GPIO_OType_PP
	#define OLED_SS_SPEED GPIO_Speed_100MHz
	#define OLED_SS_INITSTATE Bit_RESET


// APPLICATION
#define OLED_MENUWRITING_START 0,53
#define OLED_MENUWRITING_END 127,0
#define OLED_MENUWRITING_FONT Font_System5x8

#define OLED_MENUWRITING_MINIMUMWIDTH_CHARSIZE 22

#define OLED_TEXTBLOCK_DIMENSIONS 0,0,127,53
#define OLED_LINE_DIMENSIONS 0,55,127,55
#define OLED_DELAY_STARTUP 2000

// USB
/*
70	PA11	I/O	USB_OTG_FS_DM
71	PA12	I/O	USB_OTG_FS_DP
69	PA10	I/O	USB_OTG_FS_ID

	PA9		used in usb library
	PA8		used in usb library
*/
	// initialization of usb pins is found in the usb_bsp.c file.
	// configuration is found in the usb_conf.h file
	// here the macro USE_USB_OTG_FS is used


/*
	#define USB_DM_PORT GPIOA
	#define USB_DM_PIN 11
	#define USB_DM_MODE GPIO_Mode_AF
	#define USB_DM_PULL GPIO_PuPd_NOPULL
	#define USB_DM_OTYPE GPIO_OType_PP
	#define USB_DM_SPEED GPIO_Speed_100MHz
	#define USB_DM_AF GPIO_AF_OTG_FS //AF10

	#define USB_DP_PORT GPIOA
	#define USB_DP_PIN 12
	#define USB_DP_MODE GPIO_Mode_AF
	#define USB_DP_PULL GPIO_PuPd_NOPULL
	#define USB_DP_OTYPE GPIO_OType_PP
	#define USB_DP_SPEED GPIO_Speed_100MHz
	#define USB_DP_AF GPIO_AF_OTG_FS //AF10

	#define USB_ID_PORT GPIOA
	#define USB_ID_PIN 10
	#define USB_ID_MODE GPIO_Mode_AF
	#define USB_ID_PULL GPIO_PuPd_NOPULL
	#define USB_ID_OTYPE GPIO_OType_PP
	#define USB_ID_SPEED GPIO_Speed_100MHz
	#define USB_ID_AF GPIO_AF_OTG_FS //AF10
*/


// SD CARD
/*
80	PC12	I/O	SDIO_CK
83	PD2		I/O	SDIO_CMD
65	PC8		I/O	SDIO_D0
66	PC9		I/O	SDIO_D1
78	PC10	I/O	SDIO_D2
79	PC11	I/O	SDIO_D3
*/



#define SD_DETECT_PIN                    GPIO_Pin_2                 /* PC.02 */
#define SD_DETECT_GPIO_PORT              GPIOC                      /* GPIOC */
#define SD_DETECT_GPIO_CLK               RCC_AHB1Periph_GPIOC

#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40012C80)
/**
  * @brief  SDIO Intialization Frequency (400KHz max)
  */
#define SDIO_INIT_CLK_DIV                ((uint8_t)0x76)
/**
  * @brief  SDIO Data Transfer Frequency (25MHz max)
  */
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x2)

#define SD_SDIO_DMA                   DMA2
#define SD_SDIO_DMA_CLK               RCC_AHB1Periph_DMA2

//#define SD_SDIO_DMA_STREAM3	          3
#define SD_SDIO_DMA_STREAM6           6

#ifdef SD_SDIO_DMA_STREAM3
	#undef SD_SDIO_DMA_STREAM6
#endif

#ifdef SD_SDIO_DMA_STREAM6
	#undef SD_SDIO_DMA_STREAM3
#endif

#ifdef SD_SDIO_DMA_STREAM3
 #define SD_SDIO_DMA_STREAM            DMA2_Stream3
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF3
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF3
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF3
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF3
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF3
 #define SD_SDIO_DMA_IRQn              DMA2_Stream3_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream3_IRQHandler
#elif defined SD_SDIO_DMA_STREAM6
 #define SD_SDIO_DMA_STREAM            DMA2_Stream6
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF6
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF6
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF6
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF6
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF6
 #define SD_SDIO_DMA_IRQn              DMA2_Stream6_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream6_IRQHandler
#endif /* SD_SDIO_DMA_STREAM3 */

/* Uncomment the following line to select the SDIO Data transfer mode */
#if !defined (SD_DMA_MODE) && !defined (SD_POLLING_MODE)
#define SD_DMA_MODE                                ((uint32_t)0x00000000)
/*#define SD_POLLING_MODE                            ((uint32_t)0x00000002)*/
#endif
	#define SDIO_RESET_AF GPIO_AF_MCO
	#define SDIO_RESET_MODE GPIO_Mode_IN
	#define SDIO_RESET_PULL GPIO_PuPd_NOPULL
	#define SDIO_RESET_OTYPE GPIO_OType_PP
	#define SDIO_RESET_SPEED GPIO_Speed_100MHz
	// SDIO CLOCK
	#define SDIO_CK_PORT GPIOC
	#define SDIO_CK_PIN 12
	#define SDIO_CK_MODE GPIO_Mode_AF
	#define SDIO_CK_PULL GPIO_PuPd_NOPULL
	#define SDIO_CK_OTYPE GPIO_OType_PP
	#define SDIO_CK_SPEED GPIO_Speed_100MHz
	#define SDIO_CK_AF GPIO_AF_SDIO //AF12
	// SDIO COMMAND
	#define SDIO_CMD_PORT GPIOD
	#define SDIO_CMD_PIN 2
	#define SDIO_CMD_MODE GPIO_Mode_AF
	#define SDIO_CMD_PULL GPIO_PuPd_UP
	#define SDIO_CMD_OTYPE GPIO_OType_PP
	#define SDIO_CMD_SPEED GPIO_Speed_100MHz
	#define SDIO_CMD_AF GPIO_AF_SDIO //AF12
	// SDIO D0
	#define SDIO_D0_PORT GPIOC
	#define SDIO_D0_PIN 8
	#define SDIO_D0_MODE GPIO_Mode_AF
	#define SDIO_D0_PULL GPIO_PuPd_UP
	#define SDIO_D0_OTYPE GPIO_OType_PP
	#define SDIO_D0_SPEED GPIO_Speed_100MHz
	#define SDIO_D0_AF GPIO_AF_SDIO //AF12
	// SDIO D1
	#define SDIO_D1_PORT GPIOC
	#define SDIO_D1_PIN 9
	#define SDIO_D1_MODE GPIO_Mode_AF
	#define SDIO_D1_PULL GPIO_PuPd_UP
	#define SDIO_D1_OTYPE GPIO_OType_PP
	#define SDIO_D1_SPEED GPIO_Speed_100MHz
	#define SDIO_D1_AF GPIO_AF_SDIO //AF12
	// SDIO D2
	#define SDIO_D2_PORT GPIOC
	#define SDIO_D2_PIN 10
	#define SDIO_D2_MODE GPIO_Mode_AF
	#define SDIO_D2_PULL GPIO_PuPd_UP
	#define SDIO_D2_OTYPE GPIO_OType_PP
	#define SDIO_D2_SPEED GPIO_Speed_100MHz
	#define SDIO_D2_AF GPIO_AF_SDIO //AF12
	// SDIO D3
	#define SDIO_D3_PORT GPIOC
	#define SDIO_D3_PIN 11
	#define SDIO_D3_MODE GPIO_Mode_AF
	#define SDIO_D3_PULL GPIO_PuPd_UP
	#define SDIO_D3_OTYPE GPIO_OType_PP
	#define SDIO_D3_SPEED GPIO_Speed_100MHz
	#define SDIO_D3_AF GPIO_AF_SDIO //AF12
// USART
/*
87	PD6	I/O	USART2_RX
86	PD5	I/O	USART2_TX
	*/
	// UART2 as printf
	#define TERMINAL_UART USART2
	#define TERMINAL_UART_IRQN USART2_IRQn
	#define TERMINAL_BAUDRATE 230400
	#define TERMINAL_WORDLENGTH USART_WordLength_8b
	#define TERMINAL_STOPBITS USART_StopBits_1
	#define TERMINAL_PARITY USART_Parity_No
	#define TERMINAL_FLOWCONTROL USART_HardwareFlowControl_None
	#define TERMINAL_MODE USART_Mode_Rx | USART_Mode_Tx
	#define TERMINAL_IT_RX USART_IT_RXNE
	#define TERMINAL_IT_RX_IRQChannelPreemptionPriority 0x02
	#define TERMINAL_IT_RX_IRQChannelSubPriority 0
	#define TERMINAL_IT_RX_MAXSTRINGLENGTH 512
	extern char TERMINAL_receiverbuffer[TERMINAL_IT_RX_MAXSTRINGLENGTH];
	// RX PIN
	#define UART2_PORT_RX GPIOD
	#define UART2_PIN_RX 6
	#define UART2_MODE_RX GPIO_Mode_AF
	#define UART2_PULL_RX GPIO_PuPd_UP
	#define UART2_OTYPE_RX GPIO_OType_PP
	#define UART2_SPEED_RX GPIO_Speed_100MHz
	#define UART2_AF_RX GPIO_AF_USART2 // AF7
	// TX PIN
	#define UART2_PORT_TX GPIOD
	#define UART2_PIN_TX 5
	#define UART2_MODE_TX GPIO_Mode_AF
	#define UART2_PULL_TX GPIO_PuPd_UP
	#define UART2_OTYPE_TX GPIO_OType_PP
	#define UART2_SPEED_TX GPIO_Speed_100MHz
	#define UART2_AF_TX GPIO_AF_USART2 // AF7

// RTC
/*
8	PC14-OSC32_IN	I/O	RCC_OSC32_IN
9	PC15-OSC32_OUT	I/O	RCC_OSC32_OUT*/
	#define RTC_CLOCK_SOURCE_LSE          /* LSE used as RTC source clock */
	//#define RTC_CLOCK_SOURCE_LSI     /* LSI used as RTC source clock. The RTC Clock */
	#define RTC_EXTI_Line EXTI_Line17
	#define RTC_EXTI_Mode EXTI_Mode_Interrupt
	#define RTC_EXTI_Trigger EXTI_Trigger_Rising

// Crystal
/*
12	PH0-OSC_IN	I/O	RCC_OSC_IN
13	PH1-OSC_OUT	I/O	RCC_OSC_OUT
	*/

// TESTPOINTS

/*
7	PC13-ANTI_TAMP	Output	GPIO_Output	TESTPOINT 3
26	PA3				Output	GPIO_Output	TESTPOINT 1
77	PA15			Output	GPIO_Output	TESTPOINT 2
81	PD0				Output	GPIO_Output	TESTPOINT 8
82	PD1				Output	GPIO_Output	TESTPOINT 9
84	PD3				Output	GPIO_Output	TESTPOINT 11
85	PD4				Output	GPIO_Output	TESTPOINT 12
88	PD7				Output	GPIO_Output	TESTPOINT 10
90	PB4				Output	GPIO_Output	TESTPOINT 6
96	PB9				Output	GPIO_Output	TESTPOINT 7
97	PE0				Output	GPIO_Output	TESTPOINT 4
98	PE1				Output	GPIO_Output	TESTPOINT 5
*/

#define TESTPOINT_1_PORT GPIOA
#define TESTPOINT_1_PIN 3
#define TESTPOINT_1_MODE GPIO_Mode_OUT
#define TESTPOINT_1_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_1_OTYPE GPIO_OType_PP
#define TESTPOINT_1_SPEED GPIO_Speed_100MHz
#define TESTPOINT_1_INITSTATE Bit_RESET

#define TESTPOINT_2_PORT GPIOA
#define TESTPOINT_2_PIN 15
#define TESTPOINT_2_MODE GPIO_Mode_OUT
#define TESTPOINT_2_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_2_OTYPE GPIO_OType_PP
#define TESTPOINT_2_SPEED GPIO_Speed_100MHz
#define TESTPOINT_2_INITSTATE Bit_RESET

#define TESTPOINT_3_PORT GPIOC
#define TESTPOINT_3_PIN 13
#define TESTPOINT_3_MODE GPIO_Mode_OUT
#define TESTPOINT_3_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_3_OTYPE GPIO_OType_PP
#define TESTPOINT_3_SPEED GPIO_Speed_100MHz
#define TESTPOINT_3_INITSTATE Bit_RESET

#define TESTPOINT_4_PORT GPIOE
#define TESTPOINT_4_PIN 0
#define TESTPOINT_4_MODE GPIO_Mode_OUT
#define TESTPOINT_4_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_4_OTYPE GPIO_OType_PP
#define TESTPOINT_4_SPEED GPIO_Speed_100MHz
#define TESTPOINT_4_INITSTATE Bit_RESET

#define TESTPOINT_5_PORT GPIOE
#define TESTPOINT_5_PIN 1
#define TESTPOINT_5_MODE GPIO_Mode_OUT
#define TESTPOINT_5_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_5_OTYPE GPIO_OType_PP
#define TESTPOINT_5_SPEED GPIO_Speed_100MHz
#define TESTPOINT_5_INITSTATE Bit_RESET

#define TESTPOINT_6_PORT GPIOB
#define TESTPOINT_6_PIN 4
#define TESTPOINT_6_MODE GPIO_Mode_OUT
#define TESTPOINT_6_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_6_OTYPE GPIO_OType_PP
#define TESTPOINT_6_SPEED GPIO_Speed_100MHz
#define TESTPOINT_6_INITSTATE Bit_RESET

#define TESTPOINT_7_PORT GPOIB
#define TESTPOINT_7_PIN 9
#define TESTPOINT_7_MODE GPIO_Mode_OUT
#define TESTPOINT_7_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_7_OTYPE GPIO_OType_PP
#define TESTPOINT_7_SPEED GPIO_Speed_100MHz
#define TESTPOINT_7_INITSTATE Bit_RESET

#define TESTPOINT_8_PORT GPIOD
#define TESTPOINT_8_PIN 0
#define TESTPOINT_8_MODE GPIO_Mode_OUT
#define TESTPOINT_8_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_8_OTYPE GPIO_OType_PP
#define TESTPOINT_8_SPEED GPIO_Speed_100MHz
#define TESTPOINT_8_INITSTATE Bit_RESET

#define TESTPOINT_9_PORT GPIOD
#define TESTPOINT_9_PIN 1
#define TESTPOINT_9_MODE GPIO_Mode_OUT
#define TESTPOINT_9_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_9_OTYPE GPIO_OType_PP
#define TESTPOINT_9_SPEED GPIO_Speed_100MHz
#define TESTPOINT_9_INITSTATE Bit_RESET

#define TESTPOINT_10_PORT GPIOD
#define TESTPOINT_10_PIN 7
#define TESTPOINT_10_MODE GPIO_Mode_OUT
#define TESTPOINT_10_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_10_OTYPE GPIO_OType_PP
#define TESTPOINT_10_SPEED GPIO_Speed_100MHz
#define TESTPOINT_10_INITSTATE Bit_RESET

#define TESTPOINT_11_PORT GPIOD
#define TESTPOINT_11_PIN 3
#define TESTPOINT_11_MODE GPIO_Mode_OUT
#define TESTPOINT_11_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_11_OTYPE GPIO_OType_PP
#define TESTPOINT_11_SPEED GPIO_Speed_100MHz
#define TESTPOINT_11_INITSTATE Bit_RESET

#define TESTPOINT_12_PORT GPIOD
#define TESTPOINT_12_PIN 4
#define TESTPOINT_12_MODE GPIO_Mode_OUT
#define TESTPOINT_12_PULL GPIO_PuPd_NOPULL
#define TESTPOINT_12_OTYPE GPIO_OType_PP
#define TESTPOINT_12_SPEED GPIO_Speed_100MHz
#define TESTPOINT_12_INITSTATE Bit_RESET

// UNUSED PINS

#endif /* PROJECTCONFIG_H_ */
