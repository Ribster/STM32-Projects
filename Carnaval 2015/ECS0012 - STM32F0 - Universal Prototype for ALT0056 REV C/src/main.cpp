#include "main.h"


#ifdef FUNCTION_SYS
extern "C" void
SysTick_Handler(void){
	SYS_DELAY.interruptHandler();
}
#endif /* FUNCTION_SYS */


extern "C" void
DMA1_Channel2_3_IRQHandler(void){
#ifdef FUNCTION_WS2812B
	WS2812B_CH1.interruptHandler();
	WS2812B_CH2.interruptHandler();
#endif /* FUNCTION_WS2812B */
}


extern "C" void
EXTI2_3_IRQHandler(void){
#ifdef FUNCTION_nRF
  #ifdef nRF_Receiver

      // check if interrupt is for nrf

      if(nrf24_dataReady()){
	  NVIC_DisableIRQ(NRF_IRQN);
	      NVIC_ClearPendingIRQ(NRF_IRQN);
	      EXTI_ClearITPendingBit(NRF_IRQN);
		nRF_RECEIVER();
	      NVIC_EnableIRQ(NRF_IRQN);
      }


  #endif
#endif /* FUNCTION_nRF */
}

extern "C" void
EXTI4_15_IRQHandler(void){
#ifdef FUNCTION_PB
  PB_UP_pressed();
  PB_DOWN_pressed();
  PB_LEFT_pressed();
  PB_RIGHT_pressed();
  PB_ENTER_pressed();
#endif /* FUNCTION_PB */
}

extern "C" void
TIM2_IRQHandler(void){
#ifdef FUNCTION_IR
	if (TIM_GetITStatus(IR_timer, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(IR_timer, TIM_IT_Update);
		IR_NEC.interruptHandler();
	}
#endif /* FUNCTION_IR */
}


extern "C"
  void RTC_IRQHandler(void){
  #ifdef FUNCTION_RTC
	  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	  {
	      printf("trigger RTC interrupt \r\n");
	    RTC_HoldTransmit = false;
	    RTC_ClearITPendingBit(RTC_IT_ALRA);
	    EXTI_ClearITPendingBit(EXTI_Line17);
		RTC_ITConfig(RTC_IT_ALRA, DISABLE);
		RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
		NVIC_DisableIRQ(RTC_IRQn);
		printf("trigger RTC interrupt \r\n");
	  }
  #endif /* FUNCTION_RTC */
}


extern "C"
int __io_putchar(int ch)
{
#ifdef FUNCTION_USART
  while(USART_GetFlagStatus(USART_SELECTION, USART_FLAG_TXE) == RESET);
  USART_SendData(USART_SELECTION, (uint8_t) ch);
  return(ch);
#endif /* FUNCTION_USART */
}


extern "C"
int __io_getchar(void)
{
#ifdef FUNCTION_USART
  while(USART_GetFlagStatus(USART_SELECTION, USART_FLAG_RXNE) == RESET);
  return((int)USART_ReceiveData(USART_SELECTION));
#endif /* FUNCTION_USART */
}



int main(void){

  initALL();

#if defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120)
  SYS_lastupdate = SYS_DELAY.getMillis();
#endif

  SYS_IO.setValue(&SYS_LED_ORANGE, false);
  SYS_IO.setValue(&SYS_LED_GREEN, true);

  while(1){
#ifdef VERSION_ALL
      uint32_t i;

      if(SYS_IO.getValue(&PB_DOWN)){
	  OLED_SCREEN.setString("DOWN", make_pair(25,16), SCREEN_ALIGN_MIDMID, 1);
	      OLED_SCREEN.writeBuffer();
	  WS2812B_CH1.setRainbowScroll(&SYS_DELAY, 2);
      } else if(SYS_IO.getValue(&PB_UP)){
	  OLED_SCREEN.setString("UP", make_pair(25,46), SCREEN_ALIGN_MIDMID, 1);
	      OLED_SCREEN.writeBuffer();
	  WS2812B_CH2.setRainbowScroll(&SYS_DELAY, 2);
      } else if(SYS_IO.getValue(&PB_LEFT)){
	  OLED_SCREEN.setString("LEFT", make_pair(10,32), SCREEN_ALIGN_MIDMID, 1);
	      OLED_SCREEN.writeBuffer();
	  WS2812B_CH1.setRainbowScroll(&SYS_DELAY, 10);
      } else if(SYS_IO.getValue(&PB_RIGHT)){
	  OLED_SCREEN.setString("RIGHT", make_pair(40,32), SCREEN_ALIGN_MIDMID, 1);
	      OLED_SCREEN.writeBuffer();
	  WS2812B_CH2.setRainbowScroll(&SYS_DELAY, 10);
      } else if(SYS_IO.getValue(&PB_ENTER)){
	  OLED_SCREEN.setString("ENTER", make_pair(80,32), SCREEN_ALIGN_MIDMID, 1);
	      OLED_SCREEN.writeBuffer();
	  WS2812B_CH1.setRainbowScroll(&SYS_DELAY, 1);
	  WS2812B_CH2.setRainbowScroll(&SYS_DELAY, 1);
      } else {
	  OLED_SCREEN.writeClearScreen();
	  //WS2812B_CH1.setRainbowScroll(&SYS_DELAY, 0);
	  //WS2812B_CH2.setRainbowScroll(&SYS_DELAY, 0);

	  i++;

	      uint8_t tmp = i%25;
	      HC595_595.setState(1, (i%2)?(true):(false));
	      IR_NEC.setChannelAll(tmp);
	      IR_NEC.transmit();

	      SYS_DELAY.delayMilli(1000);



      }


#endif

#ifdef VERSION_TRANSMITTER


      if(TRANSMITTER_STATE == TRANS_STATES_ReadSD){
	  if(SD_bytesRead == SD_SINGLEREAD){
	      // read block and wait until it is transmitted
	      TRANS_ReadWaitBlock();

	  } else {
	      // wait delaytime
	      SYS_DELAY.delayMilli(SYS_loopDelay);

	      // start transmitter
	      TRANS_StartTransmitter();

	      // do first read
	      TRANS_ReadWaitBlock();
	  }
      } else if (TRANSMITTER_STATE == TRANS_STATES_ResetTimer){
	      // start transmitter
	      //SYS_DELAY.delayMilli(300 );
	      TRANS_StartTransmitter();
	      TRANSMITTER_STATE = TRANS_STATES_ReadSD;
      } else if (TRANSMITTER_STATE == TRANS_STATES_Backward1s){
	      TRANS_MoveRTCBackward(1000);
	      TRANSMITTER_STATE = TRANS_STATES_ReadSD;
      } else if (TRANSMITTER_STATE == TRANS_STATES_Forward1s){
	      TRANS_MoveRTCForward(1000);
	      TRANSMITTER_STATE = TRANS_STATES_ReadSD;
      } else if (TRANSMITTER_STATE == TRANS_STATES_Pauze){
	      //TRANS_MoveRTCBackward(10000);
	      TRANSMITTER_STATE = TRANS_STATES_ReadSD;
      }
      else if (TRANSMITTER_STATE == TRANS_STATES_Play){
	      //TRANS_MoveRTCForward(10000);
	      TRANSMITTER_STATE = TRANS_STATES_ReadSD;
       }




#endif



#if defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120)

      // everything handled in interrupts (for now)

      // if max reinits exceeded, go into random mode

      //printf("current time in millis: %ld, last update time in millis: %ld, difference: %ld, treshold reached? %s\r\n", SYS_curtime, SYS_lastupdate, SYS_curtime-SYS_lastupdate, ((SYS_curtime-SYS_lastupdate)>NRF_updateThreshold )?("TRUE"):("FALSE"));
      uint16_t i, j;
      uint32_t color;
      // going into timeout after timeout treshold
      if( (SYS_DELAY.getMillis()-SYS_lastupdate)>NRF_updateThreshold ){
	  nRF_Receiver_Mode = nRF_ReceiverModes_Timeout;
      }

      // normal routine
      if(nRF_Receiver_Mode == nRF_ReceiverModes_NormalUpdate){
	  // everything is handled in the interrupt
	  // you can add something here if you want to do it constantly.. (polling for example)
      }else if(nRF_Receiver_Mode == nRF_ReceiverModes_Rainbow){
	  for(j=0; j<256; j++) {
	    for(i=1; i<WS2812B_CH1.getPixelCount()+1; i++) {
		if(nRF_Receiver_Mode == nRF_ReceiverModes_Rainbow){
		WS2812B_CH1.setPixelColor((uint8_t)i, WS2812B_CH1.getWheel((i+j*i) & 255),0);
		WS2812B_CH2.setPixelColor((uint8_t)i, WS2812B_CH2.getWheel((i+(255-j)*i) & 255),0);
		}
	    }
	    if(nRF_Receiver_Mode == nRF_ReceiverModes_Rainbow){
	    WS2812B_CH1.show();
	    WS2812B_CH2.show();
	    IR_NEC.transmit();
	    SYS_DELAY.delayMilli(10);
	    }
	  }
	  for(j=255; j>1; j--) {
	    for(i=1; i<WS2812B_CH1.getPixelCount()+1; i++) {
		if(nRF_Receiver_Mode == nRF_ReceiverModes_Rainbow){
		WS2812B_CH1.setPixelColor((uint8_t)i, WS2812B_CH1.getWheel((i+j*i) & 255),0);
		WS2812B_CH2.setPixelColor((uint8_t)i, WS2812B_CH2.getWheel((i+(255-j)*i) & 255),0);
		}
	    }
	    if(nRF_Receiver_Mode == nRF_ReceiverModes_Rainbow){
	    WS2812B_CH1.show();
	    WS2812B_CH2.show();
	    IR_NEC.transmit();
	    SYS_DELAY.delayMilli(10);
	    }
	  }
      }else if(nRF_Receiver_Mode == nRF_ReceiverModes_RainbowFast){
	  for(j=0; j<256; j++) {
	    for(i=1; i<WS2812B_CH1.getPixelCount()+1; i++) {
		if(nRF_Receiver_Mode == nRF_ReceiverModes_RainbowFast){
		WS2812B_CH1.setPixelColor((uint8_t)i, WS2812B_CH1.getWheel((i+j*i) & 255),0);
		WS2812B_CH2.setPixelColor((uint8_t)i, WS2812B_CH2.getWheel((i+(255-j)*i) & 255),0);
		}
	    }
	    if(nRF_Receiver_Mode == nRF_ReceiverModes_RainbowFast){
	    WS2812B_CH1.show();
	    WS2812B_CH2.show();
	    IR_NEC.transmit();
	    SYS_DELAY.delayMilli(0);
	    }
	  }
	  for(j=255; j>1; j--) {
	    for(i=1; i<WS2812B_CH1.getPixelCount()+1; i++) {
		if(nRF_Receiver_Mode == nRF_ReceiverModes_RainbowFast){
		WS2812B_CH1.setPixelColor((uint8_t)i, WS2812B_CH1.getWheel((i+j*i) & 255),0);
		WS2812B_CH2.setPixelColor((uint8_t)i, WS2812B_CH2.getWheel((i+(255-j)*i) & 255),0);
		}
	    }
	    if(nRF_Receiver_Mode == nRF_ReceiverModes_RainbowFast){
	    WS2812B_CH1.show();
	    WS2812B_CH2.show();
	    IR_NEC.transmit();
	    SYS_DELAY.delayMilli(0);
	    }
	  }

      } else if (nRF_Receiver_Mode == nRF_ReceiverModes_ExpandPink){
	  color = 0xFF00FF;
	  WS2812B_CH1.setAllColor(0x000000);
	  WS2812B_CH2.setAllColor(0x000000);
	  WS2812B_CH1.show();
	  WS2812B_CH2.show();
	  IR_NEC.transmit();

	  for(i=0;i<(WS2812B_CH1.getPixelCount()/2)+1;i++){
	      // both strips the same pixelcount
		// 0 - 15
		// set pixel high, wait
	      if(nRF_Receiver_Mode == nRF_ReceiverModes_ExpandPink){
		WS2812B_CH1.setPixelColor((uint8_t)(WS2812B_CH1.getPixelCount()/2)-i, color,true);
		WS2812B_CH1.setPixelColor((uint8_t)((WS2812B_CH1.getPixelCount()/2)+1)+i, color,true);
		WS2812B_CH2.setPixelColor((uint8_t)(WS2812B_CH2.getPixelCount()/2)-i, color,true);
		WS2812B_CH2.setPixelColor((uint8_t)((WS2812B_CH2.getPixelCount()/2)+1)+i, color,true);
		WS2812B_CH1.show();
		WS2812B_CH2.show();
		IR_NEC.transmit();
		SYS_DELAY.delayMilli(20);
	      }

	  }
	  for(i=0;i<(WS2812B_CH1.getPixelCount()/2)+1;i++){
	      // both strips the same pixelcount
		// 0 - 15
		// set pixel low, wait
	      if(nRF_Receiver_Mode == nRF_ReceiverModes_ExpandPink){
		WS2812B_CH1.setPixelColor((uint8_t)(WS2812B_CH1.getPixelCount()/2)-i, 0,true);
		WS2812B_CH1.setPixelColor((uint8_t)((WS2812B_CH1.getPixelCount()/2)+1)+i, 0,true);
		WS2812B_CH2.setPixelColor((uint8_t)(WS2812B_CH2.getPixelCount()/2)-i, 0,true);
		WS2812B_CH2.setPixelColor((uint8_t)((WS2812B_CH2.getPixelCount()/2)+1)+i, 0,true);
		WS2812B_CH1.show();
		WS2812B_CH2.show();
		IR_NEC.transmit();
		SYS_DELAY.delayMilli(20);
	      }

	  }


      } else if (nRF_Receiver_Mode == nRF_ReceiverModes_ExpandYellow){
	  color = 0xFFFF00;
	  WS2812B_CH1.setAllColor(0x000000);
	  WS2812B_CH2.setAllColor(0x000000);
	  WS2812B_CH1.show();
	  WS2812B_CH2.show();

	  for(i=0;i<(WS2812B_CH1.getPixelCount()/2)+1;i++){
	      // both strips the same pixelcount
		// 0 - 15
		// set pixel high, wait
	      if(nRF_Receiver_Mode == nRF_ReceiverModes_ExpandYellow){
		WS2812B_CH1.setPixelColor((uint8_t)(WS2812B_CH1.getPixelCount()/2)-i, color,0);
		WS2812B_CH1.setPixelColor((uint8_t)((WS2812B_CH1.getPixelCount()/2)+1)+i, color,0);
		WS2812B_CH2.setPixelColor((uint8_t)(WS2812B_CH2.getPixelCount()/2)-i, color,0);
		WS2812B_CH2.setPixelColor((uint8_t)((WS2812B_CH2.getPixelCount()/2)+1)+i, color,0);
		WS2812B_CH1.show();
		WS2812B_CH2.show();
		IR_NEC.transmit();
		SYS_DELAY.delayMilli(30);
	      }

	  }
	  for(i=0;i<(WS2812B_CH1.getPixelCount()/2)+1;i++){
	      // both strips the same pixelcount
		// 0 - 15
		// set pixel low, wait
	      if(nRF_Receiver_Mode == nRF_ReceiverModes_ExpandYellow){
		WS2812B_CH1.setPixelColor((uint8_t)(WS2812B_CH1.getPixelCount()/2)-i, 0,0);
		WS2812B_CH1.setPixelColor((uint8_t)((WS2812B_CH1.getPixelCount()/2)+1)+i, 0,0);
		WS2812B_CH2.setPixelColor((uint8_t)(WS2812B_CH2.getPixelCount()/2)-i, 0,0);
		WS2812B_CH2.setPixelColor((uint8_t)((WS2812B_CH2.getPixelCount()/2)+1)+i, 0,0);
		WS2812B_CH1.show();
		WS2812B_CH2.show();
		IR_NEC.transmit();
		SYS_DELAY.delayMilli(29);
	      }

	  }
      } else if (nRF_Receiver_Mode == nRF_ReceiverModes_ScanWhite){
	  IR_NEC.transmit();
	  for(i=0;i<255;i++){
	      if(nRF_Receiver_Mode == nRF_ReceiverModes_ScanWhite){
	      WS2812B_CH1.setAllColor(i<<16|i<<8|i);
	      WS2812B_CH2.setAllColor(i<<16|i<<8|i);
	      WS2812B_CH1.show();
	      WS2812B_CH2.show();
	      IR_NEC.transmit();
	      SYS_DELAY.delayMilli(25);
	      }
	  }
	  for(i=255;i>1;i--){
	      if(nRF_Receiver_Mode == nRF_ReceiverModes_ScanWhite){
	      WS2812B_CH1.setAllColor(i<<16|i<<8|i);
	      WS2812B_CH2.setAllColor(i<<16|i<<8|i);
	      WS2812B_CH1.show();
	      WS2812B_CH2.show();
	      IR_NEC.transmit();
	      SYS_DELAY.delayMilli(15);
	      }
	  }
      } else if(nRF_Receiver_Mode == nRF_ReceiverModes_Timeout){

	  IR_NEC.setChannelAll(IR_Commands_On);
	  IR_NEC.transmit();
	  SYS_DELAY.delayMilli(100);
	  IR_NEC.setChannelAll(IR_Commands_On);
	  IR_NEC.transmit();
	  SYS_DELAY.delayMilli(100);
	  IR_NEC.setChannelAll(IR_Commands_On);
	  IR_NEC.transmit();
	  SYS_DELAY.delayMilli(100);
	  IR_NEC.setChannelAll(IR_Commands_Smooth);
	  IR_NEC.transmit();
	  SYS_DELAY.delayMilli(100);

	  for(j=0; j<256; j++) {
	    for(i=1; i<WS2812B_CH1.getPixelCount()+1; i++) {
		if(nRF_Receiver_Mode == nRF_ReceiverModes_Timeout){
		    WS2812B_CH1.setPixelColor((uint8_t)i, WS2812B_CH1.getWheel((i*j+j) & 255),0);
		    WS2812B_CH2.setPixelColor((uint8_t)i, WS2812B_CH2.getWheel((i*(256-j)+j) & 255),0);
		}

	    }
	    if(nRF_Receiver_Mode == nRF_ReceiverModes_Timeout){
		WS2812B_CH1.show();
		WS2812B_CH2.show();
		SYS_DELAY.delayMilli(10);
	    }

	  }
	  for(j=255; j>1; j--) {
	    for(i=1; i<WS2812B_CH1.getPixelCount()+1; i++) {
		if(nRF_Receiver_Mode == nRF_ReceiverModes_Timeout){
		    WS2812B_CH1.setPixelColor((uint8_t)i, WS2812B_CH1.getWheel((i*j+j) & 255),0);
		    WS2812B_CH2.setPixelColor((uint8_t)i, WS2812B_CH2.getWheel((i*(256-j)+j) & 255),0);
		}

	    }
	    if(nRF_Receiver_Mode == nRF_ReceiverModes_Timeout){
		WS2812B_CH1.show();
		WS2812B_CH2.show();
		SYS_DELAY.delayMilli(100);
	    }

	  }

	  for(j=0; j<256; j++) {
	    for(i=1; i<WS2812B_CH1.getPixelCount()+1; i++) {
		if(nRF_Receiver_Mode == nRF_ReceiverModes_Timeout){
		    WS2812B_CH1.setPixelColor((uint8_t)i, WS2812B_CH1.getWheel((i+j) & 255),0);
		    WS2812B_CH2.setPixelColor((uint8_t)i, WS2812B_CH2.getWheel((i+(255-j)) & 255),0);
		}

	    }
		if(nRF_Receiver_Mode == nRF_ReceiverModes_Timeout){
		    WS2812B_CH1.show();
		    WS2812B_CH2.show();
		    SYS_DELAY.delayMilli(10);
		}

	  }
	  for(j=255; j>1; j--) {
	    for(i=1; i<WS2812B_CH1.getPixelCount()+1; i++) {
		if(nRF_Receiver_Mode == nRF_ReceiverModes_Timeout){
		    WS2812B_CH2.setPixelColor((uint8_t)i, WS2812B_CH2.getWheel((i+j) & 255),0);
		    WS2812B_CH1.setPixelColor((uint8_t)i, WS2812B_CH1.getWheel((i+(255-j)) & 255),0);
		}

	    }
		if(nRF_Receiver_Mode == nRF_ReceiverModes_Timeout){
		  WS2812B_CH1.show();
		  WS2812B_CH2.show();
		  SYS_DELAY.delayMilli(100);
		}

	  }
      }


#endif


  }

  return 0;
}


void initALL(void){
  // Clock selection for HSI -> with PLL *12; HSI/2 as input -> 4MHz * 12 = 48MHz SysClock
  #ifndef FUNCTION_OSC
    SYS_RCC.SystemCoreClockHSIPLL2(RCC_PLLMul_12);
  #endif /* FUNCTION_OSC */



#ifdef FUNCTION_USART
    // UART INIT
    initUART();
    // Version Print
    printVersion();
#endif



#ifdef FUNCTION_SYS

    // IO INIT
    printf("[INIT]\tBegin initializing system LEDs...");
    initIO();
    printf("...Done\r\n");

    // DELAY (systick) INIT
    printf("[INIT]\tBegin initializing DELAY...");
    initDELAY();
    printf("...Done\r\n");

#endif /* FUNCTION_SYS */

#ifdef FUNCTION_PB
    // PUSHBUTTONS INIT
    printf("[INIT]\tBegin initializing PUSHBUTTONS...");
    initPBs();
    printf("...Done\r\n");
#endif /* FUNCTION_PB */

#ifdef FUNCTION_HC595

    // HC595 INIT
    printf("[INIT]\tBegin initializing HC595...");
    initHC595();
    printf("...Done\r\n");

#endif /* FUNCTION_HC595 */

#ifdef FUNCTION_IR

    // OSCILLATOR INIT
    printf("[INIT]\tBegin initializing OSCILLATOR...");
    initOSCILLATOR();
    printf("...Done\r\n");

    // IR COMMANDS INIT
    printf("[INIT]\tBegin initializing IR commands...");
    initIRCommands();
    printf("...Done\r\n");

#endif /* FUNCTION_IR */

#ifdef FUNCTION_WS2812B

    // WS2812B
    printf("[INIT]\tBegin initializing WS2812B...");
    initWS();
    printf("...Done\r\n");

#endif /* FUNCTION_WS2812B */



#ifdef FUNCTION_nRF

    // nrf
    printf("[INIT]\tBegin initializing nRF...");
    initNRF();
    printf("...Done\r\n");

#endif /* FUNCTION_nRF */

#ifdef FUNCTION_SD

    // sd
    printf("[INIT]\tBegin initializing SD...");
    initSD();
    printf("...Done\r\n");

#endif /* FUNCTION_SD */

#ifdef FUNCTION_OLED

    // OLED
    printf("[INIT]\tBegin initializing OLED...");
    initOLED();
    printf("...Done\r\n");

#endif /* FUNCTION_OLED */



#ifdef FUNCTION_RTC

    // RTC
    printf("[INIT]\tBegin initializing RTC...");
    initRTC();
    printf("...Done\r\n");

#endif /* FUNCTION_RTC */

#ifdef VERSION_TRANSMITTER
    initTRANSMITTER();
#endif

#if defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120)
    initReceiver();
#endif

    SYS_IO.setValue(&SYS_LED_GREEN, true);
    SYS_IO.setValue(&SYS_LED_ORANGE, false);
}


#ifdef VERSION_TRANSMITTER
    void initTRANSMITTER(void){
      printf("[INIT] Initialized as a transmitter!!\r\n");
    }
#endif

#if defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120)
    void initReceiver(void){
      #ifdef VERSION_RECEIVER60
	    printf("[INIT] Initialized as a receiver with %d pixels!!\r\n", 60);
      #endif
      #ifdef VERSION_RECEIVER120
	    printf("[INIT] Initialized as a receiver with %d pixels!!\r\n", 120);
      #endif
    }
#endif

// System leds + SWD


void initIO(void){
#ifdef FUNCTION_SYS
  SYS_IO.begin(&SYS_LED_GREEN, GPIOB, 8, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.begin(&SYS_LED_ORANGE, GPIOB, 9, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.setValue(&SYS_LED_ORANGE, false);
  SYS_IO.setValue(&SYS_LED_GREEN, false);
#endif /* FUNCTION_SYS */

}
#ifdef FUNCTION_SYS
void initDELAY(void){
  SYS_DELAY.begin(DELAY_Milliseconds);
}
#endif /* FUNCTION_SYS */


// HC 595 SIPO's
#ifdef FUNCTION_HC595

void initHC595(void){
  // initializing IO
  SYS_IO.begin(&HC595_DATA, GPIOB, 5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.begin(&HC595_SCK, GPIOB, 7, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.begin(&HC595_LATCH, GPIOB, 6, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
}

#endif /* FUNCTION_HC595 */

// Infrared functionality NEC protocol at 38kHz
#ifdef FUNCTION_IR

void initOSCILLATOR(void){
	SYS_IO.begin(&IR_PIN, GPIOB, 4, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIO_AF_1);
	SYS_IO.begin(&IR_LED, GPIOB, 3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);

	SYS_IO.setValue(&IR_PIN, false);
	SYS_IO.setValue(&IR_LED, false);

	IR_OSC.begin(IR_oscillator_freq, IR_oscillator_timer, IR_oscillator_channel);
}

void initIRCommands(void){
  IR_NEC.setCommand(IR_Commands_Intensity_Up, 0xA0);
  IR_NEC.setCommand(IR_Commands_Intensity_Down, 0x20);
  IR_NEC.setCommand(IR_Commands_Off, 0x60);
  IR_NEC.setCommand(IR_Commands_On, 0xE0);
  IR_NEC.setCommand(IR_Commands_Red_1, 0x90);
  IR_NEC.setCommand(IR_Commands_Green_1, 0x10);
  IR_NEC.setCommand(IR_Commands_Blue_1, 0x50);
  IR_NEC.setCommand(IR_Commands_White, 0xD0);
  IR_NEC.setCommand(IR_Commands_Red_2, 0xB0);
  IR_NEC.setCommand(IR_Commands_Green_2, 0x30);
  IR_NEC.setCommand(IR_Commands_Blue_2, 0x50);
  IR_NEC.setCommand(IR_Commands_Flash, 0xF0);
  IR_NEC.setCommand(IR_Commands_Red_3, 0xA8);
  IR_NEC.setCommand(IR_Commands_Green_3, 0x28);
  IR_NEC.setCommand(IR_Commands_Blue_3, 0x68);
  IR_NEC.setCommand(IR_Commands_Strobe, 0xE8);
  IR_NEC.setCommand(IR_Commands_Red_4, 0x98);
  IR_NEC.setCommand(IR_Commands_Green_4, 0x18);
  IR_NEC.setCommand(IR_Commands_Blue_4, 0x58);
  IR_NEC.setCommand(IR_Commands_Fade, 0xD8);
  IR_NEC.setCommand(IR_Commands_Red_5, 0x88);
  IR_NEC.setCommand(IR_Commands_Green_5, 0x08);
  IR_NEC.setCommand(IR_Commands_Blue_5, 0x48);
  IR_NEC.setCommand(IR_Commands_Smooth, 0xC8);
  IR_NEC.setCommand(IR_Commands_Nothing, 0x00);

  // for the print command list
  //printf("\r\n");

  //IR_NEC.setChannelAll(IR_Commands_On);


  //IR_NEC.printCommandList();
  //IR_NEC.printChannelList();
  //IR_NEC.printChannelBufferAll();

  IR_NEC.initializeTimer();

  IR_NEC.setChannelAll(IR_Commands_On);
  IR_NEC.transmit();
  SYS_DELAY.delayMilli(200);

  IR_NEC.setChannelAll(IR_Commands_Off);
  IR_NEC.transmit();
  SYS_DELAY.delayMilli(200);

  IR_NEC.setChannelAll(IR_Commands_On);
  IR_NEC.transmit();
  SYS_DELAY.delayMilli(200);

  IR_NEC.setChannelAll(IR_Commands_White);
  IR_NEC.transmit();
  SYS_DELAY.delayMilli(200);

  IR_NEC.setChannelAll(IR_Commands_Red_1);
  IR_NEC.transmit();
  SYS_DELAY.delayMilli(200);

  IR_NEC.setChannelAll(IR_Commands_Green_1);
  IR_NEC.transmit();
  SYS_DELAY.delayMilli(200);

  IR_NEC.setChannelAll(IR_Commands_Blue_1);
  IR_NEC.transmit();
  SYS_DELAY.delayMilli(200);

  IR_NEC.setChannelAll(IR_Commands_White);
  IR_NEC.transmit();
  SYS_DELAY.delayMilli(200);
}

#endif /* FUNCTION_HC595 */

// nRF24L01p wireless 2.4GHz transceiver
#ifdef FUNCTION_nRF


#if defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120)
    // receiver functionality
    /* Working on pipe 1 with no acknowledgment
     * Receiving data and setting the data directly to the rgb spots && pixels
     */
    void nRF_RECEIVER(void){

      // get data
      while(nrf24_dataReady()){
	  SYS_IO.setValue(&nRF_LED, true);
	  nrf24_getData(nRF_Data.nRF_Buffer);
	  /*
	  printf("[NRF] data: ");
	  for(uint8_t i=0; i<NRF_Payload; i++){
	      if(i!=0){
		  printf(", ");
	      }
	      printf("0x%x", nRF_Data.nRF_Buffer[i]);
	  }
	  printf("\r\n");
	  */
      }
      //printf("[NRF]\tReceived packet nr %d \r\n", nRF_Data.nRF_Buffer[0]);


      // if the command (totalbuff/payload-1)+1 is given (184/23)+1=9 , then the buffer is filled
      // else, fill the buffer

      if(nRF_Data.nRF_Buffer[0] == nRF_PacketCMDS_PayloadFinished){
	  //printf("[NRF]\tSetting new data...\r\n");
	// set SSR
	  HC595_595.setState(1,(nRF_Data.nRF_FULLBuffer[3] & (1<<0))?(false):(true));
	// set IR
	  //printf("channel 1: %d, channel 2: %d, channel 3: %d \r\n", nRF_Data.nRF_FULLBuffer[0], nRF_Data.nRF_FULLBuffer[1], nRF_Data.nRF_FULLBuffer[2]);

	  IR_NEC.setChannel(nRF_Data.nRF_FULLBuffer[0], 1);
	  IR_NEC.setChannel(nRF_Data.nRF_FULLBuffer[1], 2);
	  IR_NEC.setChannel(nRF_Data.nRF_FULLBuffer[2], 3);
	// set WS2812B's
	  uint8_t i;

	#ifdef VERSION_RECEIVER60
	  // range 0->29
	  for(i=0;i<WS2812B_CH1_PIXELS; i++){
	      WS2812B_CH1.setPixelColor(i+1, (nRF_Data.nRF_FULLBuffer[4+(i*3)]<<16)|(nRF_Data.nRF_FULLBuffer[4+(i*3)+1]<<8)|(nRF_Data.nRF_FULLBuffer[4+(i*3)+2]) , true);
	  }
	  for(i=0;i<WS2812B_CH2_PIXELS; i++){
	      WS2812B_CH2.setPixelColor(i+1, (nRF_Data.nRF_FULLBuffer[(WS2812B_CH1_PIXELS*3)+4+(i*3)]<<16)|(nRF_Data.nRF_FULLBuffer[(WS2812B_CH1_PIXELS*3)+4+(i*3)+1]<<8)|(nRF_Data.nRF_FULLBuffer[(WS2812B_CH1_PIXELS*3)+4+(i*3)+2]) , true);
	  }
	#endif /* VERSION_RECEIVER60 */


	#ifdef VERSION_RECEIVER120
	  // range 0->59
	  for(i=0;i<WS2812B_CH1_PIXELS; i++){
	      WS2812B_CH1.setPixelColor(i+1, (nRF_Data.nRF_FULLBuffer[4+((i/2)*3)]<<16)|(nRF_Data.nRF_FULLBuffer[4+((i/2)*3)+1]<<8)|(nRF_Data.nRF_FULLBuffer[4+((i/2)*3)+2]) , true);
	  }
	  for(i=0;i<WS2812B_CH2_PIXELS; i++){
	      WS2812B_CH2.setPixelColor(i+1, (nRF_Data.nRF_FULLBuffer[((WS2812B_CH1_PIXELS/2)*3)+4+((i/2)*3)]<<16)|(nRF_Data.nRF_FULLBuffer[((WS2812B_CH1_PIXELS/2)*3)+4+((i/2)*3)+1]<<8)|(nRF_Data.nRF_FULLBuffer[((WS2812B_CH1_PIXELS/2)*3)+4+((i/2)*3)+2]) , true);
	  }
	#endif /* VERSION_RECEIVER120 */


	  IR_NEC.transmit();
	  WS2812B_CH1.show();
	  WS2812B_CH2.show();
	  SYS_DELAY.setMillis(0);
	  SYS_lastupdate = SYS_DELAY.getMillis();
	  nRF_Receiver_Mode = nRF_ReceiverModes_NormalUpdate;
	  SYS_IO.setValue(&nRF_LED, false);
	  //printf("[NRF]\tNew data set!\r\n");
      } else if (nRF_Data.nRF_Buffer[0] >= nRF_PacketCMDS_PayloadStart && nRF_Data.nRF_Buffer[0] <= nRF_PacketCMDS_PayloadEnd) {
	  // copy data to full buffer
	  memcpy((void*)(nRF_Data.nRF_FULLBuffer+((nRF_Data.nRF_Buffer[0]-1)*(NRF_Payload-1))), (void*)(nRF_Data.nRF_Buffer+1), NRF_Payload-1);
      } else if (nRF_Data.nRF_Buffer[0] == nRF_PacketCMDS_RainbowCommand) {
	  // give the rainbow command
	  SYS_DELAY.setMillis(0);
	  SYS_lastupdate = SYS_DELAY.getMillis();
	  HC595_595.setState(1,(nRF_Data.nRF_Buffer[4] & (1<<0))?(false):(true));
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[1], 1);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[2], 2);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[3], 3);
	  nRF_Receiver_Mode = nRF_ReceiverModes_Rainbow;
	  SYS_IO.setValue(&nRF_LED, false);
      } else if (nRF_Data.nRF_Buffer[0] == nRF_PacketCMDS_RainbowFastCommand) {
	  // give the fast rainbow command
	  SYS_DELAY.setMillis(0);
	  SYS_lastupdate = SYS_DELAY.getMillis();
	  HC595_595.setState(1,(nRF_Data.nRF_Buffer[4] & (1<<0))?(false):(true));
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[1], 1);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[2], 2);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[3], 3);
	  nRF_Receiver_Mode = nRF_ReceiverModes_RainbowFast;
	  SYS_IO.setValue(&nRF_LED, false);
      } else if (nRF_Data.nRF_Buffer[0] == nRF_PacketCMDS_ExpandPink) {
	  SYS_DELAY.setMillis(0);
	  SYS_lastupdate = SYS_DELAY.getMillis();
	  HC595_595.setState(1,(nRF_Data.nRF_Buffer[4] & (1<<0))?(false):(true));
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[1], 1);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[2], 2);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[3], 3);
	  nRF_Receiver_Mode = nRF_ReceiverModes_ExpandPink;
	  SYS_IO.setValue(&nRF_LED, false);
      } else if (nRF_Data.nRF_Buffer[0] == nRF_PacketCMDS_ExpandYellow) {
	  SYS_DELAY.setMillis(0);
	  SYS_lastupdate = SYS_DELAY.getMillis();
	  HC595_595.setState(1,(nRF_Data.nRF_Buffer[4] & (1<<0))?(false):(true));
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[1], 1);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[2], 2);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[3], 3);
	  nRF_Receiver_Mode = nRF_ReceiverModes_ExpandYellow;
	  SYS_IO.setValue(&nRF_LED, false);
      } else if (nRF_Data.nRF_Buffer[0] == nRF_PacketCMDS_ScanWhite) {
	  SYS_DELAY.setMillis(0);
	  SYS_lastupdate = SYS_DELAY.getMillis();
	  HC595_595.setState(1,(nRF_Data.nRF_Buffer[4] & (1<<0))?(false):(true));
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[1], 1);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[2], 2);
	  IR_NEC.setChannel(nRF_Data.nRF_Buffer[3], 3);
	  nRF_Receiver_Mode = nRF_ReceiverModes_ScanWhite;
	  SYS_IO.setValue(&nRF_LED, false);
      }


    }
#endif /* defined(VERSION_RECEIVER120) || defined(VERSION_RECEIVER120) */

#if defined(VERSION_TRANSMITTER)
    // transmitter functionality
    /* Working on pipe 1 with no acknowledgement with receiver
     * Working on pipe 2 with acknowledgement with remote
     *
     * Getting data from SD card and sending it to the receivers
     * Receiving data from the Remote for controls && sending data to remote
     */
    void nRF_TRANSMITTER(void){

      // send SD_BUFFER out to receiver
      uint32_t i, j, k;

	      if(SD_Buffer[nRF_BufferPositions_startALTCMD] == SD_AltCommands_Normal){
		    // sending the real payload
		      for(i=0; i<(NRF_TOTALBUFFSIZE/(NRF_Payload-1)); i++){
			  // fill the buffer
			  nRF_Data.nRF_Buffer[0] = i+1;
			  for(j=0; j<NRF_Payload-1; j++){
			      nRF_Data.nRF_Buffer[j+1] = SD_Buffer[nRF_BufferPositions_startRGBSPOTS+(i*(NRF_Payload-1))+j];
			  }
			  // send the buffer
			  SYS_IO.setValue(&nRF_LED, true);
			  for(k=0;k<3;k++){
			  nrf24_send(nRF_Data.nRF_Buffer);
			  while(nrf24_isSending());
			  SYS_DELAY.delayMilli(1);
			  }
			  SYS_IO.setValue(&nRF_LED, false);
		      }
			  // sending the finish command
			  nRF_Data.nRF_Buffer[0] = nRF_PacketCMDS_PayloadFinished;

			  // send the buffer
			  SYS_IO.setValue(&nRF_LED, true);
			  for(k=0;k<3;k++){
			  nrf24_send(nRF_Data.nRF_Buffer);
			  while(nrf24_isSending());
			  SYS_DELAY.delayMilli(1);
			  }
			  SYS_IO.setValue(&nRF_LED, false);
	      } else if(SD_Buffer[nRF_BufferPositions_startALTCMD] == SD_AltCommands_Rainbow){
		      // sending the finish command
		      nRF_Data.nRF_Buffer[0] = nRF_PacketCMDS_RainbowCommand;
			  for(j=0; j<NRF_Payload-1; j++){
			      nRF_Data.nRF_Buffer[j+1] = SD_Buffer[nRF_BufferPositions_startRGBSPOTS+j];
			  }
		      // send the buffer
		      SYS_IO.setValue(&nRF_LED, true);
		      for(k=0;k<3;k++){
		      nrf24_send(nRF_Data.nRF_Buffer);
		      while(nrf24_isSending());
		      SYS_DELAY.delayMilli(1);
		      }
		      SYS_IO.setValue(&nRF_LED, false);
	      } else if(SD_Buffer[nRF_BufferPositions_startALTCMD] == SD_AltCommands_RainbowFast){
		      // sending the finish command
		      nRF_Data.nRF_Buffer[0] = nRF_PacketCMDS_RainbowFastCommand;
			  for(j=0; j<NRF_Payload-1; j++){
			      nRF_Data.nRF_Buffer[j+1] = SD_Buffer[nRF_BufferPositions_startRGBSPOTS+j];
			  }

		      // send the buffer
		      SYS_IO.setValue(&nRF_LED, true);
		      for(k=0;k<3;k++){
		      nrf24_send(nRF_Data.nRF_Buffer);
		      while(nrf24_isSending());
		      SYS_DELAY.delayMilli(1);
		      }
		      SYS_IO.setValue(&nRF_LED, false);
	      } else if(SD_Buffer[nRF_BufferPositions_startALTCMD] == SD_AltCommands_ExpandingYellow){
		      // sending the finish command
		      nRF_Data.nRF_Buffer[0] = nRF_PacketCMDS_ExpandYellow;
			  for(j=0; j<NRF_Payload-1; j++){
			      nRF_Data.nRF_Buffer[j+1] = SD_Buffer[nRF_BufferPositions_startRGBSPOTS+j];
			  }

		      // send the buffer
		      SYS_IO.setValue(&nRF_LED, true);
		      for(k=0;k<3;k++){
		      nrf24_send(nRF_Data.nRF_Buffer);
		      while(nrf24_isSending());
		      SYS_DELAY.delayMilli(1);
		      }
		      SYS_IO.setValue(&nRF_LED, false);
	      } else if(SD_Buffer[nRF_BufferPositions_startALTCMD] == SD_AltCommands_ExpandingPink){
		      // sending the finish command
		      nRF_Data.nRF_Buffer[0] = nRF_PacketCMDS_ExpandPink;
			  for(j=0; j<NRF_Payload-1; j++){
			      nRF_Data.nRF_Buffer[j+1] = SD_Buffer[nRF_BufferPositions_startRGBSPOTS+j];
			  }

		      // send the buffer
		      SYS_IO.setValue(&nRF_LED, true);
		      for(k=0;k<3;k++){
		      nrf24_send(nRF_Data.nRF_Buffer);
		      while(nrf24_isSending());
		      SYS_DELAY.delayMilli(1);
		      }
		      SYS_IO.setValue(&nRF_LED, false);
	      } else if(SD_Buffer[nRF_BufferPositions_startALTCMD] == SD_AltCommands_ScanWhite){
		      // sending the finish command
		      nRF_Data.nRF_Buffer[0] = nRF_PacketCMDS_ScanWhite;
			  for(j=0; j<NRF_Payload-1; j++){
			      nRF_Data.nRF_Buffer[j+1] = SD_Buffer[nRF_BufferPositions_startRGBSPOTS+j];
			  }

		      // send the buffer
		      SYS_IO.setValue(&nRF_LED, true);
		      for(k=0;k<3;k++){
		      nrf24_send(nRF_Data.nRF_Buffer);
		      while(nrf24_isSending());
		      SYS_DELAY.delayMilli(1);
		      }
		      SYS_IO.setValue(&nRF_LED, false);
	      }







    }
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
    void nRF_REMOTE(void){

    }
#endif /* defined(VERSION_REMOTE) */

void initNRF(void){
  //SYS_IO.begin(nRF_CE);
  SYS_IO.begin(&nRF_IRQ, GPIOB, 2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Level_3);
  SYS_IO.begin(&nRF_LED, GPIOB, 11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Level_3);

  SYS_IO.setValue(&nRF_LED, false);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);

  EXTI_InitTypeDef temp;
  temp.EXTI_Line = EXTI_Line2;
  temp.EXTI_Mode = EXTI_Mode_Interrupt;
  temp.EXTI_Trigger = EXTI_Trigger_Falling;
  temp.EXTI_LineCmd = ENABLE;
  EXTI_Init(&temp);

  NVIC_InitTypeDef tmp;
  tmp.NVIC_IRQChannel = NRF_IRQN;
  tmp.NVIC_IRQChannelCmd = ENABLE;
  tmp.NVIC_IRQChannelPriority = 2;
  NVIC_Init(&tmp);


  /* init nrf24 hardware pins */
  nrf24_init();

  /* Channel #60 , payload length: 17 */


  /* Set the device addresses */
#ifdef nRF_Receiver
  nrf24_config(NRF_Channel_Receiver,NRF_Payload);
  memset((void*)nRF_Data.nRF_rx_address, NRF_RX_Receiver, 5);
  memset((void*)nRF_Data.nRF_tx_address, NRF_TX_Receiver, 5);
#elif defined(nRF_Transmitter)
  nrf24_config(NRF_Channel_Receiver,NRF_Payload);
  memset((void*)nRF_Data.nRF_rx_address, NRF_RX_Transmitter, 5);
  memset((void*)nRF_Data.nRF_tx_address, NRF_TX_Transmitter, 5);
#elif defined(nRF_Remote)
  nrf24_config(NRF_Channel_Remote,NRF_Payload);
  memset((void*)nRF_Data.nRF_rx_address, NRF_RX_Remote, 5);
  memset((void*)nRF_Data.nRF_tx_address, NRF_TX_Remote, 5);
#endif

  nrf24_tx_address(nRF_Data.nRF_tx_address);
  nrf24_rx_address(nRF_Data.nRF_rx_address);

}

#endif /* FUNCTION_HC595 */

// OLED screen
#ifdef FUNCTION_OLED

void initOLED(void){
  // init pins
  SYS_IO.begin(&OLED_MOSI, GPIOA, 7, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIO_AF_0);
  SYS_IO.begin(&OLED_MISO, GPIOA, 6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIO_AF_0);
  SYS_IO.begin(&OLED_SCK, GPIOA, 5, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIO_AF_0);
  SYS_IO.begin(&OLED_SS, GPIOA, 15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.begin(&OLED_RST, GPIOB, 0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.begin(&OLED_DC, GPIOB, 1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);

  SYS_IO.begin(&OLED_LED, GPIOA, 0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Level_3);

  SYS_IO.setValue(&OLED_LED, false);
  // init spi

  OLED_SPI.begin(&OLED_SPI_, SPI_CPHA_1Edge, SPI_CPOL_Low, SPI_BaudRatePrescaler_2, &OLED_SCK, &OLED_MOSI, &OLED_MISO, &OLED_SS);
  // init oled
  OLED_SCREEN.initialize();
}

#endif /* FUNCTION_OLED */

// External 8MHz crystal instead of HSI
#ifdef FUNCTION_OSC
#endif /* FUNCTION_OSC */

// Pushbuttons for user input
#ifdef FUNCTION_PB

void PB_UP_pressed(void){
  if(EXTI_GetITStatus(EXTI_Line10)){
      EXTI_ClearITPendingBit(EXTI_Line10);

      // do stuff
#if defined(VERSION_TRANSMITTER)
      if(PB_isReady()){
	  if(TRANSMITTER_STATE == TRANS_STATES_Pauze){
	      RTC_SetTime(RTC_Format_BIN, &TRANSMITTER_PAUZE_TIMING);
	  }
	  TRANSMITTER_STATE = TRANS_STATES_Play;
	  PB_setClickTime();
      }
#endif

  }
}
void PB_DOWN_pressed(void){
  if(EXTI_GetITStatus(EXTI_Line6)){
      EXTI_ClearITPendingBit(EXTI_Line6);

      // do stuff
#if defined(VERSION_TRANSMITTER)
      if(PB_isReady()){
	  TRANSMITTER_STATE = TRANS_STATES_Pauze;
	  RTC_GetTime(RTC_Format_BIN, &TRANSMITTER_PAUZE_TIMING);
	  PB_setClickTime();
      }
#endif

  }
}
void PB_LEFT_pressed(void){
  if(EXTI_GetITStatus(EXTI_Line12)){
      EXTI_ClearITPendingBit(EXTI_Line12);

      // do stuff
#if defined(VERSION_TRANSMITTER)
      if(PB_isReady()){
	  TRANSMITTER_STATE = TRANS_STATES_Backward1s;
	  PB_setClickTime();
      }
#endif
  }
}
void PB_RIGHT_pressed(void){
  if(EXTI_GetITStatus(EXTI_Line11)){
      EXTI_ClearITPendingBit(EXTI_Line11);

      // do stuff
#if defined(VERSION_TRANSMITTER)
      if(PB_isReady()){
	  TRANSMITTER_STATE = TRANS_STATES_Forward1s;
	  PB_setClickTime();
      }
#endif

  }
}
void PB_ENTER_pressed(void){
  if(EXTI_GetITStatus(EXTI_Line7)){
      EXTI_ClearITPendingBit(EXTI_Line7);

      // do stuff
#if defined(VERSION_TRANSMITTER)
      if(PB_isReady()){
	  TRANSMITTER_STATE = TRANS_STATES_ResetTimer;
	  PB_setClickTime();
      }
#endif

  }

}

void PB_setClickTime(void){
  PB_LASTCLICKTIME = SYS_DELAY.getMillis();
}

bool PB_isReady(void){
  if( (SYS_DELAY.getMillis()-PB_LASTCLICKTIME) > PB_RECLICKDELAYMS){
      return true;
  }
  return false;
}

void initPBs(void){
  SYS_IO.begin(&PB_UP, GPIOA, 10, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.begin(&PB_DOWN, GPIOF, 6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.begin(&PB_LEFT, GPIOA, 12, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.begin(&PB_RIGHT, GPIOA, 11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
  SYS_IO.begin(&PB_ENTER, GPIOF, 7, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource6);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource11);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource12);

  EXTI_InitTypeDef temp;
  temp.EXTI_Line = EXTI_Line6;
  temp.EXTI_Mode = EXTI_Mode_Interrupt;
  temp.EXTI_Trigger = EXTI_Trigger_Rising;
  temp.EXTI_LineCmd = ENABLE;
  EXTI_Init(&temp);

  temp.EXTI_Line = EXTI_Line10;
  EXTI_Init(&temp);
  temp.EXTI_Line = EXTI_Line11;
  EXTI_Init(&temp);
  temp.EXTI_Line = EXTI_Line12;
  EXTI_Init(&temp);
  temp.EXTI_Line = EXTI_Line7;
  temp.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_Init(&temp);

  NVIC_InitTypeDef tmp;
  tmp.NVIC_IRQChannel = EXTI4_15_IRQn;
  tmp.NVIC_IRQChannelCmd = ENABLE;
  tmp.NVIC_IRQChannelPriority = 3;
  NVIC_Init(&tmp);

}

#endif /* FUNCTION_PB */

// Real time clock for timing
#ifdef FUNCTION_RTC

void initRTC(void){
    RTC_InitTypeDef R;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    PWR_BackupAccessCmd(ENABLE);

    RCC_RTCCLKCmd(DISABLE);
    RCC_RTCCLKCmd(ENABLE);


    RCC_LSEDriveConfig(RCC_LSEDrive_High);
    RCC_LSEConfig(RCC_LSE_ON);

    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){
    }

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    R.RTC_AsynchPrediv = 0x7;
    R.RTC_SynchPrediv = 0xFFF;


    RCC_RTCCLKCmd(ENABLE);

    RTC_WaitForSynchro();

    /* Clear the RTC Alarm Flag */
    RTC_ClearFlag(RTC_FLAG_ALRAF);

    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);


    // set time to 00:00:00::000
    R.RTC_HourFormat = RTC_HourFormat_24;
    if (RTC_Init(&R) == ERROR){
	    printf("RTC init failed \r\n");
    }
    RTC_TimeStructInit(&RTC_TimeActual);
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeActual);

    // initialize alarms

    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

    NVIC_InitTypeDef rti;
    EXTI_InitTypeDef  eti;
    EXTI_ClearITPendingBit(EXTI_Line17);
    eti.EXTI_Line = EXTI_Line17;
    eti.EXTI_Mode = EXTI_Mode_Interrupt;
    eti.EXTI_Trigger = EXTI_Trigger_Rising;
    eti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&eti);
    rti.NVIC_IRQChannel = RTC_IRQn;
    rti.NVIC_IRQChannelPriority = 1;
    rti.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&rti);

    SYS_IO.begin(&RTC_LED, GPIOC, 13, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Level_3);
    SYS_IO.setValue(&RTC_LED, false);
}

#ifdef VERSION_TRANSMITTER
void RTC_waitUntilTimeSD(void){
  // time is in SD_BUFFER
  uint8_t minutes = SD_Buffer[0];
  uint8_t seconds = SD_Buffer[1];
  uint32_t subseconds = (SD_Buffer[2] << 8) | (SD_Buffer[3]);
  bool b_min, b_sec, b_subsec;
  // match time ; if actual time bigger, escape
  do{
      // get actual time
      RTC_GetTime(RTC_Format_BIN, &RTC_TimeActual);
      // compare time
      b_min = RTC_TimeActual.RTC_Minutes >= minutes;
      b_sec = RTC_TimeActual.RTC_Seconds >= seconds;
      uint32_t subs = RTC_GetSubSecond();
      b_subsec = subs >= subseconds;
  }while(b_min == false || b_sec == false || b_subsec == false );
}
#endif

#endif /* FUNCTION_RTC */

// SD storage card
#ifdef FUNCTION_SD

void initSD(void){
  uint8_t ret;
  if(SD_Detect() == SD_NOT_PRESENT){
    printf("Please insert SD card! \r\n");
    while (SD_Detect() == SD_NOT_PRESENT)
	  {}
    printf("SD card detected \r\n");
  }

  /* SD Card Initialization */

  SD_initReturn = SD_Init();
  if (SD_initReturn != 0)
  {
  /* Some 64/128MB SD cards fail for the 1st time: workaround is to call SD_Init() twice */
      SD_initReturn = SD_Init();
  if (SD_initReturn != 0)
  {
    init = false;
  }
  }
  //printf("initialization result: %s, %d \r\n", init ? "true" : "false", resp);
  /****************** FatFs Volume Acess **************************************/
  if(init == true){
    ret = f_mount(0, &SD_filesystem);
    if (!ret)
    {
	    mounting = true;
    }
    //printf("mounting gave back: %s, %d \r\n",mounting ? "true" : "false", ret);
  }
  printf("mounting %s", (mounting)?("was succesfull"):("failed!!"));
  if(mounting==false){
      printf(" errorcode: %d", ret);
  }

  SYS_IO.begin(&SD_LED, GPIOA, 1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Level_3);
  SYS_IO.setValue(&SD_LED, false);

  SYS_IO.begin(&SD_SS, GPIOA, 4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Level_3);

}

void SD_printResult(FRESULT inputVal){
  printf("[SD]\tResult gave back: ");

  switch(inputVal){
    case FR_OK:
      printf("Succeeded");
      break;
    case FR_DISK_ERR:
      printf("A hard error occured in the low level disk I/O layer");
      break;
    case FR_INT_ERR:
      printf("Assertion failed");
      break;
    case FR_NOT_READY:
      printf("The physical drive cannot work");
      break;
    case FR_NO_FILE:
      printf("Could not find the file");
      break;
    case FR_NO_PATH:
      printf("Could not find the path");
      break;
    case FR_INVALID_NAME:
      printf("The path name format is invalid");
      break;
    case FR_DENIED:
      printf("Acces denied due to prohibited access or directory full");
      break;
    case FR_EXIST:
      printf("Acces denied due to prohibited access");
      break;
    case FR_INVALID_OBJECT:
      printf("The file/directory object is invalid");
      break;
    case FR_WRITE_PROTECTED:
      printf("The physical drive is write protected");
      break;
    case FR_INVALID_DRIVE:
      printf("The logical drive number is invalid");
      break;
    case FR_NOT_ENABLED:
      printf("The volume has no work area");
      break;
    case FR_NO_FILESYSTEM:
      printf("There is no valid FAT volume on the physical drive");
      break;
    case FR_MKFS_ABORTED:
      printf("The f_mkfs() aborted due to any parameter error");
      break;
    case FR_TIMEOUT:
      printf("Could not get a grant to access the volume within defined period");
      break;
    case FR_LOCKED:
      printf("The operation is rejected according to the file sharing policy");
      break;
    case FR_NOT_ENOUGH_CORE:
      printf("LFN working buffer could not be allocated");
      break;
    case FR_TOO_MANY_OPEN_FILES:
      printf("Number of open files > _FS_SHARE");
      break;
  }
  printf("\r\n");
}

#endif /* FUNCTION_RTC */

// USART serial communication for printing
#ifdef FUNCTION_USART

void initUART(void){
	SYS_IO.begin(&USART_RX, GPIOA, 3, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_50MHz, GPIO_AF_1);
	SYS_IO.begin(&USART_TX, GPIOA, 2, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_50MHz, GPIO_AF_1);

	USART_UART.begin(USART_SELECTION, USART_BAUDRATE, &USART_RX, &USART_TX);
}

void printVersion(void){
	printf("[VERSION]\tUsing software version: v%s\r\n", FileVersion);
}

#endif /* FUNCTION_RTC */

// WS2812B led pixels
#ifdef FUNCTION_WS2812B

void initWS(void){
  SYS_IO.begin(&WS2812B_CH1_PIN, GPIOA, 8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_Level_3, GPIO_AF_2);
  WS2812B_CH1.begin(TIM1, 2, DMA1, DMA1_Channel3);
  SYS_IO.begin(&WS2812B_CH2_PIN, GPIOA, 9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_Level_3, GPIO_AF_2);
  WS2812B_CH2.begin(TIM1, 1, DMA1, DMA1_Channel2);

    #if defined(VERSION_RECEIVER60) || defined(VERSION_RECEIVER120) || defined(VERSION_ALL)
      WS2812B_CH1.setAllColor(0xFF0000);
      WS2812B_CH2.setAllColor(0xFF0000);

      WS2812B_CH1.show();
      WS2812B_CH2.show();
      SYS_DELAY.delayMilli(500);
      WS2812B_CH1.setAllColor(0x00FF00);
      WS2812B_CH2.setAllColor(0x00FF00);
      WS2812B_CH1.show();
      WS2812B_CH2.show();
      SYS_DELAY.delayMilli(500);
      WS2812B_CH1.setAllColor(0x0000FF);
      WS2812B_CH2.setAllColor(0x0000FF);
      WS2812B_CH1.show();
      WS2812B_CH2.show();
      SYS_DELAY.delayMilli(500);
      WS2812B_CH1.setAllColor(0xFFFFFF);
      WS2812B_CH2.setAllColor(0xFFFFFF);
      WS2812B_CH1.show();
      WS2812B_CH2.show();
      SYS_DELAY.delayMilli(500);
    #endif /* VERSION_RECEIVER60 || VERSION_RECEIVER120 */
}

#endif /* FUNCTION_WS2812B */

#if defined(VERSION_TRANSMITTER)

void TRANS_MoveRTCForward(uint32_t millis){
//RTC->TAFCR |= (uint32_t)(RTC_IT & RTC_TAFCR_TAMPIE)
  //(RTC->CR & (uint32_t)(RTC_IT_ALRA & (uint32_t)RTC_TAFCR_TAMPIE)) == RTC_TAFCR_TAMPIE
  //(RTC->TAFCR & (uint32_t)(RTC_IT & (uint32_t)RTC_TAFCR_TAMPIE)) == RTC_TAFCR_TAMPIE
  if ( ((RTC->CR & (uint32_t)(RTC_IT_ALRA & (uint32_t)RTC_TAFCR_TAMPIE)) == RTC_TAFCR_TAMPIE) || ((RTC->TAFCR & (uint32_t)(RTC_IT_ALRA & (uint32_t)RTC_TAFCR_TAMPIE)) == RTC_TAFCR_TAMPIE)  ){
      // do nothing
      printf("alarm set \r\n");
  } else {
      uint32_t secondportion = millis/1000;
      RTC_GetTime(RTC_Format_BIN, &RTC_TimeActual);
      // setting RTC time to 0
      RTC_TimeActual.RTC_H12 = RTC_H12_AM;
      RTC_TimeActual.RTC_Seconds += secondportion;

      if(RTC_TimeActual.RTC_Seconds>59){
	  RTC_TimeActual.RTC_Minutes += 1;
	  RTC_TimeActual.RTC_Seconds = RTC_TimeActual.RTC_Seconds%60;
      }


      RTC_SetTime(RTC_Format_BIN, &RTC_TimeActual);
      printf("moved forward with %ld \r\n", millis);

  }
}
void TRANS_MoveRTCBackward(uint32_t millis){

  if ((RTC->CR & (uint32_t)(RTC_IT_ALRA & (uint32_t)RTC_TAFCR_TAMPIE)) == RTC_TAFCR_TAMPIE){
      // do nothing
  } else {
      uint32_t secondportion = millis/1000;
      RTC_GetTime(RTC_Format_BIN, &RTC_TimeActual);
      // setting RTC time to 0
      RTC_TimeActual.RTC_H12 = RTC_H12_AM;
      if ( ((RTC->CR & (uint32_t)(RTC_IT_ALRA & (uint32_t)RTC_TAFCR_TAMPIE)) == RTC_TAFCR_TAMPIE) || ((RTC->TAFCR & (uint32_t)(RTC_IT_ALRA & (uint32_t)RTC_TAFCR_TAMPIE)) == RTC_TAFCR_TAMPIE)  ){
          // do nothing
          printf("alarm set \r\n");
      } else {
	  uint32_t tmp = RTC_TimeActual.RTC_Seconds;

	  RTC_TimeActual.RTC_Minutes -= 1;
	  RTC_TimeActual.RTC_Seconds = 60-(secondportion-tmp);
      }

      if(RTC_TimeActual.RTC_Seconds>59){
	  RTC_TimeActual.RTC_Minutes += 1;
	  RTC_TimeActual.RTC_Seconds = RTC_TimeActual.RTC_Seconds%60;
      }



      RTC_SetTime(RTC_Format_BIN, &RTC_TimeActual);

      printf("moved backward with %ld \r\n", millis);

  }

}
void TRANS_StartTransmitter(void){
  SYS_IO.setValue(&RTC_LED, true);
  // setting RTC time to 0
  RTC_TimeActual.RTC_H12 = RTC_H12_AM;
  RTC_TimeActual.RTC_Hours = 0;
  RTC_TimeActual.RTC_Minutes = 0;
  RTC_TimeActual.RTC_Seconds = 1;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeActual);
  SYS_IO.setValue(&RTC_LED, false);

  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
  RTC_ClearFlag(RTC_FLAG_ALRAF);
  SD_blockPointer = 0;
}
void TRANS_ReadWaitBlock(void){
  // clearing buffer
  memset((void*)SD_Buffer, 0, SD_SINGLEREAD);

  /*	[1]	*/

  // reading file
  SYS_IO.setValue(&SD_LED, true);
  // opening file
  SD_RESULT = f_open (&SD_FILE, "ani.bin", FA_OPEN_EXISTING | FA_READ);
  // moving filehead
  f_lseek(&SD_FILE, SD_blockPointer);
  // read out 188 bytes
  f_read(&SD_FILE, SD_Buffer, SD_SINGLEREAD, &SD_bytesRead);

  // closing file
  f_close(&SD_FILE);

  SYS_IO.setValue(&SD_LED, false);

  SYS_IO.setValue(&SD_SS, true);

  // increment read head pointer
  SD_blockPointer += SD_bytesRead;

  if(SD_bytesRead == SD_SINGLEREAD){
	  /*	[2]	*/
	  uint8_t minutes = SD_Buffer[0];
	  uint8_t seconds = SD_Buffer[1];
	  uint32_t subseconds = (SD_Buffer[2] << 8) | (SD_Buffer[3]);
	  uint32_t subs;

	  NVIC_DisableIRQ(RTC_IRQn);
	  RTC_ITConfig(RTC_IT_ALRA, DISABLE);
	  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

	  SYS_IO.setValue(&RTC_LED, true);

	  RTC_Alarm.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
	  RTC_Alarm.RTC_AlarmTime.RTC_Hours = 0;
	  RTC_Alarm.RTC_AlarmTime.RTC_Minutes = minutes;
	  RTC_Alarm.RTC_AlarmTime.RTC_Seconds = seconds;
	  RTC_Alarm.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	  RTC_Alarm.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	  RTC_Alarm.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;
	  RTC_HoldTransmit = true;

	  RTC_AlarmSubSecondConfig(RTC_Alarm_A, subseconds, RTC_AlarmSubSecondMask_None);
	  RTC_GetTime(RTC_Format_BIN, &RTC_TimeActual);

	  RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_Alarm);

	  NVIC_EnableIRQ(RTC_IRQn);
	  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	      RTC_GetTime(RTC_Format_BIN, &RTC_TimeActual);
	      subs = RTC_GetSubSecond();
	      //&& (RTC_TimeActual.RTC_Minutes < minutes && RTC_TimeActual.RTC_Seconds < seconds && subs < subseconds)
	      //printf("%d:%d.%ld actual. %d:%d.%ld wanted.\r\n", RTC_TimeActual.RTC_Minutes, RTC_TimeActual.RTC_Seconds, subs, minutes, seconds, subseconds);
	      //&& (RTC_TimeActual.RTC_Minutes < minutes && RTC_TimeActual.RTC_Seconds < seconds && subs < subseconds)
	  while(RTC_HoldTransmit){
	      //RTC_GetTime(RTC_Format_BIN, &RTC_TimeActual);
	      RTC_GetTime(RTC_Format_BIN, &RTC_TimeActual);
	      subs = RTC_GetSubSecond();
	      if(SYS_DELAY.getMillis()%2000 == 0){
		  printf("%d:%d.%ld actual. %d:%d.%ld wanted... waiting...\r\n", RTC_TimeActual.RTC_Minutes, RTC_TimeActual.RTC_Seconds, subs, minutes, seconds, subseconds);
	      }

	      if((RTC_TimeActual.RTC_Minutes > minutes && RTC_TimeActual.RTC_Seconds > seconds && subs < subseconds)){
		  printf("timer overflow\r\n");
		  RTC_HoldTransmit = false;
	      } else if ((RTC_TimeActual.RTC_Minutes > minutes && RTC_TimeActual.RTC_Seconds > (seconds+1))){
		  printf("timer overflow plus 1 second \r\n");
		  RTC_HoldTransmit = false;
	      }
	  }

	  printf("%d:%d.%ld actual. %d:%d.%ld wanted.\r\n", RTC_TimeActual.RTC_Minutes, RTC_TimeActual.RTC_Seconds, subs, minutes, seconds, subseconds);
	  SYS_IO.setValue(&RTC_LED, false);

	  /*	[3]	*/
	  nRF_TRANSMITTER();


  }
}


#endif























