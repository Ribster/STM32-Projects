/*
 * rng.c
 *
 *  Created on: 20-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "rng.h"

void
initialize_RNG(void){
	 /* Enable RNG clock source */
	  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

	  /* RNG Peripheral enable */
	  RNG_Cmd(ENABLE);
}
