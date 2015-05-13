// standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// stm32lib libraries
#include "RCCConfig.h"
#include "IO.h"
#include "IOPin.h"
#include "DELAY.h"
#include "OSCILLATOR.h"
#include "HC595.h"
#include "NEC.h"
#include "UART.h"
#include "WS2812B.h"
#include "SPI.h"
#include "nRF24L01p_Old.h"
#include "SCREEN.h"

// fatfs libraries
#include "fatfs/diskio.h"
#include "fatfs/ff.h"
#include "fatfs/integer.h"
#include "fatfs/stm320518_eval.h"
#include "fatfs/stm320518_eval_spi_sd.h"

// stm stdlib libraries
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_rtc.h"
#include "stm32f0xx_pwr.h"

// project dependent cortexm libraries
#include "cortexm/ExceptionHandlers.h"

// namespace usage
using namespace std;
using namespace stm32lib;

// Universal Software for Hardware PCB ALT0056 REV A
#define FileVersion "0.028"

// pinout ALT0056
  // RTC
    //	PC13	RTC_LED
    //	PC14	RTC_IN
    //	PC15	RTC_OUT

  // OSC
    //	PF0	OSC_IN
    //	PF1	OSC_OUT

  // OLED
    //	PA0	OLED_LED
    //	PA5	OLED_SCK (shared with SD)
    //	PA7	OLED_MOSI (shared with SD)
    //	PB0	OLED_RESET
    //	PB1	OLED_DC
    //	PA15	OLED_SS

  // SD
    //	PA1	SD_LED
    //	PA4	SD_SS
    //	PA5	SD_SCK (shared with OLED)
    //	PA6	SD_MISO
    //	PA7	SD_MOSI (shared with OLED)

  // USART
    //	PA2	USART2_TX
    //	PA3	USART2_RX

  // nRF24L01+
    //	PB2	nRF_IRQ
    //	PB10	nRF_CE
    //	PB11	nRF_LED
    //	PB12	nRF_SS
    //	PB13	nRF_SCK
    //	PB14	nRF_MISO
    //	PB15	nRF_MOSI

  // WS2812B
    //	PA8	WS2812B_CH1
    //	PA9	WS2812B_CH2

  // PUSHBUTTONS
    //	PA10	PB_B4
    //	PA11	PB_B3
    //	PA12	PB_B2
    //	PF6	PB_B1
    //	PF7	PB_B0

  // SYSTEM
    //	PA13	SWDIO
    //	PA14	SWCLK
    //	PB8	LED_GREEN
    //	PB9	LED_ORANGE

  // INFRA RED
    //	PB3	IR_LED
    //	PB4	IR_OSC

  // HC595
    //	PB5	HC595_DATA
    //	PB6	HC595_LATCH
    //	PB7	HC595_SCK

// choose one of the following elements
//#define VERSION_ALL
//#define VERSION_RECEIVER60
//#define VERSION_RECEIVER120
#define VERSION_TRANSMITTER
//#define VERSION_REMOTE

#ifdef VERSION_ALL
// System leds + SWD
#define FUNCTION_SYS
// HC 595 SIPO's
#define FUNCTION_HC595
// Infrared functionality NEC protocol at 38kHz
#define FUNCTION_IR
// nRF24L01p wireless 2.4GHz transceiver
#define FUNCTION_nRF
// OLED screen
#define FUNCTION_OLED
// External 8MHz crystal instead of HSI
//#define FUNCTION_OSC
// Pushbuttons for user input
#define FUNCTION_PB
// Real time clock for timing
#define FUNCTION_RTC
// SD storage card
#define FUNCTION_SD
// USART serial communication for printing
#define FUNCTION_USART
// WS2812B led pixels
#define FUNCTION_WS2812B

// functionality for the receiver
#define nRF_Receiver
// functionality for the transmitter
#define nRF_Transmitter
// functionality for the remote
#define nRF_Remote

#else

  #ifdef VERSION_RECEIVER60
  #define FUNCTION_SYS
  #define FUNCTION_HC595
  #define FUNCTION_IR
  #define FUNCTION_nRF
  #define FUNCTION_USART
  #define FUNCTION_WS2812B
  #define nRF_Receiver

  #else

    #ifdef VERSION_RECEIVER120
    #define FUNCTION_SYS
    #define FUNCTION_HC595
    #define FUNCTION_IR
    #define FUNCTION_nRF
    #define FUNCTION_USART
    #define FUNCTION_WS2812B
    #define nRF_Receiver

    #else

      #ifdef VERSION_TRANSMITTER
      #define FUNCTION_SYS
      #define FUNCTION_nRF
      #define FUNCTION_OLED
      #define FUNCTION_OSC
      #define FUNCTION_PB
      #define FUNCTION_RTC
      #define FUNCTION_SD
      #define FUNCTION_USART
      #define nRF_Transmitter

      #else

	#ifdef VERSION_REMOTE
	#define FUNCTION_SYS
	#define FUNCTION_nRF
	#define FUNCTION_OLED
	#define FUNCTION_PB
	#define FUNCTION_RTC
	#define FUNCTION_USART
	#define nRF_Remote

	#endif /* VERSION_REMOTE */

      #endif /* VERSION_TRANSMITTER */

    #endif /* VERSION_RECEIVER120 */

  #endif /* VERSION_RECEIVER60 */

#endif /* VERSION_ALL */




// System leds + SWD
#ifdef FUNCTION_SYS
// SYSTEM
  //	PA13	SWDIO
  //	PA14	SWCLK
  //	PB8	LED_GREEN
  //	PB9	LED_ORANGE

    // enums
    typedef enum SYS_SM_e{
      SYS_SM_RECEIVER_STARTUP = 0x01,
      SYS_SM_RECEIVER_RECEIVINGTRANSMITTER ,
      SYS_SM_RECEIVER_UPDATINGPIXELS ,
      SYS_SM_RECEIVER_WAITINGNEWDATA ,
      SYS_SM_RECEIVER_WAITTIMEEXCEEDED ,
      SYS_SM_RECEIVER_REWAITMAXEXCEEDED ,
      SYS_SM_TRANSMITTER_STARTUP ,
      SYS_SM_TRANSMITTER_READINGSD ,
      SYS_SM_TRANSMITTER_TRANSMITTINGRECEIVER ,
      SYS_SM_TRANSMITTER_RECEIVINGREMOTE ,
      SYS_SM_TRANSMITTER_TRANSMITTINGREMOTE ,
      SYS_SM_TRANSMITTER_SETTINGOLED ,
      SYS_SM_TRANSMITTER_MENUHANDLING ,
      SYS_SM_REMOTE_STARTUP ,
      SYS_SM_REMOTE_RECEIVINGTRANSMITTER ,
      SYS_SM_REMOTE_TRANSMITTINGTRANSMITTER ,
      SYS_SM_REMOTE_SETTINGOLED ,
      SYS_SM_REMOTE_MENUHANDLING
    }SYS_SM_t;

    // variables
    volatile uint32_t SYS_delayCount;
    volatile uint32_t SYS_curtime;
    volatile uint32_t SYS_lastupdate;
    volatile bool SYS_timeout = false;
#if defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120)
    volatile SYS_SM_t SYS_currentState = SYS_SM_RECEIVER_STARTUP;
#elif defined(VERSION_TRANSMITTER)
    volatile SYS_SM_t SYS_currentState = SYS_SM_TRANSMITTER_STARTUP;
#elif defined(VERSION_REMOTE)
    volatile SYS_SM_t SYS_currentState = SYS_SM_REMOTE_STARTUP;
#endif
    uint32_t SYS_loopDelay = 200;

    // objects
    RCCConfig SYS_RCC;
    IO SYS_IO;
    DELAY SYS_DELAY(&SYS_delayCount);
    IO_t SYS_LED_GREEN;
    IO_t SYS_LED_ORANGE;

    // prototypes
    void initALL(void);
    void initIO(void);
    void initDELAY(void);
    void printVersion(void);

#endif /* FUNCTION_SYS */

// USART serial communication for printing
#ifdef FUNCTION_USART
// USART
  //	PA2	USART2_TX
  //	PA3	USART2_RX

    // constants
    #define USART_BAUDRATE 230400
    #define USART_SELECTION USART2

    // variables

    // objects
    UART_t USART_UART_;
    UART USART_UART(&USART_UART_, &SYS_IO);

    IO_t USART_RX;
    IO_t USART_TX;

    // prototypes
    void initUART(void);

#ifdef VERSION_TRANSMITTER
    void initTRANSMITTER(void);
#endif

#if defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120)
    void initReceiver(void);
#endif

#endif /* FUNCTION_ */

// HC 595 SIPO's
#ifdef FUNCTION_HC595
// HC595
  //	PB5	HC595_DATA
  //	PB6	HC595_LATCH
  //	PB7	HC595_SCK

    // constants
#define HC595_COUNT 2

    // variables

    // objects

    HC595_t HC595_595_;
    IO_t HC595_SCK;
    IO_t HC595_LATCH;
    IO_t HC595_DATA;
    uint8_t HC595_bufferBlock[HC595_COUNT];
    HC595 HC595_595(&SYS_IO, &HC595_595_, &HC595_SCK, &HC595_DATA, &HC595_LATCH, HC595_bufferBlock, HC595_COUNT);

    // prototypes
    void initHC595(void);

#endif /* FUNCTION_ */
// Infrared functionality NEC protocol at 38kHz
#ifdef FUNCTION_IR
// INFRA RED
  //	PB3	IR_LED
  //	PB4	IR_OSC

    // constants
#define IR_channels 3
#define IR_channelshift 8
#define IR_timer TIM2

#define IR_oscillator_timer TIM3
#define IR_oscillator_channel 1
#define IR_oscillator_freq 38000
    // variables

    // commandlist
    typedef enum IR_Commands_t{
      IR_Commands_Intensity_Up,
      IR_Commands_Intensity_Down,
      IR_Commands_Off,
      IR_Commands_On,
      IR_Commands_Red_1,
      IR_Commands_Green_1,
      IR_Commands_Blue_1,
      IR_Commands_White,
      IR_Commands_Red_2,
      IR_Commands_Green_2,
      IR_Commands_Blue_2,
      IR_Commands_Flash,
      IR_Commands_Red_3,
      IR_Commands_Green_3,
      IR_Commands_Blue_3,
      IR_Commands_Strobe,
      IR_Commands_Red_4,
      IR_Commands_Green_4,
      IR_Commands_Blue_4,
      IR_Commands_Fade,
      IR_Commands_Red_5,
      IR_Commands_Green_5,
      IR_Commands_Blue_5,
      IR_Commands_Smooth,
      IR_Commands_Nothing
    }IR_Commands_t;
    NEC_t IR_commandList[IR_Commands_Nothing+1];


    // objects
    OSC_t IR_OSC_;
    OSCILLATOR IR_OSC(&IR_OSC_);

    IO_t IR_LED;
    IO_t IR_PIN;

    uint8_t IR_stateBuffer[IR_channels][HC595_BUFFERDIM];
    NEC_t IR_channelList[IR_channels];
    NEC IR_NEC(IR_timer, &HC595_595, &IR_OSC, IR_commandList, IR_Commands_Nothing+1, IR_channelList, IR_channels, IR_channelshift, (uint8_t*)IR_stateBuffer, &SYS_IO, &IR_LED);



    // prototypes
    void initOSCILLATOR(void);
    void initIRCommands(void);

#endif /* FUNCTION_ */
// nRF24L01p wireless 2.4GHz transceiver
#ifdef FUNCTION_nRF
// nRF24L01+
  //	PB2	nRF_IRQ
  //	PB10	nRF_CE
  //	PB11	nRF_LED
  //	PB12	nRF_SS
  //	PB13	nRF_SCK
  //	PB14	nRF_MISO
  //	PB15	nRF_MOSI

    // constants

#define NRF_IRQN EXTI2_3_IRQn

    #define NRF_Payload 24

    #define NRF_TOTALBUFFSIZE 184

    #define NRF_Channel_Receiver 58

    #define NRF_Channel_Remote 63

    #define NRF_updateThreshold 60000
    #define NRF_updateMaxCount 10

    #define NRF_RX_Receiver 0xD7
    #define NRF_TX_Receiver 0xE7

    #define NRF_RX_Transmitter NRF_RX_Receiver
    #define NRF_TX_Transmitter NRF_RX_Receiver

    #define NRF_RX_Remote 0xDA
    #define NRF_TX_Remote 0xEA

    // variables
    struct nRF_ApplicationData{
      uint32_t nRF_updateTime;
      uint8_t nRF_Buffer[NRF_Payload];
      uint8_t nRF_FULLBuffer[NRF_TOTALBUFFSIZE];
      uint8_t nRF_rx_address[5];
      uint8_t nRF_tx_address[5];
    } nRF_Data;



    typedef enum nRF_PacketCMDS_t{
      nRF_PacketCMDS_Empty = 0x00,
      nRF_PacketCMDS_PayloadStart = 0x01,
      nRF_PacketCMDS_PayloadEnd = (NRF_TOTALBUFFSIZE/(NRF_Payload-1)),
      nRF_PacketCMDS_PayloadFinished = nRF_PacketCMDS_PayloadEnd+1,
      nRF_PacketCMDS_RainbowCommand,
      nRF_PacketCMDS_RainbowFastCommand,
      nRF_PacketCMDS_ExpandPink,
      nRF_PacketCMDS_ExpandYellow,
      nRF_PacketCMDS_ScanWhite,
    } nRF_PacketCMDS_t;


    typedef enum nRF_ReceiverModes_t{
      nRF_ReceiverModes_NormalUpdate = 0x00,
      nRF_ReceiverModes_Rainbow = 0x01,
      nRF_ReceiverModes_RainbowFast = 0x02,
      nRF_ReceiverModes_ExpandPink = 0x03,
      nRF_ReceiverModes_ExpandYellow = 0x04,
      nRF_ReceiverModes_ScanWhite = 0x05,
      nRF_ReceiverModes_Timeout
    } nRF_ReceiverModes_t ;

    typedef enum nRF_BufferPositions_t{
      nRF_BufferPositions_startTime = 0,
      nRF_BufferPositions_startALTCMD = nRF_BufferPositions_startTime+4,
      nRF_BufferPositions_startRGBSPOTS = nRF_BufferPositions_startALTCMD + 1,
      nRF_BufferPositions_startWHITESPOTS = nRF_BufferPositions_startRGBSPOTS + 3,
      nRF_BufferPositions_startPIXELS = nRF_BufferPositions_startWHITESPOTS + 8
    } nRF_BufferPositions_t;

    volatile nRF_ReceiverModes_t nRF_Receiver_Mode = nRF_ReceiverModes_NormalUpdate;




#if defined(VERSION_TRANSMITTER) || defined(VERSION_REMOTE)
    /*  Sending commandset:
    * - set reading from sd card (A1)
    * - set test pattern (A2)
    * - begin song (reset rtc clock, start begin reading sd card) (A3)
    * - pauze everything (A4)
    * - resume everything (A5)
    * - reset RTC clock (B1)
    * - get rtc time (B2)
    * - set rtc time (B3)
    * - set repeat delay (B4)
    * - get repeat delay (B5)
    * - modify rtc time forward in seconds (B6)
    * - modify rtc time forward in subseconds (B7)
    * - modify rtc time backwards in seconds (B8)
    * - modify rtc time backwards in subseconds (B9)
    */
/*
    typedef enum nRF_RemoteLayer_e{
      nRF_RemoteLayer_A1 = 0x01,	// set reading from SD card	mode 1
      nRF_RemoteLayer_A2,		// set test pattern		mode 2
      nRF_RemoteLayer_A3,		// begin song (reset RTC clock, begin reading from SD card)	push mode 1
      nRF_RemoteLayer_A4,		// pauze everything		setting 1
      nRF_RemoteLayer_A5,		// resume everything		setting 1

      nRF_RemoteLayer_B1,		// reset RTC clock		action 1	setting 2
      nRF_RemoteLayer_B2,		// get RTC time			action 2	setting 2
      nRF_RemoteLayer_B3,		// set RTC time			action 3	setting 2
      nRF_RemoteLayer_B4,		// set repeat delay		action 4	setting 3
      nRF_RemoteLayer_B5,		// get repeat delay		action 5	setting 3
      nRF_RemoteLayer_B6,		// rtc forward seconds		action 6	setting 2
      nRF_RemoteLayer_B7,		// rtc forward subseconds	action 7	setting 2
      nRF_RemoteLayer_B8,		// rtc backwards seconds	action 8	setting 2
      nRF_RemoteLayer_B9,		// rtc backwards subseconds	action 9	setting 2
      nRF_RemoteLayer_B10		// rtc set time			action 10	setting 2
    }nRF_RemoteLayer_t;
    */


    typedef enum TRANS_STATES_t{
      TRANS_STATES_ReadSD,
      TRANS_STATES_ResetTimer,
      TRANS_STATES_Forward1s,
      TRANS_STATES_Backward1s,
      TRANS_STATES_Pauze,
      TRANS_STATES_Play
    } TRANS_STATES_t;

    TRANS_STATES_t TRANSMITTER_STATE = TRANS_STATES_ReadSD;
    TRANS_STATES_t TRANSMITTER_TEMP_STATE = TRANS_STATES_ReadSD;

    RTC_TimeTypeDef TRANSMITTER_PAUZE_TIMING;

    // prototypes
    void TRANS_StartTransmitter(void);
    void TRANS_ReadWaitBlock(void);

    void TRANS_MoveRTCForward(uint32_t millis);
    void TRANS_MoveRTCBackward(uint32_t millis);

#endif


    // objects
    IO_t nRF_LED;
    IO_t nRF_IRQ;

    // prototypes
    void initNRF(void);

    // prototypes version dependent
#if defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120)
    // receiver functionality
    /* Working on pipe 1 with no acknowledgment
     *
     * Receiving data and setting the data directly to the rgb spots && pixels
     */
    void nRF_RECEIVER(void);
#endif /* defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120) */

#if defined(VERSION_TRANSMITTER)
    // transmitter functionality
    /* Working on pipe 1 with no acknowledgement with receiver
     * Working on pipe 2 with acknowledgement with remote
     *
     * Getting data from SD card and sending it to the receivers
     * Receiving commands from the Remote for controls && sending data to remote
     */
    void nRF_TRANSMITTER(void);
#endif /* defined(VERSION_TRANSMITTER) */

#if defined(VERSION_REMOTE)
    // remote functionality
    /* Working on pipe 2 with acknowledgement with transmitter
     *
     * Sending commandset:
     * - set reading from sd card (A1)
     * - set test pattern (A2)
     * - begin song (reset rtc clock, start begin reading sd card) (A3)
     * - pauze everything (A4)
     * - resume everything (A5)
     * - reset RTC clock (B1)
     * - get rtc time (B2)
     * - set rtc time (B3)
     * - set repeat delay (B4)
     * - get repeat delay (B5)
     * - modify rtc time forward in seconds (B6)
     * - modify rtc time forward in subseconds (B7)
     * - modify rtc time backwards in seconds (B8)
     * - modify rtc time backwards in subseconds (B9)
     */
    void nRF_REMOTE(void);
#endif /* defined(VERSION_REMOTE) */


#endif /* FUNCTION_nRF */

// OLED screen
#ifdef FUNCTION_OLED
// OLED
  //	PA0	OLED_LED
  //	PA5	OLED_SCK (shared with SD)
  //	PA7	OLED_MOSI (shared with SD)
  //	PB0	OLED_RESET
  //	PB1	OLED_DC
  //	PA15	OLED_SS

    // variables

    // objects
    SCREEN_t OLED_SCREEN_;
    SPI_t OLED_SPI_;
    IO_t OLED_MISO;
    IO_t OLED_MOSI;
    IO_t OLED_SCK;
    IO_t OLED_SS;
    IO_t OLED_RST;
    IO_t OLED_DC;

    IO_t OLED_LED;

    SPI OLED_SPI(&SYS_IO, &OLED_SPI_, SPI1, &USART_UART, &USART_UART_);
    SCREEN OLED_SCREEN(&SYS_IO, &SYS_DELAY, &OLED_SPI, &OLED_SCREEN_, SCREEN_SSD1306_CONFIG_4SPI, &OLED_SPI_, &OLED_RST, &OLED_DC, &OLED_LED);

    // prototypes
    void initOLED(void);

    // function remote
      // menu with rtc of remote
      // setting everything locally and then sending stuff over

    // function transmitter
      // starting sequence
      // test pattern
      // every cmd that is possible with remote

#endif /* FUNCTION_ */

// External 8MHz crystal instead of HSI
#ifdef FUNCTION_OSC
// OSC
  //	PF0	OSC_IN
  //	PF1	OSC_OUT

    // variables

    // objects


#endif /* FUNCTION_ */

// Pushbuttons for user input
#ifdef FUNCTION_PB
// PUSHBUTTONS
  //	PA10	PB_B4	SW5	UP
  //	PA11	PB_B3	SW4	RIGHT
  //	PA12	PB_B2	SW3	LEFT
  //	PF6	PB_B1	SW2	DOWN
  //	PF7	PB_B0	SW1	ENTER

    // defines
#define PB_RECLICKDELAYMS 100

    // variables
    uint32_t PB_LASTCLICKTIME;

    // objects
    IO_t PB_UP;
    IO_t PB_DOWN;
    IO_t PB_LEFT;
    IO_t PB_RIGHT;
    IO_t PB_ENTER;

    // prototypes
    void PB_UP_pressed(void);
    void PB_DOWN_pressed(void);
    void PB_LEFT_pressed(void);
    void PB_RIGHT_pressed(void);
    void PB_ENTER_pressed(void);

    void PB_setClickTime(void);
    bool PB_isReady(void);

    void initPBs(void);


#endif /* FUNCTION_ */

// Real time clock for timing
#ifdef FUNCTION_RTC
// RTC
  //	PC13	RTC_LED
  //	PC14	RTC_IN
  //	PC15	RTC_OUT

    // variables
    RTC_TimeTypeDef RTC_TimeActual;
    RTC_AlarmTypeDef RTC_Alarm;
    volatile bool RTC_HoldTransmit;

    // objects
    IO_t RTC_LED;

    // prototypes
    void initRTC(void);
    void RTC_waitUntilTimeSD(void);

#endif /* FUNCTION_RTC */

// SD storage card
#ifdef FUNCTION_SD
// SD
  //	PA1	SD_LED
  //	PA4	SD_SS
  //	PA5	SD_SCK (shared with OLED)
  //	PA6	SD_MISO
  //	PA7	SD_MOSI (shared with OLED)

#define SD_SINGLEREAD 189

    // variables
    bool init = true;
    bool mounting = false;
    FRESULT SD_RESULT = FR_OK;
    FIL SD_FILE;
    UINT SD_bytesRead;
    uint8_t SD_Buffer[SD_SINGLEREAD];
    DWORD SD_blockPointer;
    uint8_t SD_initReturn;
    FATFS SD_filesystem;

    // enum
    enum SD_AltCommands_t{
      SD_AltCommands_Normal = 0x00,
      SD_AltCommands_Rainbow = 0x01,
      SD_AltCommands_RainbowFast = 0x02,
      SD_AltCommands_ExpandingPink = 0x03,
      SD_AltCommands_ExpandingYellow = 0x04,
      SD_AltCommands_ScanWhite = 0x05
    }SD_AltCommands_t;

/*
    FATFS fs;
    FILINFO fno;
    DIR dir;
    FIL F;
    UINT BytesWritten;
    UINT BytesRead;

    uint32_t* textbuffer;
    uint8_t resp;
*/

    // objects
    IO_t SD_LED;
    IO_t SD_SS;

    // prototypes
    void initSD(void);
    void SD_printResult(FRESULT);

#endif /* FUNCTION_SD */

// WS2812B led pixels
#ifdef FUNCTION_WS2812B
// WS2812B
  //	PA8	WS2812B_CH1
  //	PA9	WS2812B_CH2

    // variables
#define WS2812B_PWMDEADTIME 41

#ifdef VERSION_RECEIVER60
  #define WS2812B_CH1_PIXELS 30
  #define WS2812B_CH2_PIXELS 30
#elif defined(VERSION_RECEIVER120)
  #define WS2812B_CH1_PIXELS 60
  #define WS2812B_CH2_PIXELS 60
#else
  #define WS2812B_CH1_PIXELS 60
  #define WS2812B_CH2_PIXELS 60
#endif

    // objects
    IO_t WS2812B_CH1_PIN;
    uint8_t WS2812B_CH1_Bitbuffer[(WS2812B_CH1_PIXELS*24)+WS2812B_PWMDEADTIME];
    WS2812B WS2812B_CH1(&WS2812B_CH1_PIN, (uint32_t)WS2812B_CH1_PIXELS, WS2812B_CH1_Bitbuffer);

    IO_t WS2812B_CH2_PIN;
    uint8_t WS2812B_CH2_Bitbuffer[(WS2812B_CH2_PIXELS*24)+WS2812B_PWMDEADTIME];
    WS2812B WS2812B_CH2(&WS2812B_CH2_PIN, (uint32_t)WS2812B_CH2_PIXELS, WS2812B_CH2_Bitbuffer);

    // prototypes
    void initWS(void);

#endif /* FUNCTION_WS2812B */




// application variables
    char itemtemp[18];






