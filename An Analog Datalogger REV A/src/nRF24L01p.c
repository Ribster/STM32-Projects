#include "nRF24L01p.h"


uint8_t payload_len;

/* init the hardware pins */
void
initialize_nRF24L01p(void){
	// intialize pins
		// CE PIN
		gpio_initStandard(
				nRF_CE_PORT,
				nRF_CE_MODE,
				nRF_CE_OTYPE,
				nRF_CE_PIN,
				nRF_CE_PULL,
				nRF_CE_SPEED);
		GPIO_WriteBit(nRF_CE_PORT, (1<<nRF_CE_PIN), nRF_CE_INITSTATE);
		// IRQ PIN
		gpio_initStandard(
				nRF_IRQ_PORT,
				nRF_IRQ_MODE,
				nRF_IRQ_OTYPE,
				nRF_IRQ_PIN,
				nRF_IRQ_PULL,
				nRF_IRQ_SPEED);
		nvic_initInterrupt(
				nRF_IRQ_EXTI_Line,
				0xFF,
				0);
		// SS PIN
		gpio_initAF(
				nRF_MISO_PORT,
				nRF_MISO_MODE,
				nRF_MISO_OTYPE,
				nRF_MISO_PIN,
				nRF_MISO_PULL,
				nRF_MISO_SPEED,
				nRF_MISO_AF);
		// MISO PIN
		gpio_initAF(
				nRF_MISO_PORT,
				nRF_MISO_MODE,
				nRF_MISO_OTYPE,
				nRF_MISO_PIN,
				nRF_MISO_PULL,
				nRF_MISO_SPEED,
				nRF_MISO_AF);
		// MOSI PIN
		gpio_initAF(
				nRF_MOSI_PORT,
				nRF_MOSI_MODE,
				nRF_MOSI_OTYPE,
				nRF_MOSI_PIN,
				nRF_MOSI_PULL,
				nRF_MOSI_SPEED,
				nRF_MOSI_AF);
		// SCK PIN
		gpio_initAF(
				nRF_SCK_PORT,
				nRF_SCK_MODE,
				nRF_SCK_OTYPE,
				nRF_SCK_PIN,
				nRF_SCK_PULL,
				nRF_SCK_SPEED,
				nRF_SCK_AF);

	// initialize SPI
		//rcc_setSPIClock(AFE_SPI, ENABLE);

		initialize_SPI(
				nRF_SPI,
				nRF_SPI_Direction,
				nRF_SPI_Mode,
				nRF_SPI_DataSize,
				nRF_SPI_CPOL,
				nRF_SPI_CPHA,
				nRF_SPI_NSS,
				nRF_SPI_BaudRatePrescaler,
				nRF_SPI_FirstBit,
				nRF_SPI_CRCPolynomial);
		SPI_Cmd(nRF_SPI, ENABLE);


}

/* configure the module */
void nrf24_config(uint8_t channel, uint8_t pay_length)
{
    /* Use static payload length ... */
    payload_len = pay_length;

    // Set RF channel
    nrf24_configRegister(RF_CH,channel);

    // Set length of incoming payload
    nrf24_configRegister(RX_PW_P0, 0x00); // Auto-ACK pipe ...
    nrf24_configRegister(RX_PW_P1, payload_len); // Data payload pipe
    nrf24_configRegister(RX_PW_P2, 0x00); // Pipe not used
    nrf24_configRegister(RX_PW_P3, 0x00); // Pipe not used
    nrf24_configRegister(RX_PW_P4, 0x00); // Pipe not used
    nrf24_configRegister(RX_PW_P5, 0x00); // Pipe not used

    // 1 Mbps, TX gain: 0dbm
    nrf24_configRegister(RF_SETUP, (0x4<<RF_DR)|((1)<<RF_PWR));

    // CRC enable, 1 byte CRC length
    nrf24_configRegister(CONFIG,nrf24_CONFIG);

    // feature
    nrf24_configRegister(FEATURE,0x01);

    // Auto Acknowledgment
    nrf24_configRegister(EN_AA,(0<<ENAA_P0)|(0<<ENAA_P1)|(0<<ENAA_P2)|(0<<ENAA_P3)|(0<<ENAA_P4)|(0<<ENAA_P5));

    // Enable RX addresses
    nrf24_configRegister(EN_RXADDR,(1<<ERX_P0)|(1<<ERX_P1)|(0<<ERX_P2)|(0<<ERX_P3)|(0<<ERX_P4)|(0<<ERX_P5));

    // Auto retransmit delay: 1000 us and Up to 15 retransmit trials
    nrf24_configRegister(SETUP_RETR,(0x00<<ARD)|(0x00<<ARC));

    // Dynamic length configurations: No dynamic length
    nrf24_configRegister(DYNPD,(0<<DPL_P0)|(0<<DPL_P1)|(0<<DPL_P2)|(0<<DPL_P3)|(0<<DPL_P4)|(0<<DPL_P5));

    // Start listening
    nrf24_powerUpRx();
}

/* Set the RX address */
void nrf24_rx_address(uint8_t * adr)
{
    CE_L();
    nrf24_writeRegister(RX_ADDR_P1,adr,nrf24_ADDR_LEN);
    CE_H();
}

/* Returns the payload length */
uint8_t nrf24_payload_length()
{
    return payload_len;
}

/* Set the TX address */
void nrf24_tx_address(uint8_t* adr)
{
    /* RX_ADDR_P0 must be set to the sending addr for auto ack to work. */
    nrf24_writeRegister(RX_ADDR_P0,adr,nrf24_ADDR_LEN);
    nrf24_writeRegister(TX_ADDR,adr,nrf24_ADDR_LEN);
}

/* Checks if data is available for reading */
/* Returns 1 if data is ready ... */
uint8_t nrf24_dataReady()
{
    // See note in getData() function - just checking RX_DR isn't good enough
    uint8_t status = nrf24_getStatus();

    // We can short circuit on RX_DR, but if it's not set, we still need
    // to check the FIFO for any pending packets
    if ( status & (1 << RX_DR) )
    {
        return 1;
    }

    return !nrf24_rxFifoEmpty();
}

/* Checks if receive FIFO is empty or not */
uint8_t nrf24_rxFifoEmpty()
{
    uint8_t fifoStatus;

    nrf24_readRegister(FIFO_STATUS,&fifoStatus,1);

    return (fifoStatus & (1 << RX_EMPTY));
}

/* Returns the length of data waiting in the RX fifo */
uint8_t nrf24_payloadLength()
{
    uint8_t status;
    CSN_L();
    spi_transfer(R_RX_PL_WID);
    status = spi_transfer(0x00);
    CSN_H();
    return status;
}

/* Reads payload bytes into data array */
void nrf24_getData(uint8_t* data)
{
    /* Pull down chip select */
    CSN_L();

    /* Send cmd to read rx payload */
    spi_transfer( R_RX_PAYLOAD );

    /* Read payload */
    nrf24_transferSync(data,data,payload_len);

    /* Pull up chip select */
    CSN_H();

    /* Reset status register */
    nrf24_configRegister(STATUS,(1<<RX_DR));
}

void nrf24_clearInterrupts(void){
    uint8_t rv;
    nrf24_readRegister(STATUS,&rv,1);
    rv |= (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT);
	nrf24_writeRegister(STATUS, &rv,1);
}

/* Returns the number of retransmissions occured for the last message */
uint8_t nrf24_retransmissionCount()
{
    uint8_t rv;
    nrf24_readRegister(OBSERVE_TX,&rv,1);
    rv = rv & 0x0F;
    return rv;
}

// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
void nrf24_send(uint8_t* value)
{
    /* Go to Standby-I first */
    CE_L();

    /* Set to transmitter mode , Power up if needed */
    nrf24_powerUpTx();

    /* Do we really need to flush TX fifo each time ? */
    #if 1
        /* Pull down chip select */
        CSN_L();

        /* Write cmd to flush transmit FIFO */
        spi_transfer(FLUSH_TX);

        /* Pull up chip select */
        CSN_H();
    #endif

    /* Pull down chip select */
    CSN_L();

    /* Write cmd to write payload */
    spi_transfer(nRF24_CMD_W_TX_PAYLOAD);

    /* Write payload */
    nrf24_transmitSync(value,payload_len);

    /* Pull up chip select */
    CSN_H();

    /* Start the transmission */
    CE_H();
}

uint8_t nrf24_isSending()
{
    uint8_t status;

    /* read the current status */
    status = nrf24_getStatus();

    /* if sending successful (TX_DS) or max retries exceded (MAX_RT). */
    if((status & ((1 << TX_DS)  | (1 << MAX_RT))))
    {
    	nrf24_clearInterrupts();
        return 0; /* false */
    }

    return 1; /* true */

}

uint8_t nrf24_getStatus()
{
    uint8_t rv;
    CSN_L();
    rv = spi_transfer(NOP);
    CSN_H();
    return rv;
}

uint8_t nrf24_lastMessageStatus()
{
    uint8_t rv;

    rv = nrf24_getStatus();

    /* Transmission went OK */
    if((rv & ((1 << TX_DS))))
    {
        return NRF24_TRANSMISSON_OK;
    }
    /* Maximum retransmission count is reached */
    /* Last message probably went missing ... */
    else if((rv & ((1 << MAX_RT))))
    {
        return NRF24_MESSAGE_LOST;
    }
    /* Probably still sending ... */
    else
    {
        return 0xFF;
    }
}

void nrf24_powerUpRx()
{
    CSN_L();
    spi_transfer(FLUSH_RX);
    CSN_H();

    nrf24_configRegister(STATUS,(1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT));

    CE_L();
    nrf24_configRegister(CONFIG,nrf24_CONFIG|((1<<PWR_UP)|(1<<PRIM_RX)));
    CE_H();
}

void nrf24_powerUpTx()
{
    nrf24_configRegister(STATUS,(1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT));

    nrf24_configRegister(CONFIG,nrf24_CONFIG|((1<<PWR_UP)|(0<<PRIM_RX)));
}

void nrf24_powerDown()
{
    CE_L();
    nrf24_configRegister(CONFIG,nrf24_CONFIG);
}

/* software spi routine */
uint8_t spi_transfer(uint8_t tx)
{
    while (SPI_I2S_GetFlagStatus(nRF_SPI, SPI_I2S_FLAG_TXE) == RESET); // Wait while DR register is not empty
    SPI_I2S_SendData(nRF_SPI, tx); // Send byte to SPI
    while (SPI_I2S_GetFlagStatus(nRF_SPI, SPI_I2S_FLAG_RXNE) == RESET); // Wait to receive byte
    return SPI_I2S_ReceiveData(nRF_SPI); // Read byte from SPI bus
}

/* send and receive multiple bytes over SPI */
void nrf24_transferSync(uint8_t* dataout,uint8_t* datain,uint8_t len)
{
    uint8_t i;

    for(i=0;i<len;i++)
    {
        datain[i] = spi_transfer(dataout[i]);
    }

}

/* send multiple bytes over SPI */
void nrf24_transmitSync(uint8_t* dataout,uint8_t len)
{
    uint8_t i;

    for(i=0;i<len;i++)
    {
        spi_transfer(dataout[i]);
    }

}

/* Clocks only one byte into the given nrf24 register */
void nrf24_configRegister(uint8_t reg, uint8_t value)
{
    CSN_L();
    spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
    spi_transfer(value);
    CSN_H();
}

/* Read single register from nrf24 */
void nrf24_readRegister(uint8_t reg, uint8_t* value, uint8_t len)
{
    CSN_L();
    spi_transfer(R_REGISTER | (REGISTER_MASK & reg));
    nrf24_transferSync(value,value,len);
    CSN_H();
}

/* Write to a single register of nrf24 */
void nrf24_writeRegister(uint8_t reg, uint8_t* value, uint8_t len)
{
    CSN_L();
    spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
    nrf24_transmitSync(value,len);
    CSN_H();
}

void
CE_L(void){
	GPIO_WriteBit(nRF_SS_PORT, nRF_SS_PIN, Bit_RESET);
}

void
CE_H(void){
	GPIO_WriteBit(nRF_SS_PORT, nRF_SS_PIN, Bit_SET);
}

void
CSN_L(void){
	GPIO_WriteBit(nRF_CE_PORT, nRF_CE_PIN, Bit_RESET);
}

void
CSN_H(void){
	GPIO_WriteBit(nRF_CE_PORT, nRF_CE_PIN, Bit_SET);
}
