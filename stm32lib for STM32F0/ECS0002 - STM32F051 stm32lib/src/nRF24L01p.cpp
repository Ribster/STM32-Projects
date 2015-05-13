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
#include "nRF24L01p.h"

namespace stm32lib {


nRF24L01p::nRF24L01p(nRF24L01p_t* nRFStructure, SPI_t* SPIStruct, SPI* SPIObject, IO* IOObject){
	setNRF(nRFStructure);
	setSPI(SPIStruct, SPIObject);
	setIO(IOObject);
}
nRF24L01p::~nRF24L01p(){

}

void nRF24L01p::setNRF(nRF24L01p_t* nRFStructure){
	nRFObj = nRFStructure;
}

void nRF24L01p::setSPI(SPI_t* SPIstructure, SPI* SPIobject){
	nRFObj->SPIStruct = SPIstructure;
	nRFObj->SPIObject = SPIobject;
}

void nRF24L01p::setIO(IO* IOObject){
	nRFObj->IOObject = IOObject;
}
void nRF24L01p::setCE(IO_t* pin){
	nRFObj->pin_CE = pin;
}
void nRF24L01p::setIRQ(IO_t* pin){
	nRFObj->pin_IRQ = pin;
}
void nRF24L01p::setLED(IO_t* pin){
	nRFObj->pin_LED = pin;
}

void nRF24L01p::begin(void){
	begin(nRFObj->pin_CE, nRFObj->pin_IRQ, nRFObj->pin_LED);
}

void nRF24L01p::begin(IO_t* pin_CE, IO_t* pin_IRQ, IO_t* pin_LED){
	if(nRFObj->SPIStruct->initDone == false){
		printf("error on nRF init SPI");
		return;
	}
	if (pin_CE->init == false || pin_IRQ->init == false || pin_LED->init == false){
		printf("error on nRF init pins");
		return;
	}
	nRFObj->pin_CE = pin_CE;
	nRFObj->pin_IRQ = pin_IRQ;
	nRFObj->pin_LED = pin_LED;
	setSS(true);
	setCE(false);
}
void nRF24L01p::init(void){
	init(nRFObj->channel, nRFObj->payloadSize, nRFObj->dataRate, nRFObj->outputPower);
}

void nRF24L01p::init(uint8_t channel, uint8_t payload, nRF24L01p_DataRate_t datarate, nRF24L01p_OutputPower_t outputPower){
    nRFObj->channel = channel;
    nRFObj->payloadSize = payload;
    nRFObj->dataRate = datarate;
    nRFObj->outputPower = outputPower;

    // Set RF Channel
    setRFChannel(channel);
    printf("rf channel set to: %d \r\n", getRFChannel());

    setRegister(nRF24L01p_Registers_Config, nRF24L01p_RegisterBits_Config_Preset);

    // Length of incoming payload
    setPayloadSize(0x00, 0);
    printf("plsize channel 0 set to: %d \r\n", getPayloadSize(0));
    setPayloadSize(payload, 1);
    printf("plsize channel 1 set to: %d \r\n", getPayloadSize(1));
    setPayloadSize(0x00, 2);
    printf("plsize channel 2 set to: %d \r\n", getPayloadSize(2));
    setPayloadSize(0x00, 3);
    printf("plsize channel 3 set to: %d \r\n", getPayloadSize(3));
    setPayloadSize(0x00, 4);
    printf("plsize channel 4 set to: %d \r\n", getPayloadSize(4));
    setPayloadSize(0x00, 5);
    printf("plsize channel 5 set to: %d \r\n", getPayloadSize(5));

    // datarate set && gain set
    setDataRate(datarate);
    printf("datarate set to: %d \r\n", getDataRate());
    setOutputPower(outputPower);
    printf("outputpower set to: %d \r\n", getOutputPower());

    // CRC enable, 1 byte CRC length
    setCRCLength(1);
    printf("crc set to: %d \r\n", getCRCLength());

    // feature
    setRegister(nRF24L01p_Registers_FEATURE, 0x01);


    // auto acknowledgement
    setRegister(nRF24L01p_Registers_EN_AA, 0x00);

    // Enable RX addresses
    setRegister(nRF24L01p_Registers_EN_RXAddr, 0x03);


    setRegister(nRF24L01p_Registers_Setup_Retr, 0x00);


    setRegister(nRF24L01p_Registers_DYNPD, 0x00);

    // start listening
    setPowerUpRX();

}

void nRF24L01p::setRFChannel(uint8_t newVal){
	setRegister(nRF24L01p_Registers_RF_CH, newVal);
}
uint8_t nRF24L01p::getRFChannel(void){
	return getRegister(nRF24L01p_Registers_RF_CH);
}

void nRF24L01p::setPayloadSize(uint8_t size, uint8_t pipe){
	uint8_t checkSize = size;

	if(checkSize > 32){
		checkSize = 32;

	}

	setAllocBuffer(checkSize);

	switch (pipe){
		case 0x00:
			setRegister(nRF24L01p_Registers_RX_PW_P0, checkSize);
			break;
		case 0x01:
			setRegister(nRF24L01p_Registers_RX_PW_P1, checkSize);
			break;
		case 0x02:
			setRegister(nRF24L01p_Registers_RX_PW_P2, checkSize);
			break;
		case 0x03:
			setRegister(nRF24L01p_Registers_RX_PW_P3, checkSize);
			break;
		case 0x04:
			setRegister(nRF24L01p_Registers_RX_PW_P4, checkSize);
			break;
		case 0x05:
			setRegister(nRF24L01p_Registers_RX_PW_P5, checkSize);
			break;
	}
}
void nRF24L01p::setPayloadSize(uint8_t size){
	setPayloadSize(0x00, 0); // for RX ack
	setPayloadSize(size, 1); // pipe one
}
uint8_t nRF24L01p::getPayloadSize(uint8_t pipe){
	switch (pipe){
		case 0x00:
			return getRegister(nRF24L01p_Registers_RX_PW_P0);
			break;
		case 0x01:
			return getRegister(nRF24L01p_Registers_RX_PW_P1);
			break;
		case 0x02:
			return getRegister(nRF24L01p_Registers_RX_PW_P2);
			break;
		case 0x03:
			return getRegister(nRF24L01p_Registers_RX_PW_P3);
			break;
		case 0x04:
			return getRegister(nRF24L01p_Registers_RX_PW_P4);
			break;
		case 0x05:
			return getRegister(nRF24L01p_Registers_RX_PW_P5);
			break;
	}
	return 0x00;
}
uint8_t nRF24L01p::getPayloadSize(void){
	return getPayloadSize(1);
}

void nRF24L01p::setOutputPower(nRF24L01p_OutputPower_t newVal){
	uint8_t tmp = getRegister(nRF24L01p_Registers_RF_Setup);
	tmp &= ~(0x3<<nRF24L01p_RegisterBits_RF_PWR);
	tmp |= (newVal << nRF24L01p_RegisterBits_RF_PWR);
	setRegister(nRF24L01p_Registers_RF_Setup, tmp);
	nRFObj->outputPower = newVal;
}
nRF24L01p_OutputPower_t nRF24L01p::getOutputPower(void){
	uint8_t tmp = (getRegister(nRF24L01p_Registers_RF_Setup) & (0x3<<nRF24L01p_RegisterBits_RF_PWR))>>nRF24L01p_RegisterBits_RF_PWR;
	switch (tmp){
	case nRF24L01p_OutputPower_M18dBm:
	case nRF24L01p_OutputPower_M12dBm:
	case nRF24L01p_OutputPower_M6dBm:
	case nRF24L01p_OutputPower_0dBm:
		return (nRF24L01p_OutputPower_t)tmp;
		break;
	}
	return nRF24L01p_OutputPower_M18dBm;
}

void nRF24L01p::setDataRate(nRF24L01p_DataRate_t newVal){
	uint8_t tmp = getRegister(nRF24L01p_Registers_RF_Setup);
	tmp &= ~(1<<nRF24L01p_RegisterBits_RF_DR_LOW | 1<<nRF24L01p_RegisterBits_RF_DR_HIGH);
	tmp |= (newVal<<nRF24L01p_RegisterBits_RF_DR_HIGH);
	setRegister(nRF24L01p_Registers_RF_Setup, tmp);
	nRFObj->dataRate = newVal;
}
nRF24L01p_DataRate_t nRF24L01p::getDataRate(void){
	uint8_t tmp = ( getRegister(nRF24L01p_Registers_RF_Setup) & (1<<nRF24L01p_RegisterBits_RF_DR_LOW | 1<<nRF24L01p_RegisterBits_RF_DR_HIGH) ) >> nRF24L01p_RegisterBits_RF_DR_HIGH;
	switch (tmp){
		case nRF24L01p_DataRate_1M:
		case nRF24L01p_DataRate_2M:
		case nRF24L01p_DataRate_250k:
			return (nRF24L01p_DataRate_t)tmp;
			break;
	}
	return (nRF24L01p_DataRate_Reserved);
}

void nRF24L01p::setCRCLength(uint8_t newVal){
	uint8_t tmp = getRegister(nRF24L01p_Registers_Config);
	tmp &= ~(1<<nRF24L01p_RegisterBits_Config_EN_CRC | 1<<nRF24L01p_RegisterBits_Config_CRCO);
	switch (newVal){
	case 0x01:
		// enable CRC set to 1
		tmp |= (1<<nRF24L01p_RegisterBits_Config_EN_CRC | 0<<nRF24L01p_RegisterBits_Config_CRCO);
		break;
	case 0x02:
		// enable CRC set to 2
		tmp |= (1<<nRF24L01p_RegisterBits_Config_EN_CRC | 1<<nRF24L01p_RegisterBits_Config_CRCO);
		break;
	}
	setRegister(nRF24L01p_Registers_Config, tmp);
	nRFObj->crclength = newVal;
}
uint8_t nRF24L01p::getCRCLength(void){
	uint8_t tmp = getRegister(nRF24L01p_Registers_Config);
	if(tmp & 1<<nRF24L01p_RegisterBits_Config_EN_CRC){
		if(tmp & 1<<nRF24L01p_RegisterBits_Config_CRCO){
			return 2;
		} else {
			return 1;
		}
	}
	return 0;
}

void nRF24L01p::setAutoAcknowledgement(bool newVal){
	setAutoAcknowledgement(1, newVal);
}

void nRF24L01p::setAutoAcknowledgement(uint8_t pipe, bool newVal){
	switch (pipe){
		case 0x00:
			setRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P0, newVal);
			break;
		case 0x01:
			setRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P1, newVal);
			break;
		case 0x02:
			setRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P2, newVal);
			break;
		case 0x03:
			setRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P3, newVal);
			break;
		case 0x04:
			setRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P4, newVal);
			break;
		case 0x05:
			setRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P5, newVal);
			break;
	}
}
bool nRF24L01p::getAutoAcknowledgement(void){
	return getAutoAcknowledgement(1);
}
bool nRF24L01p::getAutoAcknowledgement(uint8_t pipe){
	switch (pipe){
		case 0x00:
			return getRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P0);
			break;
		case 0x01:
			return getRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P1);
			break;
		case 0x02:
			return getRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P2);
			break;
		case 0x03:
			return getRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P3);
			break;
		case 0x04:
			return getRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P4);
			break;
		case 0x05:
			return getRegisterBit(nRF24L01p_Registers_EN_AA, nRF24L01p_RegisterBits_ENAA_P5);
			break;
	}
	return false;
}

void nRF24L01p::setRetransmissionDelay(uint16_t delayVal){
	uint8_t tmp = getRegister(nRF24L01p_Registers_Setup_Retr) & 0x0F;
	uint8_t times = (delayVal / 250);
	if( !(times%250) ){
		times --;
	}
	setRegister(nRF24L01p_Registers_Setup_Retr, tmp | (times << nRF24L01p_RegisterBits_ARD));
}
uint16_t nRF24L01p::getRetransmissionDelay(void){
	uint8_t tmp = getRegister(nRF24L01p_Registers_Setup_Retr) & 0xF0;
	return ( (tmp >> nRF24L01p_RegisterBits_ARD) + 1) * 250;
}

void nRF24L01p::setRetransmissionAttempts(uint8_t newVal){
	uint8_t tmp = getRegister(nRF24L01p_Registers_Setup_Retr) & 0xF0;
	if(newVal < 0x10){
		setRegister(nRF24L01p_Registers_Setup_Retr, tmp | newVal);
	} else {
		setRegister(nRF24L01p_Registers_Setup_Retr, tmp | 0x0F);
	}
}
uint8_t nRF24L01p::getRetransmissionAttempts(void){
	uint8_t tmp = getRegister(nRF24L01p_Registers_Setup_Retr) & 0x0F;
	return tmp;
}

void nRF24L01p::setDynamicPayloadLength(bool newVal){
	setDynamicPayloadLength(1, newVal);
}
void nRF24L01p::setDynamicPayloadLength(uint8_t pipe, bool newVal){
	switch (pipe){
		case (0x00):
				setRegisterBit(nRF24L01p_Registers_DYNPD, nRF24L01p_RegisterBits_DPL_P0, newVal);
				break;
		case (0x01):
				setRegisterBit(nRF24L01p_Registers_DYNPD, nRF24L01p_RegisterBits_DPL_P1, newVal);
				break;
		case (0x02):
				setRegisterBit(nRF24L01p_Registers_DYNPD, nRF24L01p_RegisterBits_DPL_P2, newVal);
				break;
		case (0x03):
				setRegisterBit(nRF24L01p_Registers_DYNPD, nRF24L01p_RegisterBits_DPL_P3, newVal);
				break;
		case (0x04):
				setRegisterBit(nRF24L01p_Registers_DYNPD, nRF24L01p_RegisterBits_DPL_P4, newVal);
				break;
		case (0x05):
				setRegisterBit(nRF24L01p_Registers_DYNPD, nRF24L01p_RegisterBits_DPL_P5, newVal);
				break;
	}
}
uint8_t nRF24L01p::getDynamicPayloadLength(void){
	uint8_t* tmp = new uint8_t[2];
	uint8_t* rec = new uint8_t[2];
	uint8_t returnVal;
	*tmp = nRF24L01p_Commands_R_RX_PL_WID;
	*(tmp+1) = 0x00;
	nRFObj->SPIObject->print(tmp, rec, 2);
	returnVal = *(rec+1);
	free(tmp);
	free(rec);
	return returnVal;
}
uint8_t nRF24L01p::getDynamicPayloadLength(uint8_t pipe){
	switch (pipe){
	case 0x00:
		return getRegister(nRF24L01p_Registers_RX_ADDR_P0);
		break;
	case 0x01:
		return getRegister(nRF24L01p_Registers_RX_ADDR_P1);
		break;
	case 0x02:
		return getRegister(nRF24L01p_Registers_RX_ADDR_P2);
		break;
	case 0x03:
		return getRegister(nRF24L01p_Registers_RX_ADDR_P3);
		break;
	case 0x04:
		return getRegister(nRF24L01p_Registers_RX_ADDR_P4);
		break;
	case 0x05:
		return getRegister(nRF24L01p_Registers_RX_ADDR_P5);
		break;
	}
	return 0x00;
}

void nRF24L01p::setAddressSize(uint8_t size){
	if(size == 3){
		nRFObj->addressSize = size;
		setRegister(nRF24L01p_Registers_Setup_AW, 0x01<<nRF24L01p_RegisterBits_AW);
	} else if (size == 4){
		nRFObj->addressSize = size;
		setRegister(nRF24L01p_Registers_Setup_AW, 0x02<<nRF24L01p_RegisterBits_AW);
	} else if (size == 5){
		nRFObj->addressSize = size;
		setRegister(nRF24L01p_Registers_Setup_AW, 0x03<<nRF24L01p_RegisterBits_AW);
	} else {
		nRFObj->addressSize = 5;
		setRegister(nRF24L01p_Registers_Setup_AW, 0x03<<nRF24L01p_RegisterBits_AW);
	}
}
uint8_t nRF24L01p::getAddressSize(void){
	uint8_t tmp = getRegister(nRF24L01p_Registers_Setup_AW) & 0x03;
	switch (tmp){
	case 0x01:
		return 3;
		break;
	case 0x02:
		return 4;
		break;
	case 0x03:
		return 5;
		break;
	}
	return 0;
}

void nRF24L01p::setTXAddress(uint8_t* buffer, uint8_t size){
    /* RX_ADDR_P0 must be set to the sending addr for auto ack to work. */

    setRXAddress(0, buffer, size);
    setRegister(nRF24L01p_Registers_TX_ADDR, buffer, size);
}

void nRF24L01p::getTXAddress(uint8_t* returnptr){
	uint8_t* tmp = new uint8_t[6];
	*tmp = getReceiveCommand(nRF24L01p_Registers_TX_ADDR);
	memset((void*)(tmp+1), 0, 5);
	nRFObj->SPIObject->print(tmp, returnptr, 6);
	free(tmp);
}

void nRF24L01p::setRXAddress(uint8_t* buffer, uint8_t size){
	setRXAddress(1, buffer, size);
}

void nRF24L01p::setRXAddress(uint8_t pipe, uint8_t* buffer, uint8_t size){
	nRF24L01p_Registers_t tmp = nRF24L01p_Registers_Config;
	if(pipe>5){
		return;
	}

	switch (pipe){
	case (0x00):
			tmp = nRF24L01p_Registers_RX_ADDR_P0;
			break;
	case (0x01):
			tmp = nRF24L01p_Registers_RX_ADDR_P1;
			break;
	case (0x02):
			tmp = nRF24L01p_Registers_RX_ADDR_P2;
			break;
	case (0x03):
			tmp = nRF24L01p_Registers_RX_ADDR_P3;
			break;
	case (0x04):
			tmp = nRF24L01p_Registers_RX_ADDR_P4;
			break;
	case (0x05):
			tmp = nRF24L01p_Registers_RX_ADDR_P5;
			break;
	}

	setCE(false);
		setRegister(tmp, buffer, size);
	setCE(true);
}
void nRF24L01p::getRXAddress(uint8_t* returnptr, uint8_t pipe){
	uint8_t* tmp = new uint8_t[6];
	if(pipe>5){
		return;
	}
	switch (pipe){
	case (0x00):
			*tmp = nRF24L01p_Registers_RX_ADDR_P0;
			break;
	case (0x01):
			*tmp = nRF24L01p_Registers_RX_ADDR_P1;
			break;
	case (0x02):
			*tmp = nRF24L01p_Registers_RX_ADDR_P2;
			break;
	case (0x03):
			*tmp = nRF24L01p_Registers_RX_ADDR_P3;
			break;
	case (0x04):
			*tmp = nRF24L01p_Registers_RX_ADDR_P4;
			break;
	case (0x05):
			*tmp = nRF24L01p_Registers_RX_ADDR_P5;
			break;
	}
	memset((void*)(tmp+1), 0, 5);
	nRFObj->SPIObject->print(tmp, returnptr, 6);
	free(tmp);
}

void nRF24L01p::setEnableRXAddress(uint8_t pipe, bool newVal){
	switch (pipe){
		case (0x00):
				setRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P0, newVal);
				break;
		case (0x01):
				setRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P1, newVal);
				break;
		case (0x02):
				setRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P2, newVal);
				break;
		case (0x03):
				setRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P3, newVal);
				break;
		case (0x04):
				setRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P4, newVal);
				break;
		case (0x05):
				setRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P5, newVal);
				break;
	}
}

bool nRF24L01p::getEnableRXAddress(uint8_t pipe){
	switch (pipe){
		case (0x00):
				return getRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P0);
				break;
		case (0x01):
				return getRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P1);
				break;
		case (0x02):
				return getRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P2);
				break;
		case (0x03):
				return getRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P3);
				break;
		case (0x04):
				return getRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P4);
				break;
		case (0x05):
				return getRegisterBit(nRF24L01p_Registers_EN_RXAddr, nRF24L01p_RegisterBits_ERX_P5);
				break;
	}
	return false;
}

void nRF24L01p::setEnableInterrupt(nRF24L01p_Interrupts_t interruptSelection, bool newVal){
	switch (interruptSelection){
		case (nRF24L01p_Interrupts_DataReady):
				setRegisterBit(nRF24L01p_Registers_Config, nRF24L01p_RegisterBits_Config_RX_DR, !newVal);
			break;
		case (nRF24L01p_Interrupts_DataSet):
				setRegisterBit(nRF24L01p_Registers_Config, nRF24L01p_RegisterBits_Config_TX_DS, !newVal);
			break;
		case (nRF24L01p_Interrupts_MaxRetransmission):
				setRegisterBit(nRF24L01p_Registers_Config, nRF24L01p_RegisterBits_Config_MAX_RT, !newVal);
			break;
	}
}

void nRF24L01p::setEnableDynamicAcknowledgement(bool newVal){
	setRegisterBit(nRF24L01p_Registers_FEATURE, nRF24L01p_RegisterBits_FEATURE_EN_DYN_ACK, newVal);
}
bool nRF24L01p::getEnableDynamicAcknowledgement(void){
	return getRegisterBit(nRF24L01p_Registers_FEATURE, nRF24L01p_RegisterBits_FEATURE_EN_DYN_ACK);
}

void nRF24L01p::setEnablePayloadAcknowledgement(bool newVal){
	setRegisterBit(nRF24L01p_Registers_FEATURE, nRF24L01p_RegisterBits_FEATURE_EN_ACK_PAY, newVal);
}
bool nRF24L01p::getEnablePayloadAcknowledgement(void){
	return getRegisterBit(nRF24L01p_Registers_FEATURE, nRF24L01p_RegisterBits_FEATURE_EN_ACK_PAY);
}

void nRF24L01p::setEnableDynamicPayloadLength(bool newVal){
	setRegisterBit(nRF24L01p_Registers_FEATURE, nRF24L01p_RegisterBits_FEATURE_EN_DPL, newVal);
}
bool nRF24L01p::getEnableDynamicPayloadLength(void){
	return getRegisterBit(nRF24L01p_Registers_FEATURE, nRF24L01p_RegisterBits_FEATURE_EN_DPL);
}

bool nRF24L01p::getInterruptStatus(nRF24L01p_Interrupts_t interruptSelection){
	switch (interruptSelection){
		case (nRF24L01p_Interrupts_DataReady):
				return getRegisterBit(nRF24L01p_Registers_Status, nRF24L01p_RegisterBits_Status_RX_DR);
			break;
		case (nRF24L01p_Interrupts_DataSet):
				return getRegisterBit(nRF24L01p_Registers_Status, nRF24L01p_RegisterBits_Status_TX_DS);
			break;
		case (nRF24L01p_Interrupts_MaxRetransmission):
				return getRegisterBit(nRF24L01p_Registers_Status, nRF24L01p_RegisterBits_Status_MAX_RT);
			break;
	}
	return false;
}
bool nRF24L01p::getInterruptConfig(nRF24L01p_Interrupts_t interruptSelection){
	switch (interruptSelection){
		case (nRF24L01p_Interrupts_DataReady):
				return !getRegisterBit(nRF24L01p_Registers_Config, nRF24L01p_RegisterBits_Config_RX_DR);
			break;
		case (nRF24L01p_Interrupts_DataSet):
				return !getRegisterBit(nRF24L01p_Registers_Config, nRF24L01p_RegisterBits_Config_TX_DS);
			break;
		case (nRF24L01p_Interrupts_MaxRetransmission):
				return !getRegisterBit(nRF24L01p_Registers_Config, nRF24L01p_RegisterBits_Config_MAX_RT);
			break;
	}
	return false;
}

void nRF24L01p::setInterruptClear(nRF24L01p_Interrupts_t interruptSelection){
	switch (interruptSelection){
		case (nRF24L01p_Interrupts_DataReady):
				setRegisterBit(nRF24L01p_Registers_Status, nRF24L01p_RegisterBits_Status_RX_DR, true);
			break;
		case (nRF24L01p_Interrupts_DataSet):
				setRegisterBit(nRF24L01p_Registers_Status, nRF24L01p_RegisterBits_Status_TX_DS, true);
			break;
		case (nRF24L01p_Interrupts_MaxRetransmission):
				setRegisterBit(nRF24L01p_Registers_Status, nRF24L01p_RegisterBits_Status_MAX_RT, true);
			break;
	}
}
void nRF24L01p::setInterruptClearAll(void){
	uint8_t tmp = getStatus();
	tmp |= ( (1<<nRF24L01p_RegisterBits_Status_RX_DR) | (1<<nRF24L01p_RegisterBits_Status_TX_DS) | (1<<nRF24L01p_RegisterBits_Status_MAX_RT) );

	setRegister(nRF24L01p_Registers_Status, tmp);
}

void nRF24L01p::setPowerUpRX(void){
	setFlushRX();
	setInterruptClearAll();
	uint8_t tmp = getRegister(nRF24L01p_Registers_Config);
	tmp |= ( (1<<nRF24L01p_RegisterBits_Config_PWR_UP) | (1<<nRF24L01p_RegisterBits_Config_PRIM_RX) );
	setCE(false);
	setRegister(nRF24L01p_Registers_Config, tmp);
	setCE(true);

}
void nRF24L01p::setPowerUpTX(void){
	setInterruptClearAll();
	uint8_t tmp = getRegister(nRF24L01p_Registers_Config);
	tmp |= (1<<nRF24L01p_RegisterBits_Config_PWR_UP);
	tmp &= ~(1<<nRF24L01p_RegisterBits_Config_PRIM_RX);
	setRegister(nRF24L01p_Registers_Config, tmp);
}
void nRF24L01p::setPowerDown(void){
	setCE(false);
	uint8_t tmp = getRegister(nRF24L01p_Registers_Config);
	tmp &= ~(1<<nRF24L01p_RegisterBits_Config_PWR_UP);
	tmp &= ~(1<<nRF24L01p_RegisterBits_Config_PRIM_RX);
	setRegister(nRF24L01p_Registers_Config, tmp);
}

bool nRF24L01p::getDataReady(void){
	return getRegisterBit(nRF24L01p_Registers_Status, nRF24L01p_RegisterBits_Status_RX_DR);
}
bool nRF24L01p::getMaxRetransmits(void){
	return getRegisterBit(nRF24L01p_Registers_Status, nRF24L01p_RegisterBits_Status_MAX_RT);
}
bool nRF24L01p::getTransmitDone(void){
	return getRegisterBit(nRF24L01p_Registers_Status, nRF24L01p_RegisterBits_Status_TX_DS);
}

bool nRF24L01p::getRXFIFOEmpty(void){
	return getRegisterBit(nRF24L01p_Registers_FIFO_Status, nRF24L01p_RegisterBits_FIFO_RX_Empty);
}
bool nRF24L01p::getRXFIFOFull(void){
	return getRegisterBit(nRF24L01p_Registers_FIFO_Status, nRF24L01p_RegisterBits_FIFO_RX_Full);
}
bool nRF24L01p::getTXFIFOEmpty(void){
	return getRegisterBit(nRF24L01p_Registers_FIFO_Status, nRF24L01p_RegisterBits_FIFO_TX_Empty);
}
bool nRF24L01p::getTXFIFOFull(void){
	return getRegisterBit(nRF24L01p_Registers_FIFO_Status, nRF24L01p_RegisterBits_FIFO_TX_Full);
}
uint8_t nRF24L01p::getPayloadWidth(void){
	return (getCommand(nRF24L01p_Commands_R_RX_PL_WID));
}
bool nRF24L01p::getIsSending(void){
	uint8_t tmp = getStatus();
	if ( (tmp & (1<<nRF24L01p_RegisterBits_Status_TX_DS)) && (tmp & (1<<nRF24L01p_RegisterBits_Status_MAX_RT)) ){
		return false;
	}
	return true;
}
uint8_t nRF24L01p::getStatus(void){
	return getRegister(nRF24L01p_Registers_Status);
}
nRF24L01p_Transmit_Status_t nRF24L01p::getLastMessageStatus(void){
	uint8_t tmp = getStatus();
	if( tmp & (1<<nRF24L01p_RegisterBits_Status_TX_DS) ){
		return nRF24L01p_Transmit_Status_Ok;
	} else if ( tmp & (1<<nRF24L01p_RegisterBits_Status_MAX_RT) ){
		return nRF24L01p_Transmit_Status_Lost;
	}
	return nRF24L01p_Transmit_Status_Sending;
}

void nRF24L01p::setData(void){
	setData(nRFObj->nrfBuffer, nRFObj->payloadSize, nRF24L01p_Commands_W_TX_Payload);
}

void nRF24L01p::setData(uint8_t* buffer){
	setData(buffer, nRFObj->payloadSize, nRF24L01p_Commands_W_TX_Payload);
}

void nRF24L01p::setDataNoAck(void){
	setData(nRFObj->nrfBuffer, nRFObj->payloadSize, nRF24L01p_Commands_W_TX_Payload_No_Ack);
}

void nRF24L01p::setData(uint8_t* buffer, uint8_t size, nRF24L01p_Commands_t cmd){
	printregister(nRF24L01p_Registers_Status);
	setCE(false);

	setPowerUpTX();

	setFlushTX();

	setSS(false);
	nRFObj->SPIObject->printnoSS(cmd);
	nRFObj->SPIObject->printnoSS(buffer, size);
	setSS(true);

	setCE(true);
	printregister(nRF24L01p_Registers_Status);
	printregister(nRF24L01p_Registers_Status);
	printregister(nRF24L01p_Registers_Status);
	printregister(nRF24L01p_Registers_Status);
	printregister(nRF24L01p_Registers_Status);
	setPowerDown();
	setPowerUpRX();
}

void nRF24L01p::getData(uint8_t* buffer, uint8_t size){

	uint8_t* tmp = new uint8_t[size+1];
	uint8_t* rec = new uint8_t[size+1];
	*tmp = nRF24L01p_Commands_R_RX_Payload;
	memset((void*) (tmp+1), 0, size);

	nRFObj->SPIObject->print(tmp, rec, size+1);

	memcpy((void*) buffer, (rec+1), size);

	free(tmp);
	free(rec);
}

void nRF24L01p::setFlushTX(void){
	setCommand(nRF24L01p_Commands_Flush_TX);
}
void nRF24L01p::setFlushRX(void){
	setCommand(nRF24L01p_Commands_Flush_RX);
}

void nRF24L01p::printregister(nRF24L01p_Registers_t reg){
	printf("register 0x%X has value 0x%X \r\n", reg, getRegister(reg));
}

void nRF24L01p::interruptHandler(void){
	// STUB
}

void nRF24L01p::setCE(bool newVal){
	nRFObj->IOObject->setValue(nRFObj->pin_CE, newVal);
}
void nRF24L01p::setLED(bool newVal){
	nRFObj->IOObject->setValue(nRFObj->pin_LED, newVal);
}
void nRF24L01p::getIRQ(bool newVal){
	nRFObj->IOObject->setValue(nRFObj->pin_IRQ, newVal);
}
void nRF24L01p::setSS(bool newVal){
	nRFObj->IOObject->setValue(nRFObj->SPIStruct->pin_SS, newVal);
}

void nRF24L01p::setRegister(nRF24L01p_Registers_t registers, uint8_t newVal){
	uint8_t tmp[2] = {getSendCommand(registers),newVal};
	nRFObj->SPIObject->print(tmp, 2);
}
void nRF24L01p::setRegister(nRF24L01p_Registers_t registers, uint8_t* buffer, uint8_t size){
	uint8_t* tmp = new uint8_t[size+1];
	memcpy((void*)(tmp+1),(void*)buffer,size);
	*tmp = getSendCommand(registers);
	nRFObj->SPIObject->print(tmp, size+1);
	free(tmp);
}
uint8_t nRF24L01p::getRegister(nRF24L01p_Registers_t registers){
	uint8_t tmp[2] = {getReceiveCommand(registers),0xFF};
	uint8_t rec[2] = {0x00, 0x00};
	nRFObj->SPIObject->print(tmp, rec, 2);
	return rec[1];
}

void nRF24L01p::setRegisterBit(nRF24L01p_Registers_t registers, uint8_t bitloc, bool newVal){
	uint8_t tmp = getRegister(registers);
	if(newVal){
		tmp |= (1<<bitloc);
	} else {
		tmp &= ~(1<<bitloc);
	}
	nRFObj->SPIObject->print(tmp);
}
bool nRF24L01p::getRegisterBit(nRF24L01p_Registers_t registers, uint8_t bitloc){
	uint8_t tmp = getRegister(registers);
	if(tmp & (1<<bitloc)){
		return true;
	}
	return false;
}

void nRF24L01p::setCommand(nRF24L01p_Commands_t cmd){
	nRFObj->SPIObject->print(cmd);
}
uint8_t nRF24L01p::getCommand(nRF24L01p_Commands_t cmd){
	uint8_t* tmp = new uint8_t[2];
	uint8_t* rec = new uint8_t[2];
	uint8_t returnVal;
	*tmp = cmd;
	*(tmp+1) = 0x00;
	nRFObj->SPIObject->print(tmp, rec, 2);
	returnVal = *(rec+1);
	free(tmp);
	free(rec);
	return returnVal;
}

uint8_t nRF24L01p::getSendCommand(nRF24L01p_Registers_t registers){
	return ( registers & 0x1F ) | 0x20 ;
}
uint8_t nRF24L01p::getReceiveCommand(nRF24L01p_Registers_t registers){
	return ( registers & 0x1F );
}

void nRF24L01p::setAllocBuffer(uint8_t newSize){
	if( newSize != nRFObj->payloadSize){
		free(nRFObj->nrfBuffer);
		nRFObj->nrfBuffer = new uint8_t[newSize];
		nRFObj->payloadSize = newSize;
	}
}
}
