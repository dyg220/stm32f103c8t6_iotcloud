
#ifndef _USART1_H
#define _USART1_H
#include "stm32f10x.h"                  // Device header
#include <stdio.h>

void USART1_Config(u32 baud);
void USART1_SendByte(u8 data);
void USART1_SendStr(u8* str);


#endif // !_USART1_H