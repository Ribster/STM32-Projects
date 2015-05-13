#include "stm32f0xx.h"
#include "stm32f0xx_usart.h"

extern "C"
int __io_putchar(int ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */

  /* Loop until the transmission buffer is empty */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

  USART_SendData(USART1, (uint8_t) ch);

  return(ch);
}

//******************************************************************************

/**
  * @brief  Retargets the C library scanf function to the USART (GNU)
  * @param  None
  * @retval None
  */
extern "C"
int __io_getchar(void)
{
  /* Place your implementation of fgetc here */
  /* e.g. read a character from the USART */

  /* Loop until the reception buffer is not empty */
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

  return((int)USART_ReceiveData(USART1));
}




int main(){
	while(1){

	}
	return 0;
}
