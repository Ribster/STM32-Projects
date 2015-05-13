/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* This library is based on this library:
*   https://github.com/aaronds/arduino-nrf24l01
* Which is based on this library:
*   http://www.tinkerer.eu/AVRLib/nRF24L01
* -----------------------------------------------------------------------------
*/

#include "stm32f0xx.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_rcc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "SPI.h"
#include "IO.h"
#include "DELAY.h"

// new lib

namespace stm32lib{

typedef enum nRF24L01p_Transmit_Status_t{
	nRF24L01p_Transmit_Status_Lost = 0,		//Message is lost, reached max retransmissions
	nRF24L01p_Transmit_Status_Ok = 1,			//Message sent successfully
	nRF24L01p_Transmit_Status_Sending = 0xFF	//Message is still sending
}nRF24L01p_Transmit_Status_t;

typedef enum nRF24L01p_DataRate_t{
	nRF24L01p_DataRate_2M = 0x01,		// 2Mbps
	nRF24L01p_DataRate_1M = 0x00,		// 1Mbps
	nRF24L01p_DataRate_250k = 0x04,		// 250kbps
	nRF24L01p_DataRate_Reserved = 0x05	// Reserved
}nRF24L01p_DataRate_t;

typedef enum nRF24L01p_OutputPower_t{
	nRF24L01p_OutputPower_M18dBm = 0x00,	// -18dBm
	nRF24L01p_OutputPower_M12dBm = 0x01,	// -12dBm
	nRF24L01p_OutputPower_M6dBm = 0x02,	// -6dBm
	nRF24L01p_OutputPower_0dBm = 0x03	// 0dBm
}nRF24L01p_OutputPower_t;

typedef enum nRF24L01p_Commands_t{
	nRF24L01p_Commands_Register = 0x1F,
	nRF24L01p_Commands_R_RX_Payload = 0x61,
	nRF24L01p_Commands_W_TX_Payload = 0xA0,
	nRF24L01p_Commands_Flush_TX = 0xE1,
	nRF24L01p_Commands_Flush_RX = 0xE2,
	nRF24L01p_Commands_Reuse_TX_PL = 0xE3,
	nRF24L01p_Commands_R_RX_PL_WID = 0x60,
	nRF24L01p_Commands_W_Ack_Payload = 0xA8,
	nRF24L01p_Commands_W_TX_Payload_No_Ack = 0xB0,
	nRF24L01p_Commands_NOP = 0xFF,
	nRF24L01p_Commands_Activate = 0x50
}nRF24L01p_Commands_t;

typedef enum nRF24L01p_Interrupts_t{
	nRF24L01p_Interrupts_DataReady,
	nRF24L01p_Interrupts_DataSet,
	nRF24L01p_Interrupts_MaxRetransmission
}nRF24L01p_Interrupts_t;

typedef enum nRF24L01p_RegisterBits_t{
	// configuration register
	nRF24L01p_RegisterBits_Config_RX_DR = 6,
	nRF24L01p_RegisterBits_Config_TX_DS = 5,
	nRF24L01p_RegisterBits_Config_MAX_RT = 4,
	nRF24L01p_RegisterBits_Config_EN_CRC = 3,
	nRF24L01p_RegisterBits_Config_CRCO = 2,
	nRF24L01p_RegisterBits_Config_PWR_UP = 1,
	nRF24L01p_RegisterBits_Config_PRIM_RX = 0,
	nRF24L01p_RegisterBits_Config_Preset = ((1 << nRF24L01p_RegisterBits_Config_EN_CRC) | (0 << nRF24L01p_RegisterBits_Config_CRCO)),
	// enable auto acknowledgement
	nRF24L01p_RegisterBits_ENAA_P5 = 5,
	nRF24L01p_RegisterBits_ENAA_P4 = 4,
	nRF24L01p_RegisterBits_ENAA_P3 = 3,
	nRF24L01p_RegisterBits_ENAA_P2 = 2,
	nRF24L01p_RegisterBits_ENAA_P1 = 1,
	nRF24L01p_RegisterBits_ENAA_P0 = 0,
	// enable rx addresses
	nRF24L01p_RegisterBits_ERX_P5 = 5,
	nRF24L01p_RegisterBits_ERX_P4 = 4,
	nRF24L01p_RegisterBits_ERX_P3 = 3,
	nRF24L01p_RegisterBits_ERX_P2 = 2,
	nRF24L01p_RegisterBits_ERX_P1 = 1,
	nRF24L01p_RegisterBits_ERX_P0 = 0,
	// setup of address width
	nRF24L01p_RegisterBits_AW = 0, 		// 2 bits
	// setup of auto re-transmission
	nRF24L01p_RegisterBits_ARD = 4,		// 4 bits
	nRF24L01p_RegisterBits_ARC = 0,		// 4 bits
	// RF setup register
	nRF24L01p_RegisterBits_RF_PLL_LOCK = 4,
	nRF24L01p_RegisterBits_RF_DR_LOW = 5,
	nRF24L01p_RegisterBits_RF_DR_HIGH = 3,
	nRF24L01p_RegisterBits_RF_DR = 3,
	nRF24L01p_RegisterBits_RF_PWR = 1,		// 2 bits
	// general status register
	nRF24L01p_RegisterBits_Status_RX_DR = 6,
	nRF24L01p_RegisterBits_Status_TX_DS = 5,
	nRF24L01p_RegisterBits_Status_MAX_RT = 4,
	nRF24L01p_RegisterBits_Status_RX_P_NO = 1,	// 3 bits
	nRF24L01p_RegisterBits_Status_TX_FULL = 0,
	// transmit observe register
	nRF24L01p_RegisterBits_TO_PLOS_CNT = 4,	// 4 bits
	nRF24L01p_RegisterBits_TO_ARC_CNT = 0,	// 4 bits
	// FIFO status
	nRF24L01p_RegisterBits_FIFO_TX_Reuse = 6,
	nRF24L01p_RegisterBits_FIFO_TX_Full = 5,
	nRF24L01p_RegisterBits_FIFO_TX_Empty = 4,
	nRF24L01p_RegisterBits_FIFO_RX_Full = 1,
	nRF24L01p_RegisterBits_FIFO_RX_Empty = 0,
	// dynamic payload length
	nRF24L01p_RegisterBits_DPL_P0 = 0,
	nRF24L01p_RegisterBits_DPL_P1 = 1,
	nRF24L01p_RegisterBits_DPL_P2 = 2,
	nRF24L01p_RegisterBits_DPL_P3 = 3,
	nRF24L01p_RegisterBits_DPL_P4 = 4,
	nRF24L01p_RegisterBits_DPL_P5 = 5,
	// transmitter power
	nRF24L01p_RegisterBits_RFPWR_m18dBm = 0,	// -18 dBm
	nRF24L01p_RegisterBits_RFPWR_m12dBm = 1,	// -12 dBm
	nRF24L01p_RegisterBits_RFPWR_m6dBm = 2,		// -6dBm
	nRF24L01p_RegisterBits_RFPWR_0dBm = 3,		// 0dBm
	// data rates
	nRF24L01p_RegisterBits_DR_2Mbps = 0,
	nRF24L01p_RegisterBits_DR_1Mbps = 1,
	nRF24L01p_RegisterBits_DR_250Kbps = 2,
	// feature register
	nRF24L01p_RegisterBits_FEATURE_EN_DYN_ACK = 0,
	nRF24L01p_RegisterBits_FEATURE_EN_ACK_PAY = 1,
	nRF24L01p_RegisterBits_FEATURE_EN_DPL = 2
}nRF24L01p_RegisterBits_t;

typedef enum nRF24L01p_Registers_t{
	nRF24L01p_Registers_Config = 0x00,		//Configuration Register
	nRF24L01p_Registers_EN_AA = 0x01,		//Enable ‘Auto Acknowledgment’ Function
	nRF24L01p_Registers_EN_RXAddr = 0x02,	//Enabled RX Addresses
	nRF24L01p_Registers_Setup_AW = 0x03,	//Setup of Address Widths (common for all data pipes)
	nRF24L01p_Registers_Setup_Retr = 0x04,	//Setup of Automatic Retransmission
	nRF24L01p_Registers_RF_CH = 0x05,		//RF Channel
	nRF24L01p_Registers_RF_Setup = 0x06,	//RF Setup Register
	nRF24L01p_Registers_Status = 0x07,		//Status Register
	nRF24L01p_Registers_Observe_TX = 0x08,	//Transmit observe register
	nRF24L01p_Registers_RPD = 0x09,
	nRF24L01p_Registers_RX_ADDR_P0 = 0x0A,	//Receive address data pipe 0. 5 Bytes maximum length.
	nRF24L01p_Registers_RX_ADDR_P1 = 0x0B,	//Receive address data pipe 1. 5 Bytes maximum length.
	nRF24L01p_Registers_RX_ADDR_P2 = 0x0C,	//Receive address data pipe 2. Only LSB
	nRF24L01p_Registers_RX_ADDR_P3 = 0x0D,	//Receive address data pipe 3. Only LSB
	nRF24L01p_Registers_RX_ADDR_P4 = 0x0E,	//Receive address data pipe 4. Only LSB
	nRF24L01p_Registers_RX_ADDR_P5 = 0x0F,	//Receive address data pipe 5. Only LSB
	nRF24L01p_Registers_TX_ADDR = 0x10,		//Transmit address. Used for a PTX device only
	nRF24L01p_Registers_RX_PW_P0 = 0x11,
	nRF24L01p_Registers_RX_PW_P1 = 0x12,
	nRF24L01p_Registers_RX_PW_P2 = 0x13,
	nRF24L01p_Registers_RX_PW_P3 = 0x14,
	nRF24L01p_Registers_RX_PW_P4 = 0x15,
	nRF24L01p_Registers_RX_PW_P5 = 0x16,
	nRF24L01p_Registers_FIFO_Status = 0x17,	//FIFO Status Register
	nRF24L01p_Registers_DYNPD = 0x1C,		//Enable dynamic payload length
	nRF24L01p_Registers_FEATURE = 0x1D
}nRF24L01p_Registers_t;

struct nRF24L01p_t{
	SPI_t* SPIStruct;
	SPI* SPIObject;
	IO* IOObject;
	IO_t* pin_CE;
	IO_t* pin_IRQ;
	IO_t* pin_LED;

	bool initDone = false;

	uint8_t payloadSize;				//Payload size
	uint8_t* nrfBuffer = NULL;
	uint8_t channel;					//Channel selected
	uint8_t crclength;

	nRF24L01p_OutputPower_t outputPower;		//Output power
	nRF24L01p_DataRate_t dataRate;		//Data rate

	uint8_t addressSize;
	uint8_t rxAddress[5];
	uint8_t txAddress[5];
};

class nRF24L01p{
public:
	nRF24L01p(nRF24L01p_t*, SPI_t*, SPI*, IO*);
	~nRF24L01p();

	void setSPI(SPI_t*, SPI*);
	void setNRF(nRF24L01p_t*);
	void setIO(IO*);
	void setCE(IO_t*);
	void setIRQ(IO_t*);
	void setLED(IO_t*);
	void begin(void);
	void begin(IO_t* pin_CE, IO_t* pin_IRQ, IO_t* pin_LED);
	void init(uint8_t channel, uint8_t payload, nRF24L01p_DataRate_t datarate, nRF24L01p_OutputPower_t outputPower);
	void init(void);

public:
	void setRFChannel(uint8_t);
	uint8_t getRFChannel(void);

	void setPayloadSize(uint8_t size, uint8_t pipe);
	void setPayloadSize(uint8_t);
	uint8_t getPayloadSize(uint8_t pipe);
	uint8_t getPayloadSize(void);

	void setOutputPower(nRF24L01p_OutputPower_t);
	nRF24L01p_OutputPower_t getOutputPower(void);

	void setDataRate(nRF24L01p_DataRate_t);
	nRF24L01p_DataRate_t getDataRate(void);

	void setCRCLength(uint8_t);
	uint8_t getCRCLength(void);

	void setAutoAcknowledgement(bool);
	void setAutoAcknowledgement(uint8_t pipe, bool);
	bool getAutoAcknowledgement(void);
	bool getAutoAcknowledgement(uint8_t pipe);

	void setRetransmissionDelay(uint16_t);
	uint16_t getRetransmissionDelay(void);

	void setRetransmissionAttempts(uint8_t);
	uint8_t getRetransmissionAttempts(void);

	void setDynamicPayloadLength(bool);
	void setDynamicPayloadLength(uint8_t pipe, bool);
	uint8_t getDynamicPayloadLength(void);
	uint8_t getDynamicPayloadLength(uint8_t pipe);

	void setAddressSize(uint8_t size);
	uint8_t getAddressSize(void);

	void setTXAddress(uint8_t* buffer, uint8_t size);
	void getTXAddress(uint8_t*);

	void setRXAddress(uint8_t pipe, uint8_t* buffer, uint8_t size);
	void setRXAddress(uint8_t* buffer, uint8_t size);
	void getRXAddress(uint8_t*, uint8_t pipe);

	void setEnableRXAddress(uint8_t pipe, bool);
	bool getEnableRXAddress(uint8_t pipe);

	void setEnableInterrupt(nRF24L01p_Interrupts_t, bool);

	void setEnableDynamicAcknowledgement(bool);
	bool getEnableDynamicAcknowledgement(void);

	void setEnablePayloadAcknowledgement(bool);
	bool getEnablePayloadAcknowledgement(void);

	void setEnableDynamicPayloadLength(bool);
	bool getEnableDynamicPayloadLength(void);

	bool getInterruptStatus(nRF24L01p_Interrupts_t);
	bool getInterruptConfig(nRF24L01p_Interrupts_t);

	void setInterruptClear(nRF24L01p_Interrupts_t);
	void setInterruptClearAll(void);

	void setPowerUpRX(void);
	void setPowerUpTX(void);
	void setPowerDown(void);

	bool getDataReady(void);
	bool getMaxRetransmits(void);
	bool getTransmitDone(void);
	bool getRXFIFOEmpty(void);
	bool getRXFIFOFull(void);
	bool getTXFIFOEmpty(void);
	bool getTXFIFOFull(void);
	uint8_t getPayloadWidth(void);
	bool getIsSending(void);
	uint8_t getStatus(void);
	nRF24L01p_Transmit_Status_t getLastMessageStatus(void);

	void setData(void);
	void setData(uint8_t*);
	void setDataNoAck(void);
	void setData(uint8_t*, uint8_t, nRF24L01p_Commands_t);

	void getData(uint8_t*,uint8_t);

	void interruptHandler(void);

	void setFlushTX(void);
	void setFlushRX(void);

	void printregister(nRF24L01p_Registers_t);


private:
	void setCE(bool);
	void setLED(bool);
	void getIRQ(bool);
	void setSS(bool);

	void setRegister(nRF24L01p_Registers_t, uint8_t);
	void setRegister(nRF24L01p_Registers_t, uint8_t*, uint8_t);
	uint8_t getRegister(nRF24L01p_Registers_t);

	void setRegisterBit(nRF24L01p_Registers_t, uint8_t, bool);
	bool getRegisterBit(nRF24L01p_Registers_t, uint8_t);

	void setCommand(nRF24L01p_Commands_t);
	uint8_t getCommand(nRF24L01p_Commands_t);

	uint8_t getSendCommand(nRF24L01p_Registers_t);
	uint8_t getReceiveCommand(nRF24L01p_Registers_t);

	void setAllocBuffer(uint8_t);

	nRF24L01p_t* nRFObj = NULL;

};

}




