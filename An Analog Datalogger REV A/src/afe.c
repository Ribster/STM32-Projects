/*
 * afe.c
 *
 *  Created on: 21-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "afe.h"

// buffers
volatile uint8_t afe_DMA_RX_Buffer[AFE_DMA_CAPTURES][AFE_DMA_BLOCKS][AFE_DMA_BLOCKSIZE];
volatile uint8_t afe_DMA_TX_Buffer[AFE_DMA_BLOCKS][AFE_DMA_BLOCKSIZE];

// general afe variables
volatile uint8_t afe_busy;
volatile uint8_t afe_enableRecording;
volatile uint32_t afe_recordingTime;
volatile timeRegistration_t afe_recordedTime;
volatile uint32_t afe_recordingPacketCount;
volatile uint8_t afe_currentPacket;
volatile uint8_t afe_currentCapture;
volatile uint8_t afe_loggingCapture;		// current capture for SD write
volatile uint8_t afe_timeStorage[4];
volatile char afe_writingFolderString[13];
volatile char afe_writingFileString[13];

// DMA settings
DMA_InitTypeDef afe_struct;

void
initialize_AFE(void){
	// clear buffers
	memset(afe_DMA_RX_Buffer, 0, AFE_DMA_CAPTURES*AFE_DMA_BLOCKS*AFE_DMA_BLOCKSIZE);

	// set transmit buffers
	for (uint32_t i = 0; i < AFE_DMA_BLOCKS; ++i) {
		for (uint32_t j = 0; j < AFE_DMA_BLOCKSIZE; ++j) {
			afe_DMA_TX_Buffer[i][j] = j%256;
		}
	}

	// set clocks
	rcc_setDMAClock(AFE_DMA_RX_DMAStream, ENABLE);
	rcc_setDMAClock(AFE_DMA_TX_DMAStream, ENABLE);

	// enable interrupts
	NVIC_EnableIRQ(AFE_DMA_TX_DMAIRQ);
	NVIC_EnableIRQ(AFE_DMA_RX_DMAIRQ);
	nvic_initInterrupt(AFE_DMA_RX_DMAIRQ,
			0,
			1);

	nvic_initInterrupt(AFE_DMA_TX_DMAIRQ,
			0,
			0);

	// intialize pins

		// MISO PIN
		gpio_initAF(
				AFE_MISO_PORT,
				AFE_MISO_MODE,
				AFE_MISO_OTYPE,
				AFE_MISO_PIN,
				AFE_MISO_PULL,
				AFE_MISO_SPEED,
				AFE_MISO_AF);
		// MOSI PIN
		gpio_initAF(
				AFE_MOSI_PORT,
				AFE_MOSI_MODE,
				AFE_MOSI_OTYPE,
				AFE_MOSI_PIN,
				AFE_MOSI_PULL,
				AFE_MOSI_SPEED,
				AFE_MOSI_AF);
		// SCK PIN
		gpio_initAF(
				AFE_SCK_PORT,
				AFE_SCK_MODE,
				AFE_SCK_OTYPE,
				AFE_SCK_PIN,
				AFE_SCK_PULL,
				AFE_SCK_SPEED,
				AFE_SCK_AF);

		// CS PIN
		gpio_initStandard(
				AFE_CS_PORT,
				AFE_CS_MODE,
				AFE_CS_OTYPE,
				AFE_CS_PIN,
				AFE_CS_PULL,
				AFE_CS_SPEED);
		// INT PIN
		gpio_initStandard(
				AFE_INT_PORT,
				AFE_INT_MODE,
				AFE_INT_OTYPE,
				AFE_INT_PIN,
				AFE_INT_PULL,
				AFE_INT_SPEED);
		//GPIO_WriteBit(AFE_INT_PORT, (1<<AFE_INT_PIN), AFE_INT_INITSTATE);
		AFE_INT_PORT->ODR &= ~(1<<AFE_INT_PIN);
		// SYNC 1
		gpio_initStandard(
				AFE_SYNC1_PORT,
				AFE_SYNC1_MODE,
				AFE_SYNC1_OTYPE,
				AFE_SYNC1_PIN,
				AFE_SYNC1_PULL,
				AFE_SYNC1_SPEED);
		//GPIO_WriteBit(AFE_SYNC1_PORT, (1<<AFE_SYNC1_PIN), AFE_SYNC1_INITSTATE);
		// SYNC 2
		gpio_initStandard(
				AFE_SYNC2_PORT,
				AFE_SYNC2_MODE,
				AFE_SYNC2_OTYPE,
				AFE_SYNC2_PIN,
				AFE_SYNC2_PULL,
				AFE_SYNC2_SPEED);

		// SYNC 3
		gpio_initStandard(
				AFE_SYNC3_PORT,
				AFE_SYNC3_MODE,
				AFE_SYNC3_OTYPE,
				AFE_SYNC3_PIN,
				AFE_SYNC3_PULL,
				AFE_SYNC3_SPEED);

		// SYNC EXT
		gpio_initStandard(
				AFE_EXT_SYNC_PORT,
				AFE_EXT_SYNC_MODE,
				AFE_EXT_SYNC_OTYPE,
				AFE_EXT_SYNC_PIN,
				AFE_EXT_SYNC_PULL,
				AFE_EXT_SYNC_SPEED);

	// initialize SPI
		//rcc_setSPIClock(AFE_SPI, ENABLE);
		initialize_SPI(
				AFE_SPI,
				AFE_SPI_Direction,
				AFE_SPI_Mode,
				AFE_SPI_DataSize,
				AFE_SPI_CPOL,
				AFE_SPI_CPHA,
				AFE_SPI_NSS,
				AFE_SPI_BaudRatePrescaler,
				AFE_SPI_FirstBit,
				AFE_SPI_CRCPolynomial);
		//SPI_Cmd(AFE_SPI, ENABLE);

		// setup common DMA between RX && TX

		afe_struct.DMA_FIFOMode = AFE_DMA_FIFOMode ;
		afe_struct.DMA_FIFOThreshold = AFE_DMA_FIFOThreshold ;
		afe_struct.DMA_MemoryBurst = AFE_DMA_MemoryBurst ;
		afe_struct.DMA_MemoryDataSize = AFE_DMA_MemoryDataSize;
		afe_struct.DMA_MemoryInc = AFE_DMA_MemoryInc;
		afe_struct.DMA_Mode = AFE_DMA_Mode;

		afe_struct.DMA_PeripheralBaseAddr = AFE_DMA_PeripheralBaseAddr;
		afe_struct.DMA_PeripheralBurst = AFE_DMA_PeripheralBurst;
		afe_struct.DMA_PeripheralDataSize = AFE_DMA_PeripheralDataSize;
		afe_struct.DMA_MemoryDataSize = AFE_DMA_MemoryDataSize;
		afe_struct.DMA_PeripheralInc = AFE_DMA_PeripheralInc;
		afe_struct.DMA_Priority = AFE_DMA_Priority;

		// initializations
		afe_currentCapture = 0x00;
		afe_currentPacket = 0x00;
		afe_enableRecording = 0x00;
		memset((uint8_t*)afe_timeStorage, 0, sizeof(afe_timeStorage));
}

void
afe_startRecording(void){
	// testing
	printf("Starting with recording!!\r\n");

	// set the updating bit for the OLED screen
	ssd1306_updateLater_1s = 0x01;
	// reset the recording time
	afe_recordingTime = 0x00;
	// reset the recording packet count
	afe_recordingPacketCount = 0x01;
	// determine the new folder name
	//afe_getNewFolderName();
	// create a new folder with this name
	//afe_createNewFolder();
	// write the info.txt file
	//afe_writeInfoHeader();
	// set the enable recording bit for the AFE as last
	afe_enableRecording = 0x01;
}

void
afe_stopRecording(void){
	// testing
	printf("Stopping with recording!!\r\n");
	// write the information to the info.txt file
	//afe_writeInfoFooter();
	// disable the recording
	afe_enableRecording = 0x00;
}

void
afe_startReadout(void){

	// reset current packet
	afe_currentPacket = 0x00;

	// reset transmission capture block
	if(afe_currentCapture >= (AFE_DMA_CAPTURES)){
		afe_currentCapture = 0x0;
	}

	// start the reading routine
	afe_read();
}

void
afe_endReadout(void){
	// if the amount of blocks is sent increase packet
	afe_currentPacket++;
	// some debugging
#ifdef DBGIO
	printf("%d\r\n", afe_currentPacket);
#endif

	if(afe_currentPacket == (AFE_DMA_BLOCKS/5) ){
		// set the sync pin low
		AFE_INT_PORT->ODR &= ~(1<<AFE_INT_PIN);

	}

	if(afe_currentPacket == AFE_DMA_BLOCKS){
		// increase the capture for next transmission
#ifdef DBGIO
		printf("Read packet %d done\r\n", afe_currentPacket);
#endif


		// increment the capture packet
		afe_currentCapture++;
		// increment the total packet count
		afe_recordingPacketCount++;

#ifdef DBGIO
		printf("Current capture %d\r\n", afe_currentCapture);
#endif



		// logging
		if(afe_currentCapture==AFE_DMA_SINGLEFILE || afe_currentCapture == (AFE_DMA_CAPTURES)){
			// add time stamp to blocks
			for(uint8_t i=0; i<AFE_DMA_SINGLEFILE; i++){
				if(afe_currentCapture == AFE_DMA_SINGLEFILE){
					afe_DMA_RX_Buffer[0+i][0][0] = afe_timeStorage[0];
					afe_DMA_RX_Buffer[0+i][0][1] = afe_timeStorage[1];
					afe_DMA_RX_Buffer[0+i][0][2] = afe_timeStorage[2];
					afe_DMA_RX_Buffer[0+i][0][3] = afe_timeStorage[3];
				} else {
					afe_DMA_RX_Buffer[AFE_DMA_SINGLEFILE+i][0][0] = afe_timeStorage[0];
					afe_DMA_RX_Buffer[AFE_DMA_SINGLEFILE+i][0][1] = afe_timeStorage[1];
					afe_DMA_RX_Buffer[AFE_DMA_SINGLEFILE+i][0][2] = afe_timeStorage[2];
					afe_DMA_RX_Buffer[AFE_DMA_SINGLEFILE+i][0][3] = afe_timeStorage[3];
				}

			}

			// set the logging bit high.
			// writing gets done in the main loop
			// if not, interrupt time will be too long and problems will arrive.
			if(afe_currentCapture == AFE_DMA_SINGLEFILE){
				afe_loggingCapture = 0;
			} else {
				afe_loggingCapture = AFE_DMA_SINGLEFILE;
			}
			sd_writeAFE = 0x01;
		}
	} else {
		// read the AFE
		// perpetual loop until the packet count is met
		afe_read();
	}

}

void
afe_read(void){
	// sanity check for the execution of the routine
	if(afe_busy != 0x00){
		return;
	}

	// set busy flag
	afe_busy = 0x01;
	leds_setLed(ledList_Blue, ENABLE);

#ifdef DBGIO
	printf("tx ptr: 0x%lx, rx ptr: 0x%lx\r\n", (uint32_t)&(afe_DMA_TX_Buffer[afe_currentPacket][0]), (uint32_t)&(afe_DMA_RX_Buffer[afe_currentCapture][afe_currentPacket][0]));
#endif

	// deinit the DMA
	DMA_DeInit(AFE_DMA_TX_DMAStream);
	DMA_DeInit(AFE_DMA_RX_DMAStream);

	//set buffersize
	if(afe_currentPacket!=(AFE_DMA_BLOCKS-1)){
		afe_struct.DMA_BufferSize = AFE_DMA_BLOCKSIZE;
	} else {
		afe_struct.DMA_BufferSize = AFE_DMA_LASTPACKET;
	}

	afe_struct.DMA_Channel = AFE_DMA_TX_DMAChannel;
	afe_struct.DMA_DIR = AFE_DMA_TX_DIR;
	afe_struct.DMA_Memory0BaseAddr = (uint32_t)&(afe_DMA_TX_Buffer[afe_currentPacket][0]);
	//DMA_Cmd(AFE_DMA_TX_DMAStream, DISABLE);
	//while (AFE_DMA_TX_DMAStream->CR & DMA_SxCR_EN);
	DMA_Init(AFE_DMA_TX_DMAStream, &afe_struct);

	// RX STREAM
		// buffersize already set
	afe_struct.DMA_Channel = AFE_DMA_RX_DMAChannel;
	afe_struct.DMA_DIR = AFE_DMA_RX_DIR;
	afe_struct.DMA_Memory0BaseAddr = (uint32_t)&(afe_DMA_RX_Buffer[afe_currentCapture][afe_currentPacket][0]);
	//DMA_Cmd(AFE_DMA_RX_DMAStream, DISABLE);
	//while (AFE_DMA_RX_DMAStream->CR & DMA_SxCR_EN);
	DMA_Init(AFE_DMA_RX_DMAStream, &afe_struct);


	/* Enable SPI DMA RX TX Requests */
	SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Tx, ENABLE);

	SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Rx, ENABLE);

	// enable the transfer complete interrupts
	DMA_ITConfig(AFE_DMA_TX_DMAStream, DMA_IT_TC, ENABLE);
	DMA_ITConfig(AFE_DMA_RX_DMAStream, DMA_IT_TC, ENABLE);

//	SPI_I2S_ClearFlag(AFE_SPI, SPI_I2S_FLAG_TXE);
//	SPI_I2S_ClearFlag(AFE_SPI, SPI_I2S_FLAG_RXNE);


	/* Enable DMA SPI TX Stream */
	DMA_Cmd(AFE_DMA_TX_DMAStream,ENABLE);

	/* Enable DMA SPI RX Stream */
	DMA_Cmd(AFE_DMA_RX_DMAStream,ENABLE);

	/* Enable the SPI peripheral */

	SPI_Cmd(AFE_SPI, ENABLE);


	// set the sync pin on the first packet
	if(afe_currentPacket == 0x00){
		// store time stamp
		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
		afe_timeStorage[0] = RTC_TimeStructure.RTC_Hours;
		afe_timeStorage[1] = RTC_TimeStructure.RTC_Minutes;
		afe_timeStorage[2] = RTC_TimeStructure.RTC_Seconds;
		afe_timeStorage[3] = 3;

		// set the sync pin high
		AFE_INT_PORT->ODR |= (1<<AFE_INT_PIN);
	}
}

void
afe_fileWrite(void){
#ifdef DBGIO
	printf("Logging captures %d to %d\r\n", afe_currentCapture-AFE_DMA_BLOCKS, afe_currentCapture);
#endif
	if(sd_busy != 0x00){
		return;
	}
	sd_busy = 0x01;
	// set to AFE directory
		//path = (char*)afe_writingFolderString;
		path = "";
		res = f_opendir(&dir, path);
		// error handling
		if(res != FR_OK){
			return;
		}

	// create file
		char tmp_fileName[13];
		strcpy(tmp_fileName, general_dec32( (afe_recordingPacketCount/AFE_DMA_SINGLEFILE)));
		strcat(tmp_fileName, ".EMS");

		res = f_open(&fil, tmp_fileName, FA_CREATE_ALWAYS | FA_WRITE);
		// error handling
		if(res != FR_OK){
			return;
		}

	// write to file
		UINT BytesWritten;
		// write the total string to the file
		for(uint8_t i=0; i<AFE_DMA_SINGLEFILE; i++){
			res = f_write(&fil, (void*)&(afe_DMA_RX_Buffer[afe_loggingCapture+i][0][0]), AFE_DMA_BLOCKSIZE*AFE_DMA_BLOCKS, &BytesWritten);
		}

		// synchronize the file
		res = f_sync(&fil);
		// close the file
		res = f_close(&fil);

	sd_busy = 0x00;
	printf("Written file %s\r\n", tmp_fileName);
	strcpy((char*)afe_writingFileString, tmp_fileName);
}

void
afe_getNewFolderName(void){
	// getting the highest folder name.

	char tmp_highestFolder[13];
	memset(tmp_highestFolder, 0, strlen(tmp_highestFolder)+1);

	if(sd_busy == 0x01){
		return;
	}

	sd_busy = 0x01;

	// set to root path
	path = "";

	// open the directory
	res = f_opendir(&dir, path);

#ifdef DBG
	if (res != FR_OK)
		printf("res = %d f_opendir\r\n", res);
#endif

	if (res == FR_OK)
	{
		while(1)
		{

			// read out the directory in sequence
			res = f_readdir(&dir, &fno);

#ifdef DBG
			// print out an error
			if (res != FR_OK)
				printf("res = %d f_readdir\r\n", res);
#endif

			// break the loop if the filename is empty or if there is an error
			if ((res != FR_OK) || (fno.fname[0] == 0))
				break;

			// check the folders and set the last written folder name
			if(fno.fattrib & AM_DIR){
				if(strcmp(fno.fname, (char*)sd_biggestFolderString) >0){
					strcpy((char*)&tmp_highestFolder, fno.fname);
				}
			}
		}
	}

	// only capital characters
	if(strcmp(tmp_highestFolder, "")!=0x00){
		general_convertToCapital(tmp_highestFolder);
	}


	strcpy((char*)afe_writingFolderString, tmp_highestFolder);
	sd_busy = 0x00;

	// increment folder name
	afe_incrementFolderName();

}

void
afe_incrementFolderName(void){
	// error handling
	if(afe_writingFolderString == 0x00){
		return;
	}

	if( strcasecmp((char*)afe_writingFolderString,"") == 0x00 ){
		// string is empty
		strcpy((char*)afe_writingFolderString, "A");
	} else {
		uint8_t tmp_size = strlen((char*)afe_writingFolderString);
		if(afe_writingFolderString[tmp_size-1]=='Z'){
			afe_writingFolderString[tmp_size-1]='A';
			strcat((char*)afe_writingFolderString, "A");
		} else {
			afe_writingFolderString[tmp_size-1] ++;
		}
	}

}

void
afe_createNewFolder(void){
	// set to root path
	path = "";

	// open the directory
	res = f_opendir(&dir, path);

	// create new directory
	//
}

void
afe_writeInfoHeader(void){
	// HEADER:
	/* Logging AFE data to SD card.
	 * Date: XX/XX/XX
	 * Start: XX:XX:XX
	 */
	char tmp_prefix[] = "Logging AFE data to SD card.\nDate: ";
	char tmp_midfix[] = "\nStart: ";
	char tmp_date[8 + 1];
	char tmp_time[8 + 1];
	char* tmp_writeString = calloc(
			strlen(tmp_prefix)+strlen(tmp_midfix)+strlen(tmp_date)+strlen(tmp_time)+1,
			sizeof(char));
	// error handling
	if(tmp_writeString == 0x00){
		return;
	}

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	// create total string
	sprintf(tmp_writeString, "%s%02d/%02d/%02d%s%02d:%02d:%02d",
			tmp_prefix,
			RTC_DateStructure.RTC_Date,
			RTC_DateStructure.RTC_Month,
			RTC_DateStructure.RTC_Year,
			tmp_midfix,
			RTC_TimeStructure.RTC_Hours,
			RTC_TimeStructure.RTC_Minutes,
			RTC_TimeStructure.RTC_Seconds);

	// create info.txt file
	//path = (char*)afe_writingFolderString;
	path = "";
	res = f_opendir(&dir, path);
	// error handling
	// error handling
	if(res != FR_OK){
		printf("could not open directory\r\n");
		return;
	}
	memset(&fil, 0, sizeof(FIL));
	res = f_open(&fil, "info.txt", FA_CREATE_ALWAYS | FA_WRITE);
	// error handling
	if(res != FR_OK){
		printf("could not open file\r\n");
		return;
	}

	// write the information for recording
		UINT BytesWritten;
		// write the total string to the file
		res = f_write(&fil, tmp_writeString, strlen(tmp_writeString)+1, &BytesWritten);
		// error handling
		if(res != FR_OK){
			printf("could not write file\r\n");
			return;
		}
#ifdef DBGIO
		printf("Written %d bytes to the sd card\r\n", BytesWritten);
#endif
		// synchronize the file
		res = f_sync(&fil);
		res = f_close(&fil);
	// free the allocated area
	free(tmp_writeString);
}

void
afe_writeInfoFooter(void){
	// FOOTER:
	/* End: XX:XX:XX
	 * Packet count: XXXXX
	 * Start packet: XXXXX
	 * End packet: XXXXX
	 */
	char tmp_prefix[] = "End: ";
	char tmp_midfix1[] = "\nPacket count: ";
	char tmp_midfix2[] = "\nStart packet: ";
	char tmp_midfix3[] = "\nEnd packet: ";
	char tmp_time[8 + 1];
	char* tmp_writeString = calloc(
			strlen(tmp_prefix)+strlen(tmp_midfix1)+strlen(tmp_midfix2)+strlen(tmp_midfix3)+strlen(tmp_time)+1,
			sizeof(char));
	// error handling
	if(tmp_writeString == 0x00){
		return;
	}

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

	// create total string
	sprintf(tmp_writeString, "%s%02d:%02d:%02d%s%ld%s%d.ems%s%ld.ems",
			tmp_prefix,
			RTC_TimeStructure.RTC_Hours,
			RTC_TimeStructure.RTC_Minutes,
			RTC_TimeStructure.RTC_Seconds,
			tmp_midfix1,
			afe_recordingPacketCount,
			tmp_midfix2,
			1,
			tmp_midfix3,
			afe_recordingPacketCount);

	// open info.txt file
	//path = (char*)afe_writingFolderString;
	path = "";
	res = f_opendir(&dir, path);
	// error handling
	if(res != FR_OK){
		printf("could not open directory\r\n");
		return;
	}
	memset(&fil, 0, sizeof(FIL));
	res = f_open(&fil, "info.txt", FA_OPEN_EXISTING | FA_WRITE);
	// error handling
	if(res != FR_OK){
		printf("could not open file\r\n");
		return;
	}

	// write the information for recording
		UINT BytesWritten;
		// write the total string to the file
		res = f_write(&fil, tmp_writeString, strlen(tmp_writeString), &BytesWritten);
		// error handling
		if(res != FR_OK){
			printf("could not write file\r\n");
			return;
		}
#ifdef DBGIO
		printf("Written %d bytes to the sd card\r\n", BytesWritten);
#endif
		// synchronize the file
		res = f_sync(&fil);
		res = f_close(&fil);

}
