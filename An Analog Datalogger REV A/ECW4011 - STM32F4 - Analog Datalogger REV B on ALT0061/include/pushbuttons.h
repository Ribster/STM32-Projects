/*
 * pushbuttons.h
 *
 *  Created on: 13-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef PUSHBUTTONS_PROJECT_H_
#define PUSHBUTTONS_PROJECT_H_

#include "projectconfig.h"
#include "leds.h"
#include "menustructure.h"

// prototype
void
initialize_pushButtons(void);
void
pushbuttons_interruptHandler_UP(void);
void
pushbuttons_interruptHandler_LEFT(void);
void
pushbuttons_interruptHandler_MIDDLE(void);
void
pushbuttons_interruptHandler_RIGHT(void);
void
pushbuttons_interruptHandler_DOWN(void);
void
pushbuttons_interruptHandler_ENTER(void);

#endif /* PUSHBUTTONS_H_ */
