/*
 * pushbuttons.c
 *
 *  Created on: 13-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "pushbuttons.h"

// implementation
void
initialize_pushButtons(void){
	// init Pushbutton 0 (UP)
	gpio_initStandard(
			PB_PORT_0,
			PB_MODE_0,
			PB_OTYPE_0,
			PB_PIN_0,
			PB_PULL_0,
			PB_SPEED_0);
	gpio_initInterruptEXTI(
			PB_PORT_0,
			PB_PIN_0,
			EXTI_Trigger_Rising,
			EXTI_Mode_Interrupt,
			PB_IT_IRQChannelPreemptionPriority,
			PB_IT_IRQChannelSubPriority);

	// init Pushbutton 1 (LEFT)
	gpio_initStandard(
			PB_PORT_1,
			PB_MODE_1,
			PB_OTYPE_1,
			PB_PIN_1,
			PB_PULL_1,
			PB_SPEED_1);
	gpio_initInterruptEXTI(
			PB_PORT_1,
			PB_PIN_1,
			EXTI_Trigger_Rising,
			EXTI_Mode_Interrupt,
			PB_IT_IRQChannelPreemptionPriority,
			PB_IT_IRQChannelSubPriority);

	// init Pushbutton 2 (MIDDLE)
	gpio_initStandard(
			PB_PORT_2,
			PB_MODE_2,
			PB_OTYPE_2,
			PB_PIN_2,
			PB_PULL_2,
			PB_SPEED_2);
	gpio_initInterruptEXTI(
			PB_PORT_2,
			PB_PIN_2,
			EXTI_Trigger_Rising,
			EXTI_Mode_Interrupt,
			PB_IT_IRQChannelPreemptionPriority,
			PB_IT_IRQChannelSubPriority);

	// init Pushbutton 3 (RIGHT)
	gpio_initStandard(
			PB_PORT_3,
			PB_MODE_3,
			PB_OTYPE_3,
			PB_PIN_3,
			PB_PULL_3,
			PB_SPEED_3);
	gpio_initInterruptEXTI(
			PB_PORT_3,
			PB_PIN_3,
			EXTI_Trigger_Rising,
			EXTI_Mode_Interrupt,
			PB_IT_IRQChannelPreemptionPriority,
			PB_IT_IRQChannelSubPriority);

	// init Pushbutton 4 (DOWN)
	gpio_initStandard(
			PB_PORT_4,
			PB_MODE_4,
			PB_OTYPE_4,
			PB_PIN_4,
			PB_PULL_4,
			PB_SPEED_4);
	gpio_initInterruptEXTI(
			PB_PORT_4,
			PB_PIN_4,
			EXTI_Trigger_Rising,
			EXTI_Mode_Interrupt,
			PB_IT_IRQChannelPreemptionPriority,
			PB_IT_IRQChannelSubPriority);

	// init Pushbutton 5 (ENTER)
	gpio_initStandard(
			PB_PORT_5,
			PB_MODE_5,
			PB_OTYPE_5,
			PB_PIN_5,
			PB_PULL_5,
			PB_SPEED_5);
	gpio_initInterruptEXTI(
			PB_PORT_5,
			PB_PIN_5,
			EXTI_Trigger_Rising,
			EXTI_Mode_Interrupt,
			PB_IT_IRQChannelPreemptionPriority,
			PB_IT_IRQChannelSubPriority);
}

void
pushbuttons_interruptHandler_UP(void){
	  if(EXTI_GetITStatus(PB_EXTI_Line_0) != RESET)
	  {
	    leds_toggleLed(ledList_White1);

	    /* Clear the EXTI line XX pending bit */
	    EXTI_ClearITPendingBit(PB_EXTI_Line_0);
	  }
}

void
pushbuttons_interruptHandler_LEFT(void){
	  if(EXTI_GetITStatus(PB_EXTI_Line_1) != RESET)
	  {
	    leds_toggleLed(ledList_Blue);

	    /* Clear the EXTI line XX pending bit */
	    EXTI_ClearITPendingBit(PB_EXTI_Line_1);
	  }
}

void
pushbuttons_interruptHandler_MIDDLE(void){
	  if(EXTI_GetITStatus(PB_EXTI_Line_2) != RESET)
	  {
	    leds_toggleLed(ledList_Orange);

	    /* Clear the EXTI line XX pending bit */
	    EXTI_ClearITPendingBit(PB_EXTI_Line_2);
	  }
}

void
pushbuttons_interruptHandler_RIGHT(void){
	  if(EXTI_GetITStatus(PB_EXTI_Line_3) != RESET)
	  {
	    leds_toggleLed(ledList_Green);

	    /* Clear the EXTI line XX pending bit */
	    EXTI_ClearITPendingBit(PB_EXTI_Line_3);
	  }
}

void
pushbuttons_interruptHandler_DOWN(void){
	  if(EXTI_GetITStatus(PB_EXTI_Line_4) != RESET)
	  {
	    leds_toggleLed(ledList_White2);

	    /* Clear the EXTI line XX pending bit */
	    EXTI_ClearITPendingBit(PB_EXTI_Line_4);
	  }
}

void
pushbuttons_interruptHandler_ENTER(void){
	  if(EXTI_GetITStatus(PB_EXTI_Line_5) != RESET)
	  {
	    leds_toggleLed(ledList_White1);
	    leds_toggleLed(ledList_Blue);
	    leds_toggleLed(ledList_Orange);
	    leds_toggleLed(ledList_Green);
	    leds_toggleLed(ledList_White2);

	    /* Clear the EXTI line XX pending bit */
	    EXTI_ClearITPendingBit(PB_EXTI_Line_5);
	  }
}
