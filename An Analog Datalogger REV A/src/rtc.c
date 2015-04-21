/*
 * rtc.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */
#include "rtc.h"


static volatile uint32_t uwAsynchPrediv = 0;
static volatile uint32_t uwSynchPrediv = 0;
static RTC_TimeTypeDef  RTC_TimeStructure;
static RTC_DateTypeDef RTC_DateStructure;

void
initialize_RTC(void){

	RTC_InitTypeDef  RTC_InitStructure;
	uint32_t trying = 0;
	//RTC_AlarmTypeDef RTC_AlarmStructure;


	  /* Enable the PWR clock */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	  /* Allow access to RTC */
	  PWR_BackupAccessCmd(ENABLE);

		// switch between external 32kHz crystal
	#if defined(RTC_CLOCK_SOURCE_LSE)
	  /* Enable the LSE OSC */
	  RCC_LSEConfig(RCC_LSE_ON);

	  printf("\r\n");

	  /* Wait till LSE is ready */
	  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	  {
		delay_milli(50);
		printf("%ld times tried waiting for LSE to get ready..\r\n", ++trying);
		if(trying == 5){
			RCC_LSEConfig(RCC_LSE_OFF);
			  /* The RTC Clock may varies due to LSI frequency dispersion */
			  /* Enable the LSI OSC */
			  RCC_LSICmd(ENABLE);

			  /* Wait till LSI is ready */
			  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			  {
			  }

			  /* Select the RTC Clock Source */
			  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

			  /* ck_spre(1Hz) = RTCCLK(LSI) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
			  uwSynchPrediv = 0xFF;
			  uwAsynchPrediv = 0x7F;
			  printf("RTC CONFIGURED AS LSI!! BE AWARE!! \r\n");
			  delay_milli(1000);
			goto escape_LSE;
		}
	  }

	  /* Select the RTC Clock Source */
	  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	  /* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
	  uwSynchPrediv = 0xFF;
	  uwAsynchPrediv = 0x7F;
	#else // switch between internal 40kHz crystal
	  /* The RTC Clock may varies due to LSI frequency dispersion */
	  /* Enable the LSI OSC */
	  RCC_LSICmd(ENABLE);

	  /* Wait till LSI is ready */
	  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	  {
	  }

	  /* Select the RTC Clock Source */
	  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	  /* ck_spre(1Hz) = RTCCLK(LSI) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
	  uwSynchPrediv = 0xFF;
	  uwAsynchPrediv = 0x7F;
	#endif /*defined(RTC_CLOCK_SOURCE_XX)*/

escape_LSE:

	  /* Enable the RTC Clock */
	  RCC_RTCCLKCmd(ENABLE);

	  /* Wait for RTC APB registers synchronisation */
	  RTC_WaitForSynchro();

	  /* Configure the RTC data register and RTC prescaler */
	  RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
	  RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	  RTC_Init(&RTC_InitStructure);

/*	   Set the alarm 05h:20min:30s
	  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
	  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x05;
	  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x20;
	  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x30;
	  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
	  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

	   Configure the RTC Alarm A register
	  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

	   Enable RTC Alarm A Interrupt
	  RTC_ITConfig(RTC_IT_ALRA, ENABLE);*/

/*	   Enable the alarm
	  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	  RTC_ClearFlag(RTC_FLAG_ALRAF);*/

	  /* Set the date: Friday January 11th 2013 */
	  RTC_DateStructure.RTC_Year = 0x15;
	  RTC_DateStructure.RTC_Month = RTC_Month_April;
	  RTC_DateStructure.RTC_Date = 0x18;
	  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
	  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	  /* Set the time to 05h 20mn 00s AM */
	  RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
	  RTC_TimeStructure.RTC_Hours   = 0x16;
	  RTC_TimeStructure.RTC_Minutes = 0x10;
	  RTC_TimeStructure.RTC_Seconds = 0x00;

	  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
}

void
rtc_printTimeTerminal(void){
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	printf("%02d:%02d:%02d",RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

void
rtc_setTimeToOLED(void){
	char time[9];
	xypair_t tmp = ssd1306_getScreenDimensions();
	uint8_t stringHeigth = (Font_System5x8.u8Height);
	uint8_t stringWidth = (Font_System5x8.u8Width*8)+(7);
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	sprintf(time, "%02d:%02d:%02d",RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	printf("%s \r\n",time);
	ssd1306_clearArea(tmp.x-stringWidth-1,tmp.y-stringHeigth, tmp.x-1, tmp.y);
	ssd1306_setString(tmp.x-stringWidth-1,tmp.y-stringHeigth-1,time,Font_System5x8);
}
