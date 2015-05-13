/*
 * general.c
 *
 *  Created on: 7-mei-2015
 *      Author: Robbe Van Assche
 */

#include "general.h"

void
general_convertToCapital(char* str){
	for(uint8_t tmp_iterator=0; tmp_iterator<strlen(str); tmp_iterator++){
		if( (str[tmp_iterator] <= 'z') || (str[tmp_iterator] >= 'a') ){
			str[tmp_iterator] += ('A' - 'a');
		}
	}
}


// convert base 2 to base 10 ASCII
char *general_dec32(unsigned long i)
{
  static char str[16];
  char *s = str + sizeof(str);

  *--s = 0;

  do
  {
    *--s = '0' + (char)(i % 10);
    i /= 10;
  }
  while(i);

  return(s);
}
