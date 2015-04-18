/*
 * leds.c
 *
 *  Created on: 13-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "leds.h"

// used variables
GPIO_TypeDef* led_portList[] = {LED_PORT_0, LED_PORT_1, LED_PORT_2, LED_PORT_3, LED_PORT_4};
uint32_t led_pinList[] = {LED_PIN_0, LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4};

// implementation
void
initialize_LEDS(void){
	// init LED 0
	gpio_initStandard(
			LED_PORT_0,
			LED_MODE_0,
			LED_OTYPE_0,
			LED_PIN_0,
			LED_PULL_0,
			LED_SPEED_0);
	// init LED 1
	gpio_initStandard(
			LED_PORT_1,
			LED_MODE_1,
			LED_OTYPE_1,
			LED_PIN_1,
			LED_PULL_1,
			LED_SPEED_1);
	// init LED 2
	gpio_initStandard(
			LED_PORT_2,
			LED_MODE_2,
			LED_OTYPE_2,
			LED_PIN_2,
			LED_PULL_2,
			LED_SPEED_2);
	// init LED 3
	gpio_initStandard(
			LED_PORT_3,
			LED_MODE_3,
			LED_OTYPE_3,
			LED_PIN_3,
			LED_PULL_3,
			LED_SPEED_3);
	// init LED 4
	gpio_initStandard(
			LED_PORT_4,
			LED_MODE_4,
			LED_OTYPE_4,
			LED_PIN_4,
			LED_PULL_4,
			LED_SPEED_4);
	// set/reset initialization state
	GPIO_WriteBit(LED_PORT_0, (1<<LED_PIN_0), LED_INITSTATE_0);
	GPIO_WriteBit(LED_PORT_1, (1<<LED_PIN_1), LED_INITSTATE_1);
	GPIO_WriteBit(LED_PORT_2, (1<<LED_PIN_2), LED_INITSTATE_2);
	GPIO_WriteBit(LED_PORT_3, (1<<LED_PIN_3), LED_INITSTATE_3);
	GPIO_WriteBit(LED_PORT_4, (1<<LED_PIN_4), LED_INITSTATE_4);
}
void
leds_setLed(uint8_t ref,
		FunctionalState newState){
	GPIO_WriteBit(led_portList[ref], (1<<led_pinList[ref]), (newState)?Bit_SET:Bit_RESET);
}
void
leds_toggleLed(uint8_t ref){
	GPIO_ToggleBits(led_portList[ref], (1<<led_pinList[ref]));
}
void
leds_ledTest(void){
	for(int i=0; i<5; i++){
		GPIO_SetBits(led_portList[i], (1<<led_pinList[i]));
		delay_milli(SYSTICK_FREQUENCY_HZ/10);
		GPIO_ResetBits(led_portList[i], (1<<led_pinList[i]));
	}
	delay_milli(SYSTICK_FREQUENCY_HZ);
	for(int i=5; i>0; i--){
		GPIO_SetBits(led_portList[i-1], (1<<led_pinList[i-1]));
		delay_milli(SYSTICK_FREQUENCY_HZ/10);
		GPIO_ResetBits(led_portList[i-1], (1<<led_pinList[i-1]));
	}
	delay_milli(SYSTICK_FREQUENCY_HZ);
}
void
leds_ledEncoderExample(void){
	// read the encoders
	int16_t valOne = encoder_getEncoderOneValue()*400;
	printf("Encoder value One: %d", encoder_getEncoderOneValue());
	leds_setLed(ledList_White1, DISABLE);
	leds_setLed(ledList_Blue, DISABLE);
	leds_setLed(ledList_Orange, DISABLE);
	leds_setLed(ledList_Green, DISABLE);
	leds_setLed(ledList_White2, DISABLE);

	if(valOne <= 0){

	} else if ( (valOne > 0) && ( valOne < (8175)) ) {
		leds_setLed(ledList_White2, ENABLE);
	} else if ( (valOne >= (8175)) && ( valOne < (16350)) ) {
		leds_setLed(ledList_White2, ENABLE);
		leds_setLed(ledList_Green, ENABLE);
	} else if ( (valOne >= (16350)) && ( valOne < (24525)) ) {
		leds_setLed(ledList_White2, ENABLE);
		leds_setLed(ledList_Green, ENABLE);
		leds_setLed(ledList_Orange, ENABLE);
	} else if ( (valOne >= (24525)) && ( valOne < (32000)) ) {
		leds_setLed(ledList_White2, ENABLE);
		leds_setLed(ledList_Green, ENABLE);
		leds_setLed(ledList_Orange, ENABLE);
		leds_setLed(ledList_Blue, ENABLE);
	} else {
		leds_setLed(ledList_White2, ENABLE);
		leds_setLed(ledList_Green, ENABLE);
		leds_setLed(ledList_Orange, ENABLE);
		leds_setLed(ledList_Blue, ENABLE);
		leds_setLed(ledList_White1, ENABLE);
	}

}
