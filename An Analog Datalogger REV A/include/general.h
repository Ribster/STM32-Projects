/*
 * general.h
 *
 *  Created on: 7-mei-2015
 *      Author: Robbe Van Assche
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "projectconfig.h"



typedef struct timeRegistration_t{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t days;
	uint8_t weeks;
}timeRegistration_t;

typedef struct xypair_t{
	uint8_t x;
	uint8_t y;
}xypair_t;

typedef struct xycorners_t{
	xypair_t topLeft;
	xypair_t topRight;
	xypair_t bottomLeft;
	xypair_t bottomRight;
}xycorners_t;


#define MAX(a,b) (((a)>(b))?(a):(b))
//#define MIN(a,b) (((a)<(b))?(a):(b))

void
general_convertToCapital(char* str);
char*
general_dec32(unsigned long i);

#endif /* GENERAL_H_ */
