#ifndef __USART_H_
#define __USART_H_

#include "stm32l1xx.h"

void USART_Configuration(void);
void Usart_SendStrings(unsigned char *str);
void Usart2_SendData(uint8_t *str, int16_t len);

#endif /* __USART_H */

