
#ifndef _USART1_H
#define _USART1_H
#include "stm32f10x.h"                  // Device header

void USART1_Init(void);
void USART1_SendByte(uint8_t data);

#endif // !_USART1_H
