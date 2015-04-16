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

// ENCODERS
/*
63	PC6			I/O	TIM8_CH1	ENC1_A
64	PC7			I/O	TIM8_CH2	ENC1_B
23	PA0-WKUP	I/O	TIM2_CH1	ENC2_A
24	PA1			I/O	TIM2_CH2	ENC2_B
	*/
#define ENC_1_A_PORT
#define ENC_1_A_PIN
#define ENC_1_A_MODE
#define ENC_1_A_PULL
#define ENC_1_A_OTYPE
#define ENC_1_A_SPEED

#define ENC_1_B_PORT
#define ENC_1_B_PIN
#define ENC_1_B_MODE
#define ENC_1_B_PULL
#define ENC_1_B_OTYPE
#define ENC_1_B_SPEED

#define ENC_2_A_PORT
#define ENC_2_A_PIN
#define ENC_2_A_MODE
#define ENC_2_A_PULL
#define ENC_2_A_OTYPE
#define ENC_2_A_SPEED

#define ENC_2_B_PORT
#define ENC_2_B_PIN
#define ENC_2_B_MODE
#define ENC_2_B_PULL
#define ENC_2_B_OTYPE
#define ENC_2_B_SPEED

// BATTERY MANAGEMENT
/*
25	PA2	I/O	ADC1_IN2	BATVOLTAGE
*/

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
#define AFE_CS_PORT
#define AFE_CS_PIN
#define AFE_CS_MODE
#define AFE_CS_PULL
#define AFE_CS_OTYPE
#define AFE_CS_SPEED

#define AFE_INT_PORT
#define AFE_INT_PIN
#define AFE_INT_MODE
#define AFE_INT_PULL
#define AFE_INT_OTYPE
#define AFE_INT_SPEED

#define AFE_MISO_PORT
#define AFE_MISO_PIN
#define AFE_MISO_MODE
#define AFE_MISO_PULL
#define AFE_MISO_OTYPE
#define AFE_MISO_SPEED

#define AFE_MOSI_PORT
#define AFE_MOSI_PIN
#define AFE_MOSI_MODE
#define AFE_MOSI_PULL
#define AFE_MOSI_OTYPE
#define AFE_MOSI_SPEED

#define AFE_SCK_PORT
#define AFE_SCK_PIN
#define AFE_SCK_MODE
#define AFE_SCK_PULL
#define AFE_SCK_OTYPE
#define AFE_SCK_SPEED

#define AFE_SYNC1_PORT
#define AFE_SYNC1_PIN
#define AFE_SYNC1_MODE
#define AFE_SYNC1_PULL
#define AFE_SYNC1_OTYPE
#define AFE_SYNC1_SPEED

#define AFE_SYNC2_PORT
#define AFE_SYNC2_PIN
#define AFE_SYNC2_MODE
#define AFE_SYNC2_PULL
#define AFE_SYNC2_OTYPE
#define AFE_SYNC2_SPEED

#define AFE_SYNC3_PORT
#define AFE_SYNC3_PIN
#define AFE_SYNC3_MODE
#define AFE_SYNC3_PULL
#define AFE_SYNC3_OTYPE
#define AFE_SYNC3_SPEED

#define AFE_EXT_SYNC_PORT
#define AFE_EXT_SYNC_PIN
#define AFE_EXT_SYNC_MODE
#define AFE_EXT_SYNC_PULL
#define AFE_EXT_SYNC_OTYPE
#define AFE_EXT_SYNC_SPEED

// nRF
/*
56	PD9	Output	GPIO_Output	nRF_CE
55	PD8		I/O	GPIO_EXTI8	nRF_IRQ
53	PB14	I/O	SPI2_MISO	nRF_MISO
54	PB15	I/O	SPI2_MOSI	nRF_MOSI
52	PB13	I/O	SPI2_SCK	nRF_SCK
51	PB12	I/O	SPI2_NSS	nRF_SS
	*/

#define nRF_CE_PORT
#define nRF_CE_PIN
#define nRF_CE_MODE
#define nRF_CE_PULL
#define nRF_CE_OTYPE
#define nRF_CE_SPEED

#define nRF_IRQ_PORT
#define nRF_IRQ_PIN
#define nRF_IRQ_MODE
#define nRF_IRQ_PULL
#define nRF_IRQ_OTYPE
#define nRF_IRQ_SPEED

#define nRF_MISO_PORT
#define nRF_MISO_PIN
#define nRF_MISO_MODE
#define nRF_MISO_PULL
#define nRF_MISO_OTYPE
#define nRF_MISO_SPEED

#define nRF_MOSI_PORT
#define nRF_MOSI_PIN
#define nRF_MOSI_MODE
#define nRF_MOSI_PULL
#define nRF_MOSI_OTYPE
#define nRF_MOSI_SPEED

#define nRF_SCK_PORT
#define nRF_SCK_PIN
#define nRF_SCK_MODE
#define nRF_SCK_PULL
#define nRF_SCK_OTYPE
#define nRF_SCK_SPEED

#define nRF_SS_PORT
#define nRF_SS_PIN
#define nRF_SS_MODE
#define nRF_SS_PULL
#define nRF_SS_OTYPE
#define nRF_SS_SPEED

// OLED
/*
92	PB6	Output	GPIO_Output	OLED_DC
91	PB5	I/O		SPI3_MOSI	OLED_MOSI
93	PB7	Output	GPIO_Output	OLED_RST
89	PB3	I/O		SPI3_SCK	OLED_SCK
95	PB8	Output	GPIO_Output	OLED_SS
	*/

#define OLED_DC_PORT
#define OLED_DC_PIN
#define OLED_DC_MODE
#define OLED_DC_PULL
#define OLED_DC_OTYPE
#define OLED_DC_SPEED

#define OLED_MOSI_PORT
#define OLED_MOSI_PIN
#define OLED_MOSI_MODE
#define OLED_MOSI_PULL
#define OLED_MOSI_OTYPE
#define OLED_MOSI_SPEED

#define OLED_RST_PORT
#define OLED_RST_PIN
#define OLED_RST_MODE
#define OLED_RST_PULL
#define OLED_RST_OTYPE
#define OLED_RST_SPEED

#define OLED_SCK_PORT
#define OLED_SCK_PIN
#define OLED_SCK_MODE
#define OLED_SCK_PULL
#define OLED_SCK_OTYPE
#define OLED_SCK_SPEED

#define OLED_SS_PORT
#define OLED_SS_PIN
#define OLED_SS_MODE
#define OLED_SS_PULL
#define OLED_SS_OTYPE
#define OLED_SS_SPEED

// USB
/*
70	PA11	I/O	USB_OTG_FS_DM
71	PA12	I/O	USB_OTG_FS_DP
69	PA10	I/O	USB_OTG_FS_ID
*/
#define USB_DM_PORT
#define USB_DM_PIN
#define USB_DM_MODE
#define USB_DM_PULL
#define USB_DM_OTYPE
#define USB_DM_SPEED

#define USB_DP_PORT
#define USB_DP_PIN
#define USB_DP_MODE
#define USB_DP_PULL
#define USB_DP_OTYPE
#define USB_DP_SPEED

#define USB_ID_PORT
#define USB_ID_PIN
#define USB_ID_MODE
#define USB_ID_PULL
#define USB_ID_OTYPE
#define USB_ID_SPEED

// SD CARD
/*
80	PC12	I/O	SDIO_CK
83	PD2		I/O	SDIO_CMD
65	PC8		I/O	SDIO_D0
66	PC9		I/O	SDIO_D1
78	PC10	I/O	SDIO_D2
79	PC11	I/O	SDIO_D3
*/

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
	#define TERMINAL_IT_RX_IRQChannelPreemptionPriority 0
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

// Crystal
/*
12	PH0-OSC_IN	I/O	RCC_OSC_IN
13	PH1-OSC_OUT	I/O	RCC_OSC_OUT
	*/

#endif /* PROJECTCONFIG_H_ */
